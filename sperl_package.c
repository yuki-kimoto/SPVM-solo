#include "sperl_package.h"
#include "sperl.h"
#include "sperl_allocator.h"
#include "sperl_constant_pool.h"

SPerl_PACKAGE* SPerl_PACKAGE_new(SPerl* sperl) {
  SPerl_PACKAGE* package = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_PACKAGE));
  
  package->op_fields = SPerl_ALLOCATOR_new_array(sperl, 0);
  
  return package;
}
