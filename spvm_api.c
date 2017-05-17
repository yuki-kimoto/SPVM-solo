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
#include "spvm_compat.h"
#include "spvm_data_object_header.h"


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
    &&case_SPVM_BYTECODE_C_CODE_ADD_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_ADD_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_ADD_INT,
    &&case_SPVM_BYTECODE_C_CODE_ADD_LONG,
    &&case_SPVM_BYTECODE_C_CODE_ADD_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_ADD_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_INT,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_LONG,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_SUBTRACT_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_INT,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_LONG,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_MULTIPLY_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_INT,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_LONG,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_DIVIDE_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_INT,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_LONG,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_REMAINDER_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_INT,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_LONG,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_NEGATE_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_INT,
    &&case_SPVM_BYTECODE_C_CODE_LEFT_SHIFT_LONG,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_INT,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_LONG,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_INT,
    &&case_SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_LONG,
    &&case_SPVM_BYTECODE_C_CODE_BIT_AND_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_BIT_AND_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_AND_INT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_AND_LONG,
    &&case_SPVM_BYTECODE_C_CODE_BIT_OR_BYTE ,
    &&case_SPVM_BYTECODE_C_CODE_BIT_OR_SHORT ,
    &&case_SPVM_BYTECODE_C_CODE_BIT_OR_INT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_OR_LONG,
    &&case_SPVM_BYTECODE_C_CODE_BIT_XOR_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_BIT_XOR_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_XOR_INT,
    &&case_SPVM_BYTECODE_C_CODE_BIT_XOR_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_INT,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_INC_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_INC_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_INC_INT,
    &&case_SPVM_BYTECODE_C_CODE_INC_LONG,
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
    &&case_SPVM_BYTECODE_C_CODE_STORE_ADDRESS,
    &&case_SPVM_BYTECODE_C_CODE_POP,
    &&case_SPVM_BYTECODE_C_CODE_POP_ADDRESS,
    &&case_SPVM_BYTECODE_C_CODE_MALLOC_OBJECT,
    &&case_SPVM_BYTECODE_C_CODE_MALLOC_STRING,
    &&case_SPVM_BYTECODE_C_CODE_MALLOC_ARRAY_NUMERIC,
    &&case_SPVM_BYTECODE_C_CODE_MALLOC_ARRAY_STRING,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_INT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_LONG,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_ADDRESS,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_INT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_LONG,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_ADDRESS,
    &&case_SPVM_BYTECODE_C_CODE_ARRAY_LENGTH,
    &&case_SPVM_BYTECODE_C_CODE_GET_FIELD_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_GET_FIELD_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_GET_FIELD_INT,
    &&case_SPVM_BYTECODE_C_CODE_GET_FIELD_LONG,
    &&case_SPVM_BYTECODE_C_CODE_GET_FIELD_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_GET_FIELD_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_GET_FIELD_ADDRESS,
    &&case_SPVM_BYTECODE_C_CODE_SET_FIELD_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_SET_FIELD_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_SET_FIELD_INT,
    &&case_SPVM_BYTECODE_C_CODE_SET_FIELD_LONG,
    &&case_SPVM_BYTECODE_C_CODE_SET_FIELD_FLOAT,
    &&case_SPVM_BYTECODE_C_CODE_SET_FIELD_DOUBLE,
    &&case_SPVM_BYTECODE_C_CODE_SET_FIELD_ADDRESS,
    &&case_SPVM_BYTECODE_C_CODE_CMP_BYTE,
    &&case_SPVM_BYTECODE_C_CODE_CMP_SHORT,
    &&case_SPVM_BYTECODE_C_CODE_CMP_FLOAT_L,
    &&case_SPVM_BYTECODE_C_CODE_CMP_FLOAT_G,
    &&case_SPVM_BYTECODE_C_CODE_CMP_DOUBLE_L,
    &&case_SPVM_BYTECODE_C_CODE_CMP_DOUBLE_G,
    &&case_SPVM_BYTECODE_C_CODE_IF_EQ_ZERO_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_NE_ZERO_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_LT_ZERO_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_GE_ZERO_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_GT_ZERO_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_LE_ZERO_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_EQ_CMP_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_NE_CMP_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_LT_CMP_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_GE_CMP_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_GT_CMP_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_LE_CMP_INT,
    &&case_SPVM_BYTECODE_C_CODE_IF_NE_ZERO_LONG,
    &&case_SPVM_BYTECODE_C_CODE_IF_EQ_CMP_LONG,
    &&case_SPVM_BYTECODE_C_CODE_IF_NE_CMP_LONG,
    &&case_SPVM_BYTECODE_C_CODE_IF_LT_CMP_LONG,
    &&case_SPVM_BYTECODE_C_CODE_IF_GE_CMP_LONG,
    &&case_SPVM_BYTECODE_C_CODE_IF_GT_CMP_LONG,
    &&case_SPVM_BYTECODE_C_CODE_IF_LE_CMP_LONG,
    &&case_SPVM_BYTECODE_C_CODE_IF_EQ_CMP_ADDRESS,
    &&case_SPVM_BYTECODE_C_CODE_IF_NE_CMP_ADDRESS,
    &&case_SPVM_BYTECODE_C_CODE_IF_NULL,
    &&case_SPVM_BYTECODE_C_CODE_IF_NON_NULL,
    &&case_SPVM_BYTECODE_C_CODE_TABLE_SWITCH_INT,
    &&case_SPVM_BYTECODE_C_CODE_LOOKUP_SWITCH_INT,
    &&case_SPVM_BYTECODE_C_CODE_TABLE_SWITCH_LONG,
    &&case_SPVM_BYTECODE_C_CODE_LOOKUP_SWITCH_LONG,
    &&case_SPVM_BYTECODE_C_CODE_GOTO,
    &&case_SPVM_BYTECODE_C_CODE_CALL_SUB,
    &&case_SPVM_BYTECODE_C_CODE_RETURN_VOID,
    &&case_SPVM_BYTECODE_C_CODE_RETURN,
    &&case_SPVM_BYTECODE_C_CODE_DIE,
    &&case_SPVM_BYTECODE_C_CODE_INC_REF_COUNT,
    &&case_SPVM_BYTECODE_C_CODE_DEC_REF_COUNT,
    &&case_SPVM_BYTECODE_C_CODE_WIDE,
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
  
  register int32_t success;
  
  int32_t call_stack_base = env->call_stack_base;
  int32_t call_stack_base_start = call_stack_base;
  
  SPVM_ALLOCATOR_RUNTIME* allocator = spvm->allocator_runtime;
  
  SPVM_CONSTANT_POOL_SUB* constant_pool_sub = NULL;
  
  // Goto subroutine
  goto CALLSUB_COMMON;
  
  // while (1) {
    // switch (*pc) {
      case_SPVM_BYTECODE_C_CODE_CALL_SUB:
      {
        // Get subroutine ID
        sub_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        
        CALLSUB_COMMON: {

          constant_pool_sub = (SPVM_CONSTANT_POOL_SUB*)&constant_pool[sub_constant_pool_address];

          // Extend call stack(current size + 2(return address + call stack base before) + lexical variable area + operand_stack area)
          int32_t call_stack_max = operand_stack_top + 2 + constant_pool_sub->my_vars_length + constant_pool_sub->operand_stack_max;
          
          while (call_stack_max > env->call_stack_capacity) {
            env->call_stack_capacity = env->call_stack_capacity * 2;
            env->call_stack = call_stack = realloc(call_stack, sizeof(intmax_t) * env->call_stack_capacity);
          }

          operand_stack_top -= constant_pool_sub->args_length;

          // Prepare arguments
          memmove(&call_stack[operand_stack_top + 4], &call_stack[operand_stack_top + 1], constant_pool_sub->args_length * sizeof(intmax_t));

          // Save return address(operand + (throw or goto exception handler))
          if (call_stack_base == call_stack_base_start) {
            *(void**)&call_stack[operand_stack_top + 1] = (void*)-1;
          }
          else {
            *(void**)&call_stack[operand_stack_top + 1] = (void*)((intptr_t)pc + 5 + 3);
          }
          
          // Save sub_constant_pool_address
          *(int32_t*)&call_stack[operand_stack_top + 2] = sub_constant_pool_address;
          
          // Save vars base before
          *(int32_t*)&call_stack[operand_stack_top + 3] = call_stack_base;
          
          // Set vars base
          call_stack_base = operand_stack_top + 4;
          
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
              goto case_SPVM_BYTECODE_C_CODE_DIE;
            }
            else {
              if (constant_pool_sub->has_return_value) {
                goto case_SPVM_BYTECODE_C_CODE_RETURN;
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
      case_SPVM_BYTECODE_C_CODE_RETURN: {
        
        // Get return value
        intmax_t return_value = *(intmax_t*)&call_stack[operand_stack_top];
        
        // Restore operand stack top
        operand_stack_top = call_stack_base - 4;
        
        // Get return address
        uint8_t* return_address = *(uint8_t**)&call_stack[call_stack_base - 3];
        
        // Get sub_constant_pool_address
        sub_constant_pool_address = *(int32_t*)&call_stack[call_stack_base - 2];
        
        // Resotre call stack base
        call_stack_base = *(int32_t*)&call_stack[call_stack_base - 1];
        
        // Push return value
        operand_stack_top++;
        *(intmax_t*)&call_stack[operand_stack_top] = return_value;
        
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
        operand_stack_top = call_stack_base - 4;
        
        // Return address
        uint8_t* return_address = *(uint8_t**)&call_stack[call_stack_base - 3];

        // Get sub_constant_pool_address
        sub_constant_pool_address = *(int32_t*)&call_stack[call_stack_base - 2];

        // Resotre vars base
        call_stack_base = *(int32_t*)&call_stack[call_stack_base - 1];
        
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
      case_SPVM_BYTECODE_C_CODE_DIE: {
        
        // Return value
        void* return_value = *(void**)&call_stack[operand_stack_top];
        
        // Restore operand stack top
        operand_stack_top = call_stack_base - 4;
        
        // Return address
        uint8_t* return_address = *(uint8_t**)&call_stack[call_stack_base - 3];

        // Get sub_constant_pool_address
        sub_constant_pool_address = *(int32_t*)&call_stack[call_stack_base - 2];
        
        // Get constant pool sub
        constant_pool_sub = (SPVM_CONSTANT_POOL_SUB*)&constant_pool[sub_constant_pool_address];
        
        // Sub name
        int32_t sub_name_constant_pool_address = constant_pool_sub->abs_name_constant_pool_address;
        const char* sub_name = SPVM_CONSTANT_POOL_get_string_value(spvm, spvm->constant_pool, sub_name_constant_pool_address);
        
        // File name
        int32_t file_name_constant_pool_address = constant_pool_sub->file_name_constant_pool_address;
        const char* file_name = SPVM_CONSTANT_POOL_get_string_value(spvm, spvm->constant_pool, file_name_constant_pool_address);
        
        SPVM_SV* sv_message = SPVM_API_get_string_sv(spvm, env, return_value);
        
        const char* pv_message = SPVM_API_get_string_value(spvm, env, return_value);
        
        // Free string if need
        if (return_value != NULL) {
          int64_t ref_count = SPVM_API_get_ref_count(spvm, env, return_value);
          if (ref_count == 0) {
            SPVM_SvREFCNT_dec(sv_message);
            SPVM_ALLOCATOR_RUNTIME_free_address(spvm, spvm->allocator_runtime, return_value);
          }
        }
        
        // New sv
        SPVM_SV* new_sv_message = SPVM_COMPAT_newSVpvn(spvm, pv_message, strlen(pv_message));
        
        void* new_message_address = SPVM_API_create_string_sv(spvm, env, new_sv_message);
        
        SPVM_COMPAT_sv_catpvn(new_sv_message, "\n from ", strlen("\n from "));
        SPVM_COMPAT_sv_catpvn(new_sv_message, sub_name, strlen(sub_name));
        SPVM_COMPAT_sv_catpvn(new_sv_message, " at ", strlen(" at "));
        SPVM_COMPAT_sv_catpvn(new_sv_message, file_name, strlen(file_name));
        
        // Resotre vars base
        call_stack_base = call_stack[call_stack_base - 1];
        
        // Push return value
        operand_stack_top++;
        *(void**)&call_stack[operand_stack_top] = new_message_address;
        
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
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_BYTE:
        *(int8_t*)&call_stack[operand_stack_top - 1]
          = *(int8_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(int8_t) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_SHORT:
        *(int16_t*)&call_stack[operand_stack_top - 1]
          = *(int16_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(int16_t) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_INT:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = *(int32_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(int32_t) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_LONG: {
        *(int64_t*)&call_stack[operand_stack_top - 1]
          = *(int64_t*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(int64_t) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_FLOAT:
        *(float*)&call_stack[operand_stack_top - 1]
          = *(float*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(float) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_DOUBLE:
        *(double*)&call_stack[operand_stack_top - 1]
          = *(double*)(*(intptr_t*)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(double) * (size_t)call_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_LOAD_ADDRESS:
        *(void**)&call_stack[operand_stack_top - 1]
          = *(void**)(*(intptr_t*)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(void*) * (size_t)call_stack[operand_stack_top]);
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
      case_SPVM_BYTECODE_C_CODE_STORE_ADDRESS: {
        int32_t vars_index = *(pc + 1);

        // Increment reference count
        SPVM_API_inc_ref_count(spvm, env, (void*)call_stack[operand_stack_top]);
        
        // Decrement reference count
        SPVM_API_dec_ref_count(spvm, env, (void*)vars[vars_index]);
        
        // Store address
        vars[vars_index] = call_stack[operand_stack_top];
        
        operand_stack_top--;
        pc += 2;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_DEC_REF_COUNT: {
        void* address = (void*)call_stack[operand_stack_top];
        
        // Decrement reference count
        SPVM_API_dec_ref_count(spvm, env, address);
        
        pc += 1;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_INC_REF_COUNT: {
        void* address = (void*)call_stack[operand_stack_top];
        
        // Increment reference count
        SPVM_API_inc_ref_count(spvm, env, address);
        
        pc += 1;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_BYTE:
        *(int8_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(int8_t) * (size_t)call_stack[operand_stack_top - 1])
          = *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_SHORT:
        *(int16_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(int16_t) * (size_t)call_stack[operand_stack_top - 1])
          = *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_INT:
        *(int32_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(int32_t) * (size_t)call_stack[operand_stack_top - 1])
          = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_LONG:
        *(int64_t*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(int64_t) * (size_t)call_stack[operand_stack_top - 1])
          = call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_FLOAT:
        *(float*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(float) * (size_t)call_stack[operand_stack_top - 1])
          = *(float*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_DOUBLE:
        *(double*)(*(intptr_t*)&call_stack[operand_stack_top - 2] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(double) * (size_t)call_stack[operand_stack_top - 1])
          = *(double*)&call_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_ARRAY_STORE_ADDRESS: {
        intptr_t array_index = *(intptr_t*)&call_stack[operand_stack_top - 2] + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(void*) * (size_t)call_stack[operand_stack_top - 1];

        // Increment reference count
        SPVM_API_inc_ref_count(spvm, env, *(void**)&call_stack[operand_stack_top]);
        
        // Decrement reference count
        SPVM_API_dec_ref_count(spvm, env, *(void**)(array_index));
        
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
      case_SPVM_BYTECODE_C_CODE_POP_ADDRESS: {
        
        void* address = (void*)call_stack[operand_stack_top];
        
        // Free object
        if (address != NULL) {
          int64_t ref_count = ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count;
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
      case_SPVM_BYTECODE_C_CODE_BIT_OR_LONG:
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
      case_SPVM_BYTECODE_C_CODE_CMP_BYTE:
        // z = (x > y) + (x < y) * -1
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(int8_t*)&call_stack[operand_stack_top - 1] > *(int8_t*)&call_stack[operand_stack_top])
          + (*(int8_t*)&call_stack[operand_stack_top - 1] < *(int8_t*)&call_stack[operand_stack_top]) * -1;
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CMP_SHORT:
        // z = (x > y) + (x < y) * -1
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(int16_t*)&call_stack[operand_stack_top - 1] > *(int16_t*)&call_stack[operand_stack_top])
          + (*(int16_t*)&call_stack[operand_stack_top - 1] < *(int16_t*)&call_stack[operand_stack_top]) * -1;
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CMP_FLOAT_L:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(float*)&call_stack[operand_stack_top - 1] > *(float*)&call_stack[operand_stack_top])
          + (*(float*)&call_stack[operand_stack_top - 1] < *(float*)&call_stack[operand_stack_top]) * -1
          + -!!(
            isnan(*(float*)&call_stack[operand_stack_top - 1])
            | isnan(*(float*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CMP_FLOAT_G:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(float*)&call_stack[operand_stack_top - 1] > *(float*)&call_stack[operand_stack_top])
          + (*(float*)&call_stack[operand_stack_top - 1] < *(float*)&call_stack[operand_stack_top]) * -1
          + !!(
            isnan(*(float*)&call_stack[operand_stack_top - 1])
            | isnan(*(float*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CMP_DOUBLE_L:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(double*)&call_stack[operand_stack_top - 1] > *(double*)&call_stack[operand_stack_top])
          + (*(double*)&call_stack[operand_stack_top - 1] < *(double*)&call_stack[operand_stack_top]) * -1
          + -!!(
            isnan(*(double*)&call_stack[operand_stack_top - 1])
            | isnan(*(double*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_CMP_DOUBLE_G:
        *(int32_t*)&call_stack[operand_stack_top - 1]
          = (*(double*)&call_stack[operand_stack_top - 1] > *(double*)&call_stack[operand_stack_top])
          + (*(double*)&call_stack[operand_stack_top - 1] < *(double*)&call_stack[operand_stack_top]) * -1
          + !!(
            isnan(*(double*)&call_stack[operand_stack_top - 1])
            | isnan(*(double*)&call_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_EQ_ZERO_INT:
        success = *(int32_t*)&call_stack[operand_stack_top] == 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_NE_ZERO_INT:
        success = *(int32_t*)&call_stack[operand_stack_top] != 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LT_ZERO_INT:
        success = *(int32_t*)&call_stack[operand_stack_top] < 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_GE_ZERO_INT:
        success = *(int32_t*)&call_stack[operand_stack_top] >= 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_GT_ZERO_INT:
        success = *(int32_t*)&call_stack[operand_stack_top] > 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LE_ZERO_INT:
        success = *(int32_t*)&call_stack[operand_stack_top] <= 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_EQ_CMP_INT:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] == *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_NE_CMP_INT:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] != *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LT_CMP_INT:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] < *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_GE_CMP_INT:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] >= *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_GT_CMP_INT:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] > *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LE_CMP_INT:
        success = *(int32_t*)&call_stack[operand_stack_top - 1] <= *(int32_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_NE_ZERO_LONG:
        success = *(int64_t*)&call_stack[operand_stack_top] != 0;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_EQ_CMP_LONG:
        success = *(int64_t*)&call_stack[operand_stack_top - 1] == *(int64_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_NE_CMP_LONG:
        success = *(int64_t*)&call_stack[operand_stack_top - 1] != *(int64_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LT_CMP_LONG:
        success = *(int64_t*)&call_stack[operand_stack_top - 1] < *(int64_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_GE_CMP_LONG:
        success = *(int64_t*)&call_stack[operand_stack_top - 1] >= *(int64_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_GT_CMP_LONG:
        success = *(int64_t*)&call_stack[operand_stack_top - 1] > *(int64_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_LE_CMP_LONG:
        success = *(int64_t*)&call_stack[operand_stack_top - 1] <= *(int64_t*)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_EQ_CMP_ADDRESS:
        success = *(void**)&call_stack[operand_stack_top - 1] == *(void**)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_NE_CMP_ADDRESS:
        success = *(void**)&call_stack[operand_stack_top - 1] != *(void**)&call_stack[operand_stack_top];
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top -= 2;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_GOTO:
        pc += (int16_t)((*(pc + 1) << 8) +  *(pc + 2));
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_TABLE_SWITCH_INT: {
        // Padding
        int32_t padding = (sizeof(int32_t) - 1) - ((intptr_t)pc % sizeof(int32_t));
        
        // default offset
        int32_t default_offset = *(int32_t*)(pc + padding + 1);
        
        // min
        int32_t min = *(int32_t*)(pc + padding + sizeof(int32_t) + 1);
        
        // max
        int32_t max = *(int32_t*)(pc + padding + sizeof(int32_t) * 2 + 1);
        
        if (call_stack[operand_stack_top] >= min && call_stack[operand_stack_top] <= max) {
          int32_t branch_offset = *(int32_t*)((pc + padding + sizeof(int32_t) * 3 + 1) + (call_stack[operand_stack_top] - min) * sizeof(int32_t));
          pc += branch_offset;
        }
        else {
          pc += default_offset;
        }
        
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_LOOKUP_SWITCH_INT: {

        // Padding
        int32_t padding = (sizeof(int32_t) - 1) - ((intptr_t)pc % sizeof(int32_t));

        /*
        1  default
        5  npare
        9  match  13 branch // min
        17 match 21 branch
        25 match 29 branch // max
        */
        
        // default offset
        int32_t default_offset = *(int32_t*)(pc + padding + 1);
        
        // npare
        int32_t pair_count = *(int32_t*)(pc + padding + sizeof(int32_t) + 1);
        
        // min
        int32_t min = *(int32_t*)(pc + padding + sizeof(int32_t) * 2 + 1);
        
        // max
        int32_t max = *(int32_t*)(pc + padding + sizeof(int32_t) * 2 + 1 + ((pair_count - 1) * sizeof(int32_t) * 2));
        
        if (call_stack[operand_stack_top] >= min && call_stack[operand_stack_top] <= max) {
          // 2 branch searching
          int32_t cur_min_pos = 0;
          int32_t cur_max_pos = pair_count - 1;

          while (1) {
            if (cur_max_pos < cur_min_pos) {
              pc += default_offset;
              goto *jump[*pc];
            }
            int32_t cur_half_pos = cur_min_pos + (cur_max_pos - cur_min_pos) / 2;
            int32_t cur_half = *(int32_t*)(pc + padding + sizeof(int32_t) * 2 + 1 + (cur_half_pos * sizeof(int32_t) * 2));
            
            if (call_stack[operand_stack_top] > cur_half) {
              cur_min_pos = cur_half_pos + 1;
            }
            else if (call_stack[operand_stack_top] < cur_half) {
              cur_max_pos = cur_half_pos - 1;
            }
            else {
              int32_t branch_offset = *(int32_t*)(pc + padding + sizeof(int32_t) * 2 + 1 + (cur_half_pos * sizeof(int32_t) * 2) + sizeof(int32_t));
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
      case_SPVM_BYTECODE_C_CODE_TABLE_SWITCH_LONG: {
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
      case_SPVM_BYTECODE_C_CODE_LOOKUP_SWITCH_LONG: {

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
        int32_t allocate_size = sizeof(SPVM_DATA_OBJECT_HEADER) + fields_byte_size;
        void* address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
        
        // Set type
        ((SPVM_DATA_OBJECT_HEADER*)address)->type = SPVM_DATA_OBJECT_HEADER_C_TYPE_OBJECT;
        
        // Set byte size
        ((SPVM_DATA_OBJECT_HEADER*)address)->byte_size = allocate_size;
        
        // Set reference count
        ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count = 0;
        
        // Initialize fields area by 0
        memset((void*)((intptr_t)address + sizeof(SPVM_DATA_OBJECT_HEADER)), 0, fields_byte_size);
        
        // Push address
        operand_stack_top++;
        *(void**)&call_stack[operand_stack_top] = address;
        
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_MALLOC_STRING: {
        int64_t* string_info_ptr = &constant_pool[(*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4)];
        
        int64_t length = string_info_ptr[0];
        char* pv = (int8_t*)&string_info_ptr[1];

        // New sv
        SPVM_SV* sv = SPVM_COMPAT_newSVpvn(spvm, pv, length);
        
        // Create string
        void* address = SPVM_API_create_string_sv(spvm, env, sv);
        
        // Set string
        operand_stack_top++;
        *(void**)&call_stack[operand_stack_top] = address;
        
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_MALLOC_ARRAY_NUMERIC: {
        int32_t size = (int32_t)*(pc + 1);
        
        // Array length
        int64_t length = *(int64_t*)&call_stack[operand_stack_top];
        
        // Allocate array
        int64_t allocate_size = sizeof(SPVM_DATA_OBJECT_HEADER) + size * length;
        SPVM_DATA_OBJECT_HEADER* array = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
        
        // Set type
        array->type = SPVM_DATA_OBJECT_HEADER_C_TYPE_ARRAY_NUMERIC;
        
        // Set byte size
        array->byte_size = allocate_size;
        
        // Set reference count
        array->ref_count = 0;
        
        // Set array length
        array->array_length_or_sv = length;
        
        // Set array
        *(void**)&call_stack[operand_stack_top] = array;
        
        pc += 2;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_MALLOC_ARRAY_STRING: {
        // Array length
        int64_t length = call_stack[operand_stack_top];
        
        // Allocate array
        int64_t allocate_size = sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(intptr_t) * length;
        void* address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
        memset(address, 0, allocate_size);
        
        // Set type
        ((SPVM_DATA_OBJECT_HEADER*)address)->type = SPVM_DATA_OBJECT_HEADER_C_TYPE_ARRAY_STRING;
        
        // Set byte size
        ((SPVM_DATA_OBJECT_HEADER*)address)->byte_size = allocate_size;
        
        // Set reference count
        ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count = 0;
        
        // Set array length
        ((SPVM_DATA_OBJECT_HEADER*)address)->array_length_or_sv = length;
        
        // Set array
        *(void**)&call_stack[operand_stack_top] = address;
        
        pc += 1;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_ARRAY_LENGTH:
        *(int64_t*)&call_stack[operand_stack_top] = ((SPVM_DATA_OBJECT_HEADER*)*(void**)&call_stack[operand_stack_top])->array_length_or_sv;
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
        else if (*(pc + 1) == SPVM_BYTECODE_C_CODE_STORE_ADDRESS) {
          int32_t var_index = (*(pc + 2) << 8) + *(pc + 3);

          // Increment reference count
          SPVM_API_inc_ref_count(spvm, env, (void*)call_stack[operand_stack_top]);
          
          // Decrement reference count if original object is not null
          SPVM_API_dec_ref_count(spvm, env, (void*)vars[var_index]);
          
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
      case_SPVM_BYTECODE_C_CODE_IF_NULL:
        success = *(void**)&call_stack[operand_stack_top] == (void*)NULL;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_IF_NON_NULL:
        success = *(void**)&call_stack[operand_stack_top] != (void*)NULL;
        pc += success * (int16_t)((*(pc + 1) << 8) +  *(pc + 2)) + (~success & 1) * 3;
        operand_stack_top--;
        goto *jump[*pc];
      case_SPVM_BYTECODE_C_CODE_GET_FIELD_BYTE: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int8_t*)&call_stack[operand_stack_top] = *(int8_t*)(*(void**)&call_stack[operand_stack_top] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_GET_FIELD_SHORT: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int16_t*)&call_stack[operand_stack_top] = *(int16_t*)(*(void**)&call_stack[operand_stack_top] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_GET_FIELD_INT: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int32_t*)&call_stack[operand_stack_top] = *(int32_t*)(*(void**)&call_stack[operand_stack_top] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_GET_FIELD_LONG: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int64_t*)&call_stack[operand_stack_top] = *(int64_t*)(*(void**)&call_stack[operand_stack_top] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_GET_FIELD_FLOAT: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(float*)&call_stack[operand_stack_top] = *(float*)(*(void**)&call_stack[operand_stack_top] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_GET_FIELD_DOUBLE: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(double*)&call_stack[operand_stack_top] = *(double*)(*(void**)&call_stack[operand_stack_top] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_GET_FIELD_ADDRESS: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(void**)&call_stack[operand_stack_top] = *(void**)(*(void**)&call_stack[operand_stack_top] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset);
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_SET_FIELD_BYTE: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int8_t*)(*(void**)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset) = *(int8_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_SET_FIELD_SHORT: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int16_t*)(*(void**)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset) = *(int16_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_SET_FIELD_INT: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int32_t*)(*(void**)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset) = *(int32_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_SET_FIELD_LONG: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(int64_t*)(*(void**)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset) = *(int64_t*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_SET_FIELD_FLOAT: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(float*)(*(void**)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset) = *(float*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_SET_FIELD_DOUBLE: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        *(double*)(*(void**)&call_stack[operand_stack_top - 1] + sizeof(SPVM_DATA_OBJECT_HEADER) + constant_pool_field->package_byte_offset) = *(double*)&call_stack[operand_stack_top];
        operand_stack_top -= 2;
        pc += 5;
        goto *jump[*pc];
      }
      case_SPVM_BYTECODE_C_CODE_SET_FIELD_ADDRESS: {
        int32_t field_constant_pool_address
          = (*(pc + 1) << 24) + (*(pc + 2) << 16) + (*(pc + 3) << 8) + *(pc + 4);
        SPVM_CONSTANT_POOL_FIELD* constant_pool_field = (SPVM_CONSTANT_POOL_FIELD*)&constant_pool[field_constant_pool_address];
        
        intptr_t field_index = (intptr_t)*(void**)&call_stack[operand_stack_top - 1] + constant_pool_field->package_byte_offset;

        // Increment reference count
        SPVM_API_inc_ref_count(spvm, env, *(void**)&call_stack[operand_stack_top]);
        
        // Decrement reference count
        SPVM_API_dec_ref_count(spvm, env, *(void**)(field_index));
        
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
    
    assert(((SPVM_DATA_OBJECT_HEADER*)address)->ref_count > 0);
    
    // Decrement reference count
    ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count -= 1;
    
    // If reference count is zero, free address.
    if (((SPVM_DATA_OBJECT_HEADER*)address)->ref_count == 0) {
      
      // Object is string
      if (((SPVM_DATA_OBJECT_HEADER*)address)->type == SPVM_DATA_OBJECT_HEADER_C_TYPE_STRING) {
        
        SPVM_SV* sv = ((SPVM_DATA_OBJECT_HEADER*)address)->array_length_or_sv;
        SPVM_SvREFCNT_dec(sv);
        SPVM_ALLOCATOR_RUNTIME_free_address(spvm, spvm->allocator_runtime, address);
      }
      // Object is array of string
      else if (((SPVM_DATA_OBJECT_HEADER*)address)->type == SPVM_DATA_OBJECT_HEADER_C_TYPE_ARRAY_STRING) {
        
        // Array length
        int64_t length = ((SPVM_DATA_OBJECT_HEADER*)address)->array_length_or_sv;
        
        for (int64_t i = 0; i < length; i++) {
          void* element_address = *(void**)((intptr_t)address + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(void*) * i);
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

void SPVM_API_dec_ref_count_object(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  if (address != NULL) {
    assert(((SPVM_DATA_OBJECT_HEADER*)address)->ref_count > 0);
    
    // Decrement reference count
    ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count -= 1;
    
    // If reference count is zero, free address.
    if (((SPVM_DATA_OBJECT_HEADER*)address)->ref_count == 0) {
      SPVM_ALLOCATOR_RUNTIME_free_address(spvm, spvm->allocator_runtime, address);
    }
  }
}

void SPVM_API_dec_ref_count_string(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  if (address != NULL) {
    
    assert(((SPVM_DATA_OBJECT_HEADER*)address)->ref_count > 0);
    
    // Decrement reference count
    ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count -= 1;
    
    // If reference count is zero, free address.
    if (((SPVM_DATA_OBJECT_HEADER*)address)->ref_count == 0) {
      SPVM_SV* sv = ((SPVM_DATA_OBJECT_HEADER*)address)->array_length_or_sv;
      SPVM_SvREFCNT_dec(sv);
      SPVM_ALLOCATOR_RUNTIME_free_address(spvm, spvm->allocator_runtime, address);
    }
  }
}

void SPVM_API_dec_ref_count_array_string(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  if (address != NULL) {
    
    assert(((SPVM_DATA_OBJECT_HEADER*)address)->ref_count > 0);
    
    // Decrement reference count
    ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count -= 1;
    
    // If reference count is zero, free address.
    if (((SPVM_DATA_OBJECT_HEADER*)address)->ref_count == 0) {
        
      // Array length
      int64_t length = ((SPVM_DATA_OBJECT_HEADER*)address)->array_length_or_sv;
      
      for (int64_t i = 0; i < length; i++) {
        void* element_address = *(void**)((intptr_t)address + sizeof(SPVM_DATA_OBJECT_HEADER) + sizeof(void*) * i);
        SPVM_API_dec_ref_count_string(spvm, env, element_address);
      }
      
      SPVM_ALLOCATOR_RUNTIME_free_address(spvm, spvm->allocator_runtime, address);
    }
  }
}

void SPVM_API_inc_ref_count(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  if (address != NULL) {
    assert(((SPVM_DATA_OBJECT_HEADER*)address)->ref_count >= 0);
    // Increment reference count
    ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count += 1;
  }
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
  
  return ((SPVM_DATA_OBJECT_HEADER*)address)->array_length_or_sv;
}

int64_t SPVM_API_get_ref_count(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count;
}

SPVM_SV* SPVM_API_get_string_sv(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (SPVM_SV*)((SPVM_DATA_OBJECT_HEADER*)address)->array_length_or_sv;
}

SPVM_SV* SPVM_API_get_string_value(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  SPVM_SV* sv = SPVM_API_get_string_sv(spvm, env, address);
  assert(sv);
  const char* value = SPVM_COMPAT_SVpv(sv);
  
  return value;
}

int8_t* SPVM_API_get_array_byte_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (int8_t*)((intptr_t)address + sizeof(SPVM_DATA_OBJECT_HEADER));
}

int16_t* SPVM_API_get_array_short_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (int16_t*)((intptr_t)address + sizeof(SPVM_DATA_OBJECT_HEADER));
}

int32_t* SPVM_API_get_array_int_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (int32_t*)((intptr_t)address + sizeof(SPVM_DATA_OBJECT_HEADER));
}

int64_t* SPVM_API_get_array_long_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (int64_t*)((intptr_t)address + sizeof(SPVM_DATA_OBJECT_HEADER));
}

float* SPVM_API_get_array_float_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (float*)((intptr_t)address + sizeof(SPVM_DATA_OBJECT_HEADER));
}

double* SPVM_API_get_array_double_values(SPVM* spvm, SPVM_ENV* env, void* address) {
  (void)spvm;
  (void)env;
  
  return (double*)((intptr_t)address + sizeof(SPVM_DATA_OBJECT_HEADER));
}

void* SPVM_API_create_string_sv(SPVM* spvm, SPVM_ENV* env, SPVM_SV* sv) {
  
  SPVM_ALLOCATOR_RUNTIME* allocator = spvm->allocator_runtime;
  
  // Allocate array
  int64_t allocate_size = sizeof(SPVM_DATA_OBJECT_HEADER);
  void* address = SPVM_ALLOCATOR_RUNTIME_alloc(spvm, allocator, allocate_size);
  
  // Set type
  ((SPVM_DATA_OBJECT_HEADER*)address)->type = SPVM_DATA_OBJECT_HEADER_C_TYPE_STRING;
  
  // Set byte size
  ((SPVM_DATA_OBJECT_HEADER*)address)->byte_size = (int32_t)allocate_size;
  
  // Set reference count
  ((SPVM_DATA_OBJECT_HEADER*)address)->ref_count = 0;
  
  // Set sv
  ((SPVM_DATA_OBJECT_HEADER*)address)->array_length_or_sv = (SPVM_SV*)sv;
  
  return address;
}
