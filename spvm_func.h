#ifndef SPVM_FUNC_H
#define SPVM_FUNC_H

#include "spvm_base.h"

void SPVM_FUNC_std_test_call1(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_test_call2(SPVM* spvm, SPVM_ENV* env);

void SPVM_FUNC_std_println(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_printb(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_prints(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_printi(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_printl(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_printf(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_printd(SPVM* spvm, SPVM_ENV* env);
void SPVM_FUNC_std_suml(SPVM* spvm, SPVM_ENV* env);

#endif
