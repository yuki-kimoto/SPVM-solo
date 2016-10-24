#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"
#include "sperl_method_info.h"
#include "sperl_class_info.h"
#include "sperl_const_value.h"
#include "sperl_my_var_info.h"
#include "sperl_field_info.h"
#include "sperl_memory_pool.h"
#include "sperl_op.h"
#include "sperl_var_info.h"
#include "sperl_word_info.h"
#include "sperl_enum_value_info.h"
#include "sperl_descripter_info.h"
#include "sperl_type_info.h"

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
  
  if (length < 40) {
    str = SPerl_MEMORY_POOL_alloc(parser->memory_pool, 40);
  }
  else {
    str = (SPerl_char*)malloc(length + 1);
    SPerl_ARRAY_push(parser->long_str_ptrs, str);
  }
  
  return str;
}

SPerl_PARSER* SPerl_PARSER_new() {
  SPerl_PARSER* parser = (SPerl_PARSER*)calloc(1, sizeof(SPerl_PARSER));
  
  // Manipulate memory
  parser->array_ptrs = SPerl_ARRAY_new(0);
  parser->hash_ptrs = SPerl_ARRAY_new(0);
  parser->long_str_ptrs = SPerl_ARRAY_new(0);
  parser->memory_pool = SPerl_MEMORY_POOL_new(0);
  
  parser->class_infos = SPerl_PARSER_new_array(parser, 0);
  parser->class_info_symtable = SPerl_PARSER_new_hash(parser, 0);
  parser->const_values = SPerl_PARSER_new_array(parser, 0);
  parser->use_info_stack = SPerl_PARSER_new_array(parser, 0);
  parser->typemap = SPerl_PARSER_new_hash(parser, 0);
  
  parser->const_pool_capacity = 1024;
  parser->const_pool = (SPerl_int*)calloc(parser->const_pool_capacity, sizeof(SPerl_int));
  
  parser->next_var_id = 1;
  
  parser->current_method_infos = SPerl_PARSER_new_array(parser, 0);
  parser->include_pathes = SPerl_PARSER_new_array(parser, 0);
  
  parser->bufptr = "";
  
  // Add core type
  SPerl_char* core_types[] = {"boolean", "char", "byte", "short", "int", "long", "float", "double"};
  for (SPerl_int i = 0; i < 8; i++) {
    // Class name
    SPerl_WORD_INFO* name = SPerl_WORD_INFO_new(parser);
    name->value = core_types[i];
    
    // Class
    SPerl_CLASS_INFO* class_info = SPerl_CLASS_INFO_new(parser);
    class_info->type = SPerl_CLASS_INFO_TYPE_CORE;
    class_info->name = name;
    
    SPerl_ARRAY_push(parser->class_infos, class_info);
    SPerl_HASH_insert(parser->class_info_symtable, name->value, strlen(name->value), class_info);
  }
  
  return parser;
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
  SPerl_int type = op->type;
  printf("%s", SPerl_OP_names[type]);
  if (type == SPerl_OP_CONST_VALUE) {
    SPerl_CONST_VALUE* const_value = op->uv.pv;
    switch(const_value->type) {
      case SPerl_CONST_VALUE_TYPE_BOOLEAN:
        printf(" boolean %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_TYPE_CHAR:
        printf(" char '%c'", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_TYPE_BYTE:
        printf(" byte %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_TYPE_SHORT:
        printf(" short %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_TYPE_INT:
        printf(" int %d", const_value->uv.int_value);
        break;
      case SPerl_CONST_VALUE_TYPE_LONG:
        printf(" long %ld", const_value->uv.long_value);
        break;
      case SPerl_CONST_VALUE_TYPE_FLOAT:
        printf(" float %f", const_value->uv.float_value);
        break;
      case SPerl_CONST_VALUE_TYPE_DOUBLE:
        printf(" double %f", const_value->uv.double_value);
        break;
      case SPerl_CONST_VALUE_TYPE_STRING:
        printf(" string \"%s\"", const_value->uv.string_value);
        break;
    }
  }
  else if (type == SPerl_OP_VAR) {
    SPerl_VAR_INFO* var_info = op->uv.pv;
    printf(" \"%s\"", var_info->name_word_info->value);
  }
  else if (type == SPerl_OP_WORD) {
    SPerl_WORD_INFO* word_info = op->uv.pv;
    printf(" \"%s\"", word_info->value);
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
  SPerl_PARSER_dump_ast(parser, parser->op_grammer, 0);
  
  printf("\n[Class infomation]\n");
  SPerl_PARSER_dump_class_infos(parser, parser->class_infos);
  
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

void SPerl_PARSER_dump_class_infos(SPerl_PARSER* parser, SPerl_ARRAY* class_infos) {
  for (SPerl_int i = 0; i < class_infos->length; i++) {
    SPerl_CLASS_INFO* class_info = (SPerl_CLASS_INFO*)SPerl_ARRAY_fetch(class_infos, i);
    
    printf("class_info[%d]\n", i);
    printf("  name => \"%s\"\n", class_info->name->value);
    printf("  descripter_infos => ");
    SPerl_ARRAY* descripter_infos = class_info->descripter_infos;
    if (descripter_infos && descripter_infos->length) {
      for (SPerl_int i = 0; i < descripter_infos->length; i++) {
        SPerl_DESCRIPTER_INFO* descripter_info = SPerl_ARRAY_fetch(descripter_infos, i);
        printf("%s ", SPerl_DESCRIPTER_INFO_type_names[descripter_info->type]);
      }
    }
    else {
      printf("(None)");
    }
    printf("\n");
    printf("  op_block => %x\n", class_info->op_block);
    
    // Class
    if (class_info->type == SPerl_CLASS_INFO_TYPE_NORMAL) {
      // Field information
      printf("  field_infos\n");
      SPerl_ARRAY* field_infos = class_info->field_infos;
      for (SPerl_int j = 0; j < field_infos->length; j++) {
        SPerl_FIELD_INFO* field_info = SPerl_ARRAY_fetch(field_infos, j);
        printf("    field_info[%" PRId32 "]\n", j);
        SPerl_PARSER_dump_field_info(parser, field_info);
      }
      
      // Method information
      printf("  method_infos\n");
      SPerl_ARRAY* method_infos = class_info->method_infos;
      for (SPerl_int j = 0; j < method_infos->length; j++) {
        SPerl_METHOD_INFO* method_info = SPerl_ARRAY_fetch(method_infos, j);
        printf("    method_info[%" PRId32 "]\n", j);
        SPerl_PARSER_dump_method_info(parser, method_info);
      }
    }
    else if (class_info->type == SPerl_CLASS_INFO_TYPE_ENUM) {
      // Enum value information
      printf("  enum_value_infos\n");
      SPerl_ARRAY* enum_value_infos = class_info->enum_value_infos;
      for (SPerl_int j = 0; j < enum_value_infos->length; j++) {
        SPerl_ENUM_VALUE_INFO* enum_value_info = SPerl_ARRAY_fetch(enum_value_infos, j);
        printf("    enum_value_info[%" PRId32 "]\n", j);
        SPerl_PARSER_dump_enum_value_info(parser, enum_value_info);
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
  switch(const_value->type) {
    case SPerl_CONST_VALUE_TYPE_BOOLEAN:
      printf("      boolean %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_TYPE_CHAR:
      printf("      char '%c'\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_TYPE_BYTE:
      printf("      byte %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_TYPE_SHORT:
      printf("      short %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_TYPE_INT:
      printf("      int %" PRId32 "\n", const_value->uv.int_value);
      break;
    case SPerl_CONST_VALUE_TYPE_LONG:
      printf("      long %" PRId64 "\n", const_value->uv.long_value);
      break;
    case SPerl_CONST_VALUE_TYPE_FLOAT:
      printf("      float %f\n", const_value->uv.float_value);
      break;
    case SPerl_CONST_VALUE_TYPE_DOUBLE:
      printf("      double %f\n", const_value->uv.double_value);
      break;
    case SPerl_CONST_VALUE_TYPE_STRING:
      printf("      string \"%s\"\n", const_value->uv.string_value);
      break;
  }
  printf("      pool_pos => %d\n", const_value->pool_pos);
}

void SPerl_PARSER_dump_method_info(SPerl_PARSER* parser, SPerl_METHOD_INFO* method_info) {
  if (method_info) {
    if (method_info->anon) {
      printf("      name => (NONE)\n");
    }
    else {
      printf("      name => \"%s\"\n", method_info->name->value);
    }
    printf("      anon => %d\n", method_info->anon);
    if (method_info->return_type_info->type == SPerl_TYPE_INFO_TYPE_WORDTYPE) {
      SPerl_WORD_INFO* return_type_name = method_info->return_type_info->uv.name;
      printf("      return_type => \"%s\"\n", return_type_name->value);
    }
    SPerl_int i;
    printf("      descripter_infos => ");
    SPerl_ARRAY* descripter_infos = method_info->descripter_infos;
    if (descripter_infos->length) {
      for (SPerl_int i = 0; i < descripter_infos->length; i++) {
        SPerl_DESCRIPTER_INFO* descripter_info = SPerl_ARRAY_fetch(descripter_infos, i);
        printf("%s ", SPerl_DESCRIPTER_INFO_type_names[descripter_info->type]);
      }
    }
    else {
      printf("(None)");
    }
    printf("\n");
    printf("      argument_count => %" PRId32 "\n", method_info->argument_count);
    printf("      my_var_infos\n");
    SPerl_ARRAY* my_var_infos = method_info->my_var_infos;
    for (SPerl_int i = 0; i < my_var_infos->length; i++) {
      SPerl_MY_VAR_INFO* my_var_info
        = (SPerl_MY_VAR_INFO*)SPerl_ARRAY_fetch(method_info->my_var_infos, i);
      printf("        my_var_info[%d]\n", i);
      SPerl_PARSER_dump_my_var_info(parser, my_var_info);
    }
    printf("      op_block => %x\n", method_info->op_block);
  }
  else {
    printf("      None\n");
  }
}


void SPerl_PARSER_dump_field_info(SPerl_PARSER* parser, SPerl_FIELD_INFO* field_info) {
  if (field_info) {
    printf("      name => \"%s\"\n", field_info->name_word_info->value);
    if (field_info->type_info->type == SPerl_TYPE_INFO_TYPE_WORDTYPE) {
      SPerl_WORD_INFO* type_name = field_info->type_info->uv.name;
      printf("      type => \"%s\"\n", type_name->value);
    }
    else if (field_info->type_info->type == SPerl_TYPE_INFO_TYPE_SUBTYPE) {
      SPerl_SUBTYPE_INFO* subtype_info = field_info->type_info->uv.subtype;
      printf("      type => sub (");
      for (SPerl_int i = 0; i < subtype_info->argument_type_infos->length; i++) {
        SPerl_TYPE_INFO* argument_type_info = SPerl_ARRAY_fetch(subtype_info->argument_type_infos, i);
        printf("%s " , argument_type_info->uv.name->value);
      }
      printf(") ");
      SPerl_TYPE_INFO* return_type_info = subtype_info->return_type_info;
      printf("%s\n", return_type_info->uv.name->value);
    }
    printf("      descripter_infos => ");
    SPerl_ARRAY* descripter_infos = field_info->descripter_infos;
    if (descripter_infos->length) {
      for (SPerl_int i = 0; i < descripter_infos->length; i++) {
        SPerl_DESCRIPTER_INFO* descripter_info = SPerl_ARRAY_fetch(descripter_infos, i);
        printf("%s ", SPerl_DESCRIPTER_INFO_type_names[descripter_info->type]);
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


void SPerl_PARSER_dump_enum_value_info(SPerl_PARSER* parser, SPerl_ENUM_VALUE_INFO* enum_value_info) {
  if (enum_value_info) {
    printf("      name => \"%s\"\n", enum_value_info->name->value);
    printf("      value => %d\n", enum_value_info->value->uv.int_value);
  }
  else {
    printf("      None\n");
  }
}

void SPerl_PARSER_dump_my_var_info(SPerl_PARSER* parser, SPerl_MY_VAR_INFO* my_var_info) {
  if (my_var_info) {
    printf("          name => \"%s\"\n", my_var_info->name_word_info->value);
    if (my_var_info->type_info->type == SPerl_TYPE_INFO_TYPE_WORDTYPE) {
      SPerl_WORD_INFO* type_name = my_var_info->type_info->uv.name;
      printf("          type => \"%s\"\n", type_name->value);
    }
    else if (my_var_info->type_info->type == SPerl_TYPE_INFO_TYPE_SUBTYPE) {
      SPerl_SUBTYPE_INFO* subtype_info = my_var_info->type_info->uv.subtype;
      printf("      type => sub (");
      for (SPerl_int i = 0; i < subtype_info->argument_type_infos->length; i++) {
        SPerl_TYPE_INFO* argument_type_info = SPerl_ARRAY_fetch(subtype_info->argument_type_infos, i);
        printf("%s " , argument_type_info->uv.name->value);
      }
      printf(") ");
      SPerl_TYPE_INFO* return_type_info = subtype_info->return_type_info;
      printf("%s\n", return_type_info->uv.name->value);
    }

    printf("          descripter_infos => ");
    SPerl_ARRAY* descripter_infos = my_var_info->descripter_infos;
    if (descripter_infos->length) {
      for (SPerl_int i = 0; i < descripter_infos->length; i++) {
        SPerl_DESCRIPTER_INFO* descripter_info = SPerl_ARRAY_fetch(descripter_infos, i);
        printf("%s ", SPerl_DESCRIPTER_INFO_type_names[descripter_info->type]);
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


