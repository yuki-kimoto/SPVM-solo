#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "sperl_yacc.h"
#include "sperl_parser.h"
#include "sperl_yacc.tab.h"
#include "sperl_constant.h"
#include "sperl_word.h"
#include "sperl_var.h"
#include "sperl_op.h"

void SPerl_yyerror_format(SPerl_PARSER* parser, SPerl_char* message_template, ...) {
  
  SPerl_int message_length = 0;
  
  // Prefix
  SPerl_char* prefix = "Error:";
  SPerl_int prefix_length = strlen(prefix);
   
  // Message template
  SPerl_int message_template_length = strlen(message_template);
  
  // Messsage template with prefix
  SPerl_int message_template_with_prefix_length = prefix_length + message_template_length;
  SPerl_char* message_template_with_prefix = SPerl_PARSER_new_string(parser, message_template_with_prefix_length);
  strncpy(message_template_with_prefix, prefix, prefix_length);
  strncpy(message_template_with_prefix + prefix_length, message_template, message_template_length);
  message_template_with_prefix[message_template_with_prefix_length] = '\0';
  message_length += message_template_with_prefix_length;
  
  va_list args;
  va_start(args, message_template);
  
  // Argument count
  SPerl_char* found_ptr = message_template_with_prefix;
  while (1) {
    found_ptr = strchr(found_ptr, '%');
    if (found_ptr) {
      if (*(found_ptr + 1) == 's') {
        SPerl_char* arg = va_arg(args, SPerl_char*);
        message_length += strlen(arg);
      }
      else if (*(found_ptr + 1) == 'd') {
        SPerl_int arg = va_arg(args, SPerl_int);
        message_length += 30;
      }
      else {
        fprintf(stderr, "Invalid arguments(SPerl_yyerror_format)\n");
        exit(1);
      }
      found_ptr++;
    }
    else {
      break;
    }
  }
  va_end(args);
  
  SPerl_char* message = SPerl_PARSER_new_string(parser, message_length);
  
  va_start(args, message_template);
  vsprintf(message, message_template_with_prefix, args);
  va_end(args);
  
  SPerl_yyerror(parser, message);
}

// Print error
void SPerl_yyerror(SPerl_PARSER* parser, const SPerl_char* message)
{
  parser->error_count++;
  
  if (memcmp(message, "Error:", 6) == 0) {
    fprintf(stderr, "%s", message);
  }
  // Syntax structure error
  else {
    // Current token
    SPerl_int length = 0;
    SPerl_int empty_count = 0;
    SPerl_char* ptr = parser->befbufptr;
    while (ptr != parser->bufptr) {
      if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
        empty_count++;
      }
      else {
        length++;
      }
      ptr++;
    }
    SPerl_char* token = calloc(length + 1, sizeof(SPerl_char));
    memcpy(token, parser->befbufptr + empty_count, length);
    token[length] = '\0';
    
    fprintf(stderr, "Error: unexpected token \"%s\" at %s line %d\n", token, parser->cur_file, parser->cur_line);
  }
}

// Print token value for debug
void SPerl_yyprint (FILE *file, int type, YYSTYPE yylval) {
  
  switch(type) {
    case WORD: {
      SPerl_WORD* word = yylval.opval->uv.word;
      fprintf(file, "\"%s\"", word->value);
      break;
    }
    case VAR: {
      SPerl_VAR* var = yylval.opval->uv.var;
      fprintf(file, "\"%s\"", var->name_word->value);
      break;
    }
    case CONSTVALUE: {
      SPerl_CONSTANT* constant = yylval.opval->uv.constant;
      
      switch(constant->code) {
        case SPerl_CONSTANT_C_CODE_BOOLEAN:
          fprintf(file, "boolean %d", constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_CHAR:
          fprintf(file, "char '%c'", (SPerl_char)constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_INT:
          fprintf(file, "int %d", constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_LONG:
          fprintf(file, "long %ld", constant->uv.long_value);
          break;
        case SPerl_CONSTANT_C_CODE_FLOAT:
          fprintf(file, "float %f", constant->uv.float_value);
          break;
        case SPerl_CONSTANT_C_CODE_DOUBLE:
          fprintf(file, "double %f", constant->uv.double_value);
          break;
        case SPerl_CONSTANT_C_CODE_STRING:
          fprintf(file, "string %s", constant->uv.string_value);
          break;
      }
    }
  }
}
