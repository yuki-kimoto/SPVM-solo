#ifndef SPerl_CONST_INFO_H
#define SPerl_CONST_INFO_H

enum SPerl_CONST_INFO_TYPE {
  SPerl_CONST_INFO_BOOLEAN,
  SPerl_CONST_INFO_CHAR,
  SPerl_CONST_INFO_BYTE,
  SPerl_CONST_INFO_SHORT,
  SPerl_CONST_INFO_INT,
  SPerl_CONST_INFO_LONG,
  SPerl_CONST_INFO_FLOAT,
  SPerl_CONST_INFO_DOUBLE,
  SPerl_CONST_INFO_STRING
};

struct SPerl_const_info {
  SPerl_char type;
  union {
    SPerl_boolean boolean_value;
    SPerl_char char_value;
    SPerl_byte byte_value;
    SPerl_short short_value;
    SPerl_int int_value;
    SPerl_int long_value;
    float float_value;
    SPerl_double double_value;
    SPerl_char* string_value;
  } uv;
  SPerl_int pool_pos;
};

SPerl_CONST_INFO* SPerl_CONST_INFO_new();

#endif
