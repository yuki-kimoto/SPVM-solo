#ifndef SPVM_DATA_ARRAY_DOUBLE_H
#define SPVM_DATA_ARRAY_DOUBLE_H

struct SPVM_data_array_double {
  int8_t type;
  int16_t byte_size;
  int32_t ref_count;
  int32_t length;
  double* values;
};

#endif
