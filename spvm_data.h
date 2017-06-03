#ifndef SPVM_DATA_H
#define SPVM_DATA_H

struct SPVM_data {
  int32_t data_count;
  int8_t type;
};

enum {
  SPVM_DATA_C_TYPE_OBJECT,
  SPVM_DATA_C_TYPE_STRING,
  SPVM_DATA_C_TYPE_ARRAY,
};

#endif
