#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <inttypes.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_toke.h"
#include "sperl_yacc_util.h"
#include "sperl_yacc.h"
#include "sperl_op.h"
#include "sperl_allocator_parser.h"
#include "sperl_allocator_util.h"
#include "sperl_constant.h"
#include "sperl_var.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_descriptor.h"
#include "sperl_type.h"

SPerl_OP* SPerl_TOKE_newOP(SPerl* sperl, int32_t type) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  SPerl_OP* op = SPerl_OP_newOP(sperl, type, parser->cur_module_path, parser->cur_line);
  
  return op;
}

// Get token
int SPerl_yylex(SPerl_YYSTYPE* yylvalp, SPerl* sperl) {
  
  // Parser
  SPerl_PARSER* parser = sperl->parser;

  // Save buf pointer
  parser->befbufptr = parser->bufptr;
  
  while(1) {
    // Get current character
    char c = *parser->bufptr;
    
    // line end
    switch(c) {
      case '\0':
        parser->cur_module_path = NULL;
        parser->cur_src = NULL;
        
        // If there are more module, load it
        SPerl_ARRAY* op_use_stack = parser->op_use_stack;
        
        while (1) {
          SPerl_OP* op_use = NULL;
          if (op_use_stack->length > 0) {
            op_use = SPerl_ARRAY_pop(sperl, op_use_stack);
          }
          
          if (op_use) {
            SPerl_OP* op_package_name = op_use->first;
            const char* package_name = op_package_name->uv.name;
            
            SPerl_OP* found_op_package = SPerl_HASH_search(sperl, parser->op_package_symtable, package_name, strlen(package_name));
            
            if (found_op_package) {
              continue;
            }
            else {
              
              // Change :: to / and add ".spvm"
              int32_t module_path_base_length = (int32_t)(strlen(package_name) + 6);
              char* module_path_base = SPerl_ALLOCATOR_PARSER_alloc_string(sperl, parser->allocator, module_path_base_length);
              const char* bufptr_orig = package_name;
              char* bufptr_to = module_path_base;
              while (*bufptr_orig) {
                if (*bufptr_orig == ':' && *(bufptr_orig + 1) == ':') {
                  *bufptr_to = '/';
                  bufptr_orig += 2;
                  bufptr_to++;
                }
                else {
                  *bufptr_to = *bufptr_orig;
                  bufptr_orig++;
                  bufptr_to++;
                }
              }
              strncpy(bufptr_to, ".spvm", 5);
              bufptr_to += 5;
              *bufptr_to = '\0';
              
              // Search module file
              char* cur_module_path = NULL;
              FILE* fh = NULL;
              int32_t include_pathes_length = parser->include_pathes->length;
              for (int32_t i = 0; i < include_pathes_length; i++) {
                const char* include_path = (const char*) SPerl_ARRAY_fetch(sperl, parser->include_pathes, i);
                
                // File name
                int32_t file_name_length = (int32_t)(strlen(include_path) + 1 + strlen(module_path_base));
                cur_module_path = SPerl_ALLOCATOR_PARSER_alloc_string(sperl, parser->allocator, file_name_length);
                sprintf(cur_module_path, "%s/%s", include_path, module_path_base);
                cur_module_path[file_name_length] = '\0';
                
                // Open source file
                fh = fopen(cur_module_path, "r");
                if (fh) {
                  parser->cur_module_path = cur_module_path;
                  break;
                }
                errno = 0;
              }
              if (!fh) {
                if (op_use) {
                  fprintf(stderr, "Can't find package \"%s\" at %s line %" PRId32 "\n", op_package_name->uv.name, op_use->file, op_use->line);
                }
                else {
                  fprintf(stderr, "Can't find file %s\n", cur_module_path);
                }
                exit(1);
              }
              
              parser->cur_module_path = cur_module_path;
              
              
              // Read file content
              fseek(fh, 0, SEEK_END);
              int32_t file_size = (int32_t)ftell(fh);
              if (file_size < 0) {
                fprintf(stderr, "Can't read file %s at %s line %" PRId32 "\n", cur_module_path, op_use->file, op_use->line);
                exit(1);
              }
              fseek(fh, 0, SEEK_SET);
              char* src = SPerl_ALLOCATOR_PARSER_alloc_string(sperl, parser->allocator, file_size);
              if ((int32_t)fread(src, 1, file_size, fh) < file_size) {
                if (op_use) {
                  fprintf(stderr, "Can't read file %s at %s line %" PRId32 "\n", cur_module_path, op_use->file, op_use->line);
                }
                else {
                  fprintf(stderr, "Can't read file %s\n", cur_module_path);
                }
                exit(1);
              }
              fclose(fh);
              src[file_size] = '\0';
              
              parser->cur_src = src;
              parser->bufptr = src;
              parser->befbufptr = src;
              parser->current_package_count = 0;
              parser->cur_line = 1;
              break;
            }
          }
          else {
            return 0;
          }
        }
        if (parser->cur_src) {
          continue;
        }
        else {
          return 0;
        }
      
      /* Skip space character */
      case ' ':
      case '\t':
        parser->bufptr++;
        continue;

      case '\n':
        parser->bufptr++;
        parser->cur_line++;
        continue;
      
      /* Addition */
      case '+':
        parser->bufptr++;
        
        if (*parser->bufptr == '+') {
          parser->bufptr++;
          yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NULL);
          return INC;
        }
        else {
          yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NULL);
          return '+';
        }
      
      /* Subtract */
      case '-':
        parser->bufptr++;
        
        if (*parser->bufptr == '>') {
          parser->bufptr++;
          yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NULL);
          return ARROW;
        }
        else if (*parser->bufptr == '-') {
          parser->bufptr++;
          yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NULL);
          return DEC;
        }
        else {
          yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NULL);;
          return '-';
        }
      /* Multiply */
      case '*':
        parser->bufptr++;
        SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_MULTIPLY);
        yylvalp->opval = op;
        return MULTIPLY;
      
      /* Divide */
      case '/': {
        parser->bufptr++;
        SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DIVIDE);
        yylvalp->opval = op;
        return DIVIDE;
      }
      case '%': {
        parser->bufptr++;
        SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_REMAINDER);
        yylvalp->opval = op;
        return REMAINDER;
      }
      case '^': {
        parser->bufptr++;
        SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_BIT_XOR);
        yylvalp->opval = op;
        return BIT_XOR;
      }
      case '|':
        parser->bufptr++;
        /* Or */
        if (*parser->bufptr == '|') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_OR);
          yylvalp->opval = op;
          return OR;
        }
        /* Bit or */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_BIT_OR);
          yylvalp->opval = op;
          return BIT_OR;
        }

      case '&':
        parser->bufptr++;
        /* Or */
        if (*parser->bufptr == '&') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_AND);
          yylvalp->opval = op;
          return AND;
        }
        /* Bit and */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_BIT_AND);
          yylvalp->opval = op;
          return BIT_AND;
        }
      
      /* Comment */
      case '#':
        parser->bufptr++;
        while(1) {
          if (*parser->bufptr == '\r' || *parser->bufptr == '\n' || *parser->bufptr == '\0') {
            break;
          }
          parser->bufptr++;
        }
        
        continue;
      
      case '=':
        parser->bufptr++;
        
        /* == */
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_EQ);
          yylvalp->opval = op;
          return REL;
        }
        /* = */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_ASSIGN);
          yylvalp->opval = op;
          return ASSIGN;
        }
        
      case '<':
        parser->bufptr++;
        
        if (*parser->bufptr == '<') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_LEFT_SHIFT);
          yylvalp->opval = op;
          return SHIFT;
        }
        /* <= */
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_LE);
          yylvalp->opval = op;
          return REL;
        }
        /* < */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_LT);
          yylvalp->opval = op;
          return REL;
        }
      
      case '>':
        parser->bufptr++;
        
        if (*parser->bufptr == '>') {
          parser->bufptr++;
          if (*parser->bufptr == '>') {
            parser->bufptr++;
            SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED);
            yylvalp->opval = op;
            return SHIFT;
          }
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_RIGHT_SHIFT);
          yylvalp->opval = op;
          return SHIFT;
        }
        /* >= */
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_GE);
          yylvalp->opval = op;
          return REL;
        }
        /* < */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_GT);
          yylvalp->opval = op;
          return REL;
        }
      case '!':
        parser->bufptr++;
        
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NE);
          yylvalp->opval = op;
          return REL;
        }
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NOT);
          yylvalp->opval = op;
          return NOT;
        }
        
      case '~': {
        parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_COMPLEMENT);
          yylvalp->opval = op;
        return '~';
      }
      case '\'': {
        parser->bufptr++;
        
        char ch;
        if (*parser->bufptr == '\'') {
          ch = '\0';
          parser->bufptr++;
        }
        else {
          ch = *parser->bufptr;
          parser->bufptr++;
          if (*parser->bufptr != '\'') {
            fprintf(stderr, "syntax error: string don't finish\n");
            exit(1);
          }
          parser->bufptr++;
        }
        
        // Constant 
        SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_CONSTANT);
        SPerl_CONSTANT* constant = SPerl_CONSTANT_new(sperl);
        constant->code = SPerl_CONSTANT_C_CODE_INT;
        constant->uv.int_value = (int32_t) (uint8_t) ch;
        constant->resolved_type = SPerl_HASH_search(sperl, parser->resolved_type_symtable, "byte", strlen("byte"));
        
        op->uv.constant = constant;
        yylvalp->opval = op;
        
        return CONSTANT;
      }
      case '"': {
        parser->bufptr++;
        
        /* Save current position */
        const char* cur_token_ptr = parser->bufptr;
        
        char* str;
        if (*(parser->bufptr + 1) == '"') {
          str = SPerl_ALLOCATOR_PARSER_alloc_string(sperl, parser->allocator, 0);
          str[0] = '\0';
          parser->bufptr++;
          parser->bufptr++;
        }
        else {
          while(*parser->bufptr != '"' && *parser->bufptr != '\0') {
            parser->bufptr++;
          }
          if (*parser->bufptr == '\0') {
            fprintf(stderr, "syntax error: string don't finish\n");
            exit(1);
          }
          
          int32_t str_len = (parser->bufptr - cur_token_ptr);
          str = SPerl_ALLOCATOR_PARSER_alloc_string(sperl, parser->allocator, str_len);
          memcpy(str, cur_token_ptr, str_len);
          str[str_len] = '\0';
          
          parser->bufptr++;
        }
        
        SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_CONSTANT);
        SPerl_CONSTANT* constant = SPerl_CONSTANT_new(sperl);
        constant->code = SPerl_CONSTANT_C_CODE_STRING;
        constant->uv.string_value = str;
        constant->resolved_type = SPerl_HASH_search(sperl, parser->resolved_type_symtable, "byte[]", strlen("byte[]"));
        op->uv.constant = constant;
        yylvalp->opval = (SPerl_OP*)op;
        
        return CONSTANT;
      }
      default:
        /* Variable */
        if (c == '$') {
          /* Save current position */
          const char* cur_token_ptr = parser->bufptr;
          
          parser->bufptr++;
          
          /* Next is graph */
          while(isalnum(*parser->bufptr) || (*parser->bufptr) == '_' || (*parser->bufptr) == ':') {
            parser->bufptr++;
          }
          
          int32_t str_len = (parser->bufptr - cur_token_ptr);
          char* var_name = SPerl_ALLOCATOR_PARSER_alloc_string(sperl, parser->allocator, str_len);
          memcpy(var_name, cur_token_ptr, str_len);
          var_name[str_len] = '\0';

          // 
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_VAR);
          SPerl_VAR* var = SPerl_VAR_new(sperl);
          
          // Name OP
          SPerl_OP* op_name = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NAME);
          op_name->uv.name = var_name;
          
          var->op_name = op_name;
          
          op->uv.var = var;
          yylvalp->opval = op;
          
          return VAR;
        }
        /* Number literal */
        else if (isdigit(c)) {
          const char* cur_token_ptr = parser->bufptr;
          
          int32_t point_count = 0;
          
          parser->bufptr++;
          
          // Scan number
          while(isdigit(*parser->bufptr) || isalpha(*parser->bufptr) || *parser->bufptr == '.') {
            if (*parser->bufptr == '.') {
              point_count++;
            }
            parser->bufptr++;
          }
          
          // Number literal(first is space for sign)
          int32_t str_len = (parser->bufptr - cur_token_ptr);
          char* num_str = (char*) SPerl_ALLOCATOR_UTIL_safe_malloc_i32(str_len + 2, sizeof(char));
          memcpy(num_str, cur_token_ptr, str_len);
          num_str[str_len] = '\0';
          
          // Constant type
          int32_t constant_code;
          if (num_str[str_len - 1] == 'f')  {
            constant_code = SPerl_CONSTANT_C_CODE_FLOAT;
            num_str[str_len - 1] = '\0';
          }
          else if (num_str[str_len - 1] == 'd')  {
            constant_code = SPerl_CONSTANT_C_CODE_DOUBLE;
            num_str[str_len - 1] = '\0';
          }
          else {
            if (point_count) {
              constant_code = SPerl_CONSTANT_C_CODE_DOUBLE;
            }
            else {
              constant_code = SPerl_CONSTANT_C_CODE_LONG;
            }
          }
          
          // Constant
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_CONSTANT);
          SPerl_CONSTANT* constant = SPerl_CONSTANT_new(sperl);
          constant->code = constant_code;
          
          // float
          char *end;
          if (constant->code == SPerl_CONSTANT_C_CODE_FLOAT) {
            float num = strtof(num_str, &end);
            if (*end != '\0') {
              fprintf(stderr, "Invalid number literal %s at %s line %" PRId32 "\n", num_str, parser->cur_module_path, parser->cur_line);
              exit(1);
            }
            constant->uv.float_value = num;
            constant->resolved_type = SPerl_HASH_search(sperl, parser->resolved_type_symtable, "float", strlen("float"));
          }
          // double
          else if (constant->code == SPerl_CONSTANT_C_CODE_DOUBLE) {
            double num = strtod(num_str, &end);
            if (*end != '\0') {
              fprintf(stderr, "Invalid number literal %s at %s line %" PRId32 "\n", num_str, parser->cur_module_path, parser->cur_line);
              exit(1);
            }
            constant->uv.double_value = num;
            constant->resolved_type = SPerl_HASH_search(sperl, parser->resolved_type_symtable, "double", strlen("double"));
          }
          // long
          else if (constant->code == SPerl_CONSTANT_C_CODE_LONG) {
            int64_t num;
            errno = 0;
            if (num_str[0] == '0' && num_str[1] == 'x') {
              num = strtol(num_str, &end, 16);
            }
            else {
              num = strtol(num_str, &end, 10);
            }
            if (*end != '\0') {
              fprintf(stderr, "Invalid number literal %s at %s line %" PRId32 "\n", num_str, parser->cur_module_path, parser->cur_line);
              exit(1);
            }
            else if (num == INT64_MAX && errno == ERANGE) {
              fprintf(stderr, "Number literal out of range %s at %s line %" PRId32 "\n", num_str, parser->cur_module_path, parser->cur_line);
              exit(1);
            }
            constant->uv.long_value = num;
            constant->resolved_type = SPerl_HASH_search(sperl, parser->resolved_type_symtable, "long", strlen("long"));
          }
          
          op->uv.constant = constant;
          yylvalp->opval = op;
          
          return CONSTANT;
        }
        // Keyname or name
        else if (isalpha(c) || c == '_') {
          // Save current position
          const char* cur_token_ptr = parser->bufptr;
          
          parser->bufptr++;
          
          while(isalnum(*parser->bufptr)
            || *parser->bufptr == '_'
            || (*parser->bufptr == ':' && *(parser->bufptr + 1) == ':'))
          {
            if (*parser->bufptr == ':' && *(parser->bufptr + 1) == ':') {
              parser->bufptr += 2;
            }
            else {
              parser->bufptr++;
            }
          }
          
          
          int32_t str_len = (parser->bufptr - cur_token_ptr);
          char* keyword = SPerl_ALLOCATOR_PARSER_alloc_string(sperl, parser->allocator, str_len);
          
          memcpy(keyword, cur_token_ptr, str_len);
          keyword[str_len] = '\0';

          // Keyname
          if (memcmp(keyword, "my", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DECL_MY_VAR);
            return MY;
          }
          else if (memcmp(keyword, "has", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DECL_FIELD);
            return HAS;
          }
          else if (memcmp(keyword, "sub", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DECL_SUB);
            return SUB;
          }
          else if (memcmp(keyword, "package", str_len) == 0) {
            // File can contains only one package
            if (parser->current_package_count) {
              fprintf(stderr, "Can't write second package declaration in file at %s line %" PRId32 "\n", parser->cur_module_path, parser->cur_line);
              exit(1);
            }
            parser->current_package_count++;
            
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DECL_PACKAGE);
            return PACKAGE;
          }
          else if (memcmp(keyword, "switch", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_SWITCH);
            return SWITCH;
          }
          else if (memcmp(keyword, "case", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_CASE);
            return CASE;
          }
          else if (memcmp(keyword, "default", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DEFAULT);
            return DEFAULT;
          }
          else if (memcmp(keyword, "if", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_IF);
            return IF;
          }
          else if (memcmp(keyword, "elsif", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_ELSIF);
            return ELSIF;
          }
          else if (memcmp(keyword, "else", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_ELSE);
            return ELSE;
          }
          else if (memcmp(keyword, "return", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_RETURN);
            return RETURN;
          }
          else if (memcmp(keyword, "for", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_FOR);
            return FOR;
          }
          else if (memcmp(keyword, "last", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_LAST);
            return LAST;
          }
          else if (memcmp(keyword, "next", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NEXT);
            return NEXT;
          }
          else if (memcmp(keyword, "use", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_USE);
            return USE;
          }
          else if (memcmp(keyword, "undef", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_UNDEF);
            return UNDEF;
          }
          else if (memcmp(keyword, "void", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_VOID);
            return VOID;
          }
          else if (memcmp(keyword, "while", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_WHILE);
            return WHILE;
          }
          else if (memcmp(keyword, "malloc", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_MALLOC);
            return MALLOC;
          }
          else if (memcmp(keyword, "enum", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DECL_ENUM);
            return ENUM;
          }
          else if (memcmp(keyword, "die", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DIE);
            return DIE;
          }
          else if (memcmp(keyword, "try", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_TRY);
            return TRY;
          }
          else if (memcmp(keyword, "catch", str_len) == 0) {
            yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_CATCH);
            return CATCH;
          }
          else if (memcmp(keyword, "native", str_len) == 0) {
            SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DESCRIPTOR);
            op->code = SPerl_DESCRIPTOR_C_CODE_NATIVE;
            yylvalp->opval = op;
            
            return DESCRIPTOR;
          }
          else if (memcmp(keyword, "const", str_len) == 0) {
            SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_DESCRIPTOR);
            op->code = SPerl_DESCRIPTOR_C_CODE_CONST;
            yylvalp->opval = op;
            
            return DESCRIPTOR;
          }
          else if (memcmp(keyword, "len", str_len) == 0) {
            parser->bufptr++;
            SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_ARRAY_LENGTH);
            yylvalp->opval = op;
            
            return ARRAY_LENGTH;
          }
          
          SPerl_OP* op = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NAME);
          op->uv.name = keyword;
          yylvalp->opval = op;
          
          return NAME;
        }
        
        /* Return character */
        parser->bufptr++;
        yylvalp->opval = SPerl_TOKE_newOP(sperl, SPerl_OP_C_CODE_NULL);
        
        return (int) (uint8_t) c;
    }
  }
}

