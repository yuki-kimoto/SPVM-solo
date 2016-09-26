#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_parser.h"
#include "sperl_method_info.h"
#include "sperl_array.h"
#include "sperl_argument_info.h"
#include "sperl_class_info.h"


SPerl_yy_parser* SPerl_new_parser() {
  SPerl_yy_parser* parser = (SPerl_yy_parser*)malloc(sizeof(SPerl_yy_parser));
  memset(parser, 0, sizeof(SPerl_yy_parser));
  
  parser->current_method_infos = SPerl_ARRAY_new(0);
  parser->current_const_ops = SPerl_ARRAY_new(0);
  parser->class_infos = SPerl_ARRAY_new(0);
  parser->line = 1;
  
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
    SPerl_ARRAY* method_infos = class_info->method_infos;
    SPerl_int j;
    printf("  method_infos\n");
    for (j = 0; j < method_infos->length; j++) {
      SPerl_METHOD_INFO* method_info = SPerl_ARRAY_fetch(method_infos, j);
      printf("    method_info[%d]\n", j);
      SPerl_PARSER_dump_method_info(method_info);
    }
  }
}

void SPerl_PARSER_dump_method_info(SPerl_METHOD_INFO* method_info) {
  if (method_info) {
    SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
    SPerl_DESCRIPTER_to_str(desc_str, method_info->desc_flags);
    
    printf("      name => \"%s\"\n", method_info->name);
    printf("      argument_count => %d\n", method_info->argument_count);
    
    SPerl_int j;
    printf("      argument_infos\n");
    for (j = 0; j < method_info->argument_count; j++) {
      SPerl_ARGUMENT_INFO* argument_info
        = (SPerl_ARGUMENT_INFO*)SPerl_ARRAY_fetch(method_info->argument_infos, j);
      printf("        argument_info[%d]\n", j);
      printf("        type => \"%s\"\n", argument_info->type);
      
      SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
      SPerl_DESCRIPTER_to_str(desc_str, argument_info->desc_flags);
      printf("        desc_flags => \"%s\"\n", desc_str);
      
      free(desc_str);
    }
    
    printf("      desc_flags => \"%s\"\n", desc_str);
    printf("      treturn_type => \"%s\"\n", method_info->return_type);
    printf("      op_block => %x\n", method_info->op_block);
    printf("      class_info->name => \"%s\"\n", method_info->class_info->name);
    
    free(desc_str);
  }
  else {
    printf("      None\n");
  }
}
