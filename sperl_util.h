#ifndef SPERL_UTIL_H
#define SPERL_UTIL_H

#include <stddef.h>
#include "sperl_base.h"

void SPerl_UTIL_exit_with_malloc_failure(SPerl* sperl);
void* SPerl_UTIL_safe_malloc(SPerl* sperl, size_t count, size_t size);
void* SPerl_UTIL_safe_malloc_zero(SPerl* sperl, size_t count, size_t size);
void* SPerl_UTIL_safe_realloc(SPerl* sperl, void* ptr, size_t count, size_t size);

#endif
