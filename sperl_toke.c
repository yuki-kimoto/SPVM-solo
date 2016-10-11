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

static SPerl_OP* _newOP(SPerl_PARSER* parser, enum SPerl_OP_CODE code) {
  SPerl_OP* op = SPerl_OP_newOP(parser, code, NULL, NULL);
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
    parser->befbufptr = src;
  }
  
  // Set before buffer pointer
  parser->befbufptr = parser->bufptr;
  
  // Get expected and retrun back to normal
  enum SPerl_OP_EXPECT expect = parser->expect;
  parser->expect = SPerl_OP_EXPECT_NORMAL;
  
  while(1) {
    // Get current character
    SPerl_char c = *parser->bufptr;
    
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
              parser->bufptr = src;
              parser->befbufptr = src;
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
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
          return INCOP;
        }
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_ASSIGN);
          op->uv.iv = SPerl_OP_ADD;
          SPerl_yylvalp->opval = op;
          return ASSIGNOP;
        }
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
          return '+';
        }
      
      /* Subtract */
      case '-':
        parser->bufptr++;
        
        if (*parser->bufptr == '>') {
          parser->bufptr++;
          parser->expect = SPerl_OP_EXPECT_WORD;
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
          return ARROW;
        }
        else if (*parser->bufptr == '-') {
          parser->bufptr++;
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
          return DECOP;
        }
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_ASSIGN);
          op->uv.iv = SPerl_OP_SUBTRACT;
          SPerl_yylvalp->opval = op;
          return ASSIGNOP;
        }
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);;
          return '-';
        }
      /* Multiply */
      case '*':
        parser->bufptr++;
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_ASSIGN);
          op->uv.iv = SPerl_OP_MULTIPLY;
          SPerl_yylvalp->opval = op;
          return ASSIGNOP;
        }
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_MULTIPLY);
          return MULOP;
        }
      
      /* Divide */
      case '/':
        parser->bufptr++;
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_ASSIGN);
          op->uv.iv = SPerl_OP_DIVIDE;
          SPerl_yylvalp->opval = op;
          return ASSIGNOP;
        }
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_DIVIDE);
          return MULOP;
        }

      case '%':
        parser->bufptr++;
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_ASSIGN);
          op->uv.iv = SPerl_OP_MODULO;
          SPerl_yylvalp->opval = op;
          return ASSIGNOP;
        }
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_MODULO);
          return MULOP;
        }

      case '^':
        parser->bufptr++;
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_ASSIGN);
          op->uv.iv = SPerl_OP_BIT_XOR;
          SPerl_yylvalp->opval = op;
          return ASSIGNOP;
        }
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_BIT_XOR);
          return MULOP;
        }
            
      case '|':
        parser->bufptr++;
        /* Or */
        if (*parser->bufptr == '|') {
          parser->bufptr++;
          if (*parser->bufptr == '=') {
            parser->bufptr++;
            SPerl_OP* op = _newOP(parser, SPerl_OP_ASSIGN);
            op->uv.iv = SPerl_OP_OR;
            SPerl_yylvalp->opval = op;
            return ASSIGNOP;
          }
          else {
            SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_OR);
            return OROP;
          }
        }
        /* Bit or */
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_BIT_OR);
          return BITOROP;
        }

      case '&':
        parser->bufptr++;
        /* Or */
        if (*parser->bufptr == '&') {
          parser->bufptr++;
          if (*parser->bufptr == '=') {
            parser->bufptr++;
            SPerl_OP* op = _newOP(parser, SPerl_OP_ASSIGN);
            op->uv.iv = SPerl_OP_AND;
            SPerl_yylvalp->opval = op;
            return ASSIGNOP;
          }
          else {
            SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_AND);
            return ANDOP;
          }
        }
        /* Bit and */
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_BIT_AND);
          return BITANDOP;
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
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_EQ);
          return RELOP;
        }
        /* = */
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_ASSIGN);
          return ASSIGNOP;
        }
        
      case '<':
        parser->bufptr++;
        
        if (*parser->bufptr == '<') {
          parser->bufptr++;
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_LEFT_SHIFT);
          return SHIFTOP;
        }
        /* <= */
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_LE);
          return RELOP;
        }
        /* < */
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_LT);
          return RELOP;
        }
      
      case '>':
        parser->bufptr++;
        
        if (*parser->bufptr == '>') {
          parser->bufptr++;
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_RIGHT_SHIFT);
          return SHIFTOP;
        }
        /* >= */
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_GE);
          return RELOP;
        }
        /* < */
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_GT);
          return RELOP;
        }
      case '!':
        parser->bufptr++;
        
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NE);
          return RELOP;
        }
        else {
          SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NOT);
          return NOTOP;
        }
        
      case '~':
        parser->bufptr++;
        SPerl_yylvalp->opval = _newOP(parser,  SPerl_OP_COMPLEMENT);
        return '~';
      
      case '\'': {
        parser->bufptr++;
        
        /* Save current position */
        SPerl_char* cur_token_ptr = parser->bufptr;
        
        SPerl_char ch;
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
        SPerl_OP* op = _newOP(parser, SPerl_OP_CONST);
        SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
        const_info->type = SPerl_CONST_INFO_CHAR;
        const_info->uv.int_value = ch;
        op->uv.pv = const_info;
        SPerl_yylvalp->opval = op;
        
        
        return CONST;
      }
      case '"': {
        parser->bufptr++;
        
        /* Save current position */
        SPerl_char* cur_token_ptr = parser->bufptr;
        
        SPerl_char* str;
        if (*(parser->bufptr + 1) == '"') {
          str = SPerl_PARSER_new_string(parser, 0);
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
          str = SPerl_PARSER_new_string(parser, str_len);
          memcpy(str, cur_token_ptr, str_len);
          str[str_len] = '\0';

          parser->bufptr++;
        }
        
        SPerl_OP* op = _newOP(parser, SPerl_OP_CONST);
        SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
        const_info->type = SPerl_CONST_INFO_STRING;
        const_info->uv.string_value = str;
        op->uv.pv = const_info;
        SPerl_yylvalp->opval = (SPerl_OP*)op;

        return CONST;
      }
      default:
        /* Variable */
        if (c == '$') {
          /* Save current position */
          SPerl_char* cur_token_ptr = parser->bufptr;
          
          parser->bufptr++;

          /* Next is graph */
          while(isalnum(*parser->bufptr) || (*parser->bufptr) == '_' || (*parser->bufptr) == ':') {
            parser->bufptr++;
          }

          size_t str_len = parser->bufptr - cur_token_ptr;
          SPerl_char* var = SPerl_PARSER_new_string(parser, str_len);
          memcpy(var, cur_token_ptr, str_len);
          var[str_len] = '\0';
          
          SPerl_OP* op = _newOP(parser, SPerl_OP_VAR);
          SPerl_VAR_INFO* var_info = SPerl_VAR_INFO_new(parser);
          var_info->name = var;
          op->uv.pv = var_info;
          SPerl_yylvalp->opval = (SPerl_OP*)op;

          return VAR;
        }
        /* Number literal */
        else if (isdigit(c)) {
          SPerl_char* cur_token_ptr = parser->bufptr;
          
          SPerl_int point_count = 0;
          
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
          SPerl_char* num_str = malloc(str_len + 1);
          memcpy(num_str, cur_token_ptr, str_len);
          num_str[str_len] = '\0';
          
          // Convert to double
          if (point_count) {
            char* ends;
            double num = strtod(num_str, &ends);
            free(num_str);

            SPerl_OP* op = _newOP(parser, SPerl_OP_CONST);
            SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
            const_info->type = SPerl_CONST_INFO_DOUBLE;
            const_info->uv.double_value = num;
            op->uv.pv = const_info;
            SPerl_yylvalp->opval = (SPerl_OP*)op;
            
            return CONST;
          }
          // Convert to integer
          else {
            SPerl_int num = atoi(num_str);
            free(num_str);
            
            SPerl_OP* op = _newOP(parser, SPerl_OP_CONST);
            SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
            const_info->type = SPerl_CONST_INFO_INT;
            const_info->uv.int_value = num;
            op->uv.pv = const_info;
            SPerl_yylvalp->opval = (SPerl_OP*)op;
            
            return CONST;
          }
        }
        /* Keyword or word */
        else if (isalpha(c) || c == '_') {
          /* Save current position */
          SPerl_char* cur_token_ptr = parser->bufptr;
          
          parser->bufptr++;
          
          while(isalnum(*parser->bufptr) || (*parser->bufptr) == '_' || (*parser->bufptr) == ':') {
            parser->bufptr++;
          }
          
          size_t str_len = parser->bufptr - cur_token_ptr;
          SPerl_char* keyword = SPerl_PARSER_new_string(parser, str_len);
          memcpy(keyword, cur_token_ptr, str_len);
          keyword[str_len] = '\0';
          
          if (expect != SPerl_OP_EXPECT_WORD) {
            if (memcmp(keyword, "my", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return MY;
            }
            else if (memcmp(keyword, "has", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              parser->expect = SPerl_OP_EXPECT_WORD;
              return HAS;
            }
            else if (memcmp(keyword, "sub", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              parser->expect = SPerl_OP_EXPECT_WORD;
              return SUB;
            }
            else if (memcmp(keyword, "package", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return PACKAGE;
            }
            else if (memcmp(keyword, "if", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return IF;
            }
            else if (memcmp(keyword, "elsif", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return ELSIF;
            }
            else if (memcmp(keyword, "else", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return ELSE;
            }
            else if (memcmp(keyword, "return", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return RETURN;
            }
            else if (memcmp(keyword, "for", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return FOR;
            }
            else if (memcmp(keyword, "last", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return LAST;
            }
            else if (memcmp(keyword, "next", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return NEXT;
            }
            else if (memcmp(keyword, "use", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return USE;
            }
            else if (memcmp(keyword, "while", str_len) == 0) {
              SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
              return WHILE;
            }
            else if (memcmp(keyword, "true", str_len) == 0) {
              SPerl_OP* op = _newOP(parser, SPerl_OP_CONST);
              SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
              const_info->type = SPerl_CONST_INFO_BOOLEAN;
              const_info->uv.int_value = 1;
              op->uv.pv = const_info;
              SPerl_yylvalp->opval = (SPerl_OP*)op;

              return CONST;
            }
            else if (memcmp(keyword, "false", str_len) == 0) {
              SPerl_OP* op = _newOP(parser, SPerl_OP_CONST);
              SPerl_CONST_INFO* const_info = SPerl_CONST_INFO_new(parser);
              const_info->type = SPerl_CONST_INFO_BOOLEAN;
              const_info->uv.int_value = 0;
              op->uv.pv = const_info;
              SPerl_yylvalp->opval = (SPerl_OP*)op;

              return CONST;
            }
          }
          
          SPerl_OP* op = _newOP(parser, SPerl_OP_WORD);
          op->uv.pv = keyword;
          SPerl_yylvalp->opval = (SPerl_OP*)op;

          return WORD;
        }
        
        /* Return character */
        parser->bufptr++;
        SPerl_yylvalp->opval = _newOP(parser, SPerl_OP_NULL);
        
        return c;
    }
  }
}

/* Function for error */
void SPerl_yyerror(SPerl_PARSER* parser, const SPerl_char* s)
{
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
