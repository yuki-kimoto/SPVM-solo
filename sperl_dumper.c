#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "sperl_dumper.h"
#include "sperl_parser.h"
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
#include "sperl_bytecodes.h"

void SPerl_DUMPER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op_base) {
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
          printf(" %d", constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_BYTE:
          printf(" '%c'", constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_INT:
          printf(" %d", constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_LONG:
          printf(" %ld", constant->uv.long_value);
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
          op_cur = SPerl_OP_sibling(parser, op_cur);
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

void SPerl_DUMPER_dump_parser(SPerl_PARSER* parser) {
  printf("\n[Abstract Syntax Tree]\n");
  SPerl_DUMPER_dump_ast(parser, parser->op_grammer);
  
  printf("\n[Resolved types]\n");
  SPerl_DUMPER_dump_resolved_types(parser, parser->resolved_types);

  printf("\n[Packages information]\n");
  SPerl_DUMPER_dump_packages(parser, parser->op_packages);
}

void SPerl_DUMPER_dump_constants(SPerl_PARSER* parser, SPerl_ARRAY* op_constants) {
  for (int32_t i = 0; i < op_constants->length; i++) {
    SPerl_OP* op_constant = SPerl_ARRAY_fetch(op_constants, i);
    SPerl_CONSTANT* constant = op_constant->uv.constant;
    printf("    constant[%" PRId32 "]\n", i);
    SPerl_DUMPER_dump_constant(parser, constant);
  }
}

void SPerl_DUMPER_dump_packages(SPerl_PARSER* parser, SPerl_ARRAY* op_packages) {
  for (int32_t i = 0; i < op_packages->length; i++) {
    printf("package[%d]\n", i);
    SPerl_OP* op_package = SPerl_ARRAY_fetch(op_packages, i);
    SPerl_PACKAGE* package = op_package->uv.package;
    printf("  name => \"%s\"\n", package->op_name->uv.name);
    
    if (package->op_type) {
      SPerl_TYPE* type = package->op_type->uv.type;
      printf("  resolved_type => \"%s\"\n", type->resolved_type->name);
    }
    
    printf("  size => %d\n", package->size);
    
    // Field information
    printf("  fields\n");
    SPerl_ARRAY* op_fields = package->op_fields;
    for (int32_t j = 0; j < op_fields->length; j++) {
      SPerl_OP* op_field = SPerl_ARRAY_fetch(op_fields, j);
      SPerl_FIELD* field = op_field->uv.field;
      printf("    field[%" PRId32 "]\n", j);
      SPerl_DUMPER_dump_field(parser, field);
    }

    printf("  constant_values\n");
    SPerl_DUMPER_dump_constants(parser, package->op_constants);

    printf("  constant_pool\n");
    SPerl_DUMPER_dump_constant_pool(parser, package->constant_pool);
    
    printf("  subs\n");
    SPerl_ARRAY* op_subs = package->op_subs;
    for (int32_t i = 0; i < op_subs->length; i++) {
      SPerl_OP* op_sub = SPerl_ARRAY_fetch(op_subs, i);
      SPerl_SUB* sub = op_sub->uv.sub;
      printf("    sub[%" PRId32 "]\n", i);
      SPerl_DUMPER_dump_sub(parser, sub);
    }
  }
}

void SPerl_DUMPER_dump_resolved_types(SPerl_PARSER* parser, SPerl_ARRAY* resolved_types) {
  for (int32_t i = 0; i < resolved_types->length; i++) {
    printf("resolved_type[%d]\n", i);
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_ARRAY_fetch(resolved_types, i);
    printf("    name => \"%s\"\n", resolved_type->name);
    printf("    id => \"%d\"\n", resolved_type->id);
  }
}

void SPerl_DUMPER_dump_constant_pool(SPerl_PARSER* parser, SPerl_CONSTANT_POOL* constant_pool) {
  for (int32_t i = 0; i < constant_pool->length; i++) {
    printf("    constant_pool[%d] %d\n", i, constant_pool->values[i]);
  }
}

void SPerl_DUMPER_dump_bytecodes(SPerl_PARSER* parser, SPerl_BYTECODES* bytecodes) {
  for (int32_t i = 0; i < bytecodes->length; i++) {
    uint8_t bytecode = bytecodes->values[i];
    printf("        [%d] %s\n", i, SPerl_BYTECODE_C_CODE_NAMES[bytecode]);
    
    // Operand
    switch (bytecode) {
      case SPerl_BYTECODE_C_CODE_WIDE: {
        i++;
        bytecode = bytecodes->values[i];
        
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
            bytecode = bytecodes->values[i];
            printf("        [%d] %d\n", i, bytecode);
            
            i++;
            bytecode = bytecodes->values[i];
            printf("        [%d] %d\n", i, bytecode);
            
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
        bytecode = bytecodes->values[i];
        printf("        [%d] %d\n", i, bytecode);
        
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
      case SPerl_BYTECODE_C_CODE_INVOKESTATIC:
      case SPerl_BYTECODE_C_CODE_JSR:
      {
        i++;
        bytecode = bytecodes->values[i];
        printf("        [%d] %d\n", i, bytecode);
        
        i++;
        bytecode = bytecodes->values[i];
        printf("        [%d] %d\n", i, bytecode);
        
        break;
      }
    }
  }
}

void SPerl_DUMPER_dump_constant(SPerl_PARSER* parser, SPerl_CONSTANT* constant) {
  switch(constant->code) {
    case SPerl_CONSTANT_C_CODE_BOOLEAN:
      printf("      boolean %" PRId32 "\n", constant->uv.int_value);
      break;
    case SPerl_CONSTANT_C_CODE_BYTE:
      printf("      char '%c'\n", constant->uv.int_value);
      break;
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
  printf("      pool_pos => %d\n", constant->pool_pos);
}

void SPerl_DUMPER_dump_sub(SPerl_PARSER* parser, SPerl_SUB* sub) {
  if (sub) {
    printf("      package_name => \"%s\"\n", sub->op_package->uv.package->op_name->uv.name);
    if (sub->anon) {
      printf("      name => (NONE)\n");
    }
    else {
      printf("      name => \"%s\"\n", sub->op_name->uv.name);
    }
    printf("      id => %d\n", sub->id);
    printf("      anon => %d\n", sub->anon);

    printf("      resolved_type => \"%s\"\n", sub->op_return_type->uv.type->resolved_type->name);

    printf("      argument_count => %" PRId32 "\n", sub->argument_count);
    printf("      my_vars\n");
    SPerl_ARRAY* op_my_vars = sub->op_my_vars;
    for (int32_t i = 0; i < op_my_vars->length; i++) {
      SPerl_OP* op_my_var = SPerl_ARRAY_fetch(sub->op_my_vars, i);
      SPerl_MY_VAR* my_var = op_my_var->uv.my_var;
      printf("        my_var[%d]\n", i);
      SPerl_DUMPER_dump_my_var(parser, my_var);
    }
    printf("      op_block => %x\n", sub->op_block);
    
    printf("      bytecodes\n");
    SPerl_DUMPER_dump_bytecodes(parser, sub->bytecodes);
  }
  else {
    printf("      None\n");
  }
}

void SPerl_DUMPER_dump_field(SPerl_PARSER* parser, SPerl_FIELD* field) {
  if (field) {
    printf("      name => \"%s\"\n", field->op_name->uv.name);
    
    SPerl_TYPE* type = field->op_type->uv.type;
    printf("      resolved_type => \"%s\"\n", type->resolved_type->name);

  }
  else {
    printf("        None\n");
  }
}


void SPerl_DUMPER_dump_enumeration_value(SPerl_PARSER* parser, SPerl_ENUMERATION_VALUE* enumeration_value) {
  if (enumeration_value) {
    printf("      name => \"%s\"\n", enumeration_value->op_name->uv.name);
    printf("      value => %d\n", enumeration_value->op_constant->uv.constant->uv.int_value);
  }
  else {
    printf("      None\n");
  }
}

void SPerl_DUMPER_dump_my_var(SPerl_PARSER* parser, SPerl_MY_VAR* my_var) {
  if (my_var) {
    printf("          name => \"%s\"\n", my_var->op_name->uv.name);
    
    SPerl_TYPE* type = my_var->op_type->uv.type;
    printf("          resolved_type => \"%s\"\n", type->resolved_type->name);
    
  }
  else {
    printf("          None\n");
  }
}
