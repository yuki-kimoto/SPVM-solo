#ifndef SPERL_API_H
#define SPERL_API_H

void SPerl_API_init_env(SPerl* sperl);
void SPerl_API_call_sub(SPerl* sperl, const char* sub_base_name);

int64_t SPerl_API_get_array_ref_count(SPerl* sperl, void* array);
int64_t SPerl_API_get_array_length(SPerl* sperl, void* array);
int8_t* SPerl_API_get_byte_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
int16_t* SPerl_API_get_short_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
int32_t* SPerl_API_get_int_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
int64_t* SPerl_API_get_long_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
float* SPerl_API_get_float_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
double* SPerl_API_get_double_array_data(SPerl* sperl, SPerl_ENV* env, void* array);

int8_t SPerl_API_get_var_byte(SPerl* sperl, int64_t index);
int16_t SPerl_API_get_var_short(SPerl* sperl, int64_t index);
int32_t SPerl_API_get_var_int(SPerl* sperl, int64_t index);
int64_t SPerl_API_get_var_long(SPerl* sperl, int64_t index);
float SPerl_API_get_var_float(SPerl* sperl, int64_t index);
double SPerl_API_get_var_double(SPerl* sperl, int64_t index);
void* SPerl_API_get_var_ref(SPerl* sperl, int64_t index);

void SPerl_API_push_var_byte(SPerl* sperl, int8_t value);
void SPerl_API_push_var_short(SPerl* sperl, int16_t value);
void SPerl_API_push_var_int(SPerl* sperl, int32_t value);
void SPerl_API_push_var_long(SPerl* sperl, int64_t value);
void SPerl_API_push_var_float(SPerl* sperl, float value);
void SPerl_API_push_var_double(SPerl* sperl, double value);
void SPerl_API_push_var_ref(SPerl* sperl, void* value);

void SPerl_API_push_ret_byte(SPerl* sperl, int8_t value);
void SPerl_API_push_ret_short(SPerl* sperl, int16_t value);
void SPerl_API_push_ret_int(SPerl* sperl, int32_t value);
void SPerl_API_push_ret_long(SPerl* sperl, int64_t value);
void SPerl_API_push_ret_float(SPerl* sperl, float value);
void SPerl_API_push_ret_double(SPerl* sperl, double value);
void SPerl_API_push_ret_ref(SPerl* sperl, void* value);

int8_t SPerl_API_pop_ret_byte(SPerl* sperl);
int16_t SPerl_API_pop_ret_short(SPerl* sperl);
int32_t SPerl_API_pop_ret_int(SPerl* sperl);
int64_t SPerl_API_pop_ret_long(SPerl* sperl);
float SPerl_API_pop_ret_float(SPerl* sperl);
double SPerl_API_pop_ret_double(SPerl* sperl);
void* SPerl_API_pop_ret_ref(SPerl* sperl);

#endif
