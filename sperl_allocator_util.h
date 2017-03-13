#ifndef SPERL_ALLOCATOR_UTIL_H
#define SPERL_ALLOCATOR_UTIL_H

void SPerl_ALLOCATOR_UTIL_exit_with_malloc_failure();
void* SPerl_ALLOCATOR_UTIL_safe_malloc(int64_t count, int64_t size);
void* SPerl_ALLOCATOR_UTIL_safe_malloc_zero(int64_t count, int64_t size);
void* SPerl_ALLOCATOR_UTIL_safe_realloc(void* ptr, int64_t count, int64_t size);

#endif
