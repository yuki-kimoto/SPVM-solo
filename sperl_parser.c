#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_const_value.h"
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

SPerl_PARSER* SPerl_PARSER_new() {
  SPerl_PARSER* parser = (SPerl_PARSER*)calloc(1, sizeof(SPerl_PARSER));
  
  // Manipulate memory
  parser->array_ptrs = SPerl_ARRAY_new(0);
  parser->hash_ptrs = SPerl_ARRAY_new(0);
  parser->long_str_ptrs = SPerl_ARRAY_new(0);
  parser->memory_pool = SPerl_MEMORY_POOL_new(0);
  
  parser->packages = SPerl_PARSER_new_array(parser, 0);
  parser->package_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->const_values = SPerl_PARSER_new_array(parser, 0);
  parser->use_stack = SPerl_PARSER_new_array(parser, 0);
  
  parser->callsubs = SPerl_PARSER_new_array(parser, 0);
  
  parser->bodys = SPerl_PARSER_new_array(parser, 0);
  parser->body_symtable = SPerl_PARSER_new_hash(parser, 0);
  
  parser->sub_complete_name_symtable = SPerl_PARSER_new_hash(parser, 0);
  
  parser->type_resolved_string_symtable = SPerl_PARSER_new_hash(parser, 0);
  
  parser->const_pool_capacity = 1024;
  parser->const_pool = (SPerl_int*)calloc(parser->const_pool_capacity, sizeof(SPerl_int));
  
  parser->next_var_id = 1;
  
  parser->current_subs = SPerl_PARSER_new_array(parser, 0);
  parser->subs = SPerl_PARSER_new_array(parser, 0);
  parser->include_pathes = SPerl_PARSER_new_array(parser, 0);
  
  parser->bufptr = "";
  
  // Add core type
  for (SPerl_int i = 0; i < 8; i++) {
    // Name
    SPerl_char* name = SPerl_BODY_CORE_C_CODE_NAMES[i];
    
    // Body core
    SPerl_BODY_CORE* body_core = SPerl_BODY_CORE_new(parser);
    body_core->code = i;
    body_core->size = SPerl_BODY_CORE_C_SIZES[i];
    
    // Body
    SPerl_BODY* body = SPerl_BODY_new(parser);
    body->code = SPerl_BODY_C_CODE_CORE;
    body->name = name;
    body->uv.body_core = body_core;
    
    SPerl_ARRAY_push(parser->bodys, body);
    SPerl_HASH_insert(parser->body_symtable, body->name, strlen(body->name), body);
    
    // Type word
    SPerl_WORD* name_word = SPerl_WORD_new(parser);
    name_word->value = name;
    SPerl_TYPE_WORD* type_word = SPerl_TYPE_WORD_new(parser);
    type_word->name_word = name_word;
    
    // Type
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_WORD;
    type->uv.type_word = type_word;
    SPerl_TYPE_build_parts(parser, type);
    
    // Package
    SPerl_PACKAGE* package = SPerl_PACKAGE_new(parser);
    package->name_word = name_word;
    package->type = type;
    
    SPerl_ARRAY_push(parser->packages, package);
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
  SPerl_char* str = (SPerl_char*)malloc(length + 1);
  
  if (length < 40) {
    str = SPerl_MEMORY_POOL_alloc(parser->memory_pool, 40);
  }
  else {
    str = (SPerl_char*)malloc(length + 1);
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
  
  free(parser->const_pool);
  free(parser);
}

void SPerl_PARSER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int depth) {
      
  if (!op) {
    return;
  }
  
  for (SPerl_int i = 0; i < depth; i++) {
    printf(" ");
  }
  SPerl_int code = op->code;
  printf("%s", SPerl_OP_C_CODE_NAMES[code]);
  if (code == SPerl_OP_C_CODE_CONST_VALUE) {
    SPerl_CONST_VALUE* const_value = op->uv.pv;
    switch(const_value->code) {
      case SPerl_CONST_VALUE_C_CODE_BOOLEAN:
        printf(" boolean %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_CHAR:
        printf(" char '%c'", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_BYTE:
        printf(" byte %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_SHORT:
        printf(" short %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_INT:
        printf(" int %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_LONG:
        printf(" long %ld", const_value->uv.long_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_FLOAT:
        printf(" float %f", const_value->uv.float_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_DOUBLE:
        printf(" double %f", const_value->uv.double_value);
        break;
      case SPerl_CONST_VALUE_C_CODE_STRING:
        printf(" string \"%s\"", const_value->uv.string_value);
        break;
    }
  }
  else if (code == SPerl_OP_C_CODE_VAR) {
    SPerl_VAR* var = op->uv.pv;
    printf(" \"%s\"", var->name_word->value);
  }
  else if (code == SPerl_OP_C_CODE_WORD) {
    SPerl_WORD* word = op->uv.pv;
    printf(" \"%s\"", word->value);
  }
  printf("\n");

  if (op->first) {
    depth++;
    SPerl_PARSER_dump_ast(parser, op->first, depth);
    depth--;
  }
  
  if (op->moresib) {
    SPerl_PARSER_dump_ast(parser, SPerl_OP_sibling(parser, op), depth);
  }
}

void SPerl_PARSER_dump_parser(SPerl_PARSER* parser) {
  printf("\n[Abstract Syntax Tree]\n");
  SPerl_PARSER_dump_ast(parser, parser->op_grammer, 0);

  printf("\n[Packages information]\n");
  SPerl_PARSER_dump_packages(parser, parser->packages);
  
  printf("\n[Body information]\n");
  SPerl_PARSER_dump_bodys(parser, parser->bodys);
  
  printf("\n[Subroutine information]\n");
  // Method information
  printf("  subs\n");
  SPerl_ARRAY* subs = parser->subs;
  for (SPerl_int i = 0; i < subs->length; i++) {
    SPerl_SUB* sub = SPerl_ARRAY_fetch(subs, i);
    printf("    sub[%" PRId32 "]\n", i);
    SPerl_PARSER_dump_sub(parser, sub);
  }
  
  printf("\n[Constant information]\n");
  SPerl_PARSER_dump_const_values(parser, parser->const_values);
  
  printf("\n[Constant pool]\n");
  SPerl_PARSER_dump_const_pool(parser, parser->const_pool, parser->const_pool_pos);
}

void SPerl_PARSER_dump_const_values(SPerl_PARSER* parser, SPerl_ARRAY* const_values) {
  for (SPerl_int i = 0; i < const_values->length; i++) {
    SPerl_CONST_VALUE* const_value = (SPerl_CONST_VALUE*)SPerl_ARRAY_fetch(const_values, i);
    printf("    const_value[%" PRId32 "]\n", i);
    SPerl_PARSER_dump_const_value(parser, const_value);
  }
}

void SPerl_PARSER_dump_packages(SPerl_PARSER* parser, SPerl_ARRAY* packages) {
  for (SPerl_int i = 0; i < packages->length; i++) {
    printf("package[%d]\n", i);
    SPerl_PACKAGE* package = SPerl_ARRAY_fetch(packages, i);
    printf("    name => \"%s\"\n", package->name_word->value);
    
    printf("    type => \"");
    SPerl_TYPE_print(parser, package->type, stdout);
    printf("\"\n");
    
    printf("    resolved_type => \"%s\"\n", package->type->resolved_string);
    printf("    type_id => %d\n", package->type->id);
  }
}

void SPerl_PARSER_dump_bodys(SPerl_PARSER* parser, SPerl_ARRAY* bodys) {
  
  for (SPerl_int i = 0; i < bodys->length; i++) {
    // Body
    SPerl_BODY* body = SPerl_ARRAY_fetch(bodys, i);

    printf("body[%d]\n", i);
    printf("  code => \"%s\"\n", SPerl_BODY_C_CODE_NAMES[body->code]);
    printf("  name => \"%s\"\n", body->name);
    
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
      SPerl_ARRAY* descripters = body_class->descripters;
      if (descripters && descripters->length) {
        for (SPerl_int i = 0; i < descripters->length; i++) {
          SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, i);
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
      SPerl_ARRAY* fields = body_class->fields;
      for (SPerl_int j = 0; j < fields->length; j++) {
        SPerl_FIELD* field = SPerl_ARRAY_fetch(fields, j);
        printf("    field[%" PRId32 "]\n", j);
        SPerl_PARSER_dump_field(parser, field);
      }
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
    printf("const_pool[%d] %d\n", i, const_pool[i]);
  }
}

void SPerl_PARSER_dump_const_value(SPerl_PARSER* parser, SPerl_CONST_VALUE* const_value) {
  switch(const_value->code) {
    case SPerl_CONST_VALUE_C_CODE_BOOLEAN:
      printf("      boolean %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_CHAR:
      printf("      char '%c'\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_BYTE:
      printf("      byte %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_SHORT:
      printf("      short %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_INT:
      printf("      int %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_LONG:
      printf("      long %" PRId64 "\n", const_value->uv.long_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_FLOAT:
      printf("      float %f\n", const_value->uv.float_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_DOUBLE:
      printf("      double %f\n", const_value->uv.double_value);
      break;
    case SPerl_CONST_VALUE_C_CODE_STRING:
      printf("      string \"%s\"\n", const_value->uv.string_value);
      break;
  }
  printf("      pool_pos => %d\n", const_value->pool_pos);
}

void SPerl_PARSER_dump_sub(SPerl_PARSER* parser, SPerl_SUB* sub) {
  if (sub) {
    printf("      package_name => \"%s\"\n", sub->package_name);
    if (sub->anon) {
      printf("      name => (NONE)\n");
    }
    else {
      printf("      name => \"%s\"\n", sub->name_word->value);
    }
    printf("      id => %d\n", sub->id);
    printf("      anon => %d\n", sub->anon);

    printf("      return_type => \"");
    SPerl_TYPE_print(parser, sub->return_type, stdout);
    printf("\"\n");
    printf("      resolved_type => \"%s\"\n", sub->return_type->resolved_string);
    printf("      type_id => %d\n", sub->return_type->id);

    SPerl_int i;
    printf("      descripters => ");
    SPerl_ARRAY* descripters = sub->descripters;
    if (descripters->length) {
      for (SPerl_int i = 0; i < descripters->length; i++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, i);
        printf("%s ", SPerl_DESCRIPTER_CODE_NAMES[descripter->code]);
      }
    }
    else {
      printf("(None)");
    }
    printf("\n");
    printf("      argument_count => %" PRId32 "\n", sub->argument_count);
    printf("      my_vars\n");
    SPerl_ARRAY* my_vars = sub->my_vars;
    for (SPerl_int i = 0; i < my_vars->length; i++) {
      SPerl_MY_VAR* my_var
        = (SPerl_MY_VAR*)SPerl_ARRAY_fetch(sub->my_vars, i);
      printf("        my_var[%d]\n", i);
      SPerl_PARSER_dump_my_var(parser, my_var);
    }
    printf("      op_block => %x\n", sub->op_block);
  }
  else {
    printf("      None\n");
  }
}

void SPerl_PARSER_dump_field(SPerl_PARSER* parser, SPerl_FIELD* field) {
  if (field) {
    printf("      name => \"%s\"\n", field->name_word->value);
    
    SPerl_TYPE* type = field->type;
    printf("      type => \"");
    SPerl_TYPE_print(parser, type, stdout);
    printf("\"\n");
    printf("      resolved_type => \"%s\"\n", type->resolved_string);
    printf("      type_id => %d\n", type->id);

    printf("      descripters => ");
    SPerl_ARRAY* descripters = field->descripters;
    if (descripters->length) {
      for (SPerl_int i = 0; i < descripters->length; i++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, i);
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
    printf("      name => \"%s\"\n", enum_value->name_word->value);
    printf("      value => %d\n", enum_value->value->uv.int_value);
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
    printf("          name => \"%s\"\n", my_var->name_word->value);
    
    SPerl_TYPE* type = my_var->type;
    printf("          type => \"");
    SPerl_TYPE_print(parser, type, stdout);
    printf("\"\n");
    printf("          resolved_type => \"%s\"\n", my_var->type->resolved_string);
    printf("          type_id => %d\n", my_var->type->id);
    
    printf("          descripters => ");
    SPerl_ARRAY* descripters = my_var->descripters;
    if (descripters->length) {
      for (SPerl_int i = 0; i < descripters->length; i++) {
        SPerl_DESCRIPTER* descripter = SPerl_ARRAY_fetch(descripters, i);
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
