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

SPerl_VM* SPerl_VM_new(SPerl* sperl) {
  SPerl_VM* vm = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_VM));
  
  // Capacity of openrad stack
  vm->operand_stack_capacity = 255;
  
  // Operand stack
  vm->operand_stack = (int64_t*) SPerl_ALLOCATOR_safe_malloc(vm->operand_stack_capacity, sizeof(int64_t));
  
  vm->frame = (SPerl_FRAME*) SPerl_ALLOCATOR_safe_malloc(1, sizeof(SPerl_FRAME));
  
  vm->call_stack_capacity = 255;
  
  vm->call_stack = (int64_t*) SPerl_ALLOCATOR_safe_malloc(vm->call_stack_capacity, sizeof(int64_t));
  
  return vm;
}

SPerl_FRAME* SPerl_VM_init_frame(SPerl* sperl, SPerl_VM* vm) {
  
  vm->frame->call_stack = vm->call_stack;
  vm->frame->operand_stack = vm->operand_stack;
  
  return vm->frame;
}

void SPerl_VM_call_sub(SPerl* sperl, SPerl_VM* vm, const char* sub_base_name) {
  
  // Subroutine
  SPerl_PARSER* parser = sperl->parser;
  
  // Constant pool
  int32_t* constant_pool = sperl->constant_pool->values;
  
  // Bytecode
  uint8_t* bytecodes = sperl->bytecode_array->values;
  
  // Operand stack
  int64_t* operand_stack = vm->operand_stack;
  
  // Call stack
  int64_t* call_stack = vm->call_stack;
  
  // Constant pool base
  int32_t constant_pool_base = 0;;
  
  // Program counter
  register int64_t pc = 0;
  
  // Top position of operand stack
  register int32_t operand_stack_top = -1;
  
  // Bottom position of operand stack
  int32_t operand_stack_bottom = -1;
  
  // Base position of call stack
  register int32_t call_stack_base = 0;
  
  // Call stack next
  int32_t call_stack_next = 0;
  
  // Get subroutine
  SPerl_SUB* sub = SPerl_HASH_search(parser->sub_name_symtable, sub_base_name, strlen(sub_base_name));
  
  // Temparary value
  int64_t tmp_int64;
  
  double tmp_double;
  
  // Goto subroutine
  goto CALLSUB_COMMON;
  
  while (1) {
    
    switch (bytecodes[pc]) {
      case SPerl_BYTECODE_C_CODE_NOP:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_ACONST_NULL:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_M1:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = -1;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_0:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 0;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_1:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 1;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_2:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 2;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_3:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_4:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 4;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_5:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 5;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LCONST_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 0L;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LCONST_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 1L;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCONST_0:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = 0.F;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCONST_1:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = 1.F;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCONST_2:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = 2.F;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCONST_0:
        operand_stack_top++;
        *(double*)&operand_stack[operand_stack_top] = 0.0;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCONST_1:
        operand_stack_top++;
        *(double*)&operand_stack[operand_stack_top] = 1.0;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_BIPUSH:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = (int8_t)bytecodes[pc + 1];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_SIPUSH:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]);
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_LDC:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = constant_pool[constant_pool_base + bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_LDC_W:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = constant_pool[constant_pool_base + (bytecodes[pc + 1] << 8) + bytecodes[pc + 2]];
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_LDC2_W:
        operand_stack_top++;
        operand_stack[operand_stack_top] = *(int64_t*)&constant_pool[constant_pool_base + (bytecodes[pc + 1] << 8) + bytecodes[pc + 2]];
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base + bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base + bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_0:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_1:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base + 1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_2:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base + 2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_3:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base + 3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_2:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_3:
        operand_stack_top++;
        memcpy(&operand_stack[operand_stack_top], &call_stack[call_stack_base + 3], 8);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_0:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_1:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base + 1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_2:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base + 2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_3:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&call_stack[call_stack_base + 3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_2:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_3:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_2:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_3:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IALOAD:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LALOAD:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FALOAD:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DALOAD:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_AALOAD:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_BALOAD:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_CALOAD:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_SALOAD:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE:
        *(int32_t*)&call_stack[call_stack_base + bytecodes[pc + 1]] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE:
        call_stack[call_stack_base + bytecodes[pc + 1]] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE:
        *(int32_t*)&call_stack[call_stack_base + bytecodes[pc + 1]] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE:
        call_stack[call_stack_base + bytecodes[pc + 1]] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE:
        call_stack[call_stack_base + bytecodes[pc + 1]] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_0:
        *(int32_t*)&call_stack[call_stack_base] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_1:
        *(int32_t*)&call_stack[call_stack_base + 1] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_2:
        *(int32_t*)&call_stack[call_stack_base + 2] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_3:
        *(int32_t*)&call_stack[call_stack_base + 3] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_0:
        call_stack[call_stack_base] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_1:
        call_stack[call_stack_base + 1] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_2:
        call_stack[call_stack_base + 2] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_3:
        call_stack[call_stack_base + 3] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_0:
        *(int32_t*)&call_stack[call_stack_base] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_1:
        *(int32_t*)&call_stack[call_stack_base + 1] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_2:
        *(int32_t*)&call_stack[call_stack_base + 2] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_3:
        *(int32_t*)&call_stack[call_stack_base + 3] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_0:
        call_stack[call_stack_base] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_1:
        call_stack[call_stack_base + 1] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_2:
        call_stack[call_stack_base + 2] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_3:
        call_stack[call_stack_base + 3] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_0:
        call_stack[call_stack_base] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_1:
        call_stack[call_stack_base + 1] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_2:
        call_stack[call_stack_base + 2] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_3:
        call_stack[call_stack_base + 3] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_BASTORE:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_CASTORE:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_SASTORE:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IASTORE:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LASTORE:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FASTORE:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DASTORE:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_AASTORE:
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
      case SPerl_BYTECODE_C_CODE_DUP:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DUP_X1:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DUP_X2:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DUP2:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DUP2_X1:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DUP2_X2:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_SWAP:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IADD:
        *(int32_t*)&operand_stack[operand_stack_top - 1] += *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LADD:
        operand_stack[operand_stack_top - 1] += operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FADD:
        *((float*)&operand_stack[operand_stack_top -1]) += *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DADD:
        *((double*)&operand_stack[operand_stack_top - 1]) += *(double*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISUB:
        *(int32_t*)&operand_stack[operand_stack_top - 1] -= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSUB:
        operand_stack[operand_stack_top - 1] -= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSUB:
        *((float*)&operand_stack[operand_stack_top - 1]) -= *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSUB:
        *((double*)&operand_stack[operand_stack_top - 1]) -= *(double*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IMUL:
        *(int32_t*)&operand_stack[operand_stack_top - 1] *= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LMUL:
        operand_stack[operand_stack_top - 1] *= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FMUL:
        *(float*)&operand_stack[operand_stack_top - 1] *= *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DMUL:
        *(double*)&operand_stack[operand_stack_top - 1] *= *(double*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IDIV:
        *(int32_t*)&operand_stack[operand_stack_top - 1] /= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LDIV:
        operand_stack[operand_stack_top - 1] /= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FDIV:
        *(float*)&operand_stack[operand_stack_top - 1] /= *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DDIV:
        *(double*)&operand_stack[operand_stack_top - 1] /= *(double*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IREM:
        *(int32_t*)&operand_stack[operand_stack_top - 1] = *(int32_t*)&operand_stack[operand_stack_top - 1] % *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LREM:
        // z = a - (a/b) * b;
        operand_stack[operand_stack_top - 1] = operand_stack[operand_stack_top - 1] % operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FREM:
        *((float*)&operand_stack[operand_stack_top - 1]) = (float)fmod((double)*((float*)&operand_stack[operand_stack_top - 1]), (double)*(float*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DREM:
        *((double*)&operand_stack[operand_stack_top - 1]) = fmod(*(double*)&operand_stack[operand_stack_top - 1], *(double*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_INEG:
        *(int32_t*)&operand_stack[operand_stack_top] = -*(int32_t*)&operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LNEG:
        operand_stack[operand_stack_top] = -operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FNEG:
        *((float*)&operand_stack[operand_stack_top]) = -*((float*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DNEG:
        *((double*)&operand_stack[operand_stack_top]) = -*((double*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISHL:
        *(int32_t*)&operand_stack[operand_stack_top - 1] <<= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSHL:
        operand_stack[operand_stack_top - 1] <<= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISHR:
        *(int32_t*)&operand_stack[operand_stack_top - 1] >>= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSHR:
        operand_stack[operand_stack_top - 1] >>= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IUSHR:
        *(int32_t*)&operand_stack[operand_stack_top - 1] = (*(int32_t*)&operand_stack[operand_stack_top - 1] >> *(int32_t*)&operand_stack[operand_stack_top]) & 0xFFFFFFFF;
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LUSHR:
        operand_stack[operand_stack_top - 1] = (operand_stack[operand_stack_top - 1] >> operand_stack[operand_stack_top]) & 0xFFFFFFFFFFFFFFFF;
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IAND:
        *(int32_t*)&operand_stack[operand_stack_top - 1] &= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LAND:
        operand_stack[operand_stack_top - 1] &= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IOR:
        *(int32_t*)&operand_stack[operand_stack_top - 1] |= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LOR:
        operand_stack[operand_stack_top - 1] |= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IXOR:
        *(int32_t*)&operand_stack[operand_stack_top - 1] ^= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LXOR:
        operand_stack[operand_stack_top - 1] ^= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IINC:
        call_stack[call_stack_base + bytecodes[pc + 1]] += bytecodes[pc + 2];
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_I2L:
        operand_stack[operand_stack_top] = (int64_t)*(int32_t*)&operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2F:
        *((float*)&operand_stack[operand_stack_top]) = (float)*(int32_t*)&operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2D:
        *((double*)&operand_stack[operand_stack_top]) = (double)*(int32_t*)&operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2I:
        operand_stack[operand_stack_top] = (int32_t)operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2F:
        *((float*)&operand_stack[operand_stack_top]) = (float)*((int64_t*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2D:
        *((double*)&operand_stack[operand_stack_top]) = (double)*((int64_t*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2I:
        *(int32_t*)&operand_stack[operand_stack_top] = (int32_t)*((float*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2L:
        operand_stack[operand_stack_top] = (int64_t)*((float*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2D:
        *((double*)&operand_stack[operand_stack_top]) = (double)*((float*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2I:
        *(int32_t*)&operand_stack[operand_stack_top] = (int32_t)*((double*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2L:
        operand_stack[operand_stack_top] = (int64_t)*((double*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2F:
        *((float*)&operand_stack[operand_stack_top]) = (float)*((double*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2B:
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&operand_stack[operand_stack_top] & 1;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2C:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_I2S:
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&operand_stack[operand_stack_top] & 0xF;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LCMP:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCMPL:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCMPG:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCMPL:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCMPG:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IFEQ:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IFNE:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IFLT:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IFGE:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IFGT:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IFLE:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPEQ:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPNE:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPLT:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPGE:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPGT:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPLE:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ACMPEQ:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ACMPNE:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_GOTO:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_JSR:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_RET:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_TABLESWITCH: {
        
        // Machine address to culculate padding
        intptr_t pc_machine_address = (intptr_t)pc;
        
        // Padding
        int32_t padding = 3 - (pc_machine_address % 4);

        // default offset
        int32_t default_offset
          = (bytecodes[pc + padding + 1] << 24)
          + (bytecodes[pc + padding + 2] << 16)
          + (bytecodes[pc + padding + 3] << 8)
          + bytecodes[pc + padding + 4];
        
        /*
        // min
        int32_t min
          = (bytecodes[pc + padding + 5] << 24)
          + (bytecodes[pc + padding + 6] << 16)
          + (bytecodes[pc + padding + 7] << 8)
          + bytecodes[pc + padding + 8];
        
        // max
        int32_t max
          = (bytecodes[pc + padding + 9] << 24)
          + (bytecodes[pc + padding + 10] << 16)
          + (bytecodes[pc + padding + 11] << 8)
          + bytecodes[pc + padding + 12];
        */
        
        pc += default_offset;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LOOKUPSWITCH: {
        
        // Machine address to culculate padding
        intptr_t pc_machine_address = (intptr_t)pc;
        
        // Padding
        int32_t padding = 3 - (pc_machine_address % 4);

        // default offset
        int32_t default_offset
          = (bytecodes[pc + padding + 1] << 24)
          + (bytecodes[pc + padding + 2] << 16)
          + (bytecodes[pc + padding + 3] << 8)
          + bytecodes[pc + padding + 4];
        
        /*
        // npare
        int32_t pair_count
          = (bytecodes[pc + padding + 5] << 24)
          + (bytecodes[pc + padding + 6] << 16)
          + (bytecodes[pc + padding + 7] << 8)
          + bytecodes[pc + padding + 8];
        */
        
        pc += default_offset;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_IRETURN: {
        
        int32_t return_value = *(int32_t*)&operand_stack[operand_stack_top];
        
        // Resotre operand stack top
        operand_stack_top = operand_stack_bottom;
        
        // Resotre operand stack base
        operand_stack_bottom = operand_stack[operand_stack_bottom];
        
        // Restore openrad stack top
        *(int32_t*)&operand_stack[operand_stack_top] = return_value;
        
        // Finish vm
        if (call_stack_base == 0) {
          vm->frame->operand_stack = &operand_stack[operand_stack_top];
          return;
        }
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre call stack top
        call_stack_next = call_stack_base - 2;
        
        // Resotre call stack base
        call_stack_base = call_stack[call_stack_base - 1];
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LRETURN: {
        int64_t return_value = operand_stack[operand_stack_top];
        
        // Resotre operand stack top
        operand_stack_top = operand_stack_bottom;
        
        // Resotre operand stack base
        operand_stack_bottom = operand_stack[operand_stack_bottom];
        
        // Restore openrad stack top
        operand_stack[operand_stack_top] = return_value;
        
        // Finish vm
        if (call_stack_base == 0) {
          vm->frame->operand_stack = &operand_stack[operand_stack_top];
          return;
        }
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre call stack top
        call_stack_next = call_stack_base - 2;
        
        // Resotre call stack base
        call_stack_base = call_stack[call_stack_base - 1];
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_FRETURN: {
        int32_t return_value = *(int32_t*)&operand_stack[operand_stack_top];
        
        // Resotre operand stack top
        operand_stack_top = operand_stack_bottom;
        
        // Resotre operand stack base
        operand_stack_bottom = operand_stack[operand_stack_bottom];
        
        // Restore openrad stack top
        *(int32_t*)&operand_stack[operand_stack_top] = return_value;
        
        // Finish vm
        if (call_stack_base == 0) {
          vm->frame->operand_stack = &operand_stack[operand_stack_top];
          return;
        }
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre call stack top
        call_stack_next = call_stack_base - 2;
        
        // Resotre call stack base
        call_stack_base = call_stack[call_stack_base - 1];
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_DRETURN: {
        int64_t return_value = operand_stack[operand_stack_top];
        
        // Resotre operand stack top
        operand_stack_top = operand_stack_bottom;
        
        // Resotre operand stack base
        operand_stack_bottom = operand_stack[operand_stack_bottom];
        
        // Restore openrad stack top
        operand_stack[operand_stack_top] = return_value;
        
        // Finish vm
        if (call_stack_base == 0) {
          vm->frame->operand_stack = &operand_stack[operand_stack_top];
          return;
        }
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre call stack top
        call_stack_next = call_stack_base - 2;
        
        // Resotre call stack base
        call_stack_base = call_stack[call_stack_base - 1];
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_ARETURN: {
        int64_t return_value = operand_stack[operand_stack_top];
        
        // Resotre operand stack top
        operand_stack_top = operand_stack_bottom;
        
        // Resotre operand stack base
        operand_stack_bottom = operand_stack[operand_stack_bottom];
        
        // Restore openrad stack top
        operand_stack[operand_stack_top] = return_value;
        
        // Finish vm
        if (call_stack_base == 0) {
          vm->frame->operand_stack = &operand_stack[operand_stack_top];
          return;
        }
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre call stack top
        call_stack_next = call_stack_base - 2;
        
        // Resotre call stack base
        call_stack_base = call_stack[call_stack_base - 1];
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_RETURN: {

        // Resotre operand stack top
        operand_stack_top = operand_stack_bottom;
        
        // Resotre operand stack base
        operand_stack_bottom = operand_stack[operand_stack_bottom];
        
        // Finish vm
        if (call_stack_base == 0) {
          vm->frame->operand_stack = &operand_stack[operand_stack_top];
          return;
        }
        
        // Return address
        int64_t return_address = call_stack[call_stack_base - 2];
        
        // Resotre call stack top
        call_stack_next = call_stack_base - 2;
        
        // Resotre call stack base
        call_stack_base = call_stack[call_stack_base - 1];
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_GETSTATIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_PUTSTATIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_GETFIELD:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_PUTFIELD:
        pc += 3;
        continue;
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
      case SPerl_BYTECODE_C_CODE_NEW:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_NEWARRAY:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ANEWARRAY:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ARRAYLENGTH:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_MULTIANEWARRAY:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ATHROW:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_CHECKCAST:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INSTANCEOF:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_MONITORENTER:
        // Not used
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_MONITOREXIT:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_WIDE:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IFNULL:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IFNONNULL:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_GOTO_W:
        pc += 5;
        continue;
      case SPerl_BYTECODE_C_CODE_JSR_W:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_BREAKPOINT:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_CALLSUB:
      {
        // Get subroutine ID
        int32_t sub_id = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        
        // Subroutine
        SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, sub_id);
        sub = op_sub->uv.sub;
        
        // Save return address to call stack
        call_stack[call_stack_next] = pc + 5;
        call_stack_next++;
        
        // Save before call stack base
        call_stack[call_stack_next] = call_stack_base;
        call_stack_next++;
        
        // Update call stack base
        call_stack_base = call_stack_next;
        
        operand_stack_top -= (int32_t) sub->op_args->length;
        
        CALLSUB_COMMON:
        
        // Extend call stack(lexical variable area + return address + before call_stack_base)
        while (call_stack_next + sub->my_vars_size > vm->call_stack_capacity) {
          vm->call_stack_capacity = vm->call_stack_capacity * 2;
          vm->call_stack = (int64_t*) SPerl_ALLOCATOR_safe_realloc(call_stack, vm->call_stack_capacity, sizeof(int64_t));
          call_stack = vm->call_stack;
        }
        
        // Initialize my variables
        memset(&call_stack[call_stack_next], 0, sub->op_my_vars->length * sizeof(int64_t));
        call_stack_next += sub->my_vars_size;

        // Extend operand stack if need(operand stack max + before operand_stack_bottom)
        while (operand_stack_top + sub->operand_stack_max + 1 > vm->operand_stack_capacity) {
          vm->operand_stack_capacity = vm->operand_stack_capacity * 2;
          vm->operand_stack = (int64_t*) SPerl_ALLOCATOR_safe_realloc(vm->operand_stack, vm->operand_stack_capacity, sizeof(int64_t));
          operand_stack = vm->operand_stack;
        }
        
        // Prepare arguments
        memcpy(&call_stack[call_stack_base], &vm->operand_stack[operand_stack_top + 1], sub->op_args->length * sizeof(int64_t));
        
        // Call native subroutine
        if (sub->is_native) {
          // Set frame
          vm->frame->operand_stack = &operand_stack[operand_stack_top + 1];
          vm->frame->call_stack = &call_stack[call_stack_base];
          
          (*sub->native_address)(vm->frame);
          
          operand_stack_top++;
          
          // Return address
          int32_t return_address = call_stack[call_stack_base - 2];
          
          // Resotre call stack top
          call_stack_next = call_stack_base - 2;
          
          // Resotre call stack base
          call_stack_base = call_stack[call_stack_base - 1];
          
          pc = return_address;
        }
        // Call normal subroutine
        else {
          
          operand_stack_top++;
          operand_stack[operand_stack_top] = operand_stack_bottom;
          
          operand_stack_bottom = operand_stack_top;
          
          // Set constant pool base
          constant_pool_base = sub->constant_pool_base;
          
          pc = sub->bytecode_base;
        }
        continue;
      }
    }
    
    assert(0);
  }
}
