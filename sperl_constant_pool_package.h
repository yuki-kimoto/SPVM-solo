#ifndef SPERL_CONSTANT_POOL_PACKAGE_H
#define SPERL_CONSTANT_POOL_PACKAGE_H

#include "sperl_base.h"

// Array
struct SPerl_constant_pool_package {
  int32_t name_constant_pool_address;
  int32_t byte_size;
};

#endif
