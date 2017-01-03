#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <inttypes.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_yacc.h"
#include "sperl_allocator.h"
#include "sperl_yacc.tab.h"
#include "sperl_constant.h"
#include "sperl_var.h"
#include "sperl_op.h"

void SPerl_yyerror_format(SPerl* sperl, const char* message_template, ...) {
  
  int32_t message_length = 0;
  
  // Prefix
  const char* prefix = "Error:";
  int32_t prefix_length = strlen(prefix);
   
  // Message template
  int32_t message_template_length = strlen(message_template);
  
  // Messsage template with prefix
  int32_t message_template_with_prefix_length = prefix_length + message_template_length;
  char* message_template_with_prefix = SPerl_ALLOCATOR_new_string(sperl, message_template_with_prefix_length);
  strncpy(message_template_with_prefix, prefix, prefix_length);
  strncpy(message_template_with_prefix + prefix_length, message_template, message_template_length);
  message_template_with_prefix[message_template_with_prefix_length] = '\0';
  message_length += message_template_with_prefix_length;
  
  va_list args;
  va_start(args, message_template);
  
  // Argument count
  char* found_ptr = message_template_with_prefix;
  while (1) {
    found_ptr = strchr(found_ptr, '%');
    if (found_ptr) {
      if (*(found_ptr + 1) == 's') {
        char* arg = va_arg(args, char*);
        message_length += strlen(arg);
      }
      else if (*(found_ptr + 1) == 'd') {
        (void) va_arg(args, int);
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
  
  char* message = SPerl_ALLOCATOR_new_string(sperl, message_length);
  
  va_start(args, message_template);
  vsprintf(message, message_template_with_prefix, args);
  va_end(args);
  
  SPerl_yyerror(sperl, message);
}

// Print error
void SPerl_yyerror(SPerl* sperl, const char* message)
{
  SPerl_PARSER* parser = sperl->parser;
  
  sperl->error_count++;
  
  if (memcmp(message, "Error:", 6) == 0) {
    fprintf(stderr, "%s", message);
  }
  // Syntax structure error
  else {
    // Current token
    int32_t length = 0;
    int32_t empty_count = 0;
    const char* ptr = parser->befbufptr;
    while (ptr != parser->bufptr) {
      if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
        empty_count++;
      }
      else {
        length++;
      }
      ptr++;
    }
    char* token = (char*) calloc(length + 1, sizeof(char));
    memcpy(token, parser->befbufptr + empty_count, length);
    token[length] = '\0';

    fprintf(stderr, "Error: unexpected token \"%s\" at %s line %d\n", token, sperl->cur_module_path, sperl->cur_line);
    free(token);
  }
}

// Print token value for debug
void SPerl_yyprint (FILE *file, int type, YYSTYPE yylval) {
  
  switch(type) {
    case NAME: {
      fprintf(file, "\"%s\"", yylval.opval->uv.name);
      break;
    }
    case VAR: {
      SPerl_VAR* var = yylval.opval->uv.var;
      fprintf(file, "\"%s\"", var->op_name->uv.name);
      break;
    }
    case CONSTANT: {
      SPerl_CONSTANT* constant = yylval.opval->uv.constant;
      
      switch(constant->code) {
        case SPerl_CONSTANT_C_CODE_BOOLEAN:
          fprintf(file, "boolean %" PRId32, constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_BYTE:
          fprintf(file, "char '%c'", (char) constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_INT:
          fprintf(file, "int %" PRId32, constant->uv.int_value);
          break;
        case SPerl_CONSTANT_C_CODE_LONG:
          fprintf(file, "long %" PRId64, constant->uv.long_value);
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
