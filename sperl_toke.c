#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sperl_op.h"
#include "sperl_parser.h"
#include "sperly.tab.h"
#include "sperl_const_info.h"
#include "sperl_var_info.h"
#include "sperl_array.h"
#include "sperl_hash.h"

static SPerl_OP* _create_op(SPerl_PARSER* parser, enum SPerl_OP_CODE code, void* ptr_value) {
  SPerl_OP* op = SPerl_OP_newOP(parser, code, NULL, NULL);
  op->uv.ptr_value = ptr_value;
  op->file = parser->cur_file;
  op->line = parser->cur_line;
  
  return op;
}

// Get token
int SPerl_yylex(YYSTYPE* SPerl_yylvalp, SPerl_PARSER* parser) {
  
  if (!parser->cur_src) {
    
    // Open source file
    FILE* fh = fopen(parser->cur_file, "r");
    if (fh == NULL) {
      printf("Can't open file %s\n", parser->cur_file);
      return -1;
    }
    
    // File size
    fseek(fh, 0, SEEK_END);
    SPerl_int file_size = ftell(fh);
    fseek(fh, 0, SEEK_SET);
    
    // Read file
    SPerl_char* src = SPerl_PARSER_new_string(parser, file_size);
    if (fread(src, 1, file_size, fh) == -1) {
      printf("Can't read file %s\n", parser->cur_file);
      return -1;
    }
    fclose(fh);
    src[file_size] = '\0';
    
    parser->cur_src = src;
    parser->bufptr = src;
  }
  
  SPerl_char* bufptr = parser->bufptr;
  enum SPerl_OP_EXPECT expect = parser->expect;
  parser->expect = SPerl_OP_EXPECT_NORMAL;
  
  while(1) {
    // Get current character
    SPerl_char c = *bufptr;
    
    // line end
    switch(c) {
      case '\0':
        parser->cur_file = NULL;
        parser->cur_src = NULL;
        
        // If there are more module, load it
        SPerl_ARRAY* class_stack = parser->class_stack;
        
        while (1) {
          SPerl_char* class_name = (SPerl_char*)SPerl_ARRAY_pop(class_stack);
          
          if (class_name) {
            SPerl_CLASS_INFO* found_class_info = SPerl_HASH_search(parser->class_info_symtable, class_name, strlen(class_name));
            if (found_class_info) {
              continue;
            }
            else {
              SPerl_char* cur_file = SPerl_PARSER_new_string(parser, strlen(class_name) + 8);
              sprintf(cur_file, "%s.pm.spvm", class_name);
              parser->cur_file = cur_file;
              
              // Open source file
              FILE* fh = fopen(parser->cur_file, "r");
              if (fh == NULL) {
                printf("Can't open file %s\n", parser->cur_file);
                return -1;
              }
              
              // File size
              fseek(fh, 0, SEEK_END);
              SPerl_int file_size = ftell(fh);
              fseek(fh, 0, SEEK_SET);
              
              // Read file
              SPerl_char* src = SPerl_PARSER_new_string(parser, file_size);
              if (fread(src, 1, file_size, fh) == -1) {
                printf("Can't read file %s\n", parser->cur_file);
                return -1;
              }
              fclose(fh);
              
              src[file_size] = '\0';
              
              parser->cur_src = src;
              bufptr = src;
              parser->bufptr = bufptr;
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

        bufptr++;
        parser->bufptr = bufptr;
        continue;

      case '\n':
        bufptr++;
        parser->bufptr = bufptr;
        parser->cur_line++;
        continue;
      
      /* Addition */
      case '+':
        bufptr++;
        
        if (*bufptr == '+') {
          bufptr++;
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_INC;
          return INCOP;
        }
        else if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_ADD;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          
          return '+';
        }
      
      /* Subtract */
      case '-':
        bufptr++;
        
        if (*bufptr == '>') {
          bufptr++;
          parser->bufptr = bufptr;
          parser->expect = SPerl_OP_EXPECT_WORD;
          return ARROW;
        }
        else if (*bufptr == '-') {
          bufptr++;
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_DEC;
          return DECOP;
        }
        else if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_SUBTRACT;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;

          return '-';
        }
      /* Multiply */
      case '*':
        bufptr++;
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_MULTIPLY;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_MULTIPLY;
          return MULOP;
        }
      
      /* Divide */
      case '/':
        bufptr++;
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_DIVIDE;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_DIVIDE;
          return MULOP;
        }

      case '%':
        bufptr++;
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_MODULO;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_MODULO;
          return MULOP;
        }

      case '^':
        bufptr++;
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_BIT_XOR;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_BIT_XOR;
          return MULOP;
        }
            
      case '|':
        bufptr++;
        /* Or */
        if (*bufptr == '|') {
          bufptr++;
          if (*bufptr == '=') {
            bufptr++;
            parser->bufptr = bufptr;
            SPerl_yylvalp->ival = SPerl_OP_OR;
            return ASSIGNOP;
          }
          else {
            parser->bufptr = bufptr;
            SPerl_yylvalp->ival = SPerl_OP_OR;
            return OROP;
          }
        }
        /* Bit or */
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_BIT_OR;
          return BITOROP;
        }

      case '&':
        bufptr++;
        /* Or */
        if (*bufptr == '&') {
          bufptr++;
          if (*bufptr == '=') {
            bufptr++;
            parser->bufptr = bufptr;
            SPerl_yylvalp->ival = SPerl_OP_AND;
            return ASSIGNOP;
          }
          else {
            parser->bufptr = bufptr;
            SPerl_yylvalp->ival = SPerl_OP_AND;
            return ANDOP;
          }
        }
        /* Bit and */
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_BIT_AND;
          return BITANDOP;
        }
      
      /* Comment */
      case '#':
        bufptr++;
        while(1) {
          if (*bufptr == '\r' || *bufptr == '\n' || *bufptr == '\0') {
            break;
          }
          bufptr++;
        }
        parser->bufptr = bufptr;
        continue;
      
      case '=':
        bufptr++;
        
        /* == */
        if (*bufptr == '=') {
          bufptr++;
          
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_EQ;
          return RELOP;
        }
        /* = */
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = 0;
          return ASSIGNOP;
        }
        
      case '<':
        bufptr++;
        
        if (*bufptr == '<') {
          bufptr++;
          
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_LEFT_SHIFT;
          return SHIFTOP;
        }
        /* <= */
        else if (*bufptr == '=') {
          bufptr++;
          
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_LE;
          return RELOP;
        }
        /* < */
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_LT;
          return RELOP;
        }
      
      case '>':
        bufptr++;
        
        if (*bufptr == '>') {
          bufptr++;
          
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_RIGHT_SHIFT;
          return SHIFTOP;
        }
        /* >= */
        else if (*bufptr == '=') {
          bufptr++;
          
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_GE;
          return RELOP;
        }
        /* < */
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_GT;
          return RELOP;
        }
      case '!':
        bufptr++;
        
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_NE;
          return RELOP;
        }
        else {
          parser->bufptr = bufptr;
          SPerl_yylvalp->ival = SPerl_OP_NOT;
          return NOTOP;
        }
        
      case '~':
        bufptr++;
        
        parser->bufptr = bufptr;
        SPerl_yylvalp->ival = SPerl_OP_COMPLEMENT;
        return '~';
      
      case '\'': {
        bufptr++;
        
        /* Save current position */
        SPerl_char* cur_token_ptr = bufptr;
        
        SPerl_char ch;
        if (*bufptr == '\'') {
          ch = '\0';
          bufptr++;
        }
        else {
          ch = *bufptr;
          bufptr++;
          if (*bufptr != '\'') {
            fprintf(stderr, "syntax error: string don't finish\n");
            exit(1);
          }
          bufptr++;
        }
        
        // Constant
        SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
        const_info->type = SPerl_CONST_INFO_CHAR;
        const_info->uv.int_value = ch;
        SPerl_yylvalp->opval = _create_op(parser, SPerl_OP_CONST, const_info);
        
        parser->bufptr = bufptr;
        return CONST;
      }
      case '"': {
        bufptr++;
        
        /* Save current position */
        SPerl_char* cur_token_ptr = bufptr;
        
        SPerl_char* str;
        if (*(bufptr + 1) == '"') {
          str = SPerl_PARSER_new_string(parser, 0);
          str[0] = '\0';
          bufptr++;
          bufptr++;
        }
        else {
          while(*bufptr != '"' && *bufptr != '\0') {
            bufptr++;
          }
          if (*bufptr == '\0') {
            fprintf(stderr, "syntax error: string don't finish\n");
            exit(1);
          }
          
          size_t str_len = bufptr - cur_token_ptr;
          str = SPerl_PARSER_new_string(parser, str_len);
          memcpy(str, cur_token_ptr, str_len);
          str[str_len] = '\0';

          bufptr++;
        }
        
        SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_CONST, NULL, NULL);

        SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
        const_info->type = SPerl_CONST_INFO_STRING;
        const_info->uv.string_value = str;
        op->uv.ptr_value = const_info;
        
        parser->bufptr = bufptr;
        SPerl_yylvalp->opval = (SPerl_OP*)op;
        return CONST;
      }
      default:
        /* Variable */
        if (c == '$') {
          /* Save current position */
          SPerl_char* cur_token_ptr = bufptr;
          
          bufptr++;

          /* Next is graph */
          while(isalnum(*bufptr) || (*bufptr) == '_' || (*bufptr) == ':') {
            bufptr++;
          }

          size_t str_len = bufptr - cur_token_ptr;
          SPerl_char* var = SPerl_PARSER_new_string(parser, str_len);
          memcpy(var, cur_token_ptr, str_len);
          var[str_len] = '\0';
          
          SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_VAR, NULL, NULL);
          SPerl_VAR_INFO* var_info = SPerl_VAR_INFO_new(parser);
          var_info->name = var;
          op->uv.ptr_value = var_info;
          
          parser->bufptr = bufptr;
          SPerl_yylvalp->opval = (SPerl_OP*)op;
          return VAR;
        }
        /* Number literal */
        else if (isdigit(c)) {
          SPerl_char* cur_token_ptr = bufptr;
          
          SPerl_int point_count = 0;
          
          bufptr++;
          
          // Scan number
          while(isdigit(*bufptr) || *bufptr == '.') {
            if (*bufptr == '.') {
              point_count++;
            }
            bufptr++;
          }
          
          if (point_count >= 2) {
            fprintf(stderr, "Invalid number literal\n");
            exit(1);
          }
          
          // Number literal
          size_t str_len = bufptr - cur_token_ptr;
          SPerl_char* num_str = malloc(str_len + 1);
          memcpy(num_str, cur_token_ptr, str_len);
          num_str[str_len] = '\0';
          
          // Convert to double
          if (point_count) {
            char* ends;
            double num = strtod(num_str, &ends);
            SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_CONST, NULL, NULL);

            SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
            const_info->type = SPerl_CONST_INFO_DOUBLE;
            const_info->uv.double_value = num;
            op->uv.ptr_value = const_info;
            
            free(num_str);
            
            parser->bufptr = bufptr;
            SPerl_yylvalp->opval = (SPerl_OP*)op;
            
            return CONST;
          }
          // Convert to integer
          else {
            SPerl_int num = atoi(num_str);
            SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_CONST, NULL, NULL);

            SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
            const_info->type = SPerl_CONST_INFO_INT;
            const_info->uv.int_value = num;
            op->uv.ptr_value = const_info;

            free(num_str);
            
            parser->bufptr = bufptr;
            SPerl_yylvalp->opval = (SPerl_OP*)op;
            
            return CONST;
          }
        }
        /* Keyword or word */
        else if (isalpha(c) || c == '_') {
          /* Save current position */
          SPerl_char* cur_token_ptr = bufptr;
          
          bufptr++;
          
          while(isalnum(*bufptr) || (*bufptr) == '_' || (*bufptr) == ':') {
            bufptr++;
          }
          
          size_t str_len = bufptr - cur_token_ptr;
          SPerl_char* keyword = SPerl_PARSER_new_string(parser, str_len);
          memcpy(keyword, cur_token_ptr, str_len);
          keyword[str_len] = '\0';
          
          if (expect != SPerl_OP_EXPECT_WORD) {
            if (memcmp(keyword, "my", str_len) == 0) {
              parser->bufptr = bufptr;
              return MY;
            }
            else if (memcmp(keyword, "has", str_len) == 0) {
              parser->expect = SPerl_OP_EXPECT_WORD;
              parser->bufptr = bufptr;
              return HAS;
            }
            else if (memcmp(keyword, "sub", str_len) == 0) {
              parser->bufptr = bufptr;
              parser->expect = SPerl_OP_EXPECT_WORD;
              return SUB;
            }
            else if (memcmp(keyword, "package", str_len) == 0) {
              parser->bufptr = bufptr;
              return PACKAGE;
            }
            else if (memcmp(keyword, "if", str_len) == 0) {
              parser->bufptr = bufptr;
              return IF;
            }
            else if (memcmp(keyword, "elsif", str_len) == 0) {
              parser->bufptr = bufptr;
              return ELSIF;
            }
            else if (memcmp(keyword, "else", str_len) == 0) {
              parser->bufptr = bufptr;
              return ELSE;
            }
            else if (memcmp(keyword, "return", str_len) == 0) {
              parser->bufptr = bufptr;
              return RETURN;
            }
            else if (memcmp(keyword, "for", str_len) == 0) {
              parser->bufptr = bufptr;
              return FOR;
            }
            else if (memcmp(keyword, "last", str_len) == 0) {
              parser->bufptr = bufptr;
              return LAST;
            }
            else if (memcmp(keyword, "next", str_len) == 0) {
              parser->bufptr = bufptr;
              return NEXT;
            }
            else if (memcmp(keyword, "use", str_len) == 0) {
              parser->bufptr = bufptr;
              return USE;
            }
            else if (memcmp(keyword, "while", str_len) == 0) {
              parser->bufptr = bufptr;
              return WHILE;
            }
            else if (memcmp(keyword, "true", str_len) == 0) {
              SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_CONST, NULL, NULL);

              SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
              const_info->type = SPerl_CONST_INFO_BOOLEAN;
              const_info->uv.int_value = 1;
              op->uv.ptr_value = const_info;

              parser->bufptr = bufptr;
              SPerl_yylvalp->opval = (SPerl_OP*)op;

              return CONST;
            }
            else if (memcmp(keyword, "false", str_len) == 0) {
              SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_CONST, NULL, NULL);

              SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
              const_info->type = SPerl_CONST_INFO_BOOLEAN;
              const_info->uv.int_value = 0;
              op->uv.ptr_value = const_info;

              parser->bufptr = bufptr;
              SPerl_yylvalp->opval = (SPerl_OP*)op;

              return CONST;
            }
          }
          
          SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_WORD, NULL, NULL);

          op->uv.string_value = keyword;
          
          parser->bufptr = bufptr;
          SPerl_yylvalp->opval = (SPerl_OP*)op;
          return WORD;
        }
        
        /* Return character */
        bufptr++;
        parser->bufptr = bufptr;
        return c;
    }
  }
}

/* Function for error */
void SPerl_yyerror(SPerl_PARSER* parser, const SPerl_char* s)
{
  fprintf(stderr, "error: %s\n", s);
}

