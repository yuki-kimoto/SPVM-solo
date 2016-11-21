#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "sperl_yacc.h"
#include "sperl_parser.h"
#include "sperl_yacc.tab.h"
#include "sperl_const_value.h"
#include "sperl_word.h"
#include "sperl_var.h"
#include "sperl_op.h"

void SPerl_yyerror_format(SPerl_PARSER* parser, SPerl_char* message_template, ...) {
  va_list args;

  SPerl_int message_length = 0;
  
  // Prefix
  SPerl_char* prefix = "Error:";
  SPerl_int prefix_length = strlen(prefix);
   
  // Message template
  SPerl_int message_template_length = strlen(message_template);
  
  // Messsage template with prefix
  SPerl_int message_template_with_prefix_length =  prefix_length + message_template_length;
  SPerl_char* message_template_with_prefix = SPerl_PARSER_new_string(parser, message_template_with_prefix_length);
  memcpy(message_template_with_prefix, prefix, prefix_length);
  memcpy(message_template_with_prefix + prefix_length, message_template, message_template_length);
  message_length += message_template_with_prefix_length;
  
  va_start(args, message_template);
  
  // File
  SPerl_char* file = va_arg(args, SPerl_char*);
  message_length += strlen(file);
  
  // Line number
  SPerl_int line = va_arg(args, SPerl_int);
  SPerl_int line_length;
  for(SPerl_int line_length = 0; line_length != 0; line_length++) {
    line_length /= 10;
  }
  message_length += line_length;
  
  // Arguments
  for (SPerl_int i = 0; i < message_length; i++) {
    SPerl_char* arg = va_arg(args , SPerl_char*);
    message_length += strlen(arg);
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
    case WORD:
      fprintf(file, "\"%s\"", ((SPerl_WORD*)yylval.opval->info)->value);
      break;
    case VAR: {
      SPerl_VAR* var = yylval.opval->info;
      fprintf(file, "\"%s\"", var->name_word->value);
      break;
    }
    case CONSTVALUE: {
      SPerl_CONST_VALUE* const_value = yylval.opval->info;
      
      switch(const_value->code) {
        case SPerl_CONST_VALUE_C_CODE_BOOLEAN:
          fprintf(file, "boolean %d", const_value->uv.int_value);
          break;
        case SPerl_CONST_VALUE_C_CODE_CHAR:
          fprintf(file, "char '%c'", (SPerl_char)const_value->uv.int_value);
          break;
        case SPerl_CONST_VALUE_C_CODE_INT:
          fprintf(file, "int %d", const_value->uv.int_value);
          break;
        case SPerl_CONST_VALUE_C_CODE_LONG:
          fprintf(file, "long %ld", const_value->uv.long_value);
          break;
        case SPerl_CONST_VALUE_C_CODE_FLOAT:
          fprintf(file, "float %f", const_value->uv.float_value);
          break;
        case SPerl_CONST_VALUE_C_CODE_DOUBLE:
          fprintf(file, "double %f", const_value->uv.double_value);
          break;
        case SPerl_CONST_VALUE_C_CODE_STRING:
          fprintf(file, "string %s", const_value->uv.string_value);
          break;
      }
    }
  }
}
