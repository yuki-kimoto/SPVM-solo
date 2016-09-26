#ifndef SPERL_CONST_INFO_H
#define SPERL_CONST_INFO_H

struct SPerl_const_info {
  SPerl_char* val_str;
  SPerl_char type;
  union {
    SPerl_boolean boolean_value;
    SPerl_char char_value;
    SPerl_byte byte_value;
    SPerl_short short_value;
    SPerl_int int_value;
    SPerl_int long_value;
    SPerl_float float_value;
    SPerl_double double_value;
  } uv;
};

#endif
