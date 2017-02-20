#ifndef SPERL_API_H
#define SPERL_API_H

int64_t SPerl_API_get_array_length(SPerl* sperl, intptr_t array);
int8_t* SPerl_API_get_byte_array_data(SPerl* sperl, intptr_t array);
int16_t* SPerl_API_get_short_array_data(SPerl* sperl, intptr_t array);
int32_t* SPerl_API_get_int_array_data(SPerl* sperl, intptr_t array);
int64_t* SPerl_API_get_long_array_data(SPerl* sperl, intptr_t array);
float* SPerl_API_get_float_array_data(SPerl* sperl, intptr_t array);
double* SPerl_API_get_double_array_data(SPerl* sperl, intptr_t array);

int8_t SPerl_API_get_var_byte(SPerl* sperl, int64_t index);
int16_t SPerl_API_get_var_short(SPerl* sperl, int64_t index);
int32_t SPerl_API_get_var_int(SPerl* sperl, int64_t index);
int64_t SPerl_API_get_var_long(SPerl* sperl, int64_t index);
float SPerl_API_get_var_float(SPerl* sperl, int64_t index);
double SPerl_API_get_var_double(SPerl* sperl, int64_t index);
intptr_t SPerl_API_get_var_address(SPerl* sperl, int64_t index);

#endif
