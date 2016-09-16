#ifndef SPERL_H
#define SPERL_H

#include <stdint.h>

// Field information
struct SPerl_field_info;
typedef struct SPerl_field_info SPerl_FIELD_INFO;
struct SPerl_field_info {
  int8_t* name;
  int16_t desc_flag;
};

// Method information
struct SPerl_method_info;
typedef struct SPerl_method_info SPerl_METHOD_INFO;
struct SPerl_METHOD_INFO {
  int8_t* name;
  int16_t desc_flag;
};

// Class information
struct SPerl_class_info;
typedef struct SPerl_class_info SPerl_CLASS_INFO;
struct SPerl_class_info {
  int8_t* name;
  int16_t desc_flag;
  int8_t* super_class_name;
  SPerl_FIELD_INFO* fields;
  SPerl_METHOD_INFO* methods;
};

#endif
