#ifndef SPVM_DATA_ARRAY_H
#define SPVM_DATA_ARRAY_H

struct SPVM_data_array {
  int8_t type;
  int16_t byte_size;
  int32_t ref_count;
  int32_t length;
};

#endif
