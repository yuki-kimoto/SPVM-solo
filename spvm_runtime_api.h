#ifndef SPVM_RUNTIME_API_API_H
#define SPVM_RUNTIME_API_API_H

#include "spvm_base.h"
#include "spvm_data_api.h"

int64_t SPVM_RUNTIME_API_calcurate_data_byte_size(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA* data);

void SPVM_RUNTIME_API_dec_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA* data);
void SPVM_RUNTIME_API_inc_ref_count(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA* data);

SPVM_SV* SPVM_RUNTIME_API_get_string_sv(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA_STRING* string);

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

char* SPVM_RUNTIME_API_get_string_value(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_DATA_STRING* data_string);

void* SPVM_RUNTIME_API_create_string_sv(SPVM* spvm, SPVM_RUNTIME* runtime, SPVM_SV* sv);
SPVM_DATA_STRING* SPVM_RUNTIME_API_create_data_string_from_pv(SPVM* spvm, SPVM_RUNTIME* runtime, const char* pv);

#endif
