# -*- mode: makefile-gmake -*-

all:
.PHONY: all

# Disable the default rule: sperl_yacc.y -> sperl_yacc.c
.SUFFIXES:
.SUFFIXES: .c .o

CC     := gcc
CFLAGS := -std=c99 -g -O
LIBS   := -lm


# sperl

sperl_SRCS := main/sperl_main.c $(wildcard *.c)
sperl_OBJS := $(sperl_SRCS:%.c=%.o)
sperl_LIBS := $(LIBS)
sperl_yacc.tab.c: sperl_yacc.y
	bison -t -p SPerl_yy -d sperl_yacc.y
sperl: $(sperl_OBJS)
	gcc -o $@ $(LDFLAGS) $(sperl_OBJS) $(sperl_LIBS)
all: sperl

# tests

.PHONY: test test2
test: sperl
	./$< Test
test2: sperl
	./$< Test2
