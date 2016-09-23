# Static Perl - Fast culculation, parallel process, GC, static typing, VM with  perlish syntax

Do you need **fast Perl**? Static Perl provide fast culculation system on Perl.

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

    bison -p SPerl_yy -d sperly.y && gcc -O -o sperl sperly.tab.c sperl_main.c sperl_toke.c sperl_op.c sperl_collection.c sperl_parser.c sperl_class.c sperl_global.c && ./sperl test.spl

Now I only print the result of **token reduction** and **abstract syntax tree**.

The above source code output:

```
[Token reduction]
WORD -> pkgname
PACKAGE pkgname ; -> statement
statement -> statements
WORD -> subname
WORD -> type (int)
type -> modiftype
NULL -> optsubargs
WORD -> type (int)
type -> modiftype
MY VAR : modiftype -> declvar
declvar -> term
INT(2) -> term
term ASSIGNOP term -> term
term ; -> statement
statement -> statements
WORD -> type (int)
type -> modiftype
MY VAR : modiftype -> declvar
declvar -> term
INT(5) -> term
term ASSIGNOP term -> term
term ; -> statement
statements statement -> statements
WORD -> type (int)
type -> modiftype
MY VAR : modiftype -> declvar
declvar -> term
WORD -> subname
VAR($num1) -> term
term -> terms
VAR($num3) -> term
terms , term -> terms
terms -> optterms
subname (optterms) -> term
term ASSIGNOP term -> term
term ; -> statement
statements statement -> statements
{ statements } -> block
SUB subname : modiftype ( optsubargs ) block -> statement
statements statement -> statements
WORD -> subname
WORD -> type (int)
type -> modiftype
WORD -> type (int)
type -> modiftype
VAR : modiftype -> subarg ($num1)
subarg -> subargs
WORD -> type (int)
type -> modiftype
VAR : modiftype -> subarg ($num2)
subargs , subarg
subargs -> optsubargs
WORD -> type (int)
type -> modiftype
MY VAR : modiftype -> declvar
declvar -> term
term ; -> statement
statement -> statements
VAR($num3) -> term
VAR($num1) -> term
VAR($num1) -> term
term + term -> term
term ASSIGNOP term -> term
term ; -> statement
statements statement -> statements
VAR($num3) -> term
RETURN term ; -> statement
statements statement -> statements
{ statements } -> block
SUB subname : modiftype ( optsubargs ) block -> statement
statements statement -> statements
statements -> grammar

[Abstract Syntax Tree]
grammer
 list
  statement
   package
    const_string "main"
  statement
   sub
    const_string "main"
    const_string "int"
    null
    block
     list
      statement
       assign
        my
         var "$num1"
         const_string "int"
        const_int 2
      statement
       assign
        my
         var "$num2"
         const_string "int"
        const_int 5
      statement
       assign
        my
         var "$num3"
         const_string "int"
        func
         const_string "sum"
         list
          var "$num1"
          var "$num3"
  statement
   sub
    const_string "sum"
    const_string "int"
    list
     subarg
      var "$num1"
      const_string "int"
     subarg
      var "$num2"
      const_string "int"
    block
     list
      statement
       my
        var "$num3"
        const_string "int"
      statement
       assign
        var "$num3"
        add
         var "$num1"
         var "$num1"
      statement
       return
        var "$num3"
```

## Source files

- **sperl.h** - Main header
- **sperl_parser.h** - Parser header
- **sperl_op.h** - Create OP tree header
- **sperl_main.c** - Program entry point
- **sperl_toke.c** - Tokenizer
- **sperly.y** - Parser generator
- **sperl_op.c** - Create OP tree
- **test.spl** - Test script

# Development

  gcc -O -o tmp_sperl_t_collection t/sperl_t_collection.c sperl_collection.c && ./tmp_sperl_t_collection

# SPVM specification

## Core type

core type is char, byte, short, int, long, float, double.

  char    unsinged integer        1byte
  byte    singed integer          1byte
  short   singed integer          2byte
  int     signed integer          4byte
  long    singed integer          8byte
  float   floating-point number   4byte
  double  floating-point number   8byte

## Class name

class name first character must be upper case. if class name contain "::", each part first character must be upper case.

  Foo
  Foo::Bar

