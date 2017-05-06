#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "spvm.h"
#include "spvm_hash.h"
#include "spvm_array.h"
#include "spvm_allocator_util.h"
#include "spvm_constant_pool.h"
#include "spvm_bytecode_array.h"
#include "spvm_func.h"
#include "spvm_api.h"
#include "spvm_bytecode.h"
#include "spvm_sub.h"
#include "spvm_op.h"
#include "spvm_constant_pool.h"
#include "spvm_package.h"
#include "spvm_allocator_runtime.h"
#include "spvm_constant_pool_sub.h"
#include "spvm_constant_pool_package.h"
#include "spvm_constant_pool_field.h"
#include "spvm_resolved_type.h"
#include "spvm_env.h"
#include "spvm_allocator_runtime.h"
#include "spvm_sv.h"

void SPVM_API_call_sub(SPVM* spvm, SPVM_ENV* env, const char* sub_abs_name) {
  (void)spvm;
  (void)env;
  
  // Jump table for direct threaded code
  static void* jump[] = {
    &&case_SPVM_BYTECODE_C_CODE_NOP,
    &&case_SPVM_BYTECODE_C_CODE_UNDEF,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_BYTE_0,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_BYTE_1,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_SHORT_0,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_SHORT_1,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_M1,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_0,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_1,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_2,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_3,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_4,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_5,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_M1,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_0,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_1,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_2,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_3,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_4,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_5,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_FLOAT_0,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_FLOAT_1,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_FLOAT_2,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_DOUBLE_0,
    &&case_SPVM_BYTECODE_C_CODE_CONSTANT_DOUBLE_1,
    &&case_SPVM_BYTECODE_C_CODE_PUSH_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_PUSH_BYTE_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_PUSH_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_PUSH_BYTE_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_PUSH_SHORT_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_PUSH_BYTE_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_PUSH_SHORT_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_LOAD_CONSTANT,
    &&case_SPVM_BYTECODE_C_CODE_LOAD_CONSTANT_W,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_INT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_LONG,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_INT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_LONG,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_POP,
    &&case_SPVM_BYTECODE_C_CODE_APOP,
    &&case_SPVM_BYTECODE_C_CODE_ADD_INT,
    &&case_SPVM_BYTECODE_C_CODE_ADD_LONG,
    &&case_SPVM_BYTECODE_C_CODE_ADD_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_ADD_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_INT,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_LONG,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_INT,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_LONG,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_INT,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_LONG,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_INT,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_LONG,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_INT,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_LONG,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_INT,
    &&case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_LONG,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_INT,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_LONG,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_INT,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_LONG,
    &&case_SPVM_BYTECODE_C_CODE_BIT_AND_INT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_AND_LONG,
    &&case_SPVM_BYTECODE_C_CODE_BIT_OR_INT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_OR_LEFT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_XOR_INT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_XOR_LONG,
    &&case_SPVM_BYTECODE_C_CODE_INC_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_ICMP,
    &&case_SPVM_BYTECODE_C_CODE_FCMPL,
    &&case_SPVM_BYTECODE_C_CODE_FCMPG,
    &&case_SPVM_BYTECODE_C_CODE_DCMPL,
    &&case_SPVM_BYTECODE_C_CODE_DCMPG,
    &&case_SPVM_BYTECODE_C_CODE_IFEQ,
    &&case_SPVM_BYTECODE_C_CODE_IFNE,
    &&case_SPVM_BYTECODE_C_CODE_IFLT,
    &&case_SPVM_BYTECODE_C_CODE_IFGE,
    &&case_SPVM_BYTECODE_C_CODE_IFGT,
    &&case_SPVM_BYTECODE_C_CODE_IFLE,
    &&case_SPVM_BYTECODE_C_CODE_IF_LCMPEQ,
    &&case_SPVM_BYTECODE_C_CODE_IF_LCMPNE,
    &&case_SPVM_BYTECODE_C_CODE_IF_LCMPLT,
    &&case_SPVM_BYTECODE_C_CODE_IF_LCMPGE,
    &&case_SPVM_BYTECODE_C_CODE_IF_LCMPGT,
    &&case_SPVM_BYTECODE_C_CODE_IF_LCMPLE,
    &&case_SPVM_BYTECODE_C_CODE_IF_ACMPEQ,
    &&case_SPVM_BYTECODE_C_CODE_IF_ACMPNE,
    &&case_SPVM_BYTECODE_C_CODE_GOTO,
    &&case_SPVM_BYTECODE_C_CODE_TABLESWITCH,
    &&case_SPVM_BYTECODE_C_CODE_LOOKUPSWITCH,
    &&case_SPVM_BYTECODE_C_CODE_ARRAYLENGTH,
    &&case_SPVM_BYTECODE_C_CODE_ATHROW,
    &&case_SPVM_BYTECODE_C_CODE_WIDE,
    &&case_SPVM_BYTECODE_C_CODE_IFNULL,
    &&case_SPVM_BYTECODE_C_CODE_IFNONNULL,
    &&case_SPVM_BYTECODE_C_CODE_INVOKESTATIC_WW,
    &&case_SPVM_BYTECODE_C_CODE_LOAD_CONSTANT_WW,
    &&case_SPVM_BYTECODE_C_CODE_BGETFIELD,
    &&case_SPVM_BYTECODE_C_CODE_SGETFIELD,
    &&case_SPVM_BYTECODE_C_CODE_IGETFIELD,
    &&case_SPVM_BYTECODE_C_CODE_LGETFIELD,
    &&case_SPVM_BYTECODE_C_CODE_FGETFIELD,
    &&case_SPVM_BYTECODE_C_CODE_DGETFIELD,
    &&case_SPVM_BYTECODE_C_CODE_BPUTFIELD,
    &&case_SPVM_BYTECODE_C_CODE_SPUTFIELD,
    &&case_SPVM_BYTECODE_C_CODE_IPUTFIELD,
    &&case_SPVM_BYTECODE_C_CODE_LPUTFIELD,
    &&case_SPVM_BYTECODE_C_CODE_FPUTFIELD,
    &&case_SPVM_BYTECODE_C_CODE_DPUTFIELD,
    &&case_SPVM_BYTECODE_C_CODE_INC_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_MALLOC_OBJECT,
    &&case_SPVM_BYTECODE_C_CODE_MALLOC_ARRAY,
    &&case_SPVM_BYTECODE_C_CODE_MALLOC_STRING,
    &&case_SPVM_BYTECODE_C_CODE_RETURN_VOID,
    &&case_SPVM_BYTECODE_C_CODE_RETURN_VALUE,
    &&case_SPVM_BYTECODE_C_CODE_LOAD,
    &&case_SPVM_BYTECODE_C_CODE_LOAD_0,
    &&case_SPVM_BYTECODE_C_CODE_LOAD_1,
    &&case_SPVM_BYTECODE_C_CODE_LOAD_2,
    &&case_SPVM_BYTECODE_C_CODE_LOAD_3,
    &&case_SPVM_BYTECODE_C_CODE_STORE,
    &&case_SPVM_BYTECODE_C_CODE_STORE_0,
    &&case_SPVM_BYTECODE_C_CODE_STORE_1,
    &&case_SPVM_BYTECODE_C_CODE_STORE_2,
    &&case_SPVM_BYTECODE_C_CODE_STORE_3,
    &&case_SPVM_BYTECODE_C_CODE_ADD_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_BIT_AND_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_BIT_OR_BYTE ,
    &&case_SPVM_BYTECODE_C_CODE_BIT_XOR_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_BCMP,
    &&case_SPVM_BYTECODE_C_CODE_ADD_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_AND_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_OR_SHORT ,
    &&case_SPVM_BYTECODE_C_CODE_BIT_XOR_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CMP_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_INC_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_INC_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_ASTORE,
    &&case_SPVM_BYTECODE_C_CODE_AASTORE,
    &&case_SPVM_BYTECODE_C_CODE_APUTFIELD,
    &&case_SPVM_BYTECODE_C_CODE_INC_REF_COUNT,
    &&case_SPVM_BYTECODE_C_CODE_DEC_REF_COUNT,
  };
  
  // Constant pool
  intmax_t* constant_pool = spvm->constant_pool->values;
  
  // Bytecode
  uint8_t* bytecodes = spvm->bytecode_array->values;
  
  // Variables
  intmax_t* vars = &env->call_stack[env->call_stack_base];
  
  // Constant pool sub
  int32_t sub_constant_pool_address = (int32_t)(intptr_t)SPVM_HASH_search(spvm, spvm->constant_pool_sub_symtable, sub_abs_name, strlen(sub_abs_name));
  
  intmax_t* call_stack = env->call_stack;
  
  // Program counter
  register uint8_t* pc = NULL;
  
  // Top position of operand stack
  register int32_t operand_stack_top = env->operand_stack_top;
  
  register _Bool success;
  
  int32_t call_stack_base = env->call_stack_base;
  int32_t call_stack_base_start = call_stack_base;
  
  SPVM_ALLOCATOR_RUNTIME* allocator = spvm->allocator_runtime;
  
  // Goto subroutine
  goto CALLSUB_COMMON;
  
  // while (1) {
    // switch (*pc) {
      case_SPVM_BYTECODE_C_CODE_INVOKESTATIC_WW:
      {
        // Get subroutine ID
        sub_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        
        CALLSUB_COMMON: {

          SPVM_CONSTANT_POOL_SUB* constant_pool_sub = (SPVM_CONSTANT_POOL_SUB*)&constant_pool[sub_constant_pool_address];

          // Extend call stack(current size + 2(return address + call stack base before) + lexical variable area + operand_stack area)
          int32_t call_stack_max = operand_stack_top + 2 + constant_pool_sub->my_vars_length + constant_pool_sub->operand_stack_max;
          
          while (call_stack_max > env->call_stack_capacity) {
            env->call_stack_capacity = env->call_stack_capacity * 2;
            env->call_stack = call_stack = realloc(call_stack, sizeof(intmax_t) * env->call_stack_capacity);
          }

          operand_stack_top -= constant_pool_sub->args_length;

          // Prepare arguments
          memmove(&call_stack[operand_stack_top + 3], &call_stack[operand_stack_top + 1], constant_pool_sub->args_length * sizeof(intmax_t));

          // Save return address(operand + (throw or goto exception handler))
          if (call_stack_base == call_stack_base_start) {
            call_stack[operand_stack_top + 1] = -1;
          }
          else {
            call_stack[operand_stack_top + 1] = (intptr_t)pc + 5 + 3;
          }
          
          // Save vars base before
          call_stack[operand_stack_top + 2] = call_stack_base;
          
          // Set vars base
          call_stack_base = operand_stack_top + 3;
          
          // Initialize my variables
          memset(&call_stack[call_stack_base + constant_pool_sub->args_length], 0, (constant_pool_sub->my_vars_length - constant_pool_sub->args_length) * sizeof(intmax_t));
          
          // Set variables to local variable
          vars = &call_stack[call_stack_base];
          
          // Set operant stack top
          operand_stack_top = call_stack_base + constant_pool_sub->my_vars_length - 1;

          // Call native sub
          if (constant_pool_sub->is_native) {
            // Set environment
            env->operand_stack_top = operand_stack_top;
            env->call_stack_base = call_stack_base;
            
            // Call native sub
            void (*native_address)(SPVM* spvm, SPVM_ENV* env) = constant_pool_sub->native_address;
            (*native_address)(spvm, env);
            
            // Get enviromnet
            operand_stack_top = env->operand_stack_top;
            call_stack_base = env->call_stack_base;
            
            if (env->abort) {
              goto case_SPVM_BYTECODE_C_CODE_ATHROW;
            }
            else {
              if (constant_pool_sub->has_return_value) {
                goto case_SPVM_BYTECODE_C_CODE_RETURN_VALUE;
              }
              else {
                goto case_SPVM_BYTECODE_C_CODE_RETURN_VOID;
              }
            }
          }
          // Call normal sub
          else {
            pc = &bytecodes[constant_pool_sub->bytecode_base];
          }
          goto *jump[*pc];
        }
      }
      case_SPVM_BYTECODE_C_CODE_RETURN_VALUE: {
        
        // Get return value
        intmax_t return_value = call_stack[operand_stack_top];
        
        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Get return address
        uint8_t* return_address = *(uint8_t**)&call_stack[call_stack_base - 2];
        
        // Resotre call stack base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Push return value
        operand_stack_top++;
        call_stack[operand_stack_top] = return_value;
        
        // Finish call sub
        if (call_stack_base == call_stack_base_start) {
          env->call_stack_base = call_stack_base;
          env->operand_stack_top = operand_stack_top;
          env->abort = 0;
          return;
        }
        else {
          // Restore vars
          vars = &call_stack[call_stack_base];
          
          pc = return_address;
          goto *jump[*pc];
        }
      }
      case_SPVM_BYTECODE_C_CODE_RETURN_VOID: {
        
        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Return address
        uint8_t* return_address = *(uint8_t**)&call_stack[call_stack_base - 2];
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Finish call sub
        if (call_stack_base == call_stack_base_start) {
          env->call_stack_base = call_stack_base;
          env->operand_stack_top = operand_stack_top;
          env->abort = 0;
          return;
        }
        else {
          // Restore vars
          vars = &call_stack[call_stack_base];

          pc = return_address;
          goto *jump[*pc];
        }
      }
      case_SPVM_BYTECODE_C_CODE_ATHROW: {
        
        // Return value
        intmax_t return_value = call_stack[operand_stack_top];
        
        // Restore operand stack top
        operand_stack_top = call_stack_base - 3;
        
        // Return address
        uint8_t* return_address = *(uint8_t**)&call_stack[call_stack_base - 2];
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Push return value
        operand_stack_top++;
        call_stack[operand_stack_top] = return_value;

        // Finish call sub with exception
        if (call_stack_base == call_stack_base_start) {
          env->call_stack_base = call_stack_base;
          env->operand_stack_top = operand_stack_top;
          env->abort = 1;
          return;
        }
        else {
          // Restore vars
          vars = &call_stack[call_stack_base];

          pc = (uint8_t*)(return_address - 3);
          goto *jump[*pc];
        }
      }
      case_SPVM_BYTECODE_C_CODE_NOP:
        // Not used
        assert(0);
      case_SPVM_BYTECODE_C_CODE_UNDEF:
        operand_stack_top++;
        *(void**)&call_stack[operand_stack_top] = (void*)NULL;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_BYTE_0:
        operand_stack_top++;
        *(int8_t*)&call_stack[operand_stack_top] = 0;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_BYTE_1:
        operand_stack_top++;
        *(int8_t*)&call_stack[operand_stack_top] = 1;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_SHORT_0:
        operand_stack_top++;
        *(int16_t*)&call_stack[operand_stack_top] = 0;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_SHORT_1:
        operand_stack_top++;
        *(int16_t*)&call_stack[operand_stack_top] = 1;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_M1:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = -1;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_0:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 0;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_1:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 1;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_2:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 2;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_3:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_4:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 4;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_INT_5:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = 5;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_M1:
        operand_stack_top++;
        call_stack[operand_stack_top] = -1L;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_0:
        operand_stack_top++;
        call_stack[operand_stack_top] = 0L;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_1:
        operand_stack_top++;
        call_stack[operand_stack_top] = 1L;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_2:
        
        operand_stack_top++;
        call_stack[operand_stack_top] = 2L;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_3:
        operand_stack_top++;
        call_stack[operand_stack_top] = 3L;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_4:
        operand_stack_top++;
        call_stack[operand_stack_top] = 4L;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_LONG_5:
        operand_stack_top++;
        call_stack[operand_stack_top] = 5L;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_FLOAT_0:
        operand_stack_top++;
        *((float*)&call_stack[operand_stack_top]) = 0.F;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_FLOAT_1:
        operand_stack_top++;
        *((float*)&call_stack[operand_stack_top]) = 1.F;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_FLOAT_2:
        operand_stack_top++;
        *((float*)&call_stack[operand_stack_top]) = 2.F;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_DOUBLE_0:
        operand_stack_top++;
        *(double*)&call_stack[operand_stack_top] = 0.0;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONSTANT_DOUBLE_1:
        operand_stack_top++;
        *(double*)&call_stack[operand_stack_top] = 1.0;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_PUSH_BYTE:
        operand_stack_top++;
        *(int8_t*)&call_stack[operand_stack_top] = (int8_t)*(pc + 1);
        pc += 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_PUSH_BYTE_TO_SHORT:
        operand_stack_top++;
        *(int16_t*)&call_stack[operand_stack_top] = (int16_t)*(pc + 1);
        pc += 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_PUSH_SHORT:
        operand_stack_top++;
        *(int16_t*)&call_stack[operand_stack_top] = (int16_t)((int16_t)(*(pc + 1) << 8) +  *(pc + 2));
        pc += 3;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_PUSH_BYTE_TO_INT:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = (int8_t)*(pc + 1);
        pc += 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_PUSH_SHORT_TO_LONG:
        operand_stack_top++;
        call_stack[operand_stack_top] = (int16_t)((int16_t)(*(pc + 1) << 8) +  *(pc + 2));
        pc += 3;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_PUSH_BYTE_TO_LONG:
        operand_stack_top++;
        call_stack[operand_stack_top] = (int8_t)*(pc + 1);
        pc += 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_PUSH_SHORT_TO_INT:
        operand_stack_top++;
        *(int32_t*)&call_stack[operand_stack_top] = (int16_t)((int16_t)(*(pc + 1) << 8) +  *(pc + 2));
        pc += 3;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LOAD_CONSTANT:
        operand_stack_top++;
        call_stack[operand_stack_top] = constant_pool[*(pc + 1)];
        pc += 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LOAD_CONSTANT_W:
        operand_stack_top++;
        call_stack[operand_stack_top] = constant_pool[(*(pc + 1) << 8) + *(pc + 2)];
        pc += 3;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LOAD_CONSTANT_WW:
        operand_stack_top++;
        call_stack[operand_stack_top] = constant_pool[(*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4)];
        pc += 5;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LOAD:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[*(pc + 1)];
        pc += 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LOAD_0:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[0];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LOAD_1:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[1];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LOAD_2:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[2];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LOAD_3:
        operand_stack_top++;
        call_stack[operand_stack_top] = vars[3];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = *(int32_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int32_t) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_LONG: {
        intptr_t address = *(intptr_t*)&call_stack[operand_stack_top - 1];
        int64_t index = *(int64_t*)&call_stack[operand_stack_top];
        
        if (address == NULL) {
          assert(0);
        }
        else {
          int64_t length = *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET);
          if (index < 0 || index > length - 1) {
            assert(0);
          }
          else {
            call_stack[operand_stack_top - 1] = *(int64_t*)(address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int64_t) * (size_t)index);
            operand_stack_top--;
            pc++;
            goto *jump[*pc];
          }
        }
      }
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_FLOAT:
        *(float*)&call_stack[operand_stack_top - 1]
          = *(float*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(float) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_DOUBLE:
        *(double*)&call_stack[operand_stack_top - 1]
          = *(double*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(double) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1]
          = *(int8_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int8_t) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1]
          = *(int16_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int16_t) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_STORE:
        vars[*(pc + 1)] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_STORE_0:
        vars[0] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_STORE_1:
        vars[1] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_STORE_2:
        vars[2] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_STORE_3:
        vars[3] = call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ASTORE: {
        int32_t vars_index = *(pc + 1);

        // Increment reference count if stored object is not null
        if ((void*)call_stack[operand_stack_top] != NULL) {
          SPVM_API_inc_ref_count(spvm, env, (void*)call_stack[operand_stack_top]);
        }
        
        // Decrement reference count if original object is not null
        if ((void*)vars[vars_index] != NULL) {
          SPVM_API_dec_ref_count(spvm, env, (void*)vars[vars_index]);
        }
        
        // Store address
        vars[vars_index] = call_stack[operand_stack_top];
        
        operand_stack_top--;
        pc += 2;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_DEC_REF_COUNT: {
        void* address = (void*)call_stack[operand_stack_top];
        
        if (address != NULL) {
          SPVM_API_dec_ref_count(spvm, env, address);
        }
        
        pc += 1;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_INC_REF_COUNT: {
        void* address = (void*)call_stack[operand_stack_top];
        
        if (address != NULL) {
          SPVM_API_inc_ref_count(spvm, env, address);
        }
        
        pc += 1;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_BYTE:
        *(int8_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int8_t) * (size_t)call_stack[operand_stack_top - 1])
          = *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_SHORT:
        *(int16_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int16_t) * (size_t)call_stack[operand_stack_top - 1])
          = *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_INT:
        *(int32_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int32_t) * (size_t)call_stack[operand_stack_top - 1])
          = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_LONG:
        *(int64_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int64_t) * (size_t)call_stack[operand_stack_top - 1])
          = call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_FLOAT:
        *(float*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(float) * (size_t)call_stack[operand_stack_top - 1])
          = *(float*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_DOUBLE:
        *(double*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(double) * (size_t)call_stack[operand_stack_top - 1])
          = *(double*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_AASTORE: {
        intptr_t array_index = *(intptr_t*)&call_stack[operand_stack_top - 2] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(void*) * (size_t)call_stack[operand_stack_top - 1];

        // Increment reference count if stored object is not null
        if (*(void**)&call_stack[operand_stack_top] != NULL) {
          SPVM_API_inc_ref_count(spvm, env, *(void**)&call_stack[operand_stack_top]);
        }
        
        // Decrement reference count if original object is not null
        if (*(void**)(array_index) != NULL) {
          SPVM_API_dec_ref_count(spvm, env, *(void**)(array_index));
        }
        
        // Store address
        *(void**)(array_index) = *(void**)&call_stack[operand_stack_top];
        
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_POP:
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_APOP: {
        
        void* address = (void*)call_stack[operand_stack_top];
        
        // Free object
        if (address != NULL) {
          int64_t ref_count = *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET);
          if (ref_count == 0) {
            SPVM_ALLOCATOR_RUNTIME_free_address(spvm, spvm->allocator_runtime, (void*)call_stack[operand_stack_top]);
          }
        }
        
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_ADD_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] += *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ADD_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] += *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ADD_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] += *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ADD_LONG:
        call_stack[operand_stack_top - 1] += call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ADD_FLOAT:
        *((float*)&call_stack[operand_stack_top -1]) += *(float*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ADD_DOUBLE:
        *((double*)&call_stack[operand_stack_top - 1]) += *(double*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_SUBTRACT_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] -= *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_SUBTRACT_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] -= *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_SUBTRACT_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] -= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_SUBTRACT_LONG:
        call_stack[operand_stack_top - 1] -= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_SUBTRACT_FLOAT:
        *((float*)&call_stack[operand_stack_top - 1]) -= *(float*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_SUBTRACT_DOUBLE:
        *((double*)&call_stack[operand_stack_top - 1]) -= *(double*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_MULTIPLY_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] *= *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_MULTIPLY_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] *= *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_MULTIPLY_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] *= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_MULTIPLY_LONG:
        call_stack[operand_stack_top - 1] *= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_MULTIPLY_FLOAT:
        *(float*)&call_stack[operand_stack_top - 1] *= *(float*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_MULTIPLY_DOUBLE:
        *(double*)&call_stack[operand_stack_top - 1] *= *(double*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_DIVIDE_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] /= *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_DIVIDE_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] /= *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_DIVIDE_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] /= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_DIVIDE_LONG:
        call_stack[operand_stack_top - 1] /= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_DIVIDE_FLOAT:
        *(float*)&call_stack[operand_stack_top - 1] /= *(float*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_DIVIDE_DOUBLE:
        *(double*)&call_stack[operand_stack_top - 1] /= *(double*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_REMAINDER_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] = *(int8_t*)&call_stack[operand_stack_top - 1] % *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_REMAINDER_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] = *(int16_t*)&call_stack[operand_stack_top - 1] % *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_REMAINDER_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] = *(int32_t*)&call_stack[operand_stack_top - 1] % *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_REMAINDER_LONG:
        // z = a - (a/b) * b;
        call_stack[operand_stack_top - 1] = call_stack[operand_stack_top - 1] % call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_REMAINDER_FLOAT:
        *((float*)&call_stack[operand_stack_top - 1]) = (float)fmod((double)*((float*)&call_stack[operand_stack_top - 1]), (double)*(float*)&call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_REMAINDER_DOUBLE:
        *((double*)&call_stack[operand_stack_top - 1]) = fmod(*(double*)&call_stack[operand_stack_top - 1], *(double*)&call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_NEGATE_BYTE:
        *(int8_t*)&call_stack[operand_stack_top] = -*(int8_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_NEGATE_SHORT:
        *(int16_t*)&call_stack[operand_stack_top] = -*(int16_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_NEGATE_INT:
        *(int32_t*)&call_stack[operand_stack_top] = -*(int32_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_NEGATE_LONG:
        call_stack[operand_stack_top] = -call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_NEGATE_FLOAT:
        *((float*)&call_stack[operand_stack_top]) = -*((float*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_NEGATE_DOUBLE:
        *((double*)&call_stack[operand_stack_top]) = -*((double*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] <<= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] <<= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] <<= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_LONG:
        call_stack[operand_stack_top - 1] <<= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] >>= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] >>= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] >>= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_LONG:
        call_stack[operand_stack_top - 1] >>= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] = (*(int8_t*)&call_stack[operand_stack_top - 1] >> call_stack[operand_stack_top]) & 0xFF;
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_SHORT:
        *(int8_t*)&call_stack[operand_stack_top - 1] = (*(int16_t*)&call_stack[operand_stack_top - 1] >> call_stack[operand_stack_top]) & 0xFFFF;
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] = (*(int32_t*)&call_stack[operand_stack_top - 1] >> call_stack[operand_stack_top]) & 0xFFFFFFFF;
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_LONG:
        call_stack[operand_stack_top - 1] = (call_stack[operand_stack_top - 1] >> call_stack[operand_stack_top]) & 0xFFFFFFFFFFFFFFFF;
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_AND_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] &= *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_AND_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] &= *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_AND_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] &= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_AND_LONG:
        call_stack[operand_stack_top - 1] &= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_OR_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] |= *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_OR_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] |= *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_OR_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] |= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_OR_LEFT:
        call_stack[operand_stack_top - 1] |= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_XOR_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1] ^= *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_XOR_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1] ^= *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_XOR_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1] ^= *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BIT_XOR_LONG:
        call_stack[operand_stack_top - 1] ^= call_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_INC_BYTE:
        *(int8_t*)&vars[*(pc + 1)] += (int8_t)*(pc + 2);
        pc += 3;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_INC_SHORT:
        *(int16_t*)&vars[*(pc + 1)] += (int8_t)*(pc + 2);
        pc += 3;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_INC_INT:
        *(int32_t*)&vars[*(pc + 1)] += (int8_t)*(pc + 2);
        pc += 3;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_INC_LONG:
        vars[*(pc + 1)] += (int8_t)*(pc + 2);
        pc += 3;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_LONG:
        call_stack[operand_stack_top] = (int64_t)*(int32_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_FLOAT:
        *((float*)&call_stack[operand_stack_top]) = (float)*(int32_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_DOUBLE:
        *((double*)&call_stack[operand_stack_top]) = (double)*(int32_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_INT:
        call_stack[operand_stack_top] = (int32_t)call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_FLOAT:
        *((float*)&call_stack[operand_stack_top]) = (float)*((int64_t*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_DOUBLE:
        *((double*)&call_stack[operand_stack_top]) = (double)*((int64_t*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_INT:
        *(int32_t*)&call_stack[operand_stack_top] = (int32_t)*((float*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_LONG:
        call_stack[operand_stack_top] = (int64_t)*((float*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_DOUBLE:
        *((double*)&call_stack[operand_stack_top]) = (double)*((float*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_INT:
        *(int32_t*)&call_stack[operand_stack_top] = (int32_t)*((double*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_LONG:
        call_stack[operand_stack_top] = (int64_t)*((double*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_FLOAT:
        *((float*)&call_stack[operand_stack_top]) = (float)*((double*)&call_stack[operand_stack_top]);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_BYTE:
        *(int8_t*)&call_stack[operand_stack_top] = (int8_t)(*(int32_t*)&call_stack[operand_stack_top] & 0xFF);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_SHORT:
        *(int16_t*)&call_stack[operand_stack_top] = (int16_t)(*(int32_t*)&call_stack[operand_stack_top] & 0xFFFF);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_INT:
        *(int32_t*)&call_stack[operand_stack_top] = (int32_t)*(int8_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_INT:
        *(int32_t*)&call_stack[operand_stack_top] = (int32_t)*(int16_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_LONG:
        call_stack[operand_stack_top] = (int64_t)*(int8_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_FLOAT:
        *(float*)&call_stack[operand_stack_top] = (float)*(int8_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_DOUBLE:
        *(double*)&call_stack[operand_stack_top] = (double)*(int8_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_BYTE:
        *(int8_t*)&call_stack[operand_stack_top] = *(int16_t*)&call_stack[operand_stack_top] & 0xFF;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_LONG:
        call_stack[operand_stack_top] = (int64_t)*(int16_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_FLOAT:
        *(float*)&call_stack[operand_stack_top] = (float)*(int16_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_DOUBLE:
        *(double*)&call_stack[operand_stack_top] = (double)*(int16_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_BYTE:
        *(int8_t*)&call_stack[operand_stack_top] = call_stack[operand_stack_top] & 0xFF;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_SHORT:
        *(int16_t*)&call_stack[operand_stack_top] = call_stack[operand_stack_top] & 0xFFFF;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_BYTE:
        // TODO
        *(int8_t*)&call_stack[operand_stack_top] = (int8_t)*(float*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_SHORT:
        // TODO
        *(int16_t*)&call_stack[operand_stack_top] = (int16_t)*(float*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_BYTE:
        // TODO
        *(int8_t*)&call_stack[operand_stack_top] = (int8_t)*(double*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_SHORT:
        // TODO
        *(int8_t*)&call_stack[operand_stack_top] = (int16_t)*(double*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_SHORT:
        *(int16_t*)&call_stack[operand_stack_top] = (int16_t)*(int8_t*)&call_stack[operand_stack_top];
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BCMP:
        // z = (x > y) + (x < y) * -1
        call_stack[operand_stack_top - 1]
          = (*(int8_t*)&call_stack[operand_stack_top - 1] > *(int8_t*)&call_stack[operand_stack_top])
          + (*(int8_t*)&call_stack[operand_stack_top - 1] < *(int8_t*)&call_stack[operand_stack_top]) * -1;
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CMP_SHORT:
        // z = (x > y) + (x < y) * -1
        call_stack[operand_stack_top - 1]
          = (*(int16_t*)&call_stack[operand_stack_top - 1] > *(int16_t*)&call_stack[operand_stack_top])
          + (*(int16_t*)&call_stack[operand_stack_top - 1] < *(int16_t*)&call_stack[operand_stack_top]) * -1;
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ICMP:
        // z = (x > y) + (x < y) * -1
        call_stack[operand_stack_top - 1]
          = (*(int32_t*)&call_stack[operand_stack_top - 1] > *(int32_t*)&call_stack[operand_stack_top])
          + (*(int32_t*)&call_stack[operand_stack_top - 1] < *(int32_t*)&call_stack[operand_stack_top]) * -1;
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_FCMPL:
        call_stack[operand_stack_top - 1]
          = (*(float*)&call_stack[operand_stack_top - 1] > *(float*)&call_stack[operand_stack_top])
          + (*(float*)&call_stack[operand_stack_top - 1] < *(float*)&call_stack[operand_stack_top]) * -1
          + -!!(
            isnan(*(float*)&call_stack[operand_stack_top - 1])
            | isnan(*(float*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_FCMPG:
        call_stack[operand_stack_top - 1]
          = (*(float*)&call_stack[operand_stack_top - 1] > *(float*)&call_stack[operand_stack_top])
          + (*(float*)&call_stack[operand_stack_top - 1] < *(float*)&call_stack[operand_stack_top]) * -1
          + !!(
            isnan(*(float*)&call_stack[operand_stack_top - 1])
            | isnan(*(float*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_DCMPL:
        call_stack[operand_stack_top - 1]
          = (*(double*)&call_stack[operand_stack_top - 1] > *(double*)&call_stack[operand_stack_top])
          + (*(double*)&call_stack[operand_stack_top - 1] < *(double*)&call_stack[operand_stack_top]) * -1
          + -!!(
            isnan(*(double*)&call_stack[operand_stack_top - 1])
            | isnan(*(double*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_DCMPG:
        call_stack[operand_stack_top - 1]
          = (*(double*)&call_stack[operand_stack_top - 1] > *(double*)&call_stack[operand_stack_top])
          + (*(double*)&call_stack[operand_stack_top - 1] < *(double*)&call_stack[operand_stack_top]) * -1
          + !!(
            isnan(*(double*)&call_stack[operand_stack_top - 1])
            | isnan(*(double*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IFEQ:
        success = call_stack[operand_stack_top] == 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IFNE:
        success = call_stack[operand_stack_top] != 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IFLT:
        success = call_stack[operand_stack_top] < 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IFGE:
        success = call_stack[operand_stack_top] >= 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IFGT:
        success = call_stack[operand_stack_top] > 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IFLE:
        success = call_stack[operand_stack_top] <= 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LCMPEQ:
        success = call_stack[operand_stack_top - 1] == call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LCMPNE:
        success = call_stack[operand_stack_top - 1] != call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LCMPLT:
        success = call_stack[operand_stack_top - 1] < call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LCMPGE:
        success = call_stack[operand_stack_top - 1] >= call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LCMPGT:
        success = call_stack[operand_stack_top - 1] > call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LCMPLE:
        success = call_stack[operand_stack_top - 1] <= call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_ACMPEQ:
        success = *(void**)&call_stack[operand_stack_top - 1] == *(void**)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_ACMPNE:
        success = *(void**)&call_stack[operand_stack_top - 1] != *(void**)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_GOTO:
        pc += (int16_t)((*(pc + 1) << 8) +  *(pc + 2));
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_TABLESWITCH: {
        // Padding
        int64_t padding = (sizeof(int64_t) - 1) - ((intptr_t)pc % sizeof(int64_t));
        
        // default offset
        int64_t default_offset = *(int64_t*)(pc + padding + 1);
        
        // min
        int64_t min = *(int64_t*)(pc + padding + sizeof(int64_t) + 1);
        
        // max
        int64_t max = *(int64_t*)(pc + padding + sizeof(int64_t) * 2 + 1);
        
        if (call_stack[operand_stack_top] >= min && call_stack[operand_stack_top] <= max) {
          int64_t branch_offset = *(int64_t*)((pc + padding + sizeof(int64_t) * 3 + 1) + (call_stack[operand_stack_top] - min) * sizeof(int64_t));
          pc += branch_offset;
        }
        else {
          pc += default_offset;
        }
        
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_LOOKUPSWITCH: {

        // Padding
        int64_t padding = (sizeof(int64_t) - 1) - ((intptr_t)pc % sizeof(int64_t));

        /*
        1  default
        5  npare
        9  match  13 branch // min
        17 match 21 branch
        25 match 29 branch // max
        */
        
        // default offset
        int64_t default_offset = *(int64_t*)(pc + padding + 1);
        
        // npare
        int64_t pair_count = *(int64_t*)(pc + padding + sizeof(int64_t) + 1);
        
        // min
        int64_t min = *(int64_t*)(pc + padding + sizeof(int64_t) * 2 + 1);
        
        // max
        int64_t max = *(int64_t*)(pc + padding + sizeof(int64_t) * 2 + 1 + ((pair_count - 1) * sizeof(int64_t) * 2));
        
        if (call_stack[operand_stack_top] >= min && call_stack[operand_stack_top] <= max) {
          // 2 branch searching
          int64_t cur_min_pos = 0;
          int64_t cur_max_pos = pair_count - 1;

          while (1) {
            if (cur_max_pos < cur_min_pos) {
              pc += default_offset;
              goto *jump[*pc];
            }
            int64_t cur_half_pos = cur_min_pos + (cur_max_pos - cur_min_pos) / 2;
            int64_t cur_half = *(int64_t*)(pc + padding + sizeof(int64_t) * 2 + 1 + (cur_half_pos * sizeof(int64_t) * 2));
            
            if (call_stack[operand_stack_top] > cur_half) {
              cur_min_pos = cur_half_pos + 1;
            }
            else if (call_stack[operand_stack_top] < cur_half) {
              cur_max_pos = cur_half_pos - 1;
            }
            else {
              int64_t branch_offset = *(int64_t*)(pc + padding + sizeof(int64_t) * 2 + 1 + (cur_half_pos * sizeof(int64_t) * 2) + sizeof(int64_t));
              pc += branch_offset;
              goto *jump[*pc];
            }
          }
        }
        else {
          pc += default_offset;
        }
        
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_MALLOC_OBJECT: {
        // Get subroutine ID
        int32_t package_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_PACKAGE* constant_pool_package = (SPVM_CONSTANT_POOL_PACKAGE*)&constant_pool[package_constant_pool_address];
        
        // Allocate memory
        int32_t fields_byte_size = constant_pool_package->byte_size;
        int32_t allocate_size = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + fields_byte_size;
        void* address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
        
        // Set type
        *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_TYPE_OBJECT;
        
        // Set byte size
        *(int32_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE_BYTE_OFFSET) = allocate_size;
        
        // Set reference count
        *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET) = 0;
        
        // Initialize fields area by 0
        memset((void*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE), 0, fields_byte_size);
        
        // Push address
        operand_stack_top++;
        *(void**)&call_stack[operand_stack_top] = address;
        
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_MALLOC_ARRAY: {
        int32_t resolved_type_id = (int32_t)*(pc + 1);
        
        // Array length
        int64_t length = call_stack[operand_stack_top];
        
        // Allocate array
        void* address;
        int64_t allocate_size;
        
        if (resolved_type_id == SPVM_RESOLVED_TYPE_C_ID_BYTE) {
          allocate_size = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int8_t) * length;
          address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
          
          // Set type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_TYPE_ARRAY;

          // Set array type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE;
        }
        else if (resolved_type_id == SPVM_RESOLVED_TYPE_C_ID_SHORT) {
          allocate_size = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int16_t) * length;
          address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
          // Set type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_TYPE_ARRAY;
          // Set array type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_SHORT;
        }
        else if (resolved_type_id == SPVM_RESOLVED_TYPE_C_ID_INT) {
          allocate_size = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int32_t) * length;
          address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
          // Set type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_TYPE_ARRAY;
          // Set array type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_INT;
        }
        else if (resolved_type_id == SPVM_RESOLVED_TYPE_C_ID_LONG) {
          allocate_size = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(int64_t) * length;
          address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
          // Set type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_TYPE_ARRAY;
          // Set array type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_LONG;
        }
        else if (resolved_type_id == SPVM_RESOLVED_TYPE_C_ID_FLOAT) {
          allocate_size = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(float) * length;
          address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
          // Set type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_TYPE_ARRAY;
          // Set array type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_FLOAT;
        }
        else if (resolved_type_id == SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
          allocate_size = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(double) * length;
          address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
          // Set type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_TYPE_ARRAY;
          // Set array type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_DOUBLE;
        }
        else if (resolved_type_id == SPVM_RESOLVED_TYPE_C_ID_STRING) {
          allocate_size = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(intptr_t) * length;
          address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
          memset(address, 0, allocate_size);
          // Set type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_TYPE_ARRAY;
          // Set array type
          *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_STRING;
        }
        else {
          assert(0);
        }
        
        // Set byte size
        *(int32_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE_BYTE_OFFSET) = allocate_size;
        
        // Set reference count
        *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET) = 0;
        
        // Set array length
        *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET) = length;
        
        // Set array
        *(void**)&call_stack[operand_stack_top] = address;
        
        pc += 2;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_MALLOC_STRING: {
        int64_t* string_info_ptr = &constant_pool[(*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4)];
        
        int64_t length = string_info_ptr[0];
        int8_t* chars_ptr = (int8_t*)&string_info_ptr[1];
        
        // Allocate array
        int64_t allocate_size = SPVM_API_C_OBJECT_HEADER_BYTE_SIZE;
        void* address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
        
        // Set type
        *(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) = SPVM_API_C_OBJECT_HEADER_TYPE_STRING;

        // Set byte size
        *(int32_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE_BYTE_OFFSET) = (int32_t)allocate_size;
        
        // Set reference count
        *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET) = 0;
        
        // New sv
        SPVM_SV* sv = SPVM_SV_new_pvn(spvm, chars_ptr, length);
        
        // Set sv
        *(intmax_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET) = (intmax_t)sv;
        
        // Set array
        operand_stack_top++;
        *(void**)&call_stack[operand_stack_top] = address;
        
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_ARRAYLENGTH:
        call_stack[operand_stack_top] = *(int64_t*)(*(void**)&call_stack[operand_stack_top] + SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET);
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_WIDE:
        // iload, fload, aload, lload, dload, istore, fstore, astore, lstore, dstore, or iinc
        
        if (*(pc + 1) == SPVM_BYTECODE_C_CODE_LOAD) {
          operand_stack_top++;
          call_stack[operand_stack_top] = vars[(*(pc + 2) << 8) + *(pc + 3)];
          pc +=4;
        }
        else if (*(pc + 1) == SPVM_BYTECODE_C_CODE_STORE) {
          vars[(*(pc + 2) << 8) + *(pc + 3)] = call_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (*(pc + 1) == SPVM_BYTECODE_C_CODE_ASTORE) {
          int32_t var_index = (*(pc + 2) << 8) + *(pc + 3);

          // Increment reference count if stored object is not null
          if ((void*)call_stack[operand_stack_top] != NULL) {
            SPVM_API_inc_ref_count(spvm, env, (void*)call_stack[operand_stack_top]);
          }
          
          // Decrement reference count if original object is not null
          if ((void*)vars[var_index] != NULL) {
            SPVM_API_dec_ref_count(spvm, env, (void*)vars[var_index]);
          }
          
          // Store address
          vars[var_index] = call_stack[operand_stack_top];
          
          operand_stack_top--;
          pc +=4;
        }
        else if (*(pc + 1) == SPVM_BYTECODE_C_CODE_INC_BYTE) {
          *(int8_t*)&vars[(*(pc + 2) << 8) + *(pc + 3)] += (int16_t)((*(pc + 4) << 8) + *(pc + 5));
          pc += 6;
        }
        else if (*(pc + 1) == SPVM_BYTECODE_C_CODE_INC_SHORT) {
          *(int16_t*)&vars[(*(pc + 2) << 8) + *(pc + 3)] += (int16_t)((*(pc + 4) << 8) + *(pc + 5));
          pc += 6;
        }
        else if (*(pc + 1) == SPVM_BYTECODE_C_CODE_INC_INT) {
          *(int32_t*)&vars[(*(pc + 2) << 8) + *(pc + 3)] += (int16_t)((*(pc + 4) << 8) + *(pc + 5));
          pc += 6;
        }
        else if (*(pc + 1) == SPVM_BYTECODE_C_CODE_INC_LONG) {
          vars[(*(pc + 2) << 8) + *(pc + 3)] += (int16_t)((*(pc + 4) << 8) + *(pc + 5));
          pc += 6;
        }
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IFNULL:
        success = *(void**)&call_stack[operand_stack_top] == (void*)NULL;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IFNONNULL:
        success = *(void**)&call_stack[operand_stack_top] != (void*)NULL;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_BGETFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int8_t*)&call_stack[operand_stack_top] = *(int8_t*)(*(void**)&call_stack[operand_stack_top] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_SGETFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int16_t*)&call_stack[operand_stack_top] = *(int16_t*)(*(void**)&call_stack[operand_stack_top] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_IGETFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)(*(void**)&call_stack[operand_stack_top] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_LGETFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int64_t*)&call_stack[operand_stack_top] = *(int64_t*)(*(void**)&call_stack[operand_stack_top] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_FGETFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(float*)&call_stack[operand_stack_top] = *(float*)(*(void**)&call_stack[operand_stack_top] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_DGETFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(double*)&call_stack[operand_stack_top] = *(double*)(*(void**)&call_stack[operand_stack_top] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_BPUTFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int8_t*)(*(void**)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset) = *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_SPUTFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int16_t*)(*(void**)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset) = *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_IPUTFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int32_t*)(*(void**)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset) = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_LPUTFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int64_t*)(*(void**)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset) = *(int64_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_FPUTFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(float*)(*(void**)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset) = *(float*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_DPUTFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(double*)(*(void**)&call_stack[operand_stack_top - 1] + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + constant_pool_field->package_byte_offset) = *(double*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_APUTFIELD: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        
        intptr_t field_index = (intptr_t)*(void**)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset;

        // Increment reference count if stored object is not null
        if (*(void**)&call_stack[operand_stack_top] != NULL) {
          SPVM_API_inc_ref_count(spvm, env, *(void**)&call_stack[operand_stack_top]);
        }
        
        // Decrement reference count if original object is not null
        if (*(void**)(field_index) != NULL) {
          SPVM_API_dec_ref_count(spvm, env, *(void**)(field_index));
        }
        
        // Store object
        *(void**)(field_index) = *(void**)&call_stack[operand_stack_top];
        
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
    // }
  // }
}

void SPVM_API_dec_ref_count(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  if (address != NULL) {
    
    assert(*(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET) > 0);
    
    // Decrement reference count
    *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET) -= 1;
    
    // If reference count is zero, free address.
    if (*(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET) == 0) {
      
      // Object is string
      if (*(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_TYPE_BYTE_OFFSET) == SPVM_API_C_OBJECT_HEADER_TYPE_STRING) {
        
        SPVM_SV* sv = *(intmax_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET);
        
        sv->ref_count--;
        
        SPVM_ALLOCATOR_RUNTIME_free_address(spvm, spvm->allocator_runtime, address);
      }
      // Object is array of string
      else if (*(int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_BYTE_OFFSET) == SPVM_API_C_OBJECT_HEADER_ARRAY_TYPE_STRING) {
        
        // Array length
        int64_t length = *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET);
        
        for (int64_t i = 0; i < length; i++) {
          void* element_address = *(void**)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE + sizeof(void*) * i);
          SPVM_API_dec_ref_count(spvm, env, element_address);
        }
        
        SPVM_ALLOCATOR_RUNTIME_free_address(spvm, spvm->allocator_runtime, address);
      }
      else {
        SPVM_ALLOCATOR_RUNTIME_free_address(spvm, spvm->allocator_runtime, address);
      }
    }
  }
}

void SPVM_API_inc_ref_count(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  assert(address);
  assert(*(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET) >= 0);
  
  // Decrement reference count
  *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET) += 1;
}

void SPVM_API_push_return_value_byte(SPVM* spvm, SPVM_ENV* env, int8_t value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(int8_t*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_return_value_short(SPVM* spvm, SPVM_ENV* env, int16_t value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(int16_t*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_return_value_int(SPVM* spvm, SPVM_ENV* env, int32_t value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(int32_t*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_return_value_long(SPVM* spvm, SPVM_ENV* env, int64_t value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(int64_t*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_return_value_float(SPVM* spvm, SPVM_ENV* env, float value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(float*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_return_value_double(SPVM* spvm, SPVM_ENV* env, double value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(double*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_ret(SPVM* spvm, SPVM_ENV* env, void* value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(void**)&env->call_stack[env->operand_stack_top] = value;
}

int8_t SPVM_API_pop_return_value_byte(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  int8_t ret = *(int8_t*)&env->call_stack[env->operand_stack_top];
  env->operand_stack_top--;
  return ret;
}

int16_t SPVM_API_pop_return_value_short(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  int16_t ret = *(int16_t*)&env->call_stack[env->operand_stack_top];
  env->operand_stack_top--;
  return ret;
}

int32_t SPVM_API_pop_return_value_int(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  int32_t ret = *(int32_t*)&env->call_stack[env->operand_stack_top];
  env->operand_stack_top--;
  return ret;
}

int64_t SPVM_API_pop_return_value_long(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;

  int64_t ret = *(int64_t*)&env->call_stack[env->operand_stack_top];
  env->operand_stack_top--;
  return ret;
}

float SPVM_API_pop_return_value_float(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  float ret = *(float*)&env->call_stack[env->operand_stack_top];
  env->operand_stack_top--;
  return ret;
}

double SPVM_API_pop_return_value_double(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  double ret = *(double*)&env->call_stack[env->operand_stack_top];
  env->operand_stack_top--;
  return ret;
}

void* SPVM_API_pop_return_value_address(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  void* ret = *(void**)&env->call_stack[env->operand_stack_top];
  env->operand_stack_top--;
  return ret;
}

int8_t SPVM_API_get_var_byte(SPVM* spvm, SPVM_ENV* env, int64_t index) {
  (void)spvm;
  (void)env;
  
  return *(int8_t*)&env->call_stack[env->call_stack_base + (size_t)index];
}

int16_t SPVM_API_get_var_short(SPVM* spvm, SPVM_ENV* env, int64_t index) {
  (void)spvm;
  (void)env;
  
  return *(int16_t*)&env->call_stack[env->call_stack_base + (size_t)index];
}

int32_t SPVM_API_get_var_int(SPVM* spvm, SPVM_ENV* env, int64_t index) {
  (void)spvm;
  (void)env;
  
  return *(int32_t*)&env->call_stack[env->call_stack_base + (size_t)index];
}

int64_t SPVM_API_get_var_long(SPVM* spvm, SPVM_ENV* env, int64_t index) {
  (void)spvm;
  (void)env;
  
  return *(int64_t*)&env->call_stack[env->call_stack_base + (size_t)index];
}

float SPVM_API_get_var_float(SPVM* spvm, SPVM_ENV* env, int64_t index) {
  (void)spvm;
  (void)env;
  
  return *(float*)&env->call_stack[env->call_stack_base + (size_t)index];
}

double SPVM_API_get_var_double(SPVM* spvm, SPVM_ENV* env, int64_t index) {
  (void)spvm;
  (void)env;
  
  return *(double*)&env->call_stack[env->call_stack_base + (size_t)index];
}

void* SPVM_API_get_var_address(SPVM* spvm, SPVM_ENV* env, int64_t index) {
  (void)spvm;
  (void)env;
  
  return *(void**)&env->call_stack[env->call_stack_base + (size_t)index];
}

void SPVM_API_push_var_byte(SPVM* spvm, SPVM_ENV* env, int8_t value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(int8_t*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_var_short(SPVM* spvm, SPVM_ENV* env, int16_t value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(int16_t*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_var_int(SPVM* spvm, SPVM_ENV* env, int32_t value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(int32_t*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_var_long(SPVM* spvm, SPVM_ENV* env, int64_t value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(int64_t*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_var_float(SPVM* spvm, SPVM_ENV* env, float value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(float*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_var_double(SPVM* spvm, SPVM_ENV* env, double value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(double*)&env->call_stack[env->operand_stack_top] = value;
}

void SPVM_API_push_var_address(SPVM* spvm, SPVM_ENV* env, void* value) {
  (void)spvm;
  (void)env;
  
  env->operand_stack_top++;
  *(void**)&env->call_stack[env->operand_stack_top] = value;
}

int64_t SPVM_API_get_array_length(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET);
}

int64_t SPVM_API_get_ref_count(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return *(int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_REF_COUNT_BYTE_OFFSET);
}

SPVM_SV* SPVM_API_get_string_sv(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (SPVM_SV*)*(intmax_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_LENGTH_OR_ADDRESS_BYTE_OFFSET);
}

int8_t* SPVM_API_get_array_byte_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (int8_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE);
}

int16_t* SPVM_API_get_array_short_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (int16_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE);
}

int32_t* SPVM_API_get_array_int_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (int32_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE);
}

int64_t* SPVM_API_get_array_long_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (int64_t*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE);
}

float* SPVM_API_get_array_float_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (float*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE);
}

double* SPVM_API_get_array_double_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (double*)((intptr_t)address + SPVM_API_C_OBJECT_HEADER_BYTE_SIZE);
}
