#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

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
#include "sperl_word.h"
#include "sperl_enum_value.h"
#include "sperl_descripter.h"
#include "sperl_type.h"
#include "sperl_type_word.h"
#include "sperl_type_array.h"
#include "sperl_type_sub.h"
#include "sperl_body.h"
#include "sperl_body_core.h"
#include "sperl_body_class.h"
#include "sperl_body_enum.h"
#include "sperl_package.h"
#include "sperl_op_info.h"
#include "sperl_resolved_type.h"

SPerl_PARSER* SPerl_PARSER_new() {
  SPerl_PARSER* parser = calloc(1, sizeof(SPerl_PARSER));
  
  // Manipulate memory. This is freed last.
  parser->array_ptrs = SPerl_ARRAY_new(0);
  parser->hash_ptrs = SPerl_ARRAY_new(0);
  parser->long_str_ptrs = SPerl_ARRAY_new(0);
  parser->memory_pool = SPerl_MEMORY_POOL_new(0);
  
  // Parser information
  parser->op_subs = SPerl_PARSER_new_array(parser, 0);
  parser->sub_complete_name_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->op_packages = SPerl_PARSER_new_array(parser, 0);
  parser->package_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->op_types = SPerl_PARSER_new_array(parser, 0);
  parser->use_stack = SPerl_PARSER_new_array(parser, 0);
  parser->bodys = SPerl_PARSER_new_array(parser, 0);
  parser->body_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->field_complete_name_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->enum_complete_name_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->include_pathes = SPerl_PARSER_new_array(parser, 0);
  parser->bufptr = "";
  parser->resolved_types = SPerl_PARSER_new_array(parser, 0);
  parser->resolved_type_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->next_var_id = 1;

  // Core types
  for (SPerl_int i = 0; i < 8; i++) {
    // Name
    SPerl_char* name = SPerl_BODY_CORE_C_CODE_NAMES[i];
    
    // Body core
    SPerl_BODY_CORE* body_core = SPerl_BODY_CORE_new(parser);
    body_core->code = i;
    body_core->size = SPerl_BODY_CORE_C_SIZES[i];
    
    // Name
    SPerl_OP* op_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_WORD, NULL, NULL);
    SPerl_WORD* name_word = SPerl_WORD_new(parser);
    name_word->value = name;
    op_name->uv.word = name_word;
    
    // Body
    SPerl_BODY* body = SPerl_BODY_new(parser);
    body->code = SPerl_BODY_C_CODE_CORE;
    body->op_name = op_name;
    body->uv.body_core = body_core;
    
    SPerl_ARRAY_push(parser->bodys, body);
    SPerl_HASH_insert(parser->body_symtable, name, strlen(name), body);
    
    // Resolved type
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(parser);
    resolved_type->code = SPerl_RESOLVED_TYPE_C_CODE_CORE;
    SPerl_ARRAY_push(resolved_type->part_names, name);
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = i;
    resolved_type->body = body;
    SPerl_ARRAY_push(parser->resolved_types, resolved_type);
    SPerl_HASH_insert(parser->resolved_type_symtable, name, strlen(name), resolved_type);
    
    // Type word
    SPerl_TYPE_WORD* type_word = SPerl_TYPE_WORD_new(parser);
    type_word->name_word = name_word;
    
    // Type
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_WORD;
    type->uv.type_word = type_word;
    type->resolved_type = resolved_type;
    type->resolved = 1;
    
    // Type OP
    SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, NULL, NULL);
    op_type->uv.type = type;
    
    // Add type
    SPerl_ARRAY_push(parser->op_types, op_type);
    
    // Package
    SPerl_PACKAGE* package = SPerl_PACKAGE_new(parser);
    SPerl_OP* op_package_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_WORD, NULL, NULL);
    op_package_name->uv.word = name_word;
    package->op_name = op_package_name;
    package->op_type = op_type;
    
    // Package OP
    SPerl_OP* op_package = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PACKAGE, NULL, NULL);
    op_package->uv.package = package;
    
    SPerl_ARRAY_push(parser->op_packages, op_package);
    SPerl_HASH_insert(parser->package_symtable, name, strlen(name), type);
  }
  
  return parser;
}

SPerl_ARRAY* SPerl_PARSER_new_array(SPerl_PARSER* parser, SPerl_int capacity) {
  SPerl_ARRAY* array = SPerl_ARRAY_new(capacity);
  
  SPerl_ARRAY_push(parser->array_ptrs, array);
  
  return array;
}

SPerl_HASH* SPerl_PARSER_new_hash(SPerl_PARSER* parser, SPerl_int capacity) {
  SPerl_HASH* hash = SPerl_HASH_new(capacity);
  
  SPerl_ARRAY_push(parser->hash_ptrs, hash);
  
  return hash;
}

SPerl_int* SPerl_PARSER_new_int(SPerl_PARSER* parser) {
  SPerl_int* value = SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_int));
  
  return value;
}

SPerl_char* SPerl_PARSER_new_string(SPerl_PARSER* parser, SPerl_int length) {
  SPerl_char* str = malloc(length + 1);
  
  if (length < 40) {
    str = SPerl_MEMORY_POOL_alloc(parser->memory_pool, 40);
  }
  else {
    str = malloc(length + 1);
    SPerl_ARRAY_push(parser->long_str_ptrs, str);
  }
  
  return str;
}

void SPerl_PARSER_free(SPerl_PARSER* parser) {

  // Free all array pointers
  for (SPerl_int i = 0; i < parser->array_ptrs->length; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(parser->array_ptrs, i);
    SPerl_ARRAY_free(array);
  }
  SPerl_ARRAY_free(parser->array_ptrs);
  
  // Free all hash pointers
  for (SPerl_int i = 0; i < parser->hash_ptrs->length; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(parser->hash_ptrs, i);
    SPerl_HASH_free(hash);
  }
  SPerl_ARRAY_free(parser->hash_ptrs);
  
  // Free all string pointers;
  for (SPerl_int i = 0; i < parser->long_str_ptrs->length; i++) {
    SPerl_char* str = SPerl_ARRAY_fetch(parser->long_str_ptrs, i);
    free(str);
  }
  SPerl_ARRAY_free(parser->long_str_ptrs);
  
  // Free memory pool */
  SPerl_MEMORY_POOL_free(parser->memory_pool);
  
  free(parser);
}

void SPerl_PARSER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op_base) {
  SPerl_int depth = 0;
  
  // Run OPs
  SPerl_OP* op_cur = op_base;
  SPerl_boolean finish = 0;
  while (op_cur) {
    // [START]Preorder traversal position
    
    for (SPerl_int i = 0; i < depth; i++) {
      printf(" ");
    }
    SPerl_int code = op_cur->code;
    printf("%s", SPerl_OP_C_CODE_NAMES[code]);
    if (op_cur->group == SPerl_OP_C_GROUP_CONST) {
      SPerl_CONSTANT* constant = op_cur->uv.constant;
      switch(op_cur->code) {
        case SPerl_OP_C_CODE_CONSTBOOLEAN:
          printf(" %d", constant->uv.int_value);
          break;
        case SPerl_OP_C_CODE_CONSTCHAR:
          printf(" '%c'", constant->uv.int_value);
          break;
        case SPerl_OP_C_CODE_CONSTINT:
          printf(" %d", constant->uv.int_value);
          break;
        case SPerl_OP_C_CODE_CONSTLONG:
          printf(" %ld", constant->uv.long_value);
          break;
        case SPerl_OP_C_CODE_CONSTFLOAT:
          printf(" %f", constant->uv.float_value);
          break;
        case SPerl_OP_C_CODE_CONSTDOUBLE:
          printf(" %f", constant->uv.double_value);
          break;
        case SPerl_OP_C_CODE_CONSTSTRING:
          printf(" \"%s\"", constant->uv.string_value);
          break;
      }
    }
    else if (code == SPerl_OP_C_CODE_VAR) {
      SPerl_VAR* var = op_cur->uv.var;
      printf(" \"%s\"", var->op_name->uv.word->value);
    }
    else if (code == SPerl_OP_C_CODE_WORD) {
      SPerl_WORD* word = op_cur->uv.word;
      printf(" \"%s\"", word->value);
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

void SPerl_PARSER_dump_parser(SPerl_PARSER* parser) {
  printf("\n[Abstract Syntax Tree]\n");
  SPerl_PARSER_dump_ast(parser, parser->op_grammer);

  printf("\n[Packages information]\n");
  SPerl_PARSER_dump_packages(parser, parser->op_packages);
  
  printf("\n[Body information]\n");
  SPerl_PARSER_dump_bodys(parser, parser->bodys);
  
  printf("\n[Resolved types]\n");
  SPerl_PARSER_dump_resolved_types(parser, parser->resolved_types);
  
  printf("\n[Subroutine information]\n");
  printf("  subs\n");
  SPerl_ARRAY* op_subs = parser->op_subs;
  for (SPerl_int i = 0; i < op_subs->length; i++) {
    SPerl_OP* op_sub = SPerl_ARRAY_fetch(op_subs, i);
    SPerl_SUB* sub = op_sub->uv.sub;
    printf("    sub[%" PRId32 "]\n", i);
    SPerl_PARSER_dump_sub(parser, sub);
  }
}

void SPerl_PARSER_dump_constants(SPerl_PARSER* parser, SPerl_ARRAY* op_constants) {
  for (SPerl_int i = 0; i < op_constants->length; i++) {
    SPerl_OP* op_constant = SPerl_ARRAY_fetch(op_constants, i);
    SPerl_CONSTANT* constant = op_constant->uv.constant;
    printf("        constant[%" PRId32 "]\n", i);
    SPerl_PARSER_dump_constant(parser, constant);
  }
}

void SPerl_PARSER_dump_packages(SPerl_PARSER* parser, SPerl_ARRAY* op_packages) {
  for (SPerl_int i = 0; i < op_packages->length; i++) {
    printf("package[%d]\n", i);
    SPerl_OP* op_package = SPerl_ARRAY_fetch(op_packages, i);
    SPerl_PACKAGE* package = op_package->uv.package;
    printf("    name => \"%s\"\n", package->op_name->uv.word->value);
    
    if (package->op_type) {
      SPerl_TYPE* type = package->op_type->uv.type;
      printf("    type => \"%s\"\n", type->name);
      printf("    resolved_type => \"%s\"\n", type->resolved_type->name);
      printf("    resolved_type_id => %d\n", type->resolved_type->id);
    }
  }
}

void SPerl_PARSER_dump_resolved_types(SPerl_PARSER* parser, SPerl_ARRAY* resolved_types) {
  for (SPerl_int i = 0; i < resolved_types->length; i++) {
    printf("resolved_type[%d]\n", i);
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_ARRAY_fetch(resolved_types, i);
    printf("    name => \"%s\"\n", resolved_type->name);
    printf("    id => \"%d\"\n", resolved_type->id);
  }
}

void SPerl_PARSER_dump_bodys(SPerl_PARSER* parser, SPerl_ARRAY* bodys) {
  
  for (SPerl_int i = 0; i < bodys->length; i++) {
    // Body
    SPerl_BODY* body = SPerl_ARRAY_fetch(bodys, i);

    printf("body[%d]\n", i);
    printf("  code => \"%s\"\n", SPerl_BODY_C_CODE_NAMES[body->code]);
    printf("  name => \"%s\"\n", body->op_name->uv.word->value);
    
    // Core body
    if (body->code == SPerl_BODY_C_CODE_CORE) {
      SPerl_BODY_CORE* body_core = body->uv.body_core;
      printf("  code => \"%s\"\n", SPerl_BODY_CORE_C_CODE_NAMES[body_core->code]);
      printf("  size => %d\n", body_core->size);
    }
    // Class body
    else if (body->code == SPerl_BODY_C_CODE_CLASS) {
      SPerl_BODY_CLASS* body_class = body->uv.body_class;
      
      printf("  descripters => ");
      SPerl_ARRAY* op_descripters = body_class->op_descripters;
      if (op_descripters && op_descripters->length) {
        for (SPerl_int i = 0; i < op_descripters->length; i++) {
          SPerl_OP* op_descripter = SPerl_ARRAY_fetch(op_descripters, i);
          SPerl_DESCRIPTER* descripter = op_descripter->uv.descripter;
          printf("%s ", SPerl_DESCRIPTER_CODE_NAMES[descripter->code]);
        }
      }
      else {
        printf("(None)");
      }
      printf("\n");
      printf("  op_block => %x\n", body_class->op_block);
      
      // Field information
      printf("  fields\n");
      SPerl_ARRAY* op_fields = body_class->op_fields;
      for (SPerl_int j = 0; j < op_fields->length; j++) {
        SPerl_OP* op_field = SPerl_ARRAY_fetch(op_fields, j);
        SPerl_FIELD* field = op_field->uv.field;
        printf("    field[%" PRId32 "]\n", j);
        SPerl_PARSER_dump_field(parser, field);
      }
      
      printf("  is_value_class => %d\n", body_class->is_value_class);
    }
    // Enum body
    else if (body->code == SPerl_BODY_C_CODE_ENUM) {
      SPerl_BODY_ENUM* body_enum = body->uv.body_enum;
      
      // Enum value information
      printf("  enum_values\n");
      SPerl_ARRAY* enum_values = body_enum->enum_values;
      for (SPerl_int j = 0; j < enum_values->length; j++) {
        SPerl_ENUM_VALUE* enum_value = SPerl_ARRAY_fetch(enum_values, j);
        printf("    enum_value[%" PRId32 "]\n", j);
        SPerl_PARSER_dump_enum_value(parser, enum_value);
      }
    }
  }
}

void SPerl_PARSER_dump_const_pool(SPerl_PARSER* parser, SPerl_int* const_pool, SPerl_int const_pool_pos) {
  for (SPerl_int i = 0; i < const_pool_pos; i++) {
    printf("        const_pool[%d] %d\n", i, const_pool[i]);
  }
}

void SPerl_PARSER_dump_constant(SPerl_PARSER* parser, SPerl_CONSTANT* constant) {
  switch(constant->code) {
    case SPerl_CONSTANT_C_CODE_BOOLEAN:
      printf("          boolean %" PRId32 "\n", constant->uv.int_value);
      break;
    case SPerl_CONSTANT_C_CODE_CHAR:
      printf("          char '%c'\n", constant->uv.int_value);
      break;
    case SPerl_CONSTANT_C_CODE_INT:
      printf("          int %" PRId32 "\n", constant->uv.int_value);
      break;
    case SPerl_CONSTANT_C_CODE_LONG:
      printf("          long %" PRId64 "\n", constant->uv.long_value);
      break;
    case SPerl_CONSTANT_C_CODE_FLOAT:
      printf("          float %f\n", constant->uv.float_value);
      break;
    case SPerl_CONSTANT_C_CODE_DOUBLE:
      printf("          double %f\n", constant->uv.double_value);
      break;
    case SPerl_CONSTANT_C_CODE_STRING:
      printf("          string \"%s\"\n", constant->uv.string_value);
      break;
  }
  printf("          pool_pos => %d\n", constant->pool_pos);
}

void SPerl_PARSER_dump_sub(SPerl_PARSER* parser, SPerl_SUB* sub) {
  if (sub) {
    printf("      package_name => \"%s\"\n", sub->op_package->uv.package->op_name->uv.word->value);
    if (sub->anon) {
      printf("      name => (NONE)\n");
    }
    else {
      printf("      name => \"%s\"\n", sub->op_name->uv.word->value);
    }
    printf("      id => %d\n", sub->id);
    printf("      anon => %d\n", sub->anon);

    printf("      return_type => \"%s\"\n", sub->op_return_type->uv.type->name);
    printf("      resolved_type => \"%s\"\n", sub->op_return_type->uv.type->resolved_type->name);
    printf("      resolved_type_id => %d\n", sub->op_return_type->uv.type->resolved_type->id);

    SPerl_int i;
    printf("      descripters => ");
    SPerl_ARRAY* op_descripters = sub->op_descripters;
    if (op_descripters->length) {
      for (SPerl_int i = 0; i < op_descripters->length; i++) {
        SPerl_OP* op_descripter = SPerl_ARRAY_fetch(op_descripters, i);
        SPerl_DESCRIPTER* descripter = op_descripter->uv.descripter;
        printf("%s ", SPerl_DESCRIPTER_CODE_NAMES[descripter->code]);
      }
    }
    else {
      printf("(None)");
    }
    printf("\n");
    printf("      argument_count => %" PRId32 "\n", sub->argument_count);
    printf("      my_vars\n");
    SPerl_ARRAY* op_my_vars = sub->op_my_vars;
    for (SPerl_int i = 0; i < op_my_vars->length; i++) {
      SPerl_OP* op_my_var = SPerl_ARRAY_fetch(sub->op_my_vars, i);
      SPerl_MY_VAR* my_var = op_my_var->uv.my_var;
      printf("        my_var[%d]\n", i);
      SPerl_PARSER_dump_my_var(parser, my_var);
    }
    printf("      op_block => %x\n", sub->op_block);
  
    printf("      constant_values\n");
    SPerl_PARSER_dump_constants(parser, sub->op_constants);
  
    printf("      const_pool\n");
    SPerl_PARSER_dump_const_pool(parser, sub->const_pool, sub->const_pool_length);
  }
  else {
    printf("      None\n");
  }
}

void SPerl_PARSER_dump_field(SPerl_PARSER* parser, SPerl_FIELD* field) {
  if (field) {
    printf("      name => \"%s\"\n", field->op_name->uv.word->value);
    
    SPerl_TYPE* type = field->op_type->uv.type;
    printf("      type => \"%s\"\n", type->name);
    printf("      resolved_type => \"%s\"\n", type->resolved_type->name);
    printf("      resolved_type_id => %d\n", type->resolved_type->id);

    printf("      descripters => ");
    SPerl_ARRAY* op_descripters = field->op_descripters;
    if (op_descripters->length) {
      for (SPerl_int i = 0; i < op_descripters->length; i++) {
        SPerl_OP* op_descripter = SPerl_ARRAY_fetch(op_descripters, i);
        SPerl_DESCRIPTER* descripter = op_descripter->uv.descripter;
        printf("%s ", SPerl_DESCRIPTER_CODE_NAMES[descripter->code]);
      }
    }
    else {
      printf("(None)");
    }
    printf("\n");
  }
  else {
    printf("        None\n");
  }
}


void SPerl_PARSER_dump_enum_value(SPerl_PARSER* parser, SPerl_ENUM_VALUE* enum_value) {
  if (enum_value) {
    printf("      name => \"%s\"\n", enum_value->op_name->uv.word->value);
    printf("      value => %d\n", enum_value->op_constant->uv.constant->uv.int_value);
  }
  else {
    printf("      None\n");
  }
}

void* SPerl_PARSER_alloc_memory_pool(SPerl_PARSER* parser, SPerl_int size) {
  return SPerl_MEMORY_POOL_alloc(parser->memory_pool, size);
}

void SPerl_PARSER_dump_my_var(SPerl_PARSER* parser, SPerl_MY_VAR* my_var) {
  if (my_var) {
    printf("          name => \"%s\"\n", my_var->op_name->uv.word->value);
    
    SPerl_TYPE* type = my_var->op_type->uv.type;
    printf("          type => \"%s\"\n", type->name);
    printf("          resolved_type => \"%s\"\n", type->resolved_type->name);
    printf("          resolved_type_id => %d\n", type->resolved_type->id);
    
    printf("          descripters => ");
    SPerl_ARRAY* op_descripters = my_var->op_descripters;
    if (op_descripters->length) {
      for (SPerl_int i = 0; i < op_descripters->length; i++) {
        SPerl_OP* op_descripter = SPerl_ARRAY_fetch(op_descripters, i);
        SPerl_DESCRIPTER* descripter = op_descripter->uv.descripter;
        printf("%s ", SPerl_DESCRIPTER_CODE_NAMES[descripter->code]);
      }
    }
    else {
      printf("(None)");
    }
    printf("\n");
  }
  else {
    printf("          None\n");
  }
}
