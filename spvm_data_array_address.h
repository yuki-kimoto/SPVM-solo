#ifndef SPVM_DATA_ARRAY_ADDRESS_H
#define SPVM_DATA_ARRAY_ADDRESS_H

struct SPVM_data_array_address {
  int8_t type;
  int16_t byte_size;
  int32_t ref_count;
  int32_t length;
  void* values;
};

#endif
