# Static Perl

This is my experiment to create static perl implementation.

Perlish syntax, fast compile, fast runtime, fast culculation, parallel, GC.

## Run

    bison -p static_yy -d static_perl.y && gcc -O -o sperl static_perl.tab.c static_perlmain.c static_toke.c && ./sperl test.spl

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
