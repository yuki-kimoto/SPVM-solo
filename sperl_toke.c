#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sperl_toke.h"
#include "sperl_yacc.h"
#include "sperl_yacc.tab.h"
#include "sperl_op.h"
#include "sperl_parser.h"
#include "sperl_allocator.h"
#include "sperl_constant.h"
#include "sperl_var.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_word.h"
#include "sperl_descripter.h"
#include "sperl_use.h"
#include "sperl_type.h"

SPerl_OP* SPerl_TOKE_newOP(SPerl_PARSER* parser, uint8_t type) {
  SPerl_OP* op = SPerl_OP_newOP(parser, type);
  op->file = parser->cur_module_path;
  op->line = parser->cur_line;
  
  return op;
}

// Get token
int SPerl_yylex(SPerl_YYSTYPE* yylvalp, SPerl_PARSER* parser) {

  // Get expected and retrun back to normal
  uint8_t expect = parser->expect;
  parser->expect = SPerl_TOKE_C_EXPECT_NORMAL;
  
  // Save buf pointer
  parser->befbufptr = parser->bufptr;
  
  while(1) {
    // Get current character
    uint8_t c = *parser->bufptr;
    
    // line end
    switch(c) {
      case '\0':
        parser->cur_module_path = NULL;
        parser->cur_src = NULL;
        
        // If there are more module, load it
        SPerl_ARRAY* op_use_stack = parser->op_use_stack;
        
        while (1) {
          SPerl_OP* op_use = SPerl_ARRAY_pop(op_use_stack);
          if (op_use) {
            SPerl_USE* use = op_use->uv.use;
            uint8_t* package_name = use->op_package_name->uv.word->value;
            
            SPerl_PACKAGE* found_package = SPerl_HASH_search(parser->package_symtable, package_name, strlen(package_name));
            if (found_package) {
              continue;
            }
            else {
              
              // Change :: to / and add ".spvm"
              uint8_t* module_path_base = SPerl_ALLOCATOR_new_string(parser, strlen(package_name)  + 6);
              uint8_t* bufptr_orig = package_name;
              uint8_t* bufptr_to = module_path_base;
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
              
              // module is template
              uint8_t* underline_ptr;
              if (underline_ptr = strchr(module_path_base, '_')) {
                *(underline_ptr - 2) = '\0';
              }
              
              // Search module file
              uint8_t* cur_module_path;
              FILE* fh;
              for (int32_t i = 0; i < parser->include_pathes->length; i++) {
                uint8_t* include_path = SPerl_ARRAY_fetch(parser->include_pathes, i);
                
                // File name
                cur_module_path = SPerl_ALLOCATOR_new_string(parser, strlen(include_path) + 1 + strlen(module_path_base));
                sprintf(cur_module_path, "%s/%s", include_path, module_path_base);
                
                // Open source file
                fh = fopen(cur_module_path, "r");
                if (fh) {
                  parser->cur_module_path = cur_module_path;
                  break;
                }
              }
              if (!fh) {
                if (op_use) {
                  fprintf(stderr, "Can't find package \"%s\" at %s line %d\n", use->op_package_name->uv.word->value, op_use->file, op_use->line);
                }
                else {
                  fprintf(stderr, "Can't find file %s\n", cur_module_path);
                }
                exit(1);
              }
              
              parser->cur_module_path = cur_module_path;
              
              
              // Read file content
              fseek(fh, 0, SEEK_END);
              int32_t file_size = ftell(fh);
              fseek(fh, 0, SEEK_SET);
              uint8_t* src = SPerl_ALLOCATOR_new_string(parser, file_size);
              if (fread(src, 1, file_size, fh) == -1) {
                if (op_use) {
                  fprintf(stderr, "Can't read file %s at %s line %d\n", cur_module_path, op_use->file, op_use->line);
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
          yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NULL);
          return INC;
        }
        else {
          yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NULL);
          return '+';
        }
      
      /* Subtract */
      case '-':
        parser->bufptr++;
        
        if (*parser->bufptr == '>') {
          parser->bufptr++;
          parser->expect = SPerl_TOKE_C_EXPECT_WORD;
          yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NULL);
          return ARROW;
        }
        else if (*parser->bufptr == '-') {
          parser->bufptr++;
          yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NULL);
          return DEC;
        }
        else {
          yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NULL);;
          return '-';
        }
      /* Multiply */
      case '*':
        parser->bufptr++;
        SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_MULTIPLY);
        yylvalp->opval = op;
        return MULTIPLY;
      
      /* Divide */
      case '/': {
        parser->bufptr++;
        SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_DIVIDE);
        yylvalp->opval = op;
        return DIVIDE;
      }
      case '%': {
        parser->bufptr++;
        SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_REMAINDER);
        yylvalp->opval = op;
        return REMAINDER;
      }
      case '^': {
        parser->bufptr++;
        SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_BIT_XOR);
        yylvalp->opval = op;
        return BIT_XOR;
      }
      case '@': {
        parser->bufptr++;
        SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_ARRAY_LENGTH);
        yylvalp->opval = op;
        return '@';
      }
      case '|':
        parser->bufptr++;
        /* Or */
        if (*parser->bufptr == '|') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_OR);
          yylvalp->opval = op;
          return OR;
        }
        /* Bit or */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_BIT_OR);
          yylvalp->opval = op;
          return BIT_OR;
        }

      case '&':
        parser->bufptr++;
        /* Or */
        if (*parser->bufptr == '&') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_AND);
          yylvalp->opval = op;
          return AND;
        }
        /* Bit and */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_BIT_AND);
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
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_EQ);
          yylvalp->opval = op;
          return REL;
        }
        /* = */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_ASSIGN);
          yylvalp->opval = op;
          return ASSIGN;
        }
        
      case '<':
        parser->bufptr++;
        
        if (*parser->bufptr == '<') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_LEFT_SHIFT);
          yylvalp->opval = op;
          return SHIFT;
        }
        /* <= */
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_LE);
          yylvalp->opval = op;
          return REL;
        }
        /* < */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_LT);
          yylvalp->opval = op;
          return REL;
        }
      
      case '>':
        parser->bufptr++;
        
        if (*parser->bufptr == '>') {
          parser->bufptr++;
          if (*parser->bufptr == '>') {
            parser->bufptr++;
            SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED);
            yylvalp->opval = op;
            return SHIFT;
          }
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_RIGHT_SHIFT);
          yylvalp->opval = op;
          return SHIFT;
        }
        /* >= */
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_GE);
          yylvalp->opval = op;
          return REL;
        }
        /* < */
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_GT);
          yylvalp->opval = op;
          return REL;
        }
      case '!':
        parser->bufptr++;
        
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NE);
          yylvalp->opval = op;
          return REL;
        }
        else {
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NOT);
          yylvalp->opval = op;
          return NOT;
        }
        
      case '~': {
        parser->bufptr++;
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_COMPLEMENT);
          yylvalp->opval = op;
        return '~';
      }
      case '\'': {
        parser->bufptr++;
        
        /* Save current position */
        uint8_t* cur_token_ptr = parser->bufptr;
        
        uint8_t ch;
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
        SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_CONSTANT);
        SPerl_CONSTANT* constant = SPerl_CONSTANT_new(parser);
        constant->code = SPerl_CONSTANT_C_CODE_BYTE;
        constant->uv.int_value = ch;
        constant->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "byte", strlen("byte"));
        
        op->uv.constant = constant;
        yylvalp->opval = op;
        
        return CONSTANT;
      }
      case '"': {
        parser->bufptr++;
        
        /* Save current position */
        uint8_t* cur_token_ptr = parser->bufptr;
        
        uint8_t* str;
        if (*(parser->bufptr + 1) == '"') {
          str = SPerl_ALLOCATOR_new_string(parser, 0);
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
          
          size_t str_len = parser->bufptr - cur_token_ptr;
          str = SPerl_ALLOCATOR_new_string(parser, str_len);
          memcpy(str, cur_token_ptr, str_len);
          str[str_len] = '\0';

          parser->bufptr++;
        }
        
        SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_CONSTANT);
        SPerl_CONSTANT* constant = SPerl_CONSTANT_new(parser);
        constant->code = SPerl_CONSTANT_C_CODE_STRING;
        constant->uv.string_value = str;
        constant->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "byte[]", strlen("byte[]"));
        op->uv.constant = constant;
        yylvalp->opval = (SPerl_OP*)op;

        return CONSTANT;
      }
      default:
        /* Variable */
        if (c == '$') {
          /* Save current position */
          uint8_t* cur_token_ptr = parser->bufptr;
          
          parser->bufptr++;
          
          /* Next is graph */
          while(isalnum(*parser->bufptr) || (*parser->bufptr) == '_' || (*parser->bufptr) == ':') {
            parser->bufptr++;
          }
          
          size_t str_len = parser->bufptr - cur_token_ptr;
          uint8_t* var_name = SPerl_ALLOCATOR_new_string(parser, str_len);
          memcpy(var_name, cur_token_ptr, str_len);
          var_name[str_len] = '\0';
          
          SPerl_WORD* var_name_word = SPerl_WORD_new(parser);
          var_name_word->value = var_name;
          
          // 
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_VAR);
          SPerl_VAR* var = SPerl_VAR_new(parser);
          
          // Name OP
          SPerl_OP* op_name = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_WORD);
          op_name->uv.word = var_name_word;
          
          var->op_name = op_name;
          
          op->uv.var = var;
          yylvalp->opval = op;
          
          return VAR;
        }
        /* Number literal */
        else if (isdigit(c)) {
          uint8_t* cur_token_ptr = parser->bufptr;
          
          int32_t point_count = 0;
          
          parser->bufptr++;
          
          // Scan number
          while(isdigit(*parser->bufptr) || *parser->bufptr == '.') {
            if (*parser->bufptr == '.') {
              point_count++;
            }
            parser->bufptr++;
          }
          
          if (point_count >= 2) {
            fprintf(stderr, "Invalid number literal\n");
            exit(1);
          }
          
          // Number literal
          size_t str_len = parser->bufptr - cur_token_ptr;
          uint8_t* num_str = malloc(str_len + 1);
          memcpy(num_str, cur_token_ptr, str_len);
          num_str[str_len] = '\0';
          
          // Convert to double
          if (point_count) {
            char* ends;
            double num = strtod(num_str, &ends);
            free(num_str);

            SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_CONSTANT);
            SPerl_CONSTANT* constant = SPerl_CONSTANT_new(parser);
            constant->code = SPerl_CONSTANT_C_CODE_DOUBLE;
            constant->uv.double_value = num;
            constant->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "double", strlen("double"));
            op->uv.constant = constant;
            yylvalp->opval = (SPerl_OP*)op;
            
            return CONSTANT;
          }
          // Convert to integer
          else {
            int32_t num = atoi(num_str);
            free(num_str);
            
            SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_CONSTANT);
            SPerl_CONSTANT* constant = SPerl_CONSTANT_new(parser);
            constant->code = SPerl_CONSTANT_C_CODE_INT;
            constant->uv.int_value = num;
            constant->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "int", strlen("int"));
            op->uv.constant = constant;
            yylvalp->opval = (SPerl_OP*)op;
            
            return CONSTANT;
          }
        }
        // Keyword or word
        else if (isalpha(c) || c == '_') {
          // Save current position
          uint8_t* cur_token_ptr = parser->bufptr;
          
          parser->bufptr++;
          
          while(isalnum(*parser->bufptr)
            || *parser->bufptr == '_'
            || *parser->bufptr == ':' && *(parser->bufptr + 1) == ':')
          {
            if (*parser->bufptr == ':' && *(parser->bufptr + 1) == ':') {
              parser->bufptr += 2;
            }
            else {
              parser->bufptr++;
            }
          }
          
          size_t str_len = parser->bufptr - cur_token_ptr;
          uint8_t* keyword = SPerl_ALLOCATOR_new_string(parser, str_len);
          memcpy(keyword, cur_token_ptr, str_len);
          keyword[str_len] = '\0';
          
          // Keyword
          if (expect != SPerl_TOKE_C_EXPECT_WORD) {
            if (memcmp(keyword, "my", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_DECL_MY_VAR);
              return MY;
            }
            else if (memcmp(keyword, "has", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_DECL_FIELD);
              parser->expect = SPerl_TOKE_C_EXPECT_WORD;
              return HAS;
            }
            else if (memcmp(keyword, "sub", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_DECL_SUB);
              parser->expect = SPerl_TOKE_C_EXPECT_WORD;
              return SUB;
            }
            else if (memcmp(keyword, "package", str_len) == 0) {
              // File can contains only one package
              if (parser->current_package_count) {
                fprintf(stderr, "Can't write second package declaration in file at %s line %d\n", parser->cur_module_path, parser->cur_line);
                exit(1);
              }
              parser->current_package_count++;
              
              // Next is package name
              parser->expect = SPerl_TOKE_C_EXPECT_PACKAGENAME;
              
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_DECL_PACKAGE);
              return PACKAGE;
            }
            else if (memcmp(keyword, "switch", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_SWITCH);
              return SWITCH;
            }
            else if (memcmp(keyword, "case", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_CASE);
              return CASE;
            }
            else if (memcmp(keyword, "default", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_DEFAULT);
              return DEFAULT;
            }
            else if (memcmp(keyword, "if", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_IF);
              return IF;
            }
            else if (memcmp(keyword, "elsif", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_ELSIF);
              return ELSIF;
            }
            else if (memcmp(keyword, "else", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_ELSE);
              return ELSE;
            }
            else if (memcmp(keyword, "return", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_RETURN);
              return RETURN;
            }
            else if (memcmp(keyword, "for", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_FOR);
              return FOR;
            }
            else if (memcmp(keyword, "last", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_LAST);
              return LAST;
            }
            else if (memcmp(keyword, "next", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NEXT);
              return NEXT;
            }
            else if (memcmp(keyword, "use", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_USE);
              return USE;
            }
            else if (memcmp(keyword, "undef", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_UNDEF);
              return UNDEF;
            }
            else if (memcmp(keyword, "while", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_WHILE);
              return WHILE;
            }
            else if (memcmp(keyword, "new", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NEW);
              return NEW;
            }
            else if (memcmp(keyword, "enum", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_DECL_ENUM);
              return ENUM;
            }
            else if (memcmp(keyword, "die", str_len) == 0) {
              yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_DIE);
              return DIE;
            }
            else if (memcmp(keyword, "true", str_len) == 0) {
              SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_CONSTANT);
              SPerl_CONSTANT* constant = SPerl_CONSTANT_new(parser);
              constant->code = SPerl_CONSTANT_C_CODE_BOOLEAN;
              constant->uv.int_value = 1;
              constant->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "boolean", strlen("boolean"));
              op->uv.constant = constant;
              yylvalp->opval = op;

              return CONSTANT;
            }
            else if (memcmp(keyword, "false", str_len) == 0) {
              SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_CONSTANT);
              SPerl_CONSTANT* constant = SPerl_CONSTANT_new(parser);
              constant->code = SPerl_CONSTANT_C_CODE_BOOLEAN;
              constant->uv.int_value = 0;
              constant->resolved_type = SPerl_HASH_search(parser->resolved_type_symtable, "boolean", strlen("boolean"));
              op->uv.constant = constant;
              yylvalp->opval = op;

              return CONSTANT;
            }
          }
          
          SPerl_OP* op = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_WORD);
          SPerl_WORD* word = SPerl_WORD_new(parser);
          word->value = keyword;
          op->uv.word = word;
          yylvalp->opval = op;

          if (expect == SPerl_TOKE_C_EXPECT_PACKAGENAME) {
            // Template class
            if (strchr(keyword, '_')) {
              
            }
          }
          
          return WORD;
        }
        
        /* Return character */
        parser->bufptr++;
        yylvalp->opval = SPerl_TOKE_newOP(parser, SPerl_OP_C_CODE_NULL);
        
        return c;
    }
  }
}

