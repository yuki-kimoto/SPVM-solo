#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_dumper.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_constant.h"
#include "sperl_field.h"
#include "sperl_sub.h"
#include "sperl_my_var.h"
#include "sperl_var.h"
#include "sperl_memory_pool.h"
#include "sperl_op.h"
#include "sperl_enumeration_value.h"
#include "sperl_type.h"
#include "sperl_type_component_name.h"
#include "sperl_type_component_array.h"
#include "sperl_type_component_sub.h"
#include "sperl_enumeration.h"
#include "sperl_package.h"
#include "sperl_resolved_type.h"
#include "sperl_constant_pool.h"
#include "sperl_bytecode.h"
#include "sperl_bytecode_array.h"

void SPerl_DUMPER_dump_ast(SPerl* sperl, SPerl_OP* op_base) {
  int32_t depth = 0;
  
  // Run OPs
  SPerl_OP* op_cur = op_base;
  _Bool finish = 0;
  while (op_cur) {
    // [START]Preorder traversal position
    
    for (int32_t i = 0; i < depth; i++) {
      printf(" ");
    }
    int32_t code = op_cur->code;
    printf("%s", SPerl_OP_C_CODE_NAMES[code]);
    if (op_cur->code == SPerl_OP_C_CODE_CONSTANT) {
      SPerl_CONSTANT* constant = op_cur->uv.constant;
      printf(" %s", SPerl_CONSTANT_C_CODE_NAMES[constant->code]);
      switch (constant->code) {
        case SPerl_CONSTANT_C_CODE_BOOLEAN:
        case SPerl_CONSTANT_C_CODE_INT:
          printf(" %" PRId32, constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_LONG:
          printf(" %" PRId64, constant->uv.long_value);
          break;
        case SPerl_CONSTANT_C_CODE_FLOAT:
          printf(" %f", constant->uv.float_value);
          break;
        case SPerl_CONSTANT_C_CODE_DOUBLE:
          printf(" %f", constant->uv.double_value);
          break;
        case SPerl_CONSTANT_C_CODE_STRING:
          printf(" \"%s\"", constant->uv.string_value);
          break;
      }
      printf(" (address %d)", constant->address);
    }
    else if (code == SPerl_OP_C_CODE_VAR) {
      SPerl_VAR* var = op_cur->uv.var;
      printf(" \"%s\"", var->op_name->uv.name);
    }
    else if (code == SPerl_OP_C_CODE_NAME) {
      printf(" \"%s\"", op_cur->uv.name);
    }
    printf("\n");
    
    // [END]Preorder traversal position
    
    if (op_cur->first) {
      op_cur = op_cur->first;
      depth++;
    }
    else {
      while (1) {
        // [START]Postorder traversal position
        
        // [END]Postorder traversal position
        
        if (op_cur == op_base) {
          finish = 1;
          break;
        }
        
        // Next sibling
        if (op_cur->moresib) {
          op_cur = SPerl_OP_sibling(sperl, op_cur);
          break;
        }
        // Next is parent
        else {
          op_cur = op_cur->sibparent;
          depth--;
        }
      }
      if (finish) {
        break;
      }
    }
  }
}

void SPerl_DUMPER_dump_sperl(SPerl* sperl) {
  SPerl_PARSER* parser = sperl->parser;
  
  printf("\n[Abstract Syntax Tree]\n");
  SPerl_DUMPER_dump_ast(sperl, parser->op_grammar);
  
  printf("\n[Resolved types]\n");
  SPerl_DUMPER_dump_resolved_types(sperl, parser->resolved_types);
  
  printf("\n[Constant pool]\n");
  SPerl_DUMPER_dump_constant_pool(sperl, sperl->constant_pool);
  
  printf("\n[Packages]\n");
  SPerl_DUMPER_dump_packages(sperl, parser->op_packages);
}

void SPerl_DUMPER_dump_constants(SPerl* sperl, SPerl_ARRAY* op_constants) {
  for (size_t i = 0, len = op_constants->length; i < len; i++) {
    SPerl_OP* op_constant = SPerl_ARRAY_fetch(op_constants, i);
    SPerl_CONSTANT* constant = op_constant->uv.constant;
    printf("    constant[%zu]\n", i);
    SPerl_DUMPER_dump_constant(sperl, constant);
  }
}

void SPerl_DUMPER_dump_packages(SPerl* sperl, SPerl_ARRAY* op_packages) {
  for (size_t i = 0, len = op_packages->length; i < len; i++) {
    printf("package[%zu]\n", i);
    SPerl_OP* op_package = SPerl_ARRAY_fetch(op_packages, i);
    SPerl_PACKAGE* package = op_package->uv.package;
    printf("  name => \"%s\"\n", package->op_name->uv.name);
    
    if (package->op_type) {
      SPerl_TYPE* type = package->op_type->uv.type;
      printf("  resolved_type => \"%s\"\n", type->resolved_type->name);
    }
    
    printf("  byte_size => %d\n", package->byte_size);
    
    // Field information
    printf("  fields\n");
    SPerl_ARRAY* op_fields = package->op_fields;
    for (size_t j = 0, len2 = op_fields->length; j < len2; j++) {
      SPerl_OP* op_field = SPerl_ARRAY_fetch(op_fields, j);
      SPerl_FIELD* field = op_field->uv.field;
      printf("    field[%zu]\n", j);
      SPerl_DUMPER_dump_field(sperl, field);
    }
    
    // Sub information
    printf("  subs\n");
    SPerl_ARRAY* op_subs = package->op_subs;
    for (size_t i = 0, len = op_subs->length; i < len; i++) {
      SPerl_OP* op_sub = SPerl_ARRAY_fetch(op_subs, i);
      SPerl_SUB* sub = op_sub->uv.sub;
      printf("    sub[%zu]\n", i);
      SPerl_DUMPER_dump_sub(sperl, sub);
    }
  }
}

void SPerl_DUMPER_dump_resolved_types(SPerl* sperl, SPerl_ARRAY* resolved_types) {
  (void)sperl;
  
  for (size_t i = 0, len = resolved_types->length; i < len; i++) {
    printf("resolved_type[%zu]\n", i);
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_ARRAY_fetch(resolved_types, i);
    printf("    name => \"%s\"\n", resolved_type->name);
    printf("    id => \"%" PRId32 "\"\n", resolved_type->id);
  }
}

void SPerl_DUMPER_dump_constant_pool(SPerl* sperl, SPerl_CONSTANT_POOL* constant_pool) {
  (void)sperl;

  for (int32_t i = 0; i < constant_pool->length; i++) {
    printf("      constant_pool[%" PRId32 "] %" PRId32 "\n", i, constant_pool->values[i]);
  }
}

void SPerl_DUMPER_dump_bytecode_array(SPerl* sperl, SPerl_BYTECODE_ARRAY* bytecode_array, int32_t start_pos, int32_t length) {
  (void)sperl;
  
  int32_t end_pos = start_pos + length - 1;
  
  for (int32_t i = start_pos; i <= end_pos; i++) {
    
    uint8_t bytecode = bytecode_array->values[i];
    printf("        [%" PRId32 "] %s\n", i, SPerl_BYTECODE_C_CODE_NAMES[bytecode]);
    
    // Operand
    switch (bytecode) {
      case SPerl_BYTECODE_C_CODE_WIDE: {
        i++;
        bytecode = bytecode_array->values[i];
        
        switch (bytecode) {
          // Have tow operand
          case SPerl_BYTECODE_C_CODE_ISTORE:
          case SPerl_BYTECODE_C_CODE_LSTORE:
          case SPerl_BYTECODE_C_CODE_FSTORE:
          case SPerl_BYTECODE_C_CODE_DSTORE:
          case SPerl_BYTECODE_C_CODE_ASTORE:
          case SPerl_BYTECODE_C_CODE_ILOAD:
          case SPerl_BYTECODE_C_CODE_LLOAD:
          case SPerl_BYTECODE_C_CODE_FLOAD:
          case SPerl_BYTECODE_C_CODE_DLOAD:
          case SPerl_BYTECODE_C_CODE_ALOAD:
          {
            i++;
            bytecode = bytecode_array->values[i];
            printf("        [%" PRId32 "] %d\n", i, bytecode);
            
            i++;
            bytecode = bytecode_array->values[i];
            printf("        [%" PRId32 "] %d\n", i, bytecode);
            
            break;
          }
        }
        
        break;
      }
      
      // Have one operand
      case SPerl_BYTECODE_C_CODE_BIPUSH:
      case SPerl_BYTECODE_C_CODE_ISTORE:
      case SPerl_BYTECODE_C_CODE_LSTORE:
      case SPerl_BYTECODE_C_CODE_FSTORE:
      case SPerl_BYTECODE_C_CODE_DSTORE:
      case SPerl_BYTECODE_C_CODE_ASTORE:
      case SPerl_BYTECODE_C_CODE_ILOAD:
      case SPerl_BYTECODE_C_CODE_LLOAD:
      case SPerl_BYTECODE_C_CODE_FLOAD:
      case SPerl_BYTECODE_C_CODE_DLOAD:
      case SPerl_BYTECODE_C_CODE_ALOAD:
      case SPerl_BYTECODE_C_CODE_LDC:
      {
        i++;
        bytecode = bytecode_array->values[i];
        printf("        [%" PRId32 "] %d\n", i, bytecode);
        
        break;
      }
      
      // Have tow operand
      case SPerl_BYTECODE_C_CODE_IINC:
      case SPerl_BYTECODE_C_CODE_SIPUSH:
      case SPerl_BYTECODE_C_CODE_LDC2_W:
      case SPerl_BYTECODE_C_CODE_LDC_W:
      case SPerl_BYTECODE_C_CODE_IF_ACMPEQ:
      case SPerl_BYTECODE_C_CODE_IF_ACMPNE:
      case SPerl_BYTECODE_C_CODE_IF_ICMPEQ:
      case SPerl_BYTECODE_C_CODE_IF_ICMPNE:
      case SPerl_BYTECODE_C_CODE_IF_ICMPLT:
      case SPerl_BYTECODE_C_CODE_IF_ICMPGE:
      case SPerl_BYTECODE_C_CODE_IF_ICMPGT:
      case SPerl_BYTECODE_C_CODE_IF_ICMPLE:
      case SPerl_BYTECODE_C_CODE_IFEQ:
      case SPerl_BYTECODE_C_CODE_IFNE:
      case SPerl_BYTECODE_C_CODE_IFLT:
      case SPerl_BYTECODE_C_CODE_IFGE:
      case SPerl_BYTECODE_C_CODE_IFGT:
      case SPerl_BYTECODE_C_CODE_IFLE:
      case SPerl_BYTECODE_C_CODE_IFNONNULL:
      case SPerl_BYTECODE_C_CODE_IFNULL:
      case SPerl_BYTECODE_C_CODE_GOTO:
      case SPerl_BYTECODE_C_CODE_JSR:
      case SPerl_BYTECODE_C_CODE_PUTFIELD:
      case SPerl_BYTECODE_C_CODE_GETFIELD:
      {
        i++;
        bytecode = bytecode_array->values[i];
        printf("        [%" PRId32 "] %d\n", i, bytecode);
        
        i++;
        bytecode = bytecode_array->values[i];
        printf("        [%" PRId32 "] %d\n", i, bytecode);
        
        break;
      }
      // Have four operand
      case SPerl_BYTECODE_C_CODE_PUTFIELD_WW:
      case SPerl_BYTECODE_C_CODE_GETFIELD_WW:
      case SPerl_BYTECODE_C_CODE_INVOKESTATIC_WW:
      case SPerl_BYTECODE_C_CODE_LDC_WW:
      case SPerl_BYTECODE_C_CODE_LDC2_WW:
      case SPerl_BYTECODE_C_CODE_NEWARRAY:
      case SPerl_BYTECODE_C_CODE_MULTIANEWARRAY:
      case SPerl_BYTECODE_C_CODE_ANEWARRAY:
      case SPerl_BYTECODE_C_CODE_NEW:
      {
        i++;
        bytecode = bytecode_array->values[i];
        printf("        [%" PRId32 "] %d\n", i, bytecode);
        
        i++;
        bytecode = bytecode_array->values[i];
        printf("        [%" PRId32 "] %d\n", i, bytecode);
        
        i++;
        bytecode = bytecode_array->values[i];
        printf("        [%" PRId32 "] %d\n", i, bytecode);
        
        i++;
        bytecode = bytecode_array->values[i];
        printf("        [%" PRId32 "] %d\n", i, bytecode);
        
        break;
      }
      
      case SPerl_BYTECODE_C_CODE_TABLESWITCH: {
        
        // Machine address to calculate padding
        uintptr_t pc_machine_address = (uintptr_t)&bytecode_array->values[i];
        
        // Padding
        int32_t padding = 3 - (pc_machine_address % 4);
        
        for (int32_t j = 0; j < padding; j++) {
          i++;
          bytecode = bytecode_array->values[i];
          printf("        [%" PRId32 "] %d\n", i, bytecode);
        }
        
        // Default
        for (int32_t j = 0; j < 4; j++) {
          i++;
          bytecode = bytecode_array->values[i];
          printf("        [%" PRId32 "] %d\n", i, bytecode);
        }
        
        // Low
        int32_t min = 0;
        for (int32_t j = 0; j < 4; j++) {
          i++;
          bytecode = bytecode_array->values[i];
          min += bytecode << (24 - (8 * j));
          printf("        [%" PRId32 "] %d\n", i, bytecode);
        }
        
        // High
        int32_t max = 0;
        for (int32_t j = 0; j < 4; j++) {
          i++;
          bytecode = bytecode_array->values[i];
          max += bytecode << (24 - (8 * j));
          printf("        [%" PRId32 "] %d\n", i, bytecode);
        }
        
        // Addresses
        int32_t length = max - min + 1;
        for (int32_t j = 0; j < length * 4; j++) {
          i++;
          bytecode = bytecode_array->values[i];
          printf("        [%" PRId32 "] %d\n", i, bytecode);
        }
        
        break;
      }
      case SPerl_BYTECODE_C_CODE_LOOKUPSWITCH: {
        
        // Machine address to calculate padding
        uintptr_t pc_machine_address = (uintptr_t)&bytecode_array->values[i];
        
        // Padding
        int32_t padding = 3 - (pc_machine_address % 4);
        
        for (int32_t j = 0; j < padding; j++) {
          i++;
          bytecode = bytecode_array->values[i];
          printf("        [%" PRId32 "] %d\n", i, bytecode);
        }
        
        // Default
        for (int32_t j = 0; j < 4; j++) {
          i++;
          bytecode = bytecode_array->values[i];
          printf("        [%" PRId32 "] %d\n", i, bytecode);
        }
        
        // Count
        int32_t length = 0;
        for (int32_t j = 0; j < 4; j++) {
          i++;
          bytecode = bytecode_array->values[i];
          length += bytecode << (24 - (8 * j));
          printf("        [%" PRId32 "] %d\n", i, bytecode);
        }
        
        // Addresses
        for (int32_t j = 0; j < length * 8; j++) {
          i++;
          bytecode = bytecode_array->values[i];
          printf("        [%" PRId32 "] %d\n", i, bytecode);
        }
        
        break;
      }
    }
  }
}

void SPerl_DUMPER_dump_constant(SPerl* sperl, SPerl_CONSTANT* constant) {
  (void)sperl;
  
  switch(constant->code) {
    case SPerl_CONSTANT_C_CODE_INT:
      printf("      int %" PRId32 "\n", constant->uv.int_value);
      break;
    case SPerl_CONSTANT_C_CODE_LONG:
      printf("      long %" PRId64 "\n", constant->uv.long_value);
      break;
    case SPerl_CONSTANT_C_CODE_FLOAT:
      printf("      float %f\n", constant->uv.float_value);
      break;
    case SPerl_CONSTANT_C_CODE_DOUBLE:
      printf("      double %f\n", constant->uv.double_value);
      break;
    case SPerl_CONSTANT_C_CODE_STRING:
      printf("      string \"%s\"\n", constant->uv.string_value);
      break;
  }
  printf("      address => %d\n", constant->address);
}

void SPerl_DUMPER_dump_sub(SPerl* sperl, SPerl_SUB* sub) {
  if (sub) {
    
    if (sub->anon) {
      printf("      name => (NONE)\n");
    }
    else {
      printf("      name => \"%s\"\n", sub->op_name->uv.name);
    }
    printf("      anon => %d\n", sub->anon);
    
    if (sub->op_return_type->code == SPerl_OP_C_CODE_VOID) {
      printf("      resolved_type => void");
    }
    else {
      printf("      resolved_type => \"%s\"\n", sub->op_return_type->uv.type->resolved_type->name);
    }
    printf("      args\n");
    SPerl_ARRAY* op_args = sub->op_args;
    for (size_t i = 0, len = op_args->length; i < len; i++) {
      SPerl_OP* op_arg = SPerl_ARRAY_fetch(sub->op_args, i);
      SPerl_MY_VAR* my_var = op_arg->uv.my_var;
      printf("        arg[%zu]\n", i);
      SPerl_DUMPER_dump_my_var(sperl, my_var);
    }
    
    printf("      my_vars\n");
    SPerl_ARRAY* op_my_vars = sub->op_my_vars;
    for (size_t i = 0, len = op_my_vars->length; i < len; i++) {
      SPerl_OP* op_my_var = SPerl_ARRAY_fetch(sub->op_my_vars, i);
      SPerl_MY_VAR* my_var = op_my_var->uv.my_var;
      printf("      my_var[%zu]\n", i);
      SPerl_DUMPER_dump_my_var(sperl, my_var);
    }
    
    printf("      operand_stack_max => %d\n", sub->operand_stack_max);

    printf("      bytecode_array\n");
    SPerl_DUMPER_dump_bytecode_array(sperl, sperl->bytecode_array, sub->bytecode_base, sub->bytecode_length);
  }
  else {
    printf("      None\n");
  }
}

void SPerl_DUMPER_dump_field(SPerl* sperl, SPerl_FIELD* field) {
  (void)sperl;
  
  if (field) {
    printf("      name => \"%s\"\n", field->op_name->uv.name);
    
    printf("      package_byte_offset => \"%" PRId32 "\"\n", field->package_byte_offset);
    
    SPerl_TYPE* type = field->op_type->uv.type;
    printf("      resolved_type => \"%s\"\n", type->resolved_type->name);
    printf("      byte_size => \"%" PRId32 "\"\n", SPerl_FIELD_get_byte_size(sperl, field));
    
    printf("      id => \"%" PRId32 "\"\n", field->id);
  }
  else {
    printf("        None\n");
  }
}


void SPerl_DUMPER_dump_enumeration_value(SPerl* sperl, SPerl_ENUMERATION_VALUE* enumeration_value) {
  (void)sperl;
  
  if (enumeration_value) {
    printf("      name => \"%s\"\n", enumeration_value->op_name->uv.name);
    printf("      value => %" PRId32 "\n", enumeration_value->op_constant->uv.constant->uv.int_value);
  }
  else {
    printf("      None\n");
  }
}

void SPerl_DUMPER_dump_my_var(SPerl* sperl, SPerl_MY_VAR* my_var) {
  (void)sperl;

  if (my_var) {
    printf("        name => \"%s\"\n", my_var->op_name->uv.name);
    
    SPerl_TYPE* type = my_var->op_type->uv.type;
    printf("        resolved_type => \"%s\"\n", type->resolved_type->name);
    
  }
  else {
    printf("        None\n");
  }
}
