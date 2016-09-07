# Static Perl - Fast culculation, parallel process, GC, static typing, VM with  perlish syntax

Do you need **fast** Perl? Static Perl provide fast culculation system on Perl.

- **Fast culculation** - Perl biggest weekpoint is culculation performance. Static Perl provide fast culculation.
- **Paralel process** - Support paralel process to process array fast
- **GC** - You don't need to think about freeing memory
- **Static typing** - Static typing for performance
- **VM** - Byte code is created and you can run it by Static Perl VM
- **Perlish syntax** - syntax is very similar with Perl

This is now **developping**.

I have implmented only parts of **tokenizer** and **abstract syntax tree generator**.

```
package main;

sub main : int () {
  my $num1 : int = 2;
  my $num2 : int = 5;

  my $num3 : int = sum($num1, $num3);
}

sub sum : int ($num1 : int, $num2 : int) {
  my $num3 : int;

  $num3 = $num1 + $num1;

  return $num3;
}
```

## Run

    bison -p SPerl_yy -d sperly.y && gcc -O -o sperl sperly.tab.c sperl_main.c sperl_toke.c sperl_op.c && ./sperl test.spl

Now I only print the result of **token reduction** and **abstract syntax tree**.

## Source files

- **sperl.h** - Main header
- **sperl_parser.h** - Parser header
- **sperl_op.h** - Create OP tree header
- **sperl_main.c** - Program entry point
- **sperl_toke.c** - Tokenizer
- **sperly.y** - Parser generator
- **sperl_op.c** - Create OP tree
- **test.spl** - Test script
