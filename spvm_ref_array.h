#ifndef SPVM_REF_ARRAY_H
#define SPVM_REF_ARRAY_H

struct SPVM_ref_array {
  int8_t type;
  int8_t sub_type;
  int16_t byte_size;
  int32_t ref_count;
  int32_t length;
};

#endif
