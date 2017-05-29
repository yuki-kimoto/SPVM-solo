# Static Perl - Fast calculation, parallel processing, GC, static typing, VM with  perlish syntax

Do you need a **fast Perl**? Static Perl is a fast calculation system of Perl.

- **Fast calculation** - The Perl's biggest weak point is the calculation performance. Static Perl provides fast calculations.
- **GC** - You don't need to care about freeing memory
- **Static typing** - Static typing for performance
- **VM** - Byte codes are generated so that you can run them on Static Perl VM
- **Perlish syntax** - the syntax is very similar to that of Perl
- **Perl module** - Static Perl will be used from Perl itself as module.
- No memory limitation

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
    
    my $num3 = sum($num1, $num2);
    
    printi($num3);
    
    return 0;
  }

  sub sum ($num1 : int, $num2 : int) : int {
    my $num3 = $num1 + $num2;
    
    return $num3;
  }
}
```

## Run

    make && ./spvm Main

Now only AST and bytecodes are printed.

The output for the above source code will be

```
[Abstract Syntax Tree]
GRAMMAR
 LIST
  PUSHMARK
  DECL_PACKAGE
   NAME "CORE"
   CLASS_BLOCK
    LIST
     PUSHMARK
     DECL_SUB
      NAME "printi"
      LIST
       PUSHMARK
       VAR "$value"
        DECL_MY_VAR
         TYPE
          NAME "int"
      LIST
       PUSHMARK
       IF
      TYPE
       NAME "int"
      BLOCK
       LIST
        PUSHMARK
     DECL_SUB
      NAME "printl"
      LIST
       PUSHMARK
       VAR "$value"
        DECL_MY_VAR
         TYPE
          NAME "long"
      LIST
       PUSHMARK
       IF
      TYPE
       NAME "int"
      BLOCK
       LIST
        PUSHMARK
     DECL_SUB
      NAME "printf"
      LIST
       PUSHMARK
       VAR "$value"
        DECL_MY_VAR
         TYPE
          NAME "float"
      LIST
       PUSHMARK
       IF
      TYPE
       NAME "int"
      BLOCK
       LIST
        PUSHMARK
     DECL_SUB
      NAME "printd"
      LIST
       PUSHMARK
       VAR "$value"
        DECL_MY_VAR
         TYPE
          NAME "double"
      LIST
       PUSHMARK
       IF
      TYPE
       NAME "int"
      BLOCK
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
          CONSTANT INT 2 (address 0)
        POP
         ASSIGN
          VAR "$num2"
           DECL_MY_VAR
            NULL
          CONSTANT INT 5 (address 0)
        POP
         ASSIGN
          VAR "$num3"
           DECL_MY_VAR
            NULL
          CALL_SUB
           NAME "sum"
           LIST
            PUSHMARK
            VAR "$num1"
            VAR "$num2"
        POP
         CALL_SUB
          NAME "printi"
          LIST
           PUSHMARK
           VAR "$num3"
        RETURN
         CONSTANT INT 0 (address 0)
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
      LIST
       PUSHMARK
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
           VAR "$num2"
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
    name => "CORE"
    id => "14"
resolved_type[15]
    name => "Main"
    id => "15"

[Packages]
package[0]
  name => "boolean"
  resolved_type => "boolean"
  size => 1
  fields
package[1]
  name => "byte"
  resolved_type => "byte"
  size => 1
  fields
package[2]
  name => "short"
  resolved_type => "short"
  size => 1
  fields
package[3]
  name => "int"
  resolved_type => "int"
  size => 1
  fields
package[4]
  name => "long"
  resolved_type => "long"
  size => 2
  fields
package[5]
  name => "float"
  resolved_type => "float"
  size => 1
  fields
package[6]
  name => "double"
  resolved_type => "double"
  size => 2
  fields
package[7]
  name => "CORE"
  resolved_type => "CORE"
  size => 0
  fields
package[8]
  name => "Main"
  resolved_type => "Main"
  size => 0
  fields

[Constant pool]

[Subroutine]
  sub[0]
    package_name => "CORE"
    name => "printi"
    id => 0
    anon => 0
    resolved_type => "int"
    args
      arg[0]
          name => "$value"
          resolved_type => "int"
    my_vars
      my_var[0]
          name => "$value"
          resolved_type => "int"
    operand_stack_max => 32
    my_vars_size => 1
    bytecode_array
  sub[1]
    package_name => "CORE"
    name => "printl"
    id => 1
    anon => 0
    resolved_type => "int"
    args
      arg[0]
          name => "$value"
          resolved_type => "long"
    my_vars
      my_var[0]
          name => "$value"
          resolved_type => "long"
    operand_stack_max => 32
    my_vars_size => 2
    bytecode_array
  sub[2]
    package_name => "CORE"
    name => "printf"
    id => 2
    anon => 0
    resolved_type => "int"
    args
      arg[0]
          name => "$value"
          resolved_type => "float"
    my_vars
      my_var[0]
          name => "$value"
          resolved_type => "float"
    operand_stack_max => 32
    my_vars_size => 1
    bytecode_array
  sub[3]
    package_name => "CORE"
    name => "printd"
    id => 3
    anon => 0
    resolved_type => "int"
    args
      arg[0]
          name => "$value"
          resolved_type => "double"
    my_vars
      my_var[0]
          name => "$value"
          resolved_type => "double"
    operand_stack_max => 32
    my_vars_size => 2
    bytecode_array
  sub[4]
    package_name => "Main"
    name => "main"
    id => 4
    anon => 0
    resolved_type => "int"
    args
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
    operand_stack_max => 84
    my_vars_size => 3
    bytecode_array
      [0] ICONST_2
      [1] ISTORE_0
      [2] ICONST_5
      [3] ISTORE_1
      [4] ILOAD_0
      [5] ILOAD_1
      [6] CALLSUB
      [7] 0
      [8] 0
      [9] 0
      [10] 5
      [11] ISTORE_2
      [12] ILOAD_2
      [13] CALLSUB
      [14] 0
      [15] 0
      [16] 0
      [17] 0
      [18] POP
      [19] ICONST_0
      [20] IRETURN
  sub[5]
    package_name => "Main"
    name => "sum"
    id => 5
    anon => 0
    resolved_type => "int"
    args
      arg[0]
          name => "$num1"
          resolved_type => "int"
      arg[1]
          name => "$num2"
          resolved_type => "int"
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
    operand_stack_max => 58
    my_vars_size => 3
    bytecode_array
      [21] ILOAD_0
      [22] ILOAD_1
      [23] IADD
      [24] ISTORE_2
      [25] ILOAD_2
      [26] IRETURN
TEST: 7
TEST return_value: 0
```

# Development

## Run
    
    make DEFINE=-DDEBUG  && ./spvm Test

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

# Limitation

- Array can't have object except for string
- Object can't have object except for array and string

# SPVM specification

## Type

### Value type

Value types are `byte`, `short`, `int`, `long`, `float`, `double`.

    byte    signed integer          1byte
    short   signed integer          2byte
    int     signed integer          4byte
    long    signed integer          8byte
    float   floating-point number   4byte
    double  floating-point number   8byte

Declaration

    my $value : byte;
    my $value : short;
    my $value : int;
    my $value : long;
    my $value : float;
    my $value : double;

### Reference type

Reference types are String type, Array type, Object type.

**String type**

    string

Declaration
    
    my $message : string;

**Array type**

    byte[]   byte array
    short[]  short array
    int[]    int array array
    long[]   long array
    doube[]  double array
    string[] string array

Declaration

    my $values : byte[];
    my $values : short[];
    my $values : int[];
    my $values : long[];
    my $values : float[];
    my $values : double[];
    
**Object type**

    ClassName

Declaration

    my $object : ClassName;

## Type inference

If the type of right value is known, the type of left value is automatically decided.
    
    # Type of $value2 is byte.
    my $value1 : byte;
    my $value2 = $value1;
    
    # Type of $values2 is int[]
    my $values1 = new int[3];
    my $values2 = $values1;
    
    # Type of $object2 is ClassName
    my $object1 = new ClassName
    my $object2 = $object1;

## Constant

### Constant type

Type of constant default integral value is `int`.
    
    # int type
    1;
    3;

Type of constant default floating-point value is `double`.

    # double
    1.2
    5.3
    
Type of constant is specified by type specifier.
    
    # long
    3L
    
    # float
    3.2f
    
    # double
    3.2d

## Name

### Package name

Package name is a conbination of alphabets, numbers, and `::`. Numbers should not appear as the first character. `_` can't be used in class name.
    
    # OK
    Foo
    Foo::Bar
    Foo1::Bar1
    
    # Not OK
    1Foo
    Foo::2Bar
    Foo_Bar;

### Subroutine name

Subroutine name is a conbination of alphabets, numbers, and `_` separaters. continual `_`(For example `__`) can't be used in subroutine name.

    # OK
    foo
    foo1
    foo_bar
    
    # Not OK
    1foo
    foo__bar

### Field name

Field name is a conbination of alphabets, numbers, and `_` separaters. continual `_`(For example `__`) can't be used in field name.

    # OK
    foo
    foo1
    foo_bar
    
    # Not OK
    1foo
    foo__bar

### Absolute name

Absolute name is combination of package name and subroutine name, or package name and field name.

    ClassName1::foo
    ClassName1::ClassName2::foo_bar

### Coresponding C name

C name is corresponding to SPVM name, 1 by 1.

    # SPVM name
    ClassName1::foo
    ClassName1::ClassName2::foo_bar
    
    # C name
    ClassName1__foo
    ClassName1__ClassName2__foo_bar

Array type

    # SPVM name
    byte[]
    short[]
    int[]
    long[]
    doube[]
    
    # C name
    byte__array
    short__array
    int__array
    long__array
    doube__array

# Contributors

* [akinomyoga](https://github.com/akinomyoga) (Koichi Murase)
* [NAGAYASU Shinya](https://github.com/nagayasu-shinya)

# Caution

Static Perl will be renamed because the same name project "static perl" already exists.
