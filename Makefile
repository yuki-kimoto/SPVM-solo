# -*- mode: makefile-gmake -*-

all:
.PHONY: all

# Disable the default rule: sperl_yacc.y -> sperl_yacc.c
.SUFFIXES:
.SUFFIXES: .c .o

CC     := gcc
CFLAGS := -std=c99 -g -O
LIBS   := -lm
DIRS   :=

CPPFLAGS = -MD -MF $(@:%.o=%.Po)

# sperl

sperl_SRCS := main/sperl_main.c $(wildcard *.c)
sperl_OBJS := $(sperl_SRCS:%.c=objs/%.o)
sperl_LIBS := $(LIBS)

DIRS += objs/main objs
objs/%.o: %.c | objs/main objs
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

-include $(sperl_OBJS:%.o=%.Po)
sperl_yacc.tab.c: sperl_yacc.y
	bison -t -p SPerl_yy -d sperl_yacc.y
sperl: $(sperl_OBJS)
	$(CC) -o $@ $(LDFLAGS) $(sperl_OBJS) $(sperl_LIBS)
all: sperl

# tests

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
	-rm -f $(sperl_OBJS) $(sperl_OBJS:%.o=%.Po)
