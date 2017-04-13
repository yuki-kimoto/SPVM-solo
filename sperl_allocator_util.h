#ifndef SPERL_ALLOCATOR_UTIL_H
#define SPERL_ALLOCATOR_UTIL_H

void* SPerl_ALLOCATOR_UTIL_safe_malloc(size_t count, size_t size);
void* SPerl_ALLOCATOR_UTIL_safe_realloc(void* ptr, size_t count, size_t size);

#endif
