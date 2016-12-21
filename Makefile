# -*- mode: makefile-gmake -*-

all:
.PHONY: all

# Disable the default rule: sperl_yacc.y -> sperl_yacc.c
.SUFFIXES:
.SUFFIXES: .c .o

CC     	 := gcc
CFLAGS 	 := -std=c99 -g -O
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
sperl_yacc.tab.c: sperl_yacc.y
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
