#include <string.h>
#include <math.h>

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

void SPerl_VM_run(SPerl* sperl) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  const char* entry_point = parser->entry_point;
  
  SPerl_SUB* sub_entry_point = SPerl_HASH_search(parser->sub_abs_name_symtable, entry_point, strlen(entry_point));
  
  SPerl_BYTECODE_ARRAY* bytecode_array = parser->bytecode_array;
  uint8_t* bytecodes = bytecode_array->values;
  
  // Program counter
  int32_t pc = sub_entry_point->bytecode_start_pos;
  int32_t end_pos = sub_entry_point->bytecode_start_pos + sub_entry_point->bytecode_length - 1;
  
  int32_t op_stack[255];
  int32_t op_stack_pos = -1;
  
  while (pc <= end_pos) {
    
    switch (bytecodes[pc]) {
      
      case SPerl_BYTECODE_C_CODE_NOP:
        // None
        break;
      case SPerl_BYTECODE_C_CODE_ACONST_NULL:
        
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_M1:
        op_stack_pos++;
        op_stack[op_stack_pos] = -1;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_0:
        op_stack_pos++;
        op_stack[op_stack_pos] = 0;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_1:
        op_stack_pos++;
        op_stack[op_stack_pos] = 1;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_2:
        op_stack_pos++;
        op_stack[op_stack_pos] = 2;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_3:
        op_stack_pos++;
        op_stack[op_stack_pos] = 3;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_4:
        op_stack_pos++;
        op_stack[op_stack_pos] = 4;
        break;
      case SPerl_BYTECODE_C_CODE_ICONST_5:
        op_stack_pos++;
        op_stack[op_stack_pos] = 5;
        break;
      case SPerl_BYTECODE_C_CODE_LCONST_0:
        op_stack_pos++;
        *((int64_t*)&op_stack[op_stack_pos]) = 0L;
        op_stack_pos++;
        break;
      case SPerl_BYTECODE_C_CODE_LCONST_1:
        op_stack_pos++;
        *((int64_t*)&op_stack[op_stack_pos]) = 1L;
        op_stack_pos++;
        break;
      case SPerl_BYTECODE_C_CODE_FCONST_0:
        op_stack_pos++;
        *((float*)&op_stack[op_stack_pos]) = 0.F;
        break;
      case SPerl_BYTECODE_C_CODE_FCONST_1:
        op_stack_pos++;
        *((float*)&op_stack[op_stack_pos]) = 1.F;
        break;
      case SPerl_BYTECODE_C_CODE_FCONST_2:
        op_stack_pos++;
        *((float*)&op_stack[op_stack_pos]) = 2.F;
        break;
      case SPerl_BYTECODE_C_CODE_DCONST_0:
        op_stack_pos++;
        *((double*)&op_stack[op_stack_pos]) = 0.;
        op_stack_pos++;
        break;
      case SPerl_BYTECODE_C_CODE_DCONST_1:
        op_stack_pos++;
        *((double*)&op_stack[op_stack_pos]) = 1.;
        op_stack_pos++;
        break;
      case SPerl_BYTECODE_C_CODE_BIPUSH:
        op_stack_pos++;
        
        pc++;
        op_stack[op_stack_pos] = bytecodes[pc];
        
        break;
      case SPerl_BYTECODE_C_CODE_SIPUSH:
        op_stack_pos++;
        
        pc++;
        op_stack[op_stack_pos] = bytecodes[pc] << 8;
        
        pc++;
        op_stack[op_stack_pos] += bytecodes[pc];
        
        break;
      case SPerl_BYTECODE_C_CODE_LDC:
      
        break;
      case SPerl_BYTECODE_C_CODE_LDC_W:
      
        break;
      case SPerl_BYTECODE_C_CODE_LDC2_W:
      
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD:
      
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_ILOAD_3:
      
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_LLOAD_3:
      
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_FLOAD_3:
      
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_DLOAD_3:
      
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_ALOAD_3:
      
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
      
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE:
      
        break;
      case SPerl_BYTECODE_C_CODE_ISTORE_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_ISTORE_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_ISTORE_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_ISTORE_3:
      
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_LSTORE_3:
      
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_FSTORE_3:
      
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_DSTORE_3:
      
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE_0:
      
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE_1:
      
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE_2:
      
        break;
      case SPerl_BYTECODE_C_CODE_ASTORE_3:
      
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
        op_stack_pos--;
        
        break;
      case SPerl_BYTECODE_C_CODE_POP2:
        op_stack_pos -= 2;
        
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
        op_stack[op_stack_pos - 1] += op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_LADD:
        *((int64_t*)&op_stack[op_stack_pos - 3]) += (int64_t)op_stack[op_stack_pos - 1];
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FADD:
        *((float*)&op_stack[op_stack_pos - 1]) += (float)op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_DADD:
        *((double*)&op_stack[op_stack_pos - 3]) += (double)op_stack[op_stack_pos - 1];
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_ISUB:
        op_stack[op_stack_pos - 1] -= op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_LSUB:
        *((int64_t*)&op_stack[op_stack_pos - 3]) -= (int64_t)op_stack[op_stack_pos - 1];
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FSUB:
        *((float*)&op_stack[op_stack_pos - 1]) -= (float)op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_DSUB:
        *((double*)&op_stack[op_stack_pos - 3]) -= (double)op_stack[op_stack_pos - 1];
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IMUL:
        op_stack[op_stack_pos - 1] *= op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_LMUL:
        *((int64_t*)&op_stack[op_stack_pos - 3]) *= (int64_t)op_stack[op_stack_pos - 1];
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FMUL:
        *((float*)&op_stack[op_stack_pos - 1]) *= (float)op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_DMUL:
        *((double*)&op_stack[op_stack_pos - 3]) *= (double)op_stack[op_stack_pos - 1];
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IDIV:
        op_stack[op_stack_pos - 1] /= op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_LDIV:
        *((int64_t*)&op_stack[op_stack_pos - 3]) /= (int64_t)op_stack[op_stack_pos - 1];
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FDIV:
        *((float*)&op_stack[op_stack_pos - 1]) /= (float)op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_DDIV:
        *((double*)&op_stack[op_stack_pos - 3]) /= (double)op_stack[op_stack_pos - 1];
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_IREM:
        op_stack[op_stack_pos - 1] %= op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_LREM:
        *((int64_t*)&op_stack[op_stack_pos - 3]) %= (int64_t)op_stack[op_stack_pos - 1];
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_FREM:
        *((float*)&op_stack[op_stack_pos - 1]) = (float)fmod((double)*((float*)&op_stack[op_stack_pos - 1]), (double)(float)op_stack[op_stack_pos]);
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_DREM:
        *((double*)&op_stack[op_stack_pos - 3]) = fmod(*((double*)&op_stack[op_stack_pos - 3]), (double)op_stack[op_stack_pos - 1]);
        op_stack_pos -= 2;
        break;
      case SPerl_BYTECODE_C_CODE_INEG:
      
        break;
      case SPerl_BYTECODE_C_CODE_LNEG:
      
        break;
      case SPerl_BYTECODE_C_CODE_FNEG:
      
        break;
      case SPerl_BYTECODE_C_CODE_DNEG:
      
        break;
      case SPerl_BYTECODE_C_CODE_ISHL:
      
        break;
      case SPerl_BYTECODE_C_CODE_LSHL:
      
        break;
      case SPerl_BYTECODE_C_CODE_ISHR:
      
        break;
      case SPerl_BYTECODE_C_CODE_LSHR:
      
        break;
      case SPerl_BYTECODE_C_CODE_IUSHR:
      
        break;
      case SPerl_BYTECODE_C_CODE_LUSHR:
      
        break;
      case SPerl_BYTECODE_C_CODE_IAND:
      
        break;
      case SPerl_BYTECODE_C_CODE_LAND:
      
        break;
      case SPerl_BYTECODE_C_CODE_IOR:
      
        break;
      case SPerl_BYTECODE_C_CODE_LOR:
      
        break;
      case SPerl_BYTECODE_C_CODE_IXOR:
      
        break;
      case SPerl_BYTECODE_C_CODE_LXOR:
      
        break;
      case SPerl_BYTECODE_C_CODE_IINC:
      
        break;
      case SPerl_BYTECODE_C_CODE_I2L:
        op_stack_pos++;
        *((int64_t*)&op_stack[op_stack_pos - 1]) = (int64_t)op_stack[op_stack_pos - 1];
        break;
      case SPerl_BYTECODE_C_CODE_I2F:
        *((float*)&op_stack[op_stack_pos]) = (float)op_stack[op_stack_pos];
        break;
      case SPerl_BYTECODE_C_CODE_I2D:
        op_stack_pos++;
        *((double*)&op_stack[op_stack_pos - 1]) = (double)op_stack[op_stack_pos - 1];
        break;
      case SPerl_BYTECODE_C_CODE_L2I:
        op_stack[op_stack_pos - 1] = op_stack[op_stack_pos];
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_L2F:
        *((float*)&op_stack[op_stack_pos - 1]) = (float)*((int64_t*)&op_stack[op_stack_pos - 1]);
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_L2D:
        *((double*)&op_stack[op_stack_pos - 1]) = (double)*((int64_t*)&op_stack[op_stack_pos - 1]);
        break;
      case SPerl_BYTECODE_C_CODE_F2I:
        op_stack[op_stack_pos] = (int32_t)*((float*)&op_stack[op_stack_pos]);
        break;
      case SPerl_BYTECODE_C_CODE_F2L:
        op_stack_pos++;
        *((int64_t*)&op_stack[op_stack_pos]) = (int64_t)*((float*)&op_stack[op_stack_pos - 1]);
        break;
      case SPerl_BYTECODE_C_CODE_F2D:
        op_stack_pos++;
        *((double*)&op_stack[op_stack_pos]) = (double)*((float*)&op_stack[op_stack_pos - 1]);
        break;
      case SPerl_BYTECODE_C_CODE_D2I:
        op_stack[op_stack_pos - 1] = (int32_t)*((double*)&op_stack[op_stack_pos - 1]);
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_D2L:
        *((int64_t*)&op_stack[op_stack_pos - 1]) = (int64_t)*((double*)&op_stack[op_stack_pos - 1]);
        break;
      case SPerl_BYTECODE_C_CODE_D2F:
        *((float*)&op_stack[op_stack_pos - 1]) = (float)*((double*)&op_stack[op_stack_pos - 1]);
        op_stack_pos--;
        break;
      case SPerl_BYTECODE_C_CODE_I2B:
        op_stack[op_stack_pos] = op_stack[op_stack_pos] & 1;
        break;
      case SPerl_BYTECODE_C_CODE_I2C:
        /* Not used */
        break;
      case SPerl_BYTECODE_C_CODE_I2S:
        op_stack[op_stack_pos] = op_stack[op_stack_pos] & 0xF;
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
      
        break;
      case SPerl_BYTECODE_C_CODE_LRETURN:
      
        break;
      case SPerl_BYTECODE_C_CODE_FRETURN:
      
        break;
      case SPerl_BYTECODE_C_CODE_DRETURN:
      
        break;
      case SPerl_BYTECODE_C_CODE_ARETURN:
      
        break;
      case SPerl_BYTECODE_C_CODE_RETURN:
      
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
