#ifndef SPVM_DATA_API_H
#define SPVM_DATA_API_H

struct SPVM_data_array;
typedef struct SPVM_data_array SPVM_DATA_ARRAY;

struct SPVM_data;
typedef struct SPVM_data SPVM_DATA;

struct SPVM_data_string;
typedef struct SPVM_data_string SPVM_DATA_STRING;

struct SPVM_data_object;
typedef struct SPVM_data_object SPVM_DATA_OBJECT;

// SPVM_DATA
enum {
  SPVM_DATA_C_HEADER_BYTE_SIZE = 32
};

enum {
  SPVM_DATA_C_TYPE_OBJECT = 0,
  SPVM_DATA_C_TYPE_STRING = 1,
  SPVM_DATA_C_TYPE_ARRAY = 2,
};

struct SPVM_data {
  int32_t* constant_pool;
  int8_t type;
  int32_t ref_count;
};

// SPVM_DATA_ARRAY
struct SPVM_data_array {
  int32_t* constant_pool;
  int8_t type;
  int8_t value_type;
  int32_t ref_count;
  int32_t length;
};

enum {
  SPVM_DATA_ARRAY_C_VALUE_TYPE_BYTE = 0,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_SHORT = 1,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_INT = 2,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_LONG = 3,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_FLOAT = 4,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_DOUBLE = 5,
  SPVM_DATA_ARRAY_C_VALUE_TYPE_REF = 6,
};

extern const int32_t SPVM_DATA_ARRAY_C_VALUE_SIZES[];

// SPVM_DATA_OBJECT
struct SPVM_data_object {
  int32_t* constant_pool;
  int8_t type;
  int32_t ref_count;
  int32_t package_constant_pool_address;
  int32_t field_name_indexes_constant_pool_address;
};

// SPVM_DATA_STRING
struct SPVM_data_string {
  int32_t* constant_pool;
  int8_t type;
  int32_t ref_count;
};

SPVM_SV* SPVM_DATA_API_get_string_sv(SPVM_DATA_STRING* data_string);
int32_t* SPVM_DATA_API_get_constant_pool(SPVM_DATA* data);
int32_t SPVM_DATA_API_get_ref_count(SPVM_DATA* data);
int32_t SPVM_DATA_API_get_array_length(SPVM_DATA_ARRAY* data_array);
int8_t* SPVM_DATA_API_get_array_values_byte(SPVM_DATA_ARRAY* data_array);
int16_t* SPVM_DATA_API_get_array_values_short(SPVM_DATA_ARRAY* data_array);
int32_t* SPVM_DATA_API_get_array_values_int(SPVM_DATA_ARRAY* data_array);
int64_t* SPVM_DATA_API_get_array_values_long(SPVM_DATA_ARRAY* data_array);
float* SPVM_DATA_API_get_array_values_float(SPVM_DATA_ARRAY* data_array);
double* SPVM_DATA_API_get_array_values_double(SPVM_DATA_ARRAY* data_array);

int32_t SPVM_DATA_API_get_object_fields_length(SPVM_DATA_OBJECT* data_object);
int32_t SPVM_DATA_API_dump_object_field_names(SPVM_DATA_OBJECT* data_object);

#endif
