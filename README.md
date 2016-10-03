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
package Main {

  sub main () : int {
    my $num1 : int = 2;
    my $num2 : int = 5;

    my $num3 : int = sum($num1, $num3);
  }

  sub sum ($num1 : int, $num2 : int) : int {
    my $num3 : int;

    $num3 = $num1 + $num1;

    return $num3;
  }
}
```

## Run

    bison -p SPerl_yy -d sperly.y && gcc -O -o sperl *.c && ./sperl test.spl

Now I only print the result of **token reduction** and **abstract syntax tree**.

The above source code output:

```
[Token reduction]
WORD -> pkgname
WORD -> subname
NULL -> optsubargs
WORD -> type (int)
type -> desctype
WORD -> type (int)
type -> desctype
MY VAR : desctype -> declvar
declvar -> term
CONST(int 2) -> term
term ASSIGNOP term -> term
term ; -> statement
statement -> statements
WORD -> type (int)
type -> desctype
MY VAR : desctype -> declvar
declvar -> term
CONST(int 5) -> term
term ASSIGNOP term -> term
term ; -> statement
statements statement -> statements
WORD -> type (int)
type -> desctype
MY VAR : desctype -> declvar
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
SUB subname ( optsubargs ) : desctype block -> statement
statement -> statements
WORD -> subname
WORD -> type (int)
type -> desctype
VAR : desctype -> subarg ($num1)
subarg -> subargs
WORD -> type (int)
type -> desctype
VAR : desctype -> subarg ($num2)
subargs , subarg
subargs -> optsubargs
WORD -> type (int)
type -> desctype
WORD -> type (int)
type -> desctype
MY VAR : desctype -> declvar
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
SUB subname ( optsubargs ) : desctype block -> statement
statements statement -> statements
{ statements } -> block
PACKAGE pkgname block -> package
pacakge -> packages
packages -> grammar

[Abstract Syntax Tree]
grammer
 package
  const string "Main"
  block
   sub
    const string "main"
    null
    const string "int"
    block
     assign
      my
       var "$num1"
       const string "int"
      const int 2
     assign
      my
       var "$num2"
       const string "int"
      const int 5
     assign
      my
       var "$num3"
       const string "int"
      callsub
       const string "sum"
       list
        var "$num1"
        var "$num3"
   sub
    const string "sum"
    list
     subarg
      var "$num1"
      const string "int"
     subarg
      var "$num2"
      const string "int"
    const string "int"
    block
     my
      var "$num3"
      const string "int"
     assign
      var "$num3"
      add
       var "$num1"
       var "$num1"
     return
      var "$num3"

[Method infomation]
[0]name => main, argument_count => 0, return_type => int, op_block => 1b731c10
[1]name => sum, argument_count => 2, return_type => int, op_block => 1b732280
```

# Development

## Test

  gcc -O -o tmp_sperl_t_array t/sperl_t_array.c sperl_array.c && ./tmp_sperl_t_array
  gcc -O -o tmp_sperl_t_hash t/sperl_t_hash.c sperl_hash.c && ./tmp_sperl_t_hash
  gcc -O -o tmp_sperl_t_allocator t/sperl_t_allocator.c sperl_allocator.c sperl_array.c && ./tmp_sperl_t_allocator

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
  ABC
  ABC::DEF
