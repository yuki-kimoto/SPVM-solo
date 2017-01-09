# Static Perl - Fast calculation, parallel processing, GC, static typing, VM with  perlish syntax

Do you need a **fast Perl**? Static Perl is a fast calculation system of Perl.

- **Fast calculation** - The Perl's biggest weak point is the calculation performance. Static Perl provides fast calculations.
- **Parallel processing** - Support parallel processing to manipulate arrays faster
- **GC** - You don't need to care about freeing memory
- **Static typing** - Static typing for performance
- **VM** - Byte codes are generated so that you can run them on Static Perl VM
- **Perlish syntax** - the syntax is very similar to that of Perl
- **Perl module** - Static Perl will be used from Perl itself as module.

This is now under **developing**.

I have implemented only the following parts.

1. tokenizer
1. abstract syntax tree builder
1. bytecode builder
1. some parts of virtual machine

```
package Main {

  sub main () : int {
    my $num1 = 2;
    my $num2 = 5;
    
    my $num3 = sum($num1, $num3);
  }

  sub sum ($num1 : int, $num2 : int) : int {
    my $num3 = $num1 + $num1;
    
    return $num3;
  }
}
```

## Run

    make && ./sperl Main

Now only AST and bytecodes are printed.

The output for the above source code will be

```
[Abstract Syntax Tree]
GRAMMAR
 LIST
  PUSHMARK
  DECL_PACKAGE
   NAME "Main"
   CLASS_BLOCK
    LIST
     PUSHMARK
     DECL_SUB
      NAME "main"
      LIST
       PUSHMARK
      TYPE
       NAME "int"
      BLOCK
       LIST
        PUSHMARK
        POP
         ASSIGN
          VAR "$num1"
           DECL_MY_VAR
            NULL
          CONSTANT INT 2
        POP
         ASSIGN
          VAR "$num2"
           DECL_MY_VAR
            NULL
          CONSTANT INT 5
        POP
         ASSIGN
          VAR "$num3"
           DECL_MY_VAR
            NULL
          CALL_SUB
           NAME "Main::sum"
           LIST
            PUSHMARK
            VAR "$num1"
            VAR "$num3"
     DECL_SUB
      NAME "sum"
      LIST
       PUSHMARK
       VAR "$num1"
        DECL_MY_VAR
         TYPE
          NAME "int"
       VAR "$num2"
        DECL_MY_VAR
         TYPE
          NAME "int"
      TYPE
       NAME "int"
      BLOCK
       LIST
        PUSHMARK
        POP
         ASSIGN
          VAR "$num3"
           DECL_MY_VAR
            NULL
          ADD
           VAR "$num1"
           VAR "$num1"
        RETURN
         VAR "$num3"

[Resolved types]
resolved_type[0]
    name => "boolean"
    id => "0"
resolved_type[1]
    name => "byte"
    id => "1"
resolved_type[2]
    name => "short"
    id => "2"
resolved_type[3]
    name => "int"
    id => "3"
resolved_type[4]
    name => "long"
    id => "4"
resolved_type[5]
    name => "float"
    id => "5"
resolved_type[6]
    name => "double"
    id => "6"
resolved_type[7]
    name => "boolean[]"
    id => "7"
resolved_type[8]
    name => "byte[]"
    id => "8"
resolved_type[9]
    name => "short[]"
    id => "9"
resolved_type[10]
    name => "int[]"
    id => "10"
resolved_type[11]
    name => "long[]"
    id => "11"
resolved_type[12]
    name => "float[]"
    id => "12"
resolved_type[13]
    name => "double[]"
    id => "13"
resolved_type[14]
    name => "Main"
    id => "14"

[Packages information]
package[0]
  name => "boolean"
  resolved_type => "boolean"
  size => 1
  fields
  constant_values
  constant_pool
  subs
package[1]
  name => "byte"
  resolved_type => "byte"
  size => 1
  fields
  constant_values
  constant_pool
  subs
package[2]
  name => "short"
  resolved_type => "short"
  size => 1
  fields
  constant_values
  constant_pool
  subs
package[3]
  name => "int"
  resolved_type => "int"
  size => 1
  fields
  constant_values
  constant_pool
  subs
package[4]
  name => "long"
  resolved_type => "long"
  size => 2
  fields
  constant_values
  constant_pool
  subs
package[5]
  name => "float"
  resolved_type => "float"
  size => 1
  fields
  constant_values
  constant_pool
  subs
package[6]
  name => "double"
  resolved_type => "double"
  size => 2
  fields
  constant_values
  constant_pool
  subs
package[7]
  name => "Main"
  resolved_type => "Main"
  size => 0
  fields
  constant_values
    constant[0]
      int 2
      pool_pos => 0
    constant[1]
      int 5
      pool_pos => 1
  constant_pool
    constant_pool[0] 2
    constant_pool[1] 5
  subs
    sub[0]
      package_name => "Main"
      name => "main"
      id => 0
      anon => 0
      resolved_type => "int"
      argument_count => 0
      my_vars
        my_var[0]
          name => "$num1"
          resolved_type => "int"
        my_var[1]
          name => "$num2"
          resolved_type => "int"
        my_var[2]
          name => "$num3"
          resolved_type => "int"
      op_block => 970a58
      bytecodes
        [0] ICONST_2
        [1] ISTORE_0
        [2] ICONST_5
        [3] ISTORE_1
        [4] ILOAD_0
        [5] ILOAD_2
        [6] INVOKESTATIC
        [7] 0
        [8] 1
        [9] ISTORE_2
    sub[1]
      package_name => "Main"
      name => "sum"
      id => 1
      anon => 0
      resolved_type => "int"
      argument_count => 2
      my_vars
        my_var[0]
          name => "$num1"
          resolved_type => "int"
        my_var[1]
          name => "$num2"
          resolved_type => "int"
        my_var[2]
          name => "$num3"
          resolved_type => "int"
      op_block => 971970
      bytecodes
        [0] ILOAD_0
        [1] ILOAD_0
        [2] IADD
        [3] ISTORE_2
        [4] ILOAD_2
        [5] IRETURN
```

# Development

## Run
    
    make && ./sperl Test

## Test
    
    make test

## Types

Use the following types in source codes. This is defined in the standard header `<stdint.h>`.

    _Bool
    uint8_t
    int8_t
    int16_t
    int32_t
    int64_t
    float
    double

# SPVM specification

## Core type

Core types are `byte`, `short`, `int`, `long`, `float`, `double`.

    byte    signed integer          1byte
    short   signed integer          2byte
    int     signed integer          4byte
    long    signed integer          8byte
    float   floating-point number   4byte
    double  floating-point number   8byte

Calculation and type convertion rules are same as Java.

## Package name

Package name is a conbination of alphabets, numbers, and "::". Numbers should not appear as the first character.
    
    # OK
    Foo
    Foo::Bar
    Foo1::Bar1
    
    # Not OK
    1Foo
    Foo::2Bar

# Contributors

* [akinomyoga](https://github.com/akinomyoga) (Koichi Murase)
* [NAGAYASU Shinya](https://github.com/nagayasu-shinya)

# Caution

Static Perl will be renamed because the same name project "static perl" already exists.
