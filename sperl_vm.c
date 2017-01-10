#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_vm.h"
#include "sperl_allocator.h"
#include "sperl_hash.h"
#include "sperl_bytecode_array.h"
#include "sperl_bytecode.h"
#include "sperl_sub.h"

SPerl_VM* SPerl_VM_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_VM));
}



void SPerl_VM_run(SPerl* sperl, const char* sub_name) {
  
  SPerl_PARSER* parser = sperl->parser;
  SPerl_SUB* sub = SPerl_HASH_search(parser->sub_abs_name_symtable, sub_name, strlen(sub_name));
  
  SPerl_BYTECODE_ARRAY* bytecode_array = parser->bytecode_array;
  uint8_t* bytecodes = bytecode_array->values;
  
  // Program counter
  register uint8_t* pc = &bytecodes[sub->bytecode_start_pos];
  
  // Capacity of openrad stack
  int32_t operand_stack_capacity = 255;
  
  // Operand stack
  int32_t* operand_stack = malloc(sizeof(int32_t) * operand_stack_capacity);
  
  // Top position of operand stack
  register int32_t operand_stack_top = -1;
  
  // Capacity of operand stack
  int32_t call_stack_capacity = 255;
  
  // Call stack
  int32_t* call_stack = malloc(sizeof(int32_t) * call_stack_capacity);
  
  // Next position of call stack
  int32_t call_stack_next = 0;
  
  // Base position of call stack
  register int32_t call_stack_base = 0;
  
  // Before position of call stack;
  int32_t call_stack_base_before = 0;
  
  // Prepare call subroutine
  {
    // Extend operand stack
    while (operand_stack_top + sub->operand_stack_max > operand_stack_capacity) {
      operand_stack_capacity = operand_stack_capacity * 2;
      operand_stack = realloc(operand_stack, sizeof(int32_t) * operand_stack_capacity);
    }
    
    // Extend call stack
    while (call_stack_next + sub->my_vars_size + 2 > call_stack_capacity) {
      call_stack_capacity = call_stack_capacity * 2;
      call_stack = realloc(call_stack, sizeof(int32_t) * call_stack_capacity);
    }
    
    // Save base address
    call_stack_base_before = call_stack_base;
    
    // Update call_stack_base
    call_stack_base = call_stack_next;
    
    // Push my variable area
    memset(&call_stack[call_stack_next], 0, sub->my_vars_size * 4);
    call_stack_next += sub->my_vars_size;
  }
  
  int32_t frame_count = 1;
  
  while (1) {
    switch (*pc) {
      case SPerl_BYTECODE_C_CODE_NOP:
        // None
        break;
      case SPerl_BYTECODE_C_CODE_ACONST_NULL:
        
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_M1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = -1;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 0;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 1;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_2:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 2;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_3:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 3;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_4:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 4;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_5:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 5;
        break;
      case SPerl_BYTECODE_C_CODE_LCONST_0:
        operand_stack_top++;
        *((int64_t*)&operand_stack[operand_stack_top]) = 0L;
        operand_stack_top++;
        break;
      case SPerl_BYTECODE_C_CODE_LCONST_1:
        operand_stack_top++;
        *((int64_t*)&operand_stack[operand_stack_top]) = 1L;
        operand_stack_top++;
        break;
      case SPerl_BYTECODE_C_CODE_FCONST_0:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = 0.F;
        break;
      case SPerl_BYTECODE_C_CODE_FCONST_1:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = 1.F;
        break;
      case SPerl_BYTECODE_C_CODE_FCONST_2:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = 2.F;
        break;
      case SPerl_BYTECODE_C_CODE_DCONST_0:
        operand_stack_top++;
        *((double*)&operand_stack[operand_stack_top]) = 0.;
        operand_stack_top++;
        break;
      case SPerl_BYTECODE_C_CODE_DCONST_1:
        operand_stack_top++;
        *((double*)&operand_stack[operand_stack_top]) = 1.;
        operand_stack_top++;
        break;
      case SPerl_BYTECODE_C_CODE_BIPUSH:
        operand_stack_top++;
        
        pc++;
        operand_stack[operand_stack_top] = *pc;
        
        break;
      case SPerl_BYTECODE_C_CODE_SIPUSH:
        operand_stack_top++;
        
        pc++;
        operand_stack[operand_stack_top] = *pc << 8;
        
        pc++;
        operand_stack[operand_stack_top] += *pc;
        
        break;
      case SPerl_BYTECODE_C_CODE_LDC:
      
        break;
      case SPerl_BYTECODE_C_CODE_LDC_W:
      
        break;
      case SPerl_BYTECODE_C_CODE_LDC2_W:
      
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD:
        pc++;
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + *pc];
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD:
        pc++;
        operand_stack_top += 2;
        *((int64_t*)&operand_stack[operand_stack_top - 1]) = *((int64_t*)&call_stack[call_stack_base + *pc]);
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD:
        pc++;
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = *((float*)&call_stack[call_stack_base + *pc]);
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD:
        pc++;
        operand_stack_top += 2;
        *((double*)&operand_stack[operand_stack_top - 1]) = *((double*)&call_stack[call_stack_base + *pc]);
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD:
        pc++;
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + *pc];
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base];
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 1];
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD_2:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 2];
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD_3:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 3];
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD_0:
        operand_stack_top += 2;
        *((int64_t*)&operand_stack[operand_stack_top - 1]) = *((int64_t*)&call_stack[call_stack_base]);
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD_1:
        operand_stack_top += 2;
        *((int64_t*)&operand_stack[operand_stack_top - 1]) = *((int64_t*)&call_stack[call_stack_base + 1]);
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD_2:
        operand_stack_top += 2;
        *((int64_t*)&operand_stack[operand_stack_top - 1]) = *((int64_t*)&call_stack[call_stack_base + 2]);
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD_3:
        operand_stack_top += 2;
        *((int64_t*)&operand_stack[operand_stack_top - 1]) = *((int64_t*)&call_stack[call_stack_base + 3]);
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD_0:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = *((float*)&call_stack[call_stack_base]);
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD_1:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = *((float*)&call_stack[call_stack_base + 1]);
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD_2:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = *((float*)&call_stack[call_stack_base + 2]);
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD_3:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = *((float*)&call_stack[call_stack_base + 3]);
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD_0:
        operand_stack_top += 2;
        *((double*)&operand_stack[operand_stack_top - 1]) = *((double*)&call_stack[call_stack_base]);
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD_1:
        operand_stack_top += 2;
        *((double*)&operand_stack[operand_stack_top - 1]) = *((double*)&call_stack[call_stack_base + 1]);
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD_2:
        operand_stack_top += 2;
        *((double*)&operand_stack[operand_stack_top - 1]) = *((double*)&call_stack[call_stack_base + 2]);
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD_3:
        operand_stack_top += 2;
        *((double*)&operand_stack[operand_stack_top - 1]) = *((double*)&call_stack[call_stack_base + 3]);
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD_0:
        operand_stack_top++;
        operand_stack[operand_stack_top - 1] = call_stack[call_stack_base];
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 1];
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD_2:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 2];
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD_3:
        operand_stack_top++;
        operand_stack[operand_stack_top] = call_stack[call_stack_base + 3];
        break;
      case SPerl_BYTECODE_C_CODE_IALOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_LALOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_FALOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_DALOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_AALOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_BALOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_CALOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_SALOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_ISTORE:
        pc++;
        call_stack[call_stack_base + *pc] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE:
        pc++;
        *((int64_t*)&call_stack[call_stack_base + *pc]) = *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE:
        pc++;
        *((float*)&call_stack[call_stack_base + *pc]) = *((float*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE:
        pc++;
        *((double*)&call_stack[call_stack_base + *pc]) = *((double*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE:
        pc++;
        call_stack[call_stack_base + *pc] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_ISTORE_0:
        call_stack[call_stack_base] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_ISTORE_1:
        call_stack[call_stack_base + 1] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_ISTORE_2:
        call_stack[call_stack_base + 2] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_ISTORE_3:
        call_stack[call_stack_base + 3] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE_0:
        *((int64_t*)&call_stack[call_stack_base]) = *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE_1:
        *((int64_t*)&call_stack[call_stack_base + 1]) = *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE_2:
        *((int64_t*)&call_stack[call_stack_base + 2]) = *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE_3:
        *((int64_t*)&call_stack[call_stack_base + 3]) = *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE_0:
        *((float*)&call_stack[call_stack_base]) = *((float*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE_1:
        *((float*)&call_stack[call_stack_base + 1]) = *((float*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE_2:
        *((float*)&call_stack[call_stack_base + 2]) = *((float*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE_3:
        *((float*)&call_stack[call_stack_base + 3]) = *((float*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE_0:
        *((double*)&call_stack[call_stack_base]) = *((double*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE_1:
        *((double*)&call_stack[call_stack_base + 1]) = *((double*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE_2:
        *((double*)&call_stack[call_stack_base + 2]) = *((double*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE_3:
        *((double*)&call_stack[call_stack_base + 3]) = *((double*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE_0:
        call_stack[call_stack_base] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE_1:
        call_stack[call_stack_base + 1] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE_2:
        call_stack[call_stack_base + 2] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE_3:
        call_stack[call_stack_base + 3] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_BASTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_CASTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_SASTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_IASTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_LASTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_FASTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_DASTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_AASTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_POP:
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_POP2:
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_DUP:
      
        break;
      case SPerl_BYTECODE_C_CODE_DUP_X1:
      
        break;
      case SPerl_BYTECODE_C_CODE_DUP_X2:
      
        break;
      case SPerl_BYTECODE_C_CODE_DUP2:
      
        break;
      case SPerl_BYTECODE_C_CODE_DUP2_X1:
      
        break;
      case SPerl_BYTECODE_C_CODE_DUP2_X2:
      
        break;
      case SPerl_BYTECODE_C_CODE_SWAP:
      
        break;
      case SPerl_BYTECODE_C_CODE_IADD:
        operand_stack[operand_stack_top - 1] += operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LADD:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) += *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FADD:
        *((float*)&operand_stack[operand_stack_top - 1]) += *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_DADD:
        *((double*)&operand_stack[operand_stack_top - 3]) += *(double*)&operand_stack[operand_stack_top - 1];
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_ISUB:
        operand_stack[operand_stack_top - 1] -= operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LSUB:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) -= *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FSUB:
        *((float*)&operand_stack[operand_stack_top - 1]) -= *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_DSUB:
        *((double*)&operand_stack[operand_stack_top - 3]) -= *(double*)&operand_stack[operand_stack_top - 1];
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IMUL:
        operand_stack[operand_stack_top - 1] *= operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LMUL:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) *= *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FMUL:
        *((float*)&operand_stack[operand_stack_top - 1]) *= *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_DMUL:
        *((double*)&operand_stack[operand_stack_top - 3]) *= *(double*)&operand_stack[operand_stack_top - 1];
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IDIV:
        operand_stack[operand_stack_top - 1] /= operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LDIV:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) /= *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FDIV:
        *((float*)&operand_stack[operand_stack_top - 1]) /= *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_DDIV:
        *((double*)&operand_stack[operand_stack_top - 3]) /= *(double*)&operand_stack[operand_stack_top - 1];
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IREM:
        operand_stack[operand_stack_top - 1] = operand_stack[operand_stack_top - 1] % operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LREM:
        // z = a - (a/b) * b;
        *((int64_t*)&operand_stack[operand_stack_top - 3]) = *((int64_t*)&operand_stack[operand_stack_top - 3]) % *(int64_t*)&operand_stack[operand_stack_top - 1];
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FREM:
        *((float*)&operand_stack[operand_stack_top - 1]) = (float)fmod((double)*((float*)&operand_stack[operand_stack_top - 1]), (double)*(float*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_DREM:
        *((double*)&operand_stack[operand_stack_top - 3]) = fmod(*(double*)&operand_stack[operand_stack_top - 3], *(double*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_INEG:
        operand_stack[operand_stack_top] = -operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LNEG:
        *((int64_t*)&operand_stack[operand_stack_top - 1]) = -*((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FNEG:
        *((float*)&operand_stack[operand_stack_top - 1]) = -*((float*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_DNEG:
        *((double*)&operand_stack[operand_stack_top - 1]) = -*((double*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_ISHL:
        operand_stack[operand_stack_top - 1] <<= operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LSHL:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) <<= *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_ISHR:
        operand_stack[operand_stack_top - 1] >>= operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LSHR:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) >>= *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IUSHR:
        operand_stack[operand_stack_top - 1] = (operand_stack[operand_stack_top - 1] >> operand_stack[operand_stack_top]) & 0xFFFFFFFF;
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LUSHR:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) = (*((int64_t*)&operand_stack[operand_stack_top - 3]) >> *((int64_t*)&operand_stack[operand_stack_top - 1])) & 0xFFFFFFFFFFFFFFFF;
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IAND:
        operand_stack[operand_stack_top - 1] &= operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LAND:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) &= *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IOR:
        operand_stack[operand_stack_top - 1] |= operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LOR:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) |= *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IXOR:
        operand_stack[operand_stack_top - 1] ^= operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_LXOR:
        *((int64_t*)&operand_stack[operand_stack_top - 3]) ^= *((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IINC:
        operand_stack[operand_stack_top]++;
        break;
      case SPerl_BYTECODE_C_CODE_I2L:
        operand_stack_top++;
        *((int64_t*)&operand_stack[operand_stack_top - 1]) = *(int64_t*)&operand_stack[operand_stack_top - 1];
        break;
      case SPerl_BYTECODE_C_CODE_I2F:
        *((float*)&operand_stack[operand_stack_top]) = *(float*)&operand_stack[operand_stack_top];
        break;
      case SPerl_BYTECODE_C_CODE_I2D:
        operand_stack_top++;
        *((double*)&operand_stack[operand_stack_top - 1]) = *(double*)&operand_stack[operand_stack_top - 1];
        break;
      case SPerl_BYTECODE_C_CODE_L2I:
        operand_stack[operand_stack_top - 1] = operand_stack[operand_stack_top];
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_L2F:
        *((float*)&operand_stack[operand_stack_top - 1]) = (float)*((int64_t*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_L2D:
        *((double*)&operand_stack[operand_stack_top - 1]) = (double)*((int64_t*)&operand_stack[operand_stack_top - 1]);
        break;
      case SPerl_BYTECODE_C_CODE_F2I:
        operand_stack[operand_stack_top] = (int32_t)*((float*)&operand_stack[operand_stack_top]);
        break;
      case SPerl_BYTECODE_C_CODE_F2L:
        operand_stack_top++;
        *((int64_t*)&operand_stack[operand_stack_top]) = (int64_t)*((float*)&operand_stack[operand_stack_top - 1]);
        break;
      case SPerl_BYTECODE_C_CODE_F2D:
        operand_stack_top++;
        *((double*)&operand_stack[operand_stack_top]) = (double)*((float*)&operand_stack[operand_stack_top - 1]);
        break;
      case SPerl_BYTECODE_C_CODE_D2I:
        operand_stack[operand_stack_top - 1] = (int32_t)*((double*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_D2L:
        *((int64_t*)&operand_stack[operand_stack_top - 1]) = (int64_t)*((double*)&operand_stack[operand_stack_top - 1]);
        break;
      case SPerl_BYTECODE_C_CODE_D2F:
        *((float*)&operand_stack[operand_stack_top - 1]) = (float)*((double*)&operand_stack[operand_stack_top - 1]);
        operand_stack_top--;
        break;
      case SPerl_BYTECODE_C_CODE_I2B:
        operand_stack[operand_stack_top] = operand_stack[operand_stack_top] & 1;
        break;
      case SPerl_BYTECODE_C_CODE_I2C:
        /* Not used */
        break;
      case SPerl_BYTECODE_C_CODE_I2S:
        operand_stack[operand_stack_top] = operand_stack[operand_stack_top] & 0xF;
        break;
      case SPerl_BYTECODE_C_CODE_LCMP:
      
        break;
      case SPerl_BYTECODE_C_CODE_FCMPL:
      
        break;
      case SPerl_BYTECODE_C_CODE_FCMPG:
      
        break;
      case SPerl_BYTECODE_C_CODE_DCMPL:
      
        break;
      case SPerl_BYTECODE_C_CODE_DCMPG:
      
        break;
      case SPerl_BYTECODE_C_CODE_IFEQ:
      
        break;
      case SPerl_BYTECODE_C_CODE_IFNE:
      
        break;
      case SPerl_BYTECODE_C_CODE_IFLT:
      
        break;
      case SPerl_BYTECODE_C_CODE_IFGE:
      
        break;
      case SPerl_BYTECODE_C_CODE_IFGT:
      
        break;
      case SPerl_BYTECODE_C_CODE_IFLE:
      
        break;
      case SPerl_BYTECODE_C_CODE_IF_ICMPEQ:
      
        break;
      case SPerl_BYTECODE_C_CODE_IF_ICMPNE:
      
        break;
      case SPerl_BYTECODE_C_CODE_IF_ICMPLT:
      
        break;
      case SPerl_BYTECODE_C_CODE_IF_ICMPGE:
      
        break;
      case SPerl_BYTECODE_C_CODE_IF_ICMPGT:
      
        break;
      case SPerl_BYTECODE_C_CODE_IF_ICMPLE:
      
        break;
      case SPerl_BYTECODE_C_CODE_IF_ACMPEQ:
      
        break;
      case SPerl_BYTECODE_C_CODE_IF_ACMPNE:
      
        break;
      case SPerl_BYTECODE_C_CODE_GOTO:
      
        break;
      case SPerl_BYTECODE_C_CODE_JSR:
      
        break;
      case SPerl_BYTECODE_C_CODE_RET:
      
        break;
      case SPerl_BYTECODE_C_CODE_TABLESWITCH:
      
        break;
      case SPerl_BYTECODE_C_CODE_LOOKUPSWITCH:
      
        break;
      case SPerl_BYTECODE_C_CODE_IRETURN:
        frame_count--;
        if (frame_count == 0) {
          return;
        }
        break;
      case SPerl_BYTECODE_C_CODE_LRETURN:
        frame_count--;
        if (frame_count == 0) {
          return;
        }
        break;
      case SPerl_BYTECODE_C_CODE_FRETURN:
        frame_count--;
        if (frame_count == 0) {
          return;
        }
        break;
      case SPerl_BYTECODE_C_CODE_DRETURN:
        frame_count--;
        if (frame_count == 0) {
          return;
        }
        break;
      case SPerl_BYTECODE_C_CODE_ARETURN:
        frame_count--;
        if (frame_count == 0) {
          return;
        }
        break;
      case SPerl_BYTECODE_C_CODE_RETURN:
        frame_count--;
        if (frame_count == 0) {
          return;
        }
        break;
      case SPerl_BYTECODE_C_CODE_GETSTATIC:
      
        break;
      case SPerl_BYTECODE_C_CODE_PUTSTATIC:
      
        break;
      case SPerl_BYTECODE_C_CODE_GETFIELD:
      
        break;
      case SPerl_BYTECODE_C_CODE_PUTFIELD:
      
        break;
      case SPerl_BYTECODE_C_CODE_INVOKEVIRTUAL:
      
        break;
      case SPerl_BYTECODE_C_CODE_INVOKESPECIAL:
      
        break;
      case SPerl_BYTECODE_C_CODE_INVOKESTATIC:
      
        break;
      case SPerl_BYTECODE_C_CODE_INVOKEINTERFACE:
      
        break;
      case SPerl_BYTECODE_C_CODE_INVOKEDYNAMIC:
      
        break;
      case SPerl_BYTECODE_C_CODE_NEW:
      
        break;
      case SPerl_BYTECODE_C_CODE_NEWARRAY:
      
        break;
      case SPerl_BYTECODE_C_CODE_ANEWARRAY:
      
        break;
      case SPerl_BYTECODE_C_CODE_ARRAYLENGTH:
        
        break;
      case SPerl_BYTECODE_C_CODE_MULTIANEWARRAY:
      
        break;
      case SPerl_BYTECODE_C_CODE_ATHROW:
      
        break;
      case SPerl_BYTECODE_C_CODE_CHECKCAST:
      
        break;
      case SPerl_BYTECODE_C_CODE_INSTANCEOF:
      
        break;
      case SPerl_BYTECODE_C_CODE_MONITORENTER:
      
        break;
      case SPerl_BYTECODE_C_CODE_MONITOREXIT:
      
        break;
      case SPerl_BYTECODE_C_CODE_WIDE:
      
        break;
      case SPerl_BYTECODE_C_CODE_IFNULL:
      
        break;
      case SPerl_BYTECODE_C_CODE_IFNONNULL:
      
        break;
      case SPerl_BYTECODE_C_CODE_GOTO_W:
      
        break;
      case SPerl_BYTECODE_C_CODE_JSR_W:
      
        break;
      case SPerl_BYTECODE_C_CODE_BREAKPOINT:
      
        break;
      case SPerl_BYTECODE_C_CODE_UNDEFINED:
      
        break;
      case SPerl_BYTECODE_C_CODE_IMPDEP1:
      
        break;
      case SPerl_BYTECODE_C_CODE_IMPDEP2:
      
        break;
    }
    
    pc++;
  }
}
