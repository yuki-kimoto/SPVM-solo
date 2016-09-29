#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "sperl_type.h"
#include "sperl_parser.h"
#include "sperl_method_info.h"
#include "sperl_array.h"
#include "sperl_argument_info.h"
#include "sperl_class_info.h"
#include "sperl_const_info.h"


SPerl_yy_parser* SPerl_new_parser() {
  SPerl_yy_parser* parser = (SPerl_yy_parser*)calloc(1, sizeof(SPerl_yy_parser));
  
  parser->current_field_infos = SPerl_ARRAY_new(0);
  parser->current_method_infos = SPerl_ARRAY_new(0);
  parser->current_const_infos = SPerl_ARRAY_new(0);
  parser->current_my_var_infos = SPerl_ARRAY_new(0);
  parser->class_infos = SPerl_ARRAY_new(0);
  parser->same_const_h = SPerl_HASH_new(0);
  parser->line = 1;
  parser->const_pool_size = 1024;
  parser->const_pool = (SPerl_int*)calloc(parser->const_pool_size, sizeof(SPerl_int));
  
  return parser;
}

void SPerl_PARSER_dump_class_infos(SPerl_yy_parser* parser) {
  SPerl_int i;
  
  if (!parser) {
    return;
  }
  
  SPerl_ARRAY* class_infos = parser->class_infos;
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
      SPerl_PARSER_dump_field_info(field_info);
    }
    
    printf("  method_infos\n");
    SPerl_ARRAY* method_infos = class_info->method_infos;
    for (j = 0; j < method_infos->length; j++) {
      SPerl_METHOD_INFO* method_info = SPerl_ARRAY_fetch(method_infos, j);
      printf("    method_info[%" PRId32 "]\n", j);
      SPerl_PARSER_dump_method_info(method_info);
    }
    
    printf("  const_infos\n");
    SPerl_ARRAY* const_infos = class_info->const_infos;
    for (j = 0; j < const_infos->length; j++) {
      SPerl_CONST_INFO* const_info = (SPerl_CONST_INFO*)SPerl_ARRAY_fetch(const_infos, j);
      printf("    const_info[%" PRId32 "]\n", j);
      SPerl_PARSER_dump_const_info(const_info);
    }
  }
}

void SPerl_PARSER_dump_const_pool(SPerl_yy_parser* parser) {
  SPerl_int* const_pool = parser->const_pool;
  SPerl_int const_pool_length = parser->const_pool_pos;
  
  SPerl_int i;
  for (i = 0; i < const_pool_length; i++) {
    printf("const_pool[%d] %d\n", i, const_pool[i]);
  }
}

void SPerl_PARSER_dump_const_info(SPerl_CONST_INFO* const_info) {
  switch(const_info->type) {
    case SPerl_CONST_INFO_BOOLEAN:
      printf("      boolean %" PRId32 "\n", const_info->uv.boolean_value);
      break;
    case SPerl_CONST_INFO_CHAR:
      printf("      char '%c'\n", const_info->uv.char_value);
      break;
    case SPerl_CONST_INFO_BYTE:
      printf("      byte %" PRId32 "\n", const_info->uv.byte_value);
      break;
    case SPerl_CONST_INFO_SHORT:
      printf("      short %" PRId32 "\n", const_info->uv.short_value);
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

void SPerl_PARSER_dump_method_info(SPerl_METHOD_INFO* method_info) {
  if (method_info) {
    SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
    SPerl_DESCRIPTER_to_str(desc_str, method_info->desc_flags);
    
    printf("      name => \"%s\"\n", method_info->name);
    printf("      argument_count => %" PRId32 "\n", method_info->argument_count);
    
    SPerl_int j;
    printf("      argument_infos\n");
    for (j = 0; j < method_info->argument_count; j++) {
      SPerl_ARGUMENT_INFO* argument_info
        = (SPerl_ARGUMENT_INFO*)SPerl_ARRAY_fetch(method_info->argument_infos, j);
      printf("        argument_info[%" PRId32 "]\n", j);
      printf("        type => \"%s\"\n", argument_info->type);
      
      SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
      SPerl_DESCRIPTER_to_str(desc_str, argument_info->desc_flags);
      printf("        desc_flags => \"%s\"\n", desc_str);
      
      free(desc_str);
    }
    
    printf("      desc_flags => \"%s\"\n", desc_str);
    printf("      treturn_type => \"%s\"\n", method_info->return_type);
    printf("      op_block => %x\n", method_info->op_block);
    
    free(desc_str);
  }
  else {
    printf("      None\n");
  }
}


void SPerl_PARSER_dump_field_info(SPerl_FIELD_INFO* field_info) {
  if (field_info) {
    SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
    SPerl_DESCRIPTER_to_str(desc_str, field_info->desc_flags);
    
    printf("      name => \"%s\"\n", field_info->name);
    printf("      desc_flags => \"%s\"\n", desc_str);
    printf("      type => \"%s\"\n", field_info->type);
    
    free(desc_str);
  }
  else {
    printf("      None\n");
  }
}
