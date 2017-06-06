#ifndef SPVM_DATA_ARRAY_H
#define SPVM_DATA_ARRAY_H

struct SPVM_data_array {
  int32_t* constant_pool;
  int8_t type;
  int8_t value_type;
  int32_t ref_count;
  int32_t length;
};

enum {
  SPVM_DATA_ARRAY_C_VALUE_TYPE_BYTE,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_SHORT,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_INT,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_LONG,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_FLOAT,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_DOUBLE,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_REF,
};

extern const int32_t SPVM_DATA_ARRAY_C_VALUE_SIZES[];

#endif
