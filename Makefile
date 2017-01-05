# -*- mode: makefile-gmake -*-

all:
.PHONY: all

#------------------------------------------------------------------------------
# Disable the default suffix rule ".y.c: ; $(YACC) $(YFLAGS) $<"
#
#   This package contains independent two files sperl_yacc.c and sperl_yacc.y.
#   While, there is a default suffix rule .y.c to generate %.c from %.y.
#   Because of this rule, make try to overwrite sperl_yacc.c by the command
#   $(YACC) $(YFLAGS) sperl_yacc.y. To disable the default rule,
#
#   1. clear the suffices (the first line)
#   2. specify .c and .o as suffix (the second line)
#
.SUFFIXES:
.SUFFIXES: .c .o

CC     	 := gcc
CFLAGS 	 := -std=c99 -g -O -Wall -Wextra
LIBS   	 := -lm
DIRS     :=
CPPFLAGS  = -MD -MF $(@:%.o=%.Po)

OBJDIR := objs
DIRS += $(OBJDIR) $(OBJDIR)/main $(OBJDIR)/t
$(OBJDIR)/%.o: %.c | $(OBJDIR) $(OBJDIR)/main $(OBJDIR)/t
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

# libsperl
libsperl_SRCS := $(wildcard *.c)
libsperl_OBJS := $(libsperl_SRCS:%.c=$(OBJDIR)/%.o)
-include $(libsperl_OBJS:%.o=%.Po)
$(OBJDIR)/sperl_toke.o: sperl_toke.c sperl_yacc.tab.h
$(OBJDIR)/sperl_yacc.o: sperl_yacc.c sperl_yacc.tab.h 
sperl_yacc.tab.h: sperl_yacc.y
	bison -t -p SPerl_yy -d sperl_yacc.y
libsperl.a: $(libsperl_OBJS)
	$(AR) crs $@ $(libsperl_OBJS)
all: libsperl.a

# sperl
sperl_SRCS    := main/sperl_main.c
sperl_OBJS    := $(sperl_SRCS:%.c=$(OBJDIR)/%.o)
sperl_LDFLAGS := $(LDFLAGS) -L .
sperl_LIBS    := -lsperl $(LIBS)
-include $(sperl_OBJS:%.o=%.Po)
sperl: $(sperl_OBJS) libsperl.a
	$(CC) -o $@ $(sperl_LDFLAGS) $(sperl_OBJS) $(sperl_LIBS)
all: sperl

# test (t/sperl_t_*)
test_names := $(wildcard t/sperl_t_*.c)
test_names := $(test_names:t/sperl_t_%.c=sperl_t_%)

sperl_t_LDFLAGS := $(LDFLAGS) -L .
sperl_t_LIBS    := -lsperl $(LIBS)
-include $(test_names:sperl_t_%=$(OBJDIR)/t/sperl_t_%.Po)
$(OBJDIR)/t/sperl_t_%: $(OBJDIR)/t/sperl_t_%.o libsperl.a
	$(CC) $(sperl_t_LDFLAGS) -o $@ $< $(sperl_t_LIBS)
sperl_t_%: $(OBJDIR)/t/sperl_t_%
	./$<
.PRECIOUS: $(OBJDIR)/t/sperl_t_%.o $(OBJDIR)/t/sperl_t_%
check: $(test_names)
.PHONY: check

# test (*.spvm)
.PHONY: test test2
test: sperl
	./$< Test
test2: sperl
	./$< Test2

# misc

$(DIRS):
	mkdir -p $@

.PHONY: clean
clean:
	-find $(OBJDIR) -name \*.o -o -name \*.Po | xargs rm -rf
