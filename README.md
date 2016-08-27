# Static Perl

This is my experiment to create static perl implementation.

Perlish syntax, fast compile, fast runtime, fast culculation, parallel, GC.

## Run

    bison -p SPerl_yy -d sperly.y && gcc -O -o sperl sperly.tab.c sperl_main.c sperl_toke.c sperl_op.c && ./sperl test.spl

I only created syntax rule. This command ouput ruduce result.

## Files

**stoke.c**

Tokenizer

**sperl.y**

Parser

**sperl.h**

Header

**sperlmain.c**

Program entry point
