# -*- mode: makefile-gmake -*-

all:
.PHONY: all

CC     	 := gcc
CFLAGS 	 := -std=c99 -g -O -Wall -Wextra
LIBS   	 := -lm
DIRS     :=
CPPFLAGS  = -MD -MF $(@:%.o=%.Po)

OBJDIR := objs
DIRS += $(OBJDIR) $(OBJDIR)/main $(OBJDIR)/t
$(OBJDIR)/%.o: %.c | $(OBJDIR) $(OBJDIR)/main $(OBJDIR)/t
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

# libspvm
libspvm_SRCS := $(wildcard *.c) spvm_yacc.c
libspvm_OBJS := $(libspvm_SRCS:%.c=$(OBJDIR)/%.o)
-include $(libspvm_OBJS:%.o=%.Po)
generated_sources += spvm_yacc.c spvm_yacc.h
spvm_yacc.c: spvm_yacc.y
	bison -t -p SPVM_yy -d spvm_yacc.y
	mv -f spvm_yacc.tab.c $@
	mv -f spvm_yacc.tab.h $(@:%.c=%.h)
spvm_yacc.h: spvm_yacc.c
$(OBJDIR)/spvm_toke.o: spvm_toke.c spvm_yacc.h
$(OBJDIR)/spvm_yacc_util.o: spvm_yacc_util.c spvm_yacc.h
libspvm.a: $(libspvm_OBJS)
	$(AR) crs $@ $(libspvm_OBJS)
all: libspvm.a

# spvm
spvm_SRCS    := main/spvm_main.c
spvm_OBJS    := $(spvm_SRCS:%.c=$(OBJDIR)/%.o)
spvm_LDFLAGS := $(LDFLAGS) -L .
spvm_LIBS    := -lspvm $(LIBS)
-include $(spvm_OBJS:%.o=%.Po)
spvm: $(spvm_OBJS) libspvm.a
	$(CC) -o $@ $(spvm_LDFLAGS) $(spvm_OBJS) $(spvm_LIBS)
all: spvm

# test (t/spvm_t_*)
test_names := $(wildcard t/spvm_t_*.c)
test_names := $(test_names:t/spvm_t_%.c=spvm_t_%)

spvm_t_LDFLAGS := $(LDFLAGS) -L .
spvm_t_LIBS    := -lspvm $(LIBS)
-include $(test_names:spvm_t_%=$(OBJDIR)/t/spvm_t_%.Po)
$(OBJDIR)/t/spvm_t_%: $(OBJDIR)/t/spvm_t_%.o libspvm.a
	$(CC) $(spvm_t_LDFLAGS) -o $@ $< $(spvm_t_LIBS)
spvm_t_%: $(OBJDIR)/t/spvm_t_%
	./$<
.PRECIOUS: $(OBJDIR)/t/spvm_t_%.o $(OBJDIR)/t/spvm_t_%
test: $(test_names)
.PHONY: test

# misc
$(DIRS):
	mkdir -p $@

.PHONY: clean
clean:
	-find $(OBJDIR) -name \*.o -o -name \*.Po | xargs rm -rf
	-rm -rf libspvm.a $(generated_sources)
