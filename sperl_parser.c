#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"
#include "sperl_method_info.h"
#include "sperl_class_info.h"
#include "sperl_const_info.h"
#include "sperl_my_var_info.h"
#include "sperl_descripter.h"
#include "sperl_field_info.h"
#include "sperl_memory_pool.h"
#include "sperl_op.h"
#include "sperl_var_info.h"

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

SPerl_char* SPerl_PARSER_new_string(SPerl_PARSER* parser, SPerl_int length) {
  SPerl_char* str = (SPerl_char*)malloc(length + 1);
  
  SPerl_ARRAY_push(parser->str_ptrs, str);
  
  return str;
}

SPerl_PARSER* SPerl_PARSER_new() {
  SPerl_PARSER* parser = (SPerl_PARSER*)calloc(1, sizeof(SPerl_PARSER));
  
  // Manipulate memory
  parser->array_ptrs = SPerl_ARRAY_new(0);
  parser->hash_ptrs = SPerl_ARRAY_new(0);
  parser->str_ptrs = SPerl_ARRAY_new(0);
  parser->class_stack = SPerl_ARRAY_new(0);
  parser->memory_pool = SPerl_MEMORY_POOL_new(0);
  
  parser->class_infos = SPerl_PARSER_new_array(parser, 0);
  parser->class_info_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->const_infos = SPerl_PARSER_new_array(parser, 0);
  
  parser->const_pool_capacity = 1024;
  parser->const_pool = (SPerl_int*)calloc(parser->const_pool_capacity, sizeof(SPerl_int));
  
  parser->cur_line = 1;
  
  return parser;
}

void SPerl_PARSER_free(SPerl_PARSER* parser) {

  SPerl_int i;
  
  // Free all array pointers
  for (i = 0; i < parser->array_ptrs->length; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(parser->array_ptrs, i);
    SPerl_ARRAY_free(array);
  }
  SPerl_ARRAY_free(parser->array_ptrs);
  
  // Free all hash pointers
  for (i = 0; i < parser->hash_ptrs->length; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(parser->hash_ptrs, i);
    SPerl_HASH_free(hash);
  }
  SPerl_ARRAY_free(parser->hash_ptrs);
  
  // Free all string pointers;
  for (i = 0; i < parser->str_ptrs->length; i++) {
    SPerl_char* str = SPerl_ARRAY_fetch(parser->str_ptrs, i);
    free(str);
  }
  SPerl_ARRAY_free(parser->str_ptrs);
  
  // Free memory pool */
  SPerl_MEMORY_POOL_free(parser->memory_pool);
  
  free(parser->const_pool);
  free(parser);
}

void SPerl_PARSER_dump_ast(SPerl_PARSER* parser, SPerl_OP* op, SPerl_int depth) {

  
  SPerl_int i;
  for (i = 0; i < depth; i++) {
    printf(" ");
  }
  SPerl_int type = op->type;
  printf("%s", SPerl_OP_names[type]);
  if (type == SPerl_OP_CONST) {
    SPerl_CONST_INFO* const_info = (SPerl_CONST_INFO*)op->uv.pv;
    switch(const_info->type) {
      case SPerl_CONST_INFO_BOOLEAN:
        printf(" boolean %d", const_info->uv.int_value);
        break;
      case SPerl_CONST_INFO_CHAR:
        printf(" char '%c'", const_info->uv.int_value);
        break;
      case SPerl_CONST_INFO_BYTE:
        printf(" byte %d", const_info->uv.int_value);
        break;
      case SPerl_CONST_INFO_SHORT:
        printf(" short %d", const_info->uv.int_value);
        break;
      case SPerl_CONST_INFO_INT:
        printf(" int %d", const_info->uv.int_value);
        break;
      case SPerl_CONST_INFO_LONG:
        printf(" long %ld", const_info->uv.long_value);
        break;
      case SPerl_CONST_INFO_FLOAT:
        printf(" float %f", const_info->uv.float_value);
        break;
      case SPerl_CONST_INFO_DOUBLE:
        printf(" double %f", const_info->uv.double_value);
        break;
      case SPerl_CONST_INFO_STRING:
        printf(" string \"%s\"", const_info->uv.string_value);
        break;
    }
  }
  else if (type == SPerl_OP_VAR) {
    SPerl_VAR_INFO* var_info = (SPerl_VAR_INFO*)op->uv.pv;
    printf(" \"%s\"", var_info->name);
  }
  else if (type == SPerl_OP_WORD) {
    printf(" \"%s\"", op->uv.pv);
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

void SPerl_PARSER_dump_parser_info(SPerl_PARSER* parser) {
  printf("\n[Abstract Syntax Tree]\n");
  SPerl_PARSER_dump_ast(parser, parser->op_root, 0);
  
  printf("\n[Class infomation]\n");
  SPerl_PARSER_dump_class_infos(parser, parser->class_infos);
  
  printf("\n[Constant information]\n");
  SPerl_PARSER_dump_const_infos(parser, parser->const_infos);
  
  printf("\n[Constant pool]\n");
  SPerl_PARSER_dump_const_pool(parser, parser->const_pool, parser->const_pool_pos);
}

void SPerl_PARSER_dump_const_infos(SPerl_PARSER* parser, SPerl_ARRAY* const_infos) {
  SPerl_int i;
  for (i = 0; i < const_infos->length; i++) {
    SPerl_CONST_INFO* const_info = (SPerl_CONST_INFO*)SPerl_ARRAY_fetch(const_infos, i);
    printf("    const_info[%" PRId32 "]\n", i);
    SPerl_PARSER_dump_const_info(parser, const_info);
  }
}

void SPerl_PARSER_dump_class_infos(SPerl_PARSER* parser, SPerl_ARRAY* class_infos) {
  SPerl_int i;

  for (i = 0; i < class_infos->length; i++) {
    SPerl_CLASS_INFO* class_info = (SPerl_CLASS_INFO*)SPerl_ARRAY_fetch(class_infos, i);
    
    printf("class_info[%d]\n", i);
    printf("  name => \"%s\"\n", class_info->name);
    printf("  op_block => %x\n", class_info->op_block);
    
    // Dump method informations
    SPerl_int j;
    printf("  field_infos\n");
    SPerl_ARRAY* field_infos = class_info->field_infos;
    for (j = 0; j < field_infos->length; j++) {
      SPerl_FIELD_INFO* field_info = SPerl_ARRAY_fetch(field_infos, j);
      printf("    field_info[%" PRId32 "]\n", j);
      SPerl_PARSER_dump_field_info(parser, field_info);
    }
    
    printf("  method_infos\n");
    SPerl_ARRAY* method_infos = class_info->method_infos;
    for (j = 0; j < method_infos->length; j++) {
      SPerl_METHOD_INFO* method_info = SPerl_ARRAY_fetch(method_infos, j);
      printf("    method_info[%" PRId32 "]\n", j);
      SPerl_PARSER_dump_method_info(parser, method_info);
    }
  }
}

void SPerl_PARSER_dump_const_pool(SPerl_PARSER* parser, SPerl_int* const_pool, SPerl_int const_pool_pos) {
  SPerl_int i;
  for (i = 0; i < const_pool_pos; i++) {
    printf("const_pool[%d] %d\n", i, const_pool[i]);
  }
}

void SPerl_PARSER_dump_const_info(SPerl_PARSER* parser, SPerl_CONST_INFO* const_info) {
  switch(const_info->type) {
    case SPerl_CONST_INFO_BOOLEAN:
      printf("      boolean %" PRId32 "\n", const_info->uv.int_value);
      break;
    case SPerl_CONST_INFO_CHAR:
      printf("      char '%c'\n", const_info->uv.int_value);
      break;
    case SPerl_CONST_INFO_BYTE:
      printf("      byte %" PRId32 "\n", const_info->uv.int_value);
      break;
    case SPerl_CONST_INFO_SHORT:
      printf("      short %" PRId32 "\n", const_info->uv.int_value);
      break;
    case SPerl_CONST_INFO_INT:
      printf("      int %" PRId32 "\n", const_info->uv.int_value);
      break;
    case SPerl_CONST_INFO_LONG:
      printf("      long %" PRId64 "\n", const_info->uv.long_value);
      break;
    case SPerl_CONST_INFO_FLOAT:
      printf("      float %f\n", const_info->uv.float_value);
      break;
    case SPerl_CONST_INFO_DOUBLE:
      printf("      double %f\n", const_info->uv.double_value);
      break;
    case SPerl_CONST_INFO_STRING:
      printf("      string \"%s\"\n", const_info->uv.string_value);
      break;
  }
  printf("      pool_pos => %d\n", const_info->pool_pos);
}

void SPerl_PARSER_dump_method_info(SPerl_PARSER* parser, SPerl_METHOD_INFO* method_info) {
  if (method_info) {
    SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
    SPerl_DESCRIPTER_to_str(desc_str, method_info->desc_flags);
    
    printf("      name => \"%s\"\n", method_info->name);
    printf("      argument_count => %" PRId32 "\n", method_info->argument_count);
    
    SPerl_int j;
    
    printf("      desc_flags => \"%s\"\n", desc_str);
    printf("      return_type => \"%s\"\n", method_info->return_type);
    printf("      op_block => %x\n", method_info->op_block);

    printf("      my_var_infos\n");
    SPerl_ARRAY* my_var_infos = method_info->my_var_infos;
    for (j = 0; j < my_var_infos->length; j++) {
      SPerl_MY_VAR_INFO* my_var_info
        = (SPerl_MY_VAR_INFO*)SPerl_ARRAY_fetch(method_info->my_var_infos, j);
      printf("        my_var_info[%d]\n", j);
      SPerl_PARSER_dump_my_var_info(parser, my_var_info);
    }
    
    free(desc_str);
  }
  else {
    printf("      None\n");
  }
}


void SPerl_PARSER_dump_field_info(SPerl_PARSER* parser, SPerl_FIELD_INFO* field_info) {
  if (field_info) {
    SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
    SPerl_DESCRIPTER_to_str(desc_str, field_info->desc_flags);
    
    printf("        name => \"%s\"\n", field_info->name);
    printf("        desc_flags => \"%s\"\n", desc_str);
    printf("        type => \"%s\"\n", field_info->type);
    
    free(desc_str);
  }
  else {
    printf("        None\n");
  }
}

void SPerl_PARSER_dump_my_var_info(SPerl_PARSER* parser, SPerl_MY_VAR_INFO* my_var_info) {
  if (my_var_info) {
    SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
    SPerl_DESCRIPTER_to_str(desc_str, my_var_info->desc_flags);
    
    printf("          name => \"%s\"\n", my_var_info->name);
    printf("          type => \"%s\"\n", my_var_info->type);
    printf("          desc_flags => \"%s\"\n", desc_str);
    
    free(desc_str);
  }
  else {
    printf("          None\n");
  }
}
