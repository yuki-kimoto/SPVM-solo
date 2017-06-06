#ifndef SPVM_DATA_H
#define SPVM_DATA_H

enum {
  SPVM_DATA_C_HEADER_SIZE = 32
};

struct SPVM_data {
  int8_t type;
  int32_t ref_count;
};

enum {
  SPVM_DATA_C_TYPE_OBJECT,
  SPVM_DATA_C_TYPE_STRING,
  SPVM_DATA_C_TYPE_ARRAY,
};

#endif
