#ifndef SPVM_DATA_H
#define SPVM_DATA_H

struct SPVM_ref {
  int32_t ref_count;
  int8_t type;
};

enum {
  SPVM_REF_C_TYPE_OBJECT,
  SPVM_REF_C_TYPE_STRING,
  SPVM_REF_C_TYPE_ARRAY,
};

#endif
