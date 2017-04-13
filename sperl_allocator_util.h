#ifndef SPERL_ALLOCATOR_UTIL_H
#define SPERL_ALLOCATOR_UTIL_H

void* SPerl_ALLOCATOR_UTIL_safe_malloc(int32_t count, int32_t size);
void* SPerl_ALLOCATOR_UTIL_safe_realloc(void* ptr, int32_t count, int32_t size);

#endif
