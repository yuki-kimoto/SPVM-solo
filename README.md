# Static Perl

Perlish syntax, fast compile, fast runtime, fast culculation, parallel, GC, VM, byte code.

This is my experiment to create static perl implementation.

From now, I have implmented parts of tokenizer and abstract syntax tree generator.

```
package main;

sub main : int () {
  my $num1 : int = 2;
  my $num2 : int = 5;
  
  my $num3 = sum($num1, $num3);
}

sub sum : int ($num1 : int, $num2 : int) {
  my $num3 : int;
  
  $num3 = $num1 + $num1;
  
  return $num3;
}

```

## Run

    bison -p SPerl_yy -d sperly.y && gcc -O -o sperl sperly.tab.c sperl_main.c sperl_toke.c sperl_op.c && ./sperl test.spl

I only created syntax rule. This command ouput ruduce result.

## Source files

- **sperl.h** - Main header
- **sperl_parser.h** - Parser header
- **sperl_op.h** - Create OP tree header
- **sperl_main.c** - Program entry point
- **sperl_toke.c** - Tokenizer
- **sperly.y** - Parser generator
- **sperl_op.c** - Create OP tree
- **test.spl** - Test script
