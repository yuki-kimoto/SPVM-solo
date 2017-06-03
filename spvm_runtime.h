#ifndef SPVM_RUNTIME_H
#define SPVM_RUNTIME_H

struct SPVM_runtime {
  
  // Runtime memory allocator
  SPVM_RUNTIME_ALLOCATOR* allocator;

  // Call stack
  SPVM_VALUE* call_stack;
  
  // Call stack capacity
  int32_t call_stack_capacity;
  
  // Operand stack base
  int32_t call_stack_base;
  
  // Operand stack top
  int32_t operand_stack_top;

  // Constant pool
  int32_t* constant_pool;
  
  // Bytecodes
  uint8_t* bytecodes;
  
  // Exception
  _Bool abort;
};

SPVM_RUNTIME* SPVM_RUNTIME_init(SPVM* spvm, SPVM_RUNTIME* runtime);
SPVM_RUNTIME* SPVM_RUNTIME_new(SPVM* spvm);
void SPVM_RUNTIME_free(SPVM* spvm, SPVM_RUNTIME* runtime);

void SPVM_RUNTIME_call_sub(SPVM* spvm, SPVM_RUNTIME* runtime, int32_t sub_constant_pool_address);

#endif
