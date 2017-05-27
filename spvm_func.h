#ifndef SPVM_FUNC_H
#define SPVM_FUNC_H

#include "spvm_base.h"

void SPVM_FUNC_std_test_call1(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_test_call2(SPVM* spvm, SPVM_ENV* env);

void SPVM_FUNC_std_println(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_print_byte(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_print_short(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_print_int(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_print_long(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_print_float(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_print_double(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_sum_int(SPVM* spvm, SPVM_ENV* env);

#endif
