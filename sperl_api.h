#ifndef SPERL_API_H
#define SPERL_API_H

void SPerl_API_init_env(SPerl* sperl);
void SPerl_API_call_sub(SPerl* sperl, SPerl_ENV* env, const char* sub_base_name);

int64_t SPerl_API_get_array_ref_count(SPerl* sperl, SPerl_ENV* env, void* array);
int64_t SPerl_API_get_array_length(SPerl* sperl, SPerl_ENV* env, void* array);
int8_t* SPerl_API_get_byte_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
int16_t* SPerl_API_get_short_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
int32_t* SPerl_API_get_int_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
int64_t* SPerl_API_get_long_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
float* SPerl_API_get_float_array_data(SPerl* sperl, SPerl_ENV* env, void* array);
double* SPerl_API_get_double_array_data(SPerl* sperl, SPerl_ENV* env, void* array);

int8_t SPerl_API_get_var_byte(SPerl* sperl, SPerl_ENV* env, int64_t index);
int16_t SPerl_API_get_var_short(SPerl* sperl, SPerl_ENV* env, int64_t index);
int32_t SPerl_API_get_var_int(SPerl* sperl, SPerl_ENV* env, int64_t index);
int64_t SPerl_API_get_var_long(SPerl* sperl, SPerl_ENV* env, int64_t index);
float SPerl_API_get_var_float(SPerl* sperl, SPerl_ENV* env, int64_t index);
double SPerl_API_get_var_double(SPerl* sperl, SPerl_ENV* env, int64_t index);
void* SPerl_API_get_var_ref(SPerl* sperl, SPerl_ENV* env, int64_t index);

void SPerl_API_push_var_byte(SPerl* sperl, SPerl_ENV* env, int8_t value);
void SPerl_API_push_var_short(SPerl* sperl, SPerl_ENV* env, int16_t value);
void SPerl_API_push_var_int(SPerl* sperl, SPerl_ENV* env, int32_t value);
void SPerl_API_push_var_long(SPerl* sperl, SPerl_ENV* env, int64_t value);
void SPerl_API_push_var_float(SPerl* sperl, SPerl_ENV* env, float value);
void SPerl_API_push_var_double(SPerl* sperl, SPerl_ENV* env, double value);
void SPerl_API_push_var_ref(SPerl* sperl, SPerl_ENV* env, void* value);

void SPerl_API_push_ret_byte(SPerl* sperl, SPerl_ENV* env, int8_t value);
void SPerl_API_push_ret_short(SPerl* sperl, SPerl_ENV* env, int16_t value);
void SPerl_API_push_ret_int(SPerl* sperl, SPerl_ENV* env, int32_t value);
void SPerl_API_push_ret_long(SPerl* sperl, SPerl_ENV* env, int64_t value);
void SPerl_API_push_ret_float(SPerl* sperl, SPerl_ENV* env, float value);
void SPerl_API_push_ret_double(SPerl* sperl, SPerl_ENV* env, double value);
void SPerl_API_push_ret_ref(SPerl* sperl, SPerl_ENV* env, void* value);

int8_t SPerl_API_pop_ret_byte(SPerl* sperl, SPerl_ENV* env);
int16_t SPerl_API_pop_ret_short(SPerl* sperl, SPerl_ENV* env);
int32_t SPerl_API_pop_ret_int(SPerl* sperl, SPerl_ENV* env);
int64_t SPerl_API_pop_ret_long(SPerl* sperl, SPerl_ENV* env);
float SPerl_API_pop_ret_float(SPerl* sperl, SPerl_ENV* env);
double SPerl_API_pop_ret_double(SPerl* sperl, SPerl_ENV* env);
void* SPerl_API_pop_ret_ref(SPerl* sperl, SPerl_ENV* env);

#endif
