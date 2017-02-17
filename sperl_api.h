#ifndef SPERL_API_H
#define SPERL_API_H

int64_t SPerl_API_get_array_length(SPerl* sperl, intptr_t array);
int8_t* SPerl_API_get_byte_array_data(SPerl* sperl, intptr_t array);
int16_t* SPerl_API_get_short_array_data(SPerl* sperl, intptr_t array);
int32_t* SPerl_API_get_int_array_data(SPerl* sperl, intptr_t array);
int64_t* SPerl_API_get_long_array_data(SPerl* sperl, intptr_t array);
float* SPerl_API_get_float_array_data(SPerl* sperl, intptr_t array);
double* SPerl_API_get_double_array_data(SPerl* sperl, intptr_t array);

#endif
