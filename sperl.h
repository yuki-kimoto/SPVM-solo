#ifndef SPERL_H
#define SPERL_H

#include <stdint.h>

// Field
struct SPerl_field_info;
typedef struct SPerl_field_info SPerl_FIELD_INFO;
struct SPerl_field_info {
  int8_t* name;
};

// Method
struct SPerl_method_info;
typedef struct SPerl_method_info SPerl_METHOD_INFO;
struct SPerl_METHOD_INFO {
  int8_t* name;
};

struct SPerl_class_info;
typedef struct SPerl_class_info SPerl_CLASS_INFO;
struct SPerl_class_info {
  int8_t* name;
  int16_t attr_flag;
  int8_t* super_class_name;
  SPerl_FIELD_INFO* fields;
  SPerl_METHOD_INFO* methods;
};

#endif
