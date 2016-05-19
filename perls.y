%{
  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>


  int yylex(void);
  void yyerror(const char* s);

  struct op {
    int type;
    struct node *left;
    struct node *right;
    char* pvval;
    int ival;
    double dval;
  };
  
  typedef struct op OP;
%}


%union {
  OP* opval;
  int	ival;
}

%token <ival> MY SUB
%token <opval> WORD VAR INT

%left '+'
%left '*'

%%

program
  : statements
    { printf("statements -> program\n"); }

statements
  : statement
    { printf("statement -> statements\n"); }
  | statement statements
    { printf("statement statements -> statements\n"); }

statement
  : expression
    { printf("expression -> statement\n"); }
  | SUB subname '{' expressions '}'
    { printf("sub subname { expressions } -> statement\n"); }

subname :
  WORD
    { printf("WORD -> subname\n"); }

expressions
  : expression
    { printf("expression -> expressions\n"); }
  | expression expressions
    { printf("expression expressions -> expressions\n"); }

expression
  : term ';'
    { printf("term ; -> expression\n") }

term
  : WORD
    { printf("WORD -> term (%s)\n", ((OP*)$1)->pvval); }
  | VAR
    { printf("VAR -> term (%s)\n", ((OP*)$1)->pvval); }
  | term '+' term
    { printf("term + term -> term\n"); }
  | term '*' term
    { printf("term * term -> term\n"); }
  | MY term
    { printf("MY term -> term\n"); }
  | INT
    { printf("INT -> term (%d)\n", ((OP*)$1)->ival); }
  | term '=' term
    { printf("term = term -> term\n"); }
%%

/* まず単語を切り分けられるようになろう */

// 解析する文章全体
char* linestr = NULL;

// 解析する文章が格納されてるデータのバッファサイズ
size_t linestr_buf_len = 0;

// 解析する文章が格納されているデータのサイズ
size_t linestr_len = 0;

// 現在のバッファのポジション
char* buf_ptr = NULL;

// 現在のトークンの開始位置
char* cur_token_ptr = NULL;

// 一つ前のトークンの開始位置
char* bef_token_ptr = NULL;

/* Get token */
int yylex(void)
{
  while(1) {
    // Get current character
    char c = *buf_ptr;
    
    // line end
    switch(c) {
      // End
      case '\0':
        return 0;
      
      // Skip space character
      case ' ':
      case '\t':
      case '\n':
      case '\r':
      case EOF :
        buf_ptr++;
        continue;
      
      // Addition
      case '+':
        buf_ptr++;
        return '+';
      
      // Multiply
      case '*':
        buf_ptr++;
        return '*';
      
      // Comment
      case '#':
        buf_ptr++;
        while(1) {
          if (*buf_ptr == '\r' || *buf_ptr == '\n' || *buf_ptr == EOF || *buf_ptr == '\0') {
            break;
          }
          buf_ptr++;
        }
        continue;
      
      default:
        // Variable
        if (c == '$') {
          // Save current position
          cur_token_ptr = buf_ptr;
          
          buf_ptr++;

          // Next is graph
          while(isalnum(*buf_ptr)) {
            buf_ptr++;
          }

          size_t str_len = buf_ptr - cur_token_ptr;
          char* var = malloc(str_len + 1);
          memcpy(var, cur_token_ptr, str_len);
          var[str_len] = '\0';
          
          OP* op = malloc(sizeof(OP));
          op->type = 2;
          op->pvval = var;
          
          yylval.opval = op;
          return VAR;
        }
        // Number
        else if (isdigit(c)) {
          cur_token_ptr = buf_ptr;
          
          buf_ptr++;
          
          // Scan number
          while(isdigit(*buf_ptr)) {
            buf_ptr++;
          }
          
          if (isalpha(*buf_ptr)) {
            fprintf(stderr, "syntax error: invalid variable name\n");
            exit(1);
          }
          
          // Convert to integer
          size_t str_len = buf_ptr - cur_token_ptr;
          char* num_str = malloc(str_len + 1);
          memcpy(num_str, cur_token_ptr, str_len);
          num_str[str_len] = '\0';
          int num = atoi(num_str);
          free(num_str);
          
          OP* op = malloc(sizeof(OP));
          op->type = 3;
          op->ival = num;
          
          yylval.opval = op;
          return INT;
        }
        // Keyword or word
        else if (isalnum(c)) {
          // Save current position
          cur_token_ptr = buf_ptr;
          
          buf_ptr++;
          
          // Next is graph
          while(isalnum(*buf_ptr)) {
            buf_ptr++;
          }
          
          size_t str_len = buf_ptr - cur_token_ptr;
          char* keyword = malloc(str_len + 1);
          memcpy(keyword, cur_token_ptr, str_len);
          keyword[str_len] = '\0';
          
          if (memcmp(keyword, "my", str_len) == 0) {
            return MY;
          }
          else if (memcmp(keyword, "sub", str_len) == 0) {
            return SUB;
          }
          
          OP* op = malloc(sizeof(OP));
          op->type = 1;
          op->pvval = keyword;
          
          yylval.opval = op;
          return WORD;
        }
        
        /* Return character */
        buf_ptr++;
        return c;
    }
  }
}

/* エラー表示関数 */
void yyerror(const char* s)
{
  fprintf(stderr, "error: %s\n", s);
}

int main(void)
{
  /* ファイルから文字を読み込む */
  char* file = "a.pl";
  
  /* ファイルオープン */
  FILE* fp = fopen(file, "r");
  if (fp == NULL) {
    printf("Can't open file %s\n", file);
    return -1;
  }
  
  /* ファイル全体をバッファに読み込み */
  linestr_len = getdelim(&linestr, &linestr_buf_len, EOF, fp);
  
  /* ファイルクローズ */
  fclose(fp);
  
  /* パーサーの情報の初期化 */
  buf_ptr = linestr;
  cur_token_ptr = linestr;
  bef_token_ptr = linestr;
  
  /* call yyparse */
  int parse_success = yyparse();
  
  free(linestr);
  
  return parse_success;
}
