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

DIRS += objs objs/main objs/t
objs/%.o: %.c | objs objs/main objs/t
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

# libsperl
libsperl_SRCS := $(wildcard *.c)
libsperl_OBJS := $(libsperl_SRCS:%.c=objs/%.o)
-include $(libsperl_OBJS:%.o=%.Po)
objs/sperl_toke.o: sperl_toke.c sperl_yacc.tab.h
objs/sperl_yacc.o: sperl_yacc.c sperl_yacc.tab.h 
sperl_yacc.tab.h: sperl_yacc.y
	bison -t -p SPerl_yy -d sperl_yacc.y
libsperl.a: $(libsperl_OBJS)
	$(AR) crs $@ $(libsperl_OBJS)
all: libsperl.a

# sperl
sperl_SRCS    := main/sperl_main.c
sperl_OBJS    := $(sperl_SRCS:%.c=objs/%.o)
sperl_LDFLAGS := $(LDFLAGS) -L .
sperl_LIBS    := -lsperl $(LIBS)
-include $(sperl_OBJS:%.o=%.Po)
sperl: $(sperl_OBJS) libsperl.a
	$(CC) -o $@ $(sperl_LDFLAGS) $(sperl_OBJS) $(sperl_LIBS)
all: sperl

# test
test_names += sperl_t_array
test_names += sperl_t_hash
test_names += sperl_t_memory_pool

sperl_t_LDFLAGS := $(LDFLAGS) -L .
sperl_t_LIBS    := -lsperl $(LIBS)
-include $(test_names:sperl_t_%=objs/t/sperl_t_%.Po)
tmp_sperl_t_%: objs/t/sperl_t_%.o libsperl.a
	$(CC) $(sperl_t_LDFLAGS) -o $@ $< $(sperl_t_LIBS)
.PRECIOUS: objs/t/sperl_t_%.o
sperl_t_%: tmp_sperl_t_%
	./$<
check: $(test_names)
.PHONY: check

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
	-find objs -name \*.o -o -name \*.Po | xargs rm -rf
