#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_vm.h"
#include "sperl_allocator.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_bytecode_array.h"
#include "sperl_bytecode.h"
#include "sperl_sub.h"
#include "sperl_op.h"
#include "sperl_constant_pool.h"
#include "sperl_frame.h"
#include "sperl_package.h"
#include "sperl_heap.h"
#include "sperl_constant_pool_sub.h"
#include "sperl_constant_pool_package.h"
#include "sperl_constant_pool_field.h"
#include "sperl_resolved_type.h"

enum {
  SPerl_VM_C_ARRAY_HEADER_LENGTH = sizeof(int64_t),
};

SPerl_VM* SPerl_VM_new(SPerl* sperl) {
  SPerl_VM* vm = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_VM));
  
  vm->call_stack_capacity = 255;
  vm->call_stack = (int64_t*) SPerl_ALLOCATOR_safe_malloc(vm->call_stack_capacity, sizeof(int64_t));
  vm->frame = (SPerl_FRAME*) SPerl_ALLOCATOR_safe_malloc(1, sizeof(SPerl_FRAME));
  
  return vm;
}

void SPerl_VM_call_sub(SPerl* sperl, SPerl_VM* vm, const char* sub_base_name) {
  
  // Subroutine
  SPerl_PARSER* parser = sperl->parser;
  
  // Constant pool
  int32_t* constant_pool = sperl->constant_pool->values;
  
  // Bytecode
  uint8_t* bytecodes = sperl->bytecode_array->values;
  
  // Variables
  int64_t* vars = NULL;

  // Constant pool sub
  SPerl_CONSTANT_POOL_SUB* constant_pool_sub;
  {
    SPerl_SUB* sub = SPerl_HASH_search(parser->sub_symtable, sub_base_name, strlen(sub_base_name));
    constant_pool_sub
      = (SPerl_CONSTANT_POOL_SUB*)&constant_pool[sub->constant_pool_address];
  }
  
  int64_t* call_stack = vm->call_stack;
  
  // Program counter
  register int32_t pc = -1;
  
  // Top position of operand stack
  register int32_t operand_stack_top = -1;
  
  register int32_t success;
  
  int32_t call_stack_base = 0;
  
  // Goto subroutine
  goto CALLSUB_COMMON;
  
  while (1) {
    switch (bytecodes[pc]) {
      case SPerl_BYTECODE_C_CODE_NOP:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_ACONST_NULL:
        operand_stack_top++;
        *(intptr_t*)&call_stack[operand_stack_top] = NULL;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_M1:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = -1;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_0:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 0;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_1:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 1;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_2:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 2;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_3:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_4:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 4;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_5:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 5;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LCONST_0:
        operand_stack_top++;
        call_stack[operand_stack_top] = 0L;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LCONST_1:
        operand_stack_top++;
        call_stack[operand_stack_top] = 1L;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCONST_0:
        operand_stack_top++;
        *((float*)&call_stack[operand_stack_top]) = 0.F;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCONST_1:
        operand_stack_top++;
        *((float*)&call_stack[operand_stack_top]) = 1.F;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCONST_2:
        operand_stack_top++;
        *((float*)&call_stack[operand_stack_top]) = 2.F;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCONST_0:
        operand_stack_top++;
        *(double*)&call_stack[operand_stack_top] = 0.0;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCONST_1:
        operand_stack_top++;
        *(double*)&call_stack[operand_stack_top] = 1.0;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_BIPUSH:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = (int8_t)bytecodes[pc + 1];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_SIPUSH:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = (int16_t)((int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]);
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_LDC:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = constant_pool[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_LDC_W:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = constant_pool[(bytecodes[pc + 1] << 8) + bytecodes[pc + 2]];
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_LDC2_W:
        operand_stack_top++;
        call_stack[operand_stack_top] = *(int64_t*)&constant_pool[(bytecodes[pc + 1] << 8) + bytecodes[pc + 2]];
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD:
        operand_stack_top++;
        *(intptr_t*)&call_stack[operand_stack_top] = *(intptr_t*)&vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_0:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_1:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_2:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_3:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_0:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_1:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_2:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_3:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_0:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_1:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_2:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_3:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_0:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_1:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_2:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_3:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_0:
        operand_stack_top++;
        *(intptr_t*)&call_stack[operand_stack_top] = *(intptr_t*)&vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_1:
        operand_stack_top++;
        *(intptr_t*)&call_stack[operand_stack_top] = *(intptr_t*)&vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_2:
        operand_stack_top++;
        *(intptr_t*)&call_stack[operand_stack_top] = *(intptr_t*)&vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_3:
        operand_stack_top++;
        *(intptr_t*)&call_stack[operand_stack_top] = *(intptr_t*)&vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IALOAD:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = *(int32_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int32_t) * call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LALOAD:
        call_stack[operand_stack_top - 1]
          = *(int64_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int64_t) * call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FALOAD:
        *(float*)&call_stack[operand_stack_top - 1]
          = *(float*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(float) * call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DALOAD:
        *(double*)&call_stack[operand_stack_top - 1]
          = *(double*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(double) * call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_BALOAD:
        *(int8_t*)&call_stack[operand_stack_top - 1]
          = (int32_t)*(int8_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPerl_VM_C_ARRAY_HEADER_LENGTH + call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_SALOAD:
        *(int16_t*)&call_stack[operand_stack_top - 1]
          = (int32_t)*(int16_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int16_t) * call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE:
        *(int32_t*)&vars[bytecodes[pc + 1]] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE:
        vars[bytecodes[pc + 1]] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE:
        *(int32_t*)&vars[bytecodes[pc + 1]] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE:
        vars[bytecodes[pc + 1]] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE:
        *(intptr_t*)&vars[bytecodes[pc + 1]] = *(intptr_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_0:
        *(int32_t*)&vars[0] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_1:
        *(int32_t*)&vars[1] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_2:
        *(int32_t*)&vars[2] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_3:
        *(int32_t*)&vars[3] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_0:
        vars[0] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_1:
        vars[1] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_2:
        vars[2] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_3:
        vars[3] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_0:
        *(int32_t*)&vars[0] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_1:
        *(int32_t*)&vars[1] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_2:
        *(int32_t*)&vars[2] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_3:
        *(int32_t*)&vars[3] = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_0:
        vars[0] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_1:
        vars[1] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_2:
        vars[2] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_3:
        vars[3] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_0:
        *(intptr_t*)&vars[0] = *(intptr_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_1:
        *(intptr_t*)&vars[1] = *(intptr_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_2:
        *(intptr_t*)&vars[2] = *(intptr_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_3:
        *(intptr_t*)&vars[3] = *(intptr_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_BASTORE:
        *(int8_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int8_t) * call_stack[operand_stack_top - 1])
          = (int8_t)*(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_SASTORE:
        *(int16_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int16_t) * call_stack[operand_stack_top - 1])
          = (int16_t)*(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IASTORE:
        *(int32_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int32_t) * call_stack[operand_stack_top - 1])
          = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LASTORE:
        *(int64_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int64_t) * call_stack[operand_stack_top - 1])
          = call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FASTORE:
        *(float*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(float) * call_stack[operand_stack_top - 1])
          = *(float*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DASTORE:
        *(double*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(double) * call_stack[operand_stack_top - 1])
          = *(double*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_POP:
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_POP2:
        operand_stack_top -= 2;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IADD:
        
        *(int32_t*)&call_stack[operand_stack_top - 1] += *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LADD:
        call_stack[operand_stack_top - 1] += call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FADD:
        *((float*)&call_stack[operand_stack_top -1]) += *(float*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DADD:
        *((double*)&call_stack[operand_stack_top - 1]) += *(double*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISUB:
        *(int32_t*)&call_stack[operand_stack_top - 1] -= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSUB:
        call_stack[operand_stack_top - 1] -= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSUB:
        *((float*)&call_stack[operand_stack_top - 1]) -= *(float*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSUB:
        *((double*)&call_stack[operand_stack_top - 1]) -= *(double*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IMUL:
        *(int32_t*)&call_stack[operand_stack_top - 1] *= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LMUL:
        call_stack[operand_stack_top - 1] *= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FMUL:
        *(float*)&call_stack[operand_stack_top - 1] *= *(float*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DMUL:
        *(double*)&call_stack[operand_stack_top - 1] *= *(double*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IDIV:
        *(int32_t*)&call_stack[operand_stack_top - 1] /= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LDIV:
        call_stack[operand_stack_top - 1] /= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FDIV:
        *(float*)&call_stack[operand_stack_top - 1] /= *(float*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DDIV:
        *(double*)&call_stack[operand_stack_top - 1] /= *(double*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IREM:
        *(int32_t*)&call_stack[operand_stack_top - 1] = *(int32_t*)&call_stack[operand_stack_top - 1] % *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LREM:
        // z = a - (a/b) * b;
        call_stack[operand_stack_top - 1] = call_stack[operand_stack_top - 1] % call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FREM:
        *((float*)&call_stack[operand_stack_top - 1]) = (float)fmod((double)*((float*)&call_stack[operand_stack_top - 1]), (double)*(float*)&call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DREM:
        *((double*)&call_stack[operand_stack_top - 1]) = fmod(*(double*)&call_stack[operand_stack_top - 1], *(double*)&call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_INEG:
        *(int32_t*)&call_stack[operand_stack_top] = -*(int32_t*)&call_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LNEG:
        call_stack[operand_stack_top] = -call_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FNEG:
        *((float*)&call_stack[operand_stack_top]) = -*((float*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DNEG:
        *((double*)&call_stack[operand_stack_top]) = -*((double*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISHL:
        *(int32_t*)&call_stack[operand_stack_top - 1] <<= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSHL:
        call_stack[operand_stack_top - 1] <<= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISHR:
        *(int32_t*)&call_stack[operand_stack_top - 1] >>= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSHR:
        call_stack[operand_stack_top - 1] >>= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IUSHR:
        *(int32_t*)&call_stack[operand_stack_top - 1] = (*(int32_t*)&call_stack[operand_stack_top - 1] >> call_stack[operand_stack_top]) & 0xFFFFFFFF;
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LUSHR:
        call_stack[operand_stack_top - 1] = (call_stack[operand_stack_top - 1] >> call_stack[operand_stack_top]) & 0xFFFFFFFFFFFFFFFF;
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IAND:
        *(int32_t*)&call_stack[operand_stack_top - 1] &= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LAND:
        call_stack[operand_stack_top - 1] &= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IOR:
        *(int32_t*)&call_stack[operand_stack_top - 1] |= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LOR:
        call_stack[operand_stack_top - 1] |= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IXOR:
        *(int32_t*)&call_stack[operand_stack_top - 1] ^= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LXOR:
        call_stack[operand_stack_top - 1] ^= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IINC:
        *(int32_t*)&vars[bytecodes[pc + 1]] += (int8_t)bytecodes[pc + 2];
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_LINC:
        vars[bytecodes[pc + 1]] += (int8_t)bytecodes[pc + 2];
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_I2L:
        call_stack[operand_stack_top] = (int64_t)*(int32_t*)&call_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2F:
        *((float*)&call_stack[operand_stack_top]) = (float)*(int32_t*)&call_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2D:
        *((double*)&call_stack[operand_stack_top]) = (double)*(int32_t*)&call_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2I:
        call_stack[operand_stack_top] = (int32_t)call_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2F:
        *((float*)&call_stack[operand_stack_top]) = (float)*((int64_t*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2D:
        *((double*)&call_stack[operand_stack_top]) = (double)*((int64_t*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2I:
        *(int32_t*)&call_stack[operand_stack_top] = (int32_t)*((float*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2L:
        call_stack[operand_stack_top] = (int64_t)*((float*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2D:
        *((double*)&call_stack[operand_stack_top]) = (double)*((float*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2I:
        *(int32_t*)&call_stack[operand_stack_top] = (int32_t)*((double*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2L:
        call_stack[operand_stack_top] = (int64_t)*((double*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2F:
        *((float*)&call_stack[operand_stack_top]) = (float)*((double*)&call_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2B:
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&call_stack[operand_stack_top] & 1;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2S:
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&call_stack[operand_stack_top] & 0xF;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_B2I:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_S2I:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_B2L:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_B2F:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_B2D:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_S2B:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_S2L:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_S2F:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_S2D:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2B:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2S:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2B:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2S:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2B:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2S:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_B2S:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LCMP:
        // z = (x > y) + (x < y) * -1
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (call_stack[operand_stack_top - 1] > call_stack[operand_stack_top])
          + (call_stack[operand_stack_top - 1] < call_stack[operand_stack_top]) * -1;
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCMPL:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(float*)&call_stack[operand_stack_top - 1] > *(float*)&call_stack[operand_stack_top])
          + (*(float*)&call_stack[operand_stack_top - 1] < *(float*)&call_stack[operand_stack_top]) * -1
          + -!!(
            isnan(*(float*)&call_stack[operand_stack_top - 1])
            | isnan(*(float*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCMPG:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(float*)&call_stack[operand_stack_top - 1] > *(float*)&call_stack[operand_stack_top])
          + (*(float*)&call_stack[operand_stack_top - 1] < *(float*)&call_stack[operand_stack_top]) * -1
          + !!(
            isnan(*(float*)&call_stack[operand_stack_top - 1])
            | isnan(*(float*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCMPL:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(double*)&call_stack[operand_stack_top - 1] > *(double*)&call_stack[operand_stack_top])
          + (*(double*)&call_stack[operand_stack_top - 1] < *(double*)&call_stack[operand_stack_top]) * -1
          + -!!(
            isnan(*(double*)&call_stack[operand_stack_top - 1])
            | isnan(*(double*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCMPG:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(double*)&call_stack[operand_stack_top - 1] > *(double*)&call_stack[operand_stack_top])
          + (*(double*)&call_stack[operand_stack_top - 1] < *(double*)&call_stack[operand_stack_top]) * -1
          + !!(
            isnan(*(double*)&call_stack[operand_stack_top - 1])
            | isnan(*(double*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IFEQ:
        success = *(int32_t*)&call_stack[operand_stack_top] == 0;
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFNE:
        success = *(int32_t*)&call_stack[operand_stack_top] != 0;
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFLT:
        success = *(int32_t*)&call_stack[operand_stack_top] < 0;
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFGE:
        success = *(int32_t*)&call_stack[operand_stack_top] >= 0;
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFGT:
        success = *(int32_t*)&call_stack[operand_stack_top] > 0;
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFLE:
        success = *(int32_t*)&call_stack[operand_stack_top] <= 0;
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPEQ:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] == *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPNE:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] != *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPLT:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] < *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPGE:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] >= *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPGT:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] > *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPLE:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] <= *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ACMPEQ:
        success = *(intptr_t*)&call_stack[operand_stack_top - 1] == *(intptr_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ACMPNE:
        success = *(intptr_t*)&call_stack[operand_stack_top - 1] != *(intptr_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_GOTO:
        pc += (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]);
        continue;
      case SPerl_BYTECODE_C_CODE_TABLESWITCH: {
        
        // Padding
        int64_t padding = (sizeof(int64_t) - 1) - (pc % sizeof(int64_t));
        
        // default offset
        int64_t default_offset = *(int64_t*)&bytecodes[pc + padding + 1];
        
        // min
        int64_t min = *(int64_t*)&bytecodes[pc + padding + sizeof(int64_t) + 1];
        
        // max
        int64_t max = *(int64_t*)&bytecodes[pc + padding + sizeof(int64_t) * 2 + 1];
        
        if (call_stack[operand_stack_top] >= min && call_stack[operand_stack_top] <= max) {
          int64_t branch_base = (pc + padding + sizeof(int64_t) * 3 + 1) + (call_stack[operand_stack_top] - min) * sizeof(int64_t);
          pc += *(int64_t*)&bytecodes[branch_base];
        }
        else {
          pc += default_offset;
        }
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LOOKUPSWITCH: {

        // Padding
        int64_t padding = (sizeof(int64_t) - 1) - (pc % sizeof(int64_t));

        /*
        1  default
        5  npare
        9  match  13 branch // min
        17 match 21 branch
        25 match 29 branch // max
        */
        
        // default offset
        int64_t default_offset = *(int64_t*)&bytecodes[pc + padding + 1];
        
        // npare
        int64_t pair_count = *(int64_t*)&bytecodes[pc + padding + sizeof(int64_t) + 1];
        
        // min
        int64_t min = *(int64_t*)&bytecodes[pc + padding + sizeof(int64_t) * 2 + 1];
        
        // max
        int64_t max = *(int64_t*)&bytecodes[pc + padding + sizeof(int64_t) * 2 + 1 + ((pair_count - 1) * sizeof(int64_t) * 2)];
        
        if (call_stack[operand_stack_top] >= min && call_stack[operand_stack_top] <= max) {
          // 2 branch searching
          int64_t cur_min_pos = 0;
          int64_t cur_max_pos = pair_count - 1;

          while (1) {
            if (cur_max_pos < cur_min_pos) {
              pc += default_offset;
              break;
            }
            int64_t cur_half_pos = cur_min_pos + (cur_max_pos - cur_min_pos) / 2;
            int64_t cur_half = *(int64_t*)&bytecodes[pc + padding + sizeof(int64_t) * 2 + 1 + (cur_half_pos * sizeof(int64_t) * 2)];
            
            if (call_stack[operand_stack_top] > cur_half) {
              cur_min_pos = cur_half_pos + 1;
            }
            else if (call_stack[operand_stack_top] < cur_half) {
              cur_max_pos = cur_half_pos - 1;
            }
            else {
              int64_t branch = *(int64_t*)&bytecodes[pc + padding + sizeof(int64_t) * 2 + 1 + (cur_half_pos * sizeof(int64_t) * 2) + sizeof(int64_t)];
              pc += branch;
              break;
            }
          }
        }
        else {
          pc += default_offset;
        }
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_IRETURN: {
        
        // Return value
        int32_t return_value = *(int32_t*)&call_stack[operand_stack_top];
        
        // Finish call sub
        if (call_stack_base == 0) {
          *(int32_t*)&call_stack[0] = return_value;
          return;
        }

        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Restore vars
        vars = &call_stack[call_stack_base];
        
        // Push return value
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LRETURN: {
        // Return value
        int64_t return_value = call_stack[operand_stack_top];
        
        // Finish call sub
        if (call_stack_base == 0) {
          *(int32_t*)&call_stack[0] = return_value;
          return;
        }

        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Restore vars
        vars = &call_stack[call_stack_base];
        
        // Push return value
        operand_stack_top++;
        call_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_FRETURN: {
        // Return value
        float return_value = *(float*)&call_stack[operand_stack_top];
        
        // Finish call sub
        if (call_stack_base == 0) {
          *(int32_t*)&call_stack[0] = return_value;
          return;
        }

        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Restore vars
        vars = &call_stack[call_stack_base];
        
        // Push return value
        operand_stack_top++;
        *(float*)&call_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_DRETURN: {
        // Return value
        double return_value = *(double*)&call_stack[operand_stack_top];
        
        // Finish call sub
        if (call_stack_base == 0) {
          *(int32_t*)&call_stack[0] = return_value;
          return;
        }

        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Restore vars
        vars = &call_stack[call_stack_base];
        
        // Push return value
        operand_stack_top++;
        *(double*)&call_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_ARETURN: {
        // Return value
        intptr_t return_value = *(intptr_t*)&call_stack[operand_stack_top];
        
        // Finish call sub
        if (call_stack_base == 0) {
          *(int32_t*)&call_stack[0] = return_value;
          return;
        }

        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Restore vars
        vars = &call_stack[call_stack_base];
        
        // Push return value
        operand_stack_top++;
        *(intptr_t*)&call_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_RETURN: {
        // Finish call sub
        if (call_stack_base == 0) {
          return;
        }

        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Restore vars
        vars = &call_stack[call_stack_base];
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_NEW: {
        // Get subroutine ID
        int32_t package_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_PACKAGE* constant_pool_package = (SPerl_CONSTANT_POOL_SUB*)&constant_pool[package_constant_pool_address];
        
        size_t byte_size = constant_pool_package->byte_size;
        intptr_t address = (intptr_t)SPerl_HEAP_alloc(sperl, byte_size);
        memset((void*)address, 0, byte_size);
        
        operand_stack_top++;
        *(intptr_t*)&call_stack[operand_stack_top] = address;
        
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_NEWARRAY: {
        int32_t resolved_type_id = bytecodes[pc + 1];
        
        // Array length
        int64_t length = call_stack[operand_stack_top];
        
        // Allocate array
        intptr_t array;
        if (resolved_type_id == SPerl_RESOLVED_TYPE_C_ID_BOOLEAN || resolved_type_id == SPerl_RESOLVED_TYPE_C_ID_BYTE) {
          size_t allocate_size = SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int8_t) * length;
          array = (intptr_t)SPerl_HEAP_alloc(sperl, allocate_size);
          memset((void*)array, 0, allocate_size);
        }
        else if (resolved_type_id == SPerl_RESOLVED_TYPE_C_ID_SHORT) {
          size_t allocate_size = SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int16_t) * length;
          array = (intptr_t)SPerl_HEAP_alloc(sperl, allocate_size);
          memset((void*)array, 0, allocate_size);
        }
        else if (resolved_type_id == SPerl_RESOLVED_TYPE_C_ID_INT) {
          size_t allocate_size = SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int32_t) * length;
          array = (intptr_t)SPerl_HEAP_alloc(sperl, allocate_size);
          memset((void*)array, 0, allocate_size);
        }
        else if (resolved_type_id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
          size_t allocate_size = SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(int64_t) * length;
          array = (intptr_t)SPerl_HEAP_alloc(sperl, allocate_size);
          memset((void*)array, 0, allocate_size);
        }
        else if (resolved_type_id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
          size_t allocate_size = SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(float) * length;
          array = (intptr_t)SPerl_HEAP_alloc(sperl, allocate_size);
          memset((void*)array, 0, allocate_size);
        }
        else if (resolved_type_id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
          size_t allocate_size = SPerl_VM_C_ARRAY_HEADER_LENGTH + sizeof(double) * length;
          array = (intptr_t)SPerl_HEAP_alloc(sperl, allocate_size);
          memset((void*)array, 0, allocate_size);
        }
        
        // Set array length
        *(int64_t*)array = length;
        
        // Set array
        *(intptr_t*)&call_stack[operand_stack_top] = array;
        
        pc += 2;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_ARRAYLENGTH:
        call_stack[operand_stack_top] = *(int64_t*)*(intptr_t*)&call_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ATHROW:
        
        /*
        intptr_t exception = *(intptr_t*)&call_stack[operand_stack_top];
        
        // Finish call sub
        if (call_stack_base == 0) {
          return;
        }

        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Restore vars
        vars = &call_stack[call_stack_base];

        // Push return value
        operand_stack_top++;
        *(intptr_t*)&call_stack[operand_stack_top] = exception;
        
        pc = return_address;
        
        */
        pc++;
        
        continue;
      case SPerl_BYTECODE_C_CODE_WIDE:
        // iload, fload, aload, lload, dload, istore, fstore, astore, lstore, dstore, or iinc
        
        if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_ILOAD) {
          operand_stack_top++;
          *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_LLOAD) {
          operand_stack_top++;
          call_stack[operand_stack_top] = vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_FLOAD) {
          operand_stack_top++;
          *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)&vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_DLOAD) {
          operand_stack_top++;
          call_stack[operand_stack_top] = vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_ALOAD) {
          operand_stack_top++;
          *(intptr_t*)&call_stack[operand_stack_top] = *(intptr_t*)&vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_ISTORE) {
          *(int32_t*)&vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]] = *(int32_t*)&call_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_LSTORE) {
          vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]] = call_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_FSTORE) {
          *(float*)&vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]] = *(float*)&call_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_DSTORE) {
          *(double*)&vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]] = *(double*)&call_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_ASTORE) {
          *(intptr_t*)&vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]] = *(intptr_t*)&call_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_IINC) {
          *(int32_t*)&vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]] += (int16_t)((bytecodes[pc + 4] << 8) + bytecodes[pc + 5]);
          pc += 6;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_LINC) {
          vars[(bytecodes[pc + 2] << 8) + bytecodes[pc + 3]] += (int16_t)((bytecodes[pc + 4] << 8) + bytecodes[pc + 5]);
          pc += 6;
        }
        continue;
      case SPerl_BYTECODE_C_CODE_IFNULL:
        success = *(intptr_t*)&call_stack[operand_stack_top] == NULL;
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFNONNULL:
        success = *(intptr_t*)&call_stack[operand_stack_top] != NULL;
        pc += success * (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) + (~success & 1) * 3;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_INVOKESTATIC_WW:
      {
        // Get subroutine ID
        int32_t sub_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        constant_pool_sub = (SPerl_CONSTANT_POOL_SUB*)&constant_pool[sub_constant_pool_address];
        
        // Extend call stack(current size + 2(return address + call stack base before) + lexical variable area + operand_stack area)
        int32_t call_stack_max = operand_stack_top + 2 + constant_pool_sub->my_vars_length + constant_pool_sub->operand_stack_max;
        
        while (call_stack_max > vm->call_stack_capacity) {
          vm->call_stack_capacity = vm->call_stack_capacity * 2;
          vm->call_stack = call_stack = realloc(call_stack, sizeof(int64_t) * vm->call_stack_capacity);
        }
        
        operand_stack_top -= constant_pool_sub->args_length;

        // Prepare arguments
        memmove(&call_stack[operand_stack_top + 3], &call_stack[operand_stack_top + 1], constant_pool_sub->args_length * sizeof(int64_t));

        // Save return address
        operand_stack_top++;
        call_stack[operand_stack_top] = pc + 5;
        
        // Save vars base before
        operand_stack_top++;
        call_stack[operand_stack_top] = call_stack_base;
        
        // Set vars base
        call_stack_base = operand_stack_top + 1;
        
        CALLSUB_COMMON:
        
        // Initialize my variables
        memset(&call_stack[call_stack_base + constant_pool_sub->args_length], 0, (constant_pool_sub->my_vars_length - constant_pool_sub->args_length) * sizeof(int64_t));
        
        // Set variables to local variable
        vars = &call_stack[call_stack_base];
        
        // Set operant stack top
        operand_stack_top += constant_pool_sub->my_vars_length;

        // Call native sub
        if (constant_pool_sub->is_native) {
          
          if (!constant_pool_sub->has_return_value) {
            // Set frame
            vm->frame->vars = vars;
            vm->frame->operand_stack = &call_stack[operand_stack_top];
            
            // Call native sub
            void (*native_address)(SPerl_VM* VM) = constant_pool_sub->native_address;
            (*native_address)(vm);
            
            // Finish call sub
            if (call_stack_base == 0) {
              return;
            }
            
            // Restore operand stack top
            operand_stack_top = call_stack_base - 3;
            
            // Return address
            int64_t return_address = call_stack[call_stack_base - 2];
            
            // Resotre vars base
            call_stack_base = call_stack[call_stack_base - 1];
            
            // Restore vars
            vars = &call_stack[call_stack_base];
            
            pc = return_address;
          }
          else {
            // Set frame
            vm->frame->vars = vars;
            vm->frame->operand_stack = &call_stack[operand_stack_top];
            
            // Call native sub
            operand_stack_top++;
            void (*native_address)(SPerl_VM* VM) = constant_pool_sub->native_address;
            (*native_address)(vm);
            
            // Return value
            int64_t return_value = call_stack[operand_stack_top];
            
            // Finish call sub
            if (call_stack_base == 0) {
              call_stack[0] = return_value;
              return;
            }
            
            // Restore operand stack top
            operand_stack_top = call_stack_base - 3;
            
            // Return address
            int64_t return_address = call_stack[call_stack_base - 2];
            
            // Resotre vars base
            call_stack_base = call_stack[call_stack_base - 1];
            
            // Restore vars
            vars = &call_stack[call_stack_base];
            
            // Push return value
            operand_stack_top++;
            call_stack[operand_stack_top] = return_value;
            
            pc = return_address;
          }
        }
        // Call normal sub
        else {
          pc = constant_pool_sub->bytecode_base;
        }
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LDC_WW:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top]
          = constant_pool[(bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4]];
        pc += 5;
        continue;
      case SPerl_BYTECODE_C_CODE_LDC2_WW:
        operand_stack_top++;
        call_stack[operand_stack_top]
          = *(int64_t*)&constant_pool[(bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4]];
        pc += 5;
        continue;
      case SPerl_BYTECODE_C_CODE_BGETFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int8_t*)&call_stack[operand_stack_top] = *(int8_t*)(*(intptr_t*)&call_stack[operand_stack_top] + constant_pool_field->package_byte_offset);
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_SGETFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int16_t*)&call_stack[operand_stack_top] = *(int16_t*)(*(intptr_t*)&call_stack[operand_stack_top] + constant_pool_field->package_byte_offset);
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_IGETFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)(*(intptr_t*)&call_stack[operand_stack_top] + constant_pool_field->package_byte_offset);
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LGETFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int64_t*)&call_stack[operand_stack_top] = *(int64_t*)(*(intptr_t*)&call_stack[operand_stack_top] + constant_pool_field->package_byte_offset);
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_FGETFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(float*)&call_stack[operand_stack_top] = *(float*)(*(intptr_t*)&call_stack[operand_stack_top] + constant_pool_field->package_byte_offset);
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_DGETFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(double*)&call_stack[operand_stack_top] = *(double*)(*(intptr_t*)&call_stack[operand_stack_top] + constant_pool_field->package_byte_offset);
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_AGETFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(intptr_t*)&call_stack[operand_stack_top] = *(intptr_t*)(*(intptr_t*)&call_stack[operand_stack_top] + constant_pool_field->package_byte_offset);
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_BPUTFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int8_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset) = *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_SPUTFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int16_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset) = *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_IPUTFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int32_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset) = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LPUTFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int64_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset) = *(int64_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_FPUTFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(float*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset) = *(float*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_DPUTFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(double*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset) = *(double*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_APUTFIELD: {
        int32_t field_constant_pool_address
          = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        SPerl_CONSTANT_POOL_FIELD* constant_pool_field = (SPerl_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(intptr_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset) = *(intptr_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        continue;
      }
      case SPerl_BYTECODE_C_CODE_DUP:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP_X1:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP_X2:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP2:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP2_X1:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP2_X2:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_SWAP:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_CALOAD:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_CASTORE:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_JSR:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_RET:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_I2C:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_GETSTATIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_PUTSTATIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_GETFIELD:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_PUTFIELD:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INVOKEVIRTUAL:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INVOKESPECIAL:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INVOKESTATIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INVOKEINTERFACE:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INVOKEDYNAMIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_GOTO_W:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_JSR_W:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_BREAKPOINT:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_CHECKCAST:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INSTANCEOF:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_MONITORENTER:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_MONITOREXIT:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_MULTIANEWARRAY:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_ANEWARRAY: {
        // Not used
        assert(0);
      }
      case SPerl_BYTECODE_C_CODE_AASTORE:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_AALOAD:
        // Not used
        assert(0);
    }
    
    assert(0);
  }
}
