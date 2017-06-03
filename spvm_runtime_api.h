#ifndef SPVM_RUNTIME_API_API_H
#define SPVM_RUNTIME_API_API_H

void SPVM_RUNTIME_API_dec_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF* ref);
void SPVM_RUNTIME_API_inc_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF* ref);

int32_t SPVM_RUNTIME_API_get_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF* ref);
int32_t SPVM_RUNTIME_API_get_array_length(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF_ARRAY* array);

int8_t* SPVM_RUNTIME_API_get_array_byte_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address);
int16_t* SPVM_RUNTIME_API_get_array_short_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address);
int32_t* SPVM_RUNTIME_API_get_array_int_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address);
int64_t* SPVM_RUNTIME_API_get_array_long_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address);
float* SPVM_RUNTIME_API_get_array_float_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address);
double* SPVM_RUNTIME_API_get_array_double_values(SPVM* spvm, SPVM_RUNTIME* runtime, void* address);

SPVM_SV* SPVM_RUNTIME_API_get_string_sv(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF_STRING* string);

int8_t SPVM_RUNTIME_API_get_var_byte(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index);
int16_t SPVM_RUNTIME_API_get_var_short(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index);
int32_t SPVM_RUNTIME_API_get_var_int(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index);
int64_t SPVM_RUNTIME_API_get_var_long(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index);
float SPVM_RUNTIME_API_get_var_float(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index);
double SPVM_RUNTIME_API_get_var_double(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index);
void* SPVM_RUNTIME_API_get_var_address(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t index);

void SPVM_RUNTIME_API_push_var_byte(SPVM* spvm, SPVM_RUNTIME* runtime, int8_t value);
void SPVM_RUNTIME_API_push_var_short(SPVM* spvm, SPVM_RUNTIME* runtime, int16_t value);
void SPVM_RUNTIME_API_push_var_int(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t value);
void SPVM_RUNTIME_API_push_var_long(SPVM* spvm, SPVM_RUNTIME* runtime, int64_t value);
void SPVM_RUNTIME_API_push_var_float(SPVM* spvm, SPVM_RUNTIME* runtime, float value);
void SPVM_RUNTIME_API_push_var_double(SPVM* spvm, SPVM_RUNTIME* runtime, double value);
void SPVM_RUNTIME_API_push_var_address(SPVM* spvm, SPVM_RUNTIME* runtime, void* value);

void SPVM_RUNTIME_API_push_return_value_byte(SPVM* spvm, SPVM_RUNTIME* runtime, int8_t value);
void SPVM_RUNTIME_API_push_return_value_short(SPVM* spvm, SPVM_RUNTIME* runtime, int16_t value);
void SPVM_RUNTIME_API_push_return_value_int(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t value);
void SPVM_RUNTIME_API_push_return_value_long(SPVM* spvm, SPVM_RUNTIME* runtime, int64_t value);
void SPVM_RUNTIME_API_push_return_value_float(SPVM* spvm, SPVM_RUNTIME* runtime, float value);
void SPVM_RUNTIME_API_push_return_value_double(SPVM* spvm, SPVM_RUNTIME* runtime, double value);
void SPVM_RUNTIME_API_push_return_value_address(SPVM* spvm, SPVM_RUNTIME* runtime, void* value);

int8_t SPVM_RUNTIME_API_pop_return_value_byte(SPVM* spvm, SPVM_RUNTIME* runtime);
int16_t SPVM_RUNTIME_API_pop_return_value_short(SPVM* spvm, SPVM_RUNTIME* runtime);
int32_t SPVM_RUNTIME_API_pop_return_value_int(SPVM* spvm, SPVM_RUNTIME* runtime);
int64_t SPVM_RUNTIME_API_pop_return_value_long(SPVM* spvm, SPVM_RUNTIME* runtime);
float SPVM_RUNTIME_API_pop_return_value_float(SPVM* spvm, SPVM_RUNTIME* runtime);
double SPVM_RUNTIME_API_pop_return_value_double(SPVM* spvm, SPVM_RUNTIME* runtime);
void* SPVM_RUNTIME_API_pop_return_value_address(SPVM* spvm, SPVM_RUNTIME* runtime);

char* SPVM_RUNTIME_API_get_string_value(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_REF_STRING* ref_string);

void* SPVM_RUNTIME_API_create_string_sv(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_SV* sv);
SPVM_REF_STRING* SPVM_RUNTIME_API_create_ref_string_from_pv(SPVM* spvm, SPVM_RUNTIME* runtime, const char* pv);

#endif
