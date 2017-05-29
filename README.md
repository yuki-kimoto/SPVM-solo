# SPVM - Fast calculation of number array, GC, static typing, VM with  perlish syntax

Do you need a **fast Perl**? Static Perl is a fast calculation system of Perl.

- **Fast calculation** - The Perl's biggest weak point is the calculation performance. Static Perl provides fast calculations.
- **GC** - You don't need to care about freeing memory
- **Static typing** - Static typing for performance
- **VM** - Byte codes are generated so that you can run them on Static Perl VM
- **Perlish syntax** - the syntax is very similar to that of Perl
- **Perl module** - Static Perl will be used from Perl itself as module(Not implemented).
- **No memory limitation**

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
    
    std::println_int($num3);
    
    return 0;
  }

  sub sum ($num1 : int, $num2 : int) : int {
    my $num3 = $num1 + $num2;
    
    return $num3;
  }
}
```

## Run

    make
    ./spvm Main

Output:


    7

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
