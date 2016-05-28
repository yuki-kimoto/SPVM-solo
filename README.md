# Static Perl

This is my experiment to create static perl implementation.

Perlish syntax, fast compile, fast runtime, fast culculation, parallel, GC.

## Run

    bison -d sperl.y && gcc -O -o sperl sperl.tab.c sperlmain.c stoke.c && ./sperl test.spl
