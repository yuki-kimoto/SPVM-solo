/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse SPerl_yyparse
#define yylex   SPerl_yylex
#define yyerror SPerl_yyerror
#define yylval  SPerl_yylval
#define yychar  SPerl_yychar
#define yydebug SPerl_yydebug
#define yynerrs SPerl_yynerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     MY = 258,
     OUR = 259,
     HAS = 260,
     SUB = 261,
     PACKAGE = 262,
     IF = 263,
     ELSIF = 264,
     ELSE = 265,
     RETURN = 266,
     FOR = 267,
     WHILE = 268,
     USE = 269,
     RELOP = 270,
     EQOP = 271,
     LAST = 272,
     CONTINUE = 273,
     WORD = 274,
     VAR = 275,
     INT = 276,
     STRING = 277,
     BOOL = 278,
     ASSIGNOP = 279,
     OROP = 280,
     ANDOP = 281,
     BITOROP = 282,
     BITANDOP = 283,
     SHIFTOP = 284,
     ADDOP = 285,
     MULOP = 286,
     UMINUS = 287,
     BITNOTOP = 288,
     NOTOP = 289,
     DECOP = 290,
     INCOP = 291,
     ARROW = 292
   };
#endif
/* Tokens.  */
#define MY 258
#define OUR 259
#define HAS 260
#define SUB 261
#define PACKAGE 262
#define IF 263
#define ELSIF 264
#define ELSE 265
#define RETURN 266
#define FOR 267
#define WHILE 268
#define USE 269
#define RELOP 270
#define EQOP 271
#define LAST 272
#define CONTINUE 273
#define WORD 274
#define VAR 275
#define INT 276
#define STRING 277
#define BOOL 278
#define ASSIGNOP 279
#define OROP 280
#define ANDOP 281
#define BITOROP 282
#define BITANDOP 283
#define SHIFTOP 284
#define ADDOP 285
#define MULOP 286
#define UMINUS 287
#define BITNOTOP 288
#define NOTOP 289
#define DECOP 290
#define INCOP 291
#define ARROW 292




/* Copy the first part of user declarations.  */
#line 5 "sperly.y"

  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
  
  #include "sperl_parser.h"
  #include "sperl_op.h"


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 198 "sperly.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  58
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   757

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNRULES -- Number of states.  */
#define YYNSTATES  186

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      41,    40,     2,     3,    47,     4,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    45,    46,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    42,     2,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,     2,    44,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    14,    18,    26,
      35,    38,    40,    42,    54,    60,    64,    67,    70,    74,
      80,    84,    90,    97,    99,   103,   108,   113,   118,   124,
     127,   130,   137,   139,   143,   145,   148,   151,   154,   157,
     160,   163,   166,   169,   173,   177,   181,   183,   187,   192,
     196,   200,   204,   208,   212,   216,   220,   224,   231,   239,
     243,   247,   253,   259,   266,   270,   276,   282,   289,   295,
     297,   299,   301,   303,   305,   309,   313,   315,   318,   320,
     322
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      50,     0,    -1,    -1,    51,    -1,    53,    -1,    51,    53,
      -1,    43,    44,    -1,    43,    51,    44,    -1,     8,    60,
      45,    63,    41,    40,    52,    -1,     8,    60,    45,    63,
      41,    61,    40,    52,    -1,    59,    46,    -1,    46,    -1,
      56,    -1,    14,    41,    59,    46,    59,    46,    59,    40,
      43,    51,    44,    -1,    15,    41,    59,    40,    52,    -1,
       9,    21,    46,    -1,    19,    46,    -1,    20,    46,    -1,
      13,    59,    46,    -1,    13,    41,    58,    40,    46,    -1,
      16,    21,    46,    -1,    16,    21,    41,    40,    46,    -1,
      16,    21,    41,    54,    40,    46,    -1,    21,    -1,    54,
      47,    21,    -1,     5,    22,    45,    63,    -1,     6,    22,
      45,    63,    -1,     7,    21,    45,    63,    -1,    10,    41,
      59,    40,    52,    -1,    56,    57,    -1,    12,    52,    -1,
      11,    41,    59,    40,    52,    57,    -1,    59,    -1,    58,
      47,    59,    -1,    22,    -1,    36,    59,    -1,    35,    59,
      -1,    38,    59,    -1,    59,    38,    -1,    37,    59,    -1,
      59,    37,    -1,     3,    59,    -1,     4,    59,    -1,    59,
       3,    59,    -1,    59,     4,    59,    -1,    59,    33,    59,
      -1,    25,    -1,    60,    41,    40,    -1,    60,    41,    58,
      40,    -1,    59,    17,    59,    -1,    59,    18,    59,    -1,
      59,    26,    59,    -1,    59,    28,    59,    -1,    59,    27,
      59,    -1,    59,    30,    59,    -1,    59,    29,    59,    -1,
      59,    31,    59,    -1,     8,    45,    63,    41,    40,    52,
      -1,     8,    45,    63,    41,    61,    40,    52,    -1,    41,
      59,    40,    -1,    22,    39,    21,    -1,    22,    39,    21,
      26,    59,    -1,    22,    39,    21,    41,    40,    -1,    22,
      39,    21,    41,    58,    40,    -1,    21,    39,    21,    -1,
      21,    39,    21,    26,    59,    -1,    21,    39,    21,    41,
      40,    -1,    21,    39,    21,    41,    58,    40,    -1,    22,
      39,    42,    59,    48,    -1,    55,    -1,    23,    -1,    24,
      -1,    21,    -1,    62,    -1,    61,    47,    62,    -1,    22,
      45,    63,    -1,    64,    -1,    65,    64,    -1,    21,    -1,
      21,    -1,    65,    21,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    42,    42,    43,    47,    49,    53,    55,    59,    61,
      63,    65,    67,    69,    71,    73,    75,    77,    79,    81,
      83,    85,    87,    91,    92,    95,    97,    99,   103,   105,
     109,   111,   115,   117,   121,   126,   128,   130,   132,   134,
     136,   138,   140,   142,   147,   149,   151,   153,   155,   157,
     159,   161,   163,   165,   167,   169,   171,   173,   175,   177,
     179,   181,   183,   185,   187,   189,   191,   193,   195,   197,
     199,   204,   211,   215,   217,   221,   225,   227,   231,   235,
     237
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'+'", "'-'", "MY", "OUR", "HAS", "SUB",
  "PACKAGE", "IF", "ELSIF", "ELSE", "RETURN", "FOR", "WHILE", "USE",
  "RELOP", "EQOP", "LAST", "CONTINUE", "WORD", "VAR", "INT", "STRING",
  "BOOL", "ASSIGNOP", "OROP", "ANDOP", "BITOROP", "BITANDOP", "SHIFTOP",
  "ADDOP", "MULOP", "UMINUS", "BITNOTOP", "NOTOP", "DECOP", "INCOP",
  "ARROW", "')'", "'('", "'['", "'{'", "'}'", "':'", "';'", "','", "']'",
  "$accept", "grammar", "statements", "block", "statement", "words",
  "declvar", "if", "else", "terms", "term", "subname", "subargs", "subarg",
  "modiftype", "type", "modifier", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    43,    45,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
      41,    40,    91,   123,   125,    58,    59,    44,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    52,    52,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    54,    54,    55,    55,    55,    56,    56,
      57,    57,    58,    58,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    60,    61,    61,    62,    63,    63,    64,    65,
      65
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     2,     3,     7,     8,
       2,     1,     1,    11,     5,     3,     2,     2,     3,     5,
       3,     5,     6,     1,     3,     4,     4,     4,     5,     2,
       2,     6,     1,     3,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     1,     3,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     6,     7,     3,
       3,     5,     5,     6,     3,     5,     5,     6,     5,     1,
       1,     1,     1,     1,     3,     3,     1,     2,     1,     1,
       2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    72,    34,    70,    71,    46,
       0,     0,     0,     0,     0,    11,     0,     3,     4,    69,
      12,     0,     0,     0,    41,    42,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
      17,     0,     0,    36,    35,    39,    37,     0,     1,     5,
       0,     0,    29,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,    38,    10,     0,     0,     0,
       0,    78,     0,    76,     0,     0,    15,     0,     0,    32,
      18,     0,     0,     0,    20,    64,    60,     0,    59,     0,
       0,    30,    43,    44,    49,    50,    51,    53,    52,    55,
      54,    56,    45,    47,     0,    32,    25,    26,    27,     0,
      78,    77,     0,     0,     0,     0,     0,     0,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     6,     0,    48,
       0,     0,     0,    73,     0,    28,    19,    33,     0,    14,
      21,     0,     0,    65,    66,     0,    61,    62,     0,    68,
       0,     7,     0,    57,     0,     0,     0,     0,     0,    22,
      24,    67,    63,     0,    75,    58,    74,     8,     0,     0,
      31,     9,     0,     0,     0,    13
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,    27,   101,    28,   130,    29,    30,    62,    88,
      31,    32,   142,   143,    82,    83,    84
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -95
static const yytype_int16 yypact[] =
{
     273,   481,   481,    -6,     4,     7,   -14,    19,   -27,   520,
       3,    36,    62,    33,    39,    67,    70,   -95,   -95,   -95,
     481,   481,   481,   481,   481,   -95,    95,   273,   -95,   -95,
      24,   298,    69,    81,    97,    97,    84,    94,    96,   -95,
      93,    98,   106,   481,   481,   329,   481,   481,   -29,   -95,
     -95,   119,   -12,    97,    97,    99,    99,    21,   -95,   -95,
     101,   110,   -95,   481,   481,   481,   481,   481,   481,   481,
     481,   481,   481,   481,   -95,   -95,   -95,   427,    93,    93,
      93,   137,   129,   -95,   148,    93,   -95,   536,   -13,    21,
     -95,   360,   567,    -8,   -95,    56,    58,   481,   -95,   481,
     141,   -95,    53,    53,   180,   711,   632,   663,   680,   702,
     719,    53,    97,   -95,    13,   632,   -95,   -95,   -95,   -11,
     150,   -95,   131,   110,   127,   481,   481,   110,   -95,   134,
      41,   481,   435,   481,   474,    90,   584,   -95,   185,   -95,
     136,   110,    64,   -95,    -7,   -95,   -95,   632,   391,   -95,
     -95,   140,   153,   632,   -95,    65,   632,   -95,    75,   -95,
     110,   -95,    93,   -95,   110,   174,   110,   128,   481,   -95,
     -95,   -95,   -95,    24,   -95,   -95,   -95,   -95,   110,   615,
     -95,   -95,   159,   273,   229,   -95
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -95,   -95,   -94,   -86,   -25,   -95,   -95,   -95,    30,   -59,
      -1,   206,    71,    49,   -75,   132,   -95
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -81
static const yytype_int16 yytable[] =
{
      34,    35,    59,   116,   117,   118,   138,    39,    45,    96,
     122,   140,    93,   128,    43,   140,    36,    94,   114,    53,
      54,    55,    56,    57,    63,    64,    37,   124,    38,   141,
      97,    40,   129,   166,   125,    60,    61,   145,    65,    66,
      42,   149,    87,    89,    46,    91,    92,    67,    68,    69,
      70,    71,    72,   139,    73,   163,    63,    64,    74,    75,
     125,    98,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   155,   173,   158,   115,    47,   175,    49,
     177,   151,   131,    48,   133,    50,    73,   174,   152,   184,
      74,    75,   181,    63,    64,    58,   135,   132,   136,   134,
      63,    64,    63,    64,   164,   171,    51,    65,    66,    52,
      77,   165,   125,    59,    81,   172,    67,    68,    69,    70,
      71,    72,   125,    73,   147,   148,    40,    74,    75,    78,
     153,   115,   156,   115,    74,    75,   -81,   -81,   159,    79,
      95,    80,    99,    85,     1,     2,     3,     4,     5,     6,
       7,     8,    86,   100,     9,    10,    11,    12,   -79,    59,
      13,    14,    15,    16,    17,    18,    19,   179,   178,   120,
     119,   -80,   144,   146,   170,   165,    20,    21,    22,    23,
     150,   162,    24,    63,    64,   137,   169,    25,     1,     2,
       3,     4,     5,     6,     7,     8,   140,   -81,     9,    10,
      11,    12,   183,   180,    13,    14,    15,    16,    17,    18,
      19,    72,    41,    73,   176,   167,   121,    74,    75,     0,
      20,    21,    22,    23,     0,     0,    24,     0,     0,   161,
       0,    25,     1,     2,     3,     4,     5,     6,     7,     8,
       0,     0,     9,    10,    11,    12,     0,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,    21,    22,    23,     0,     0,
      24,     0,     0,   185,     0,    25,     1,     2,     3,     4,
       5,     6,     7,     8,     0,     0,     9,    10,    11,    12,
       0,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    63,    64,     0,     0,     0,     0,     0,    20,    21,
      22,    23,     0,     0,    24,    65,    66,     0,     0,    25,
       0,     0,     0,     0,    67,    68,    69,    70,    71,    72,
       0,    73,    63,    64,     0,    74,    75,     0,     0,     0,
       0,     0,     0,     0,    76,     0,    65,    66,     0,     0,
       0,     0,     0,     0,     0,    67,    68,    69,    70,    71,
      72,     0,    73,    63,    64,     0,    74,    75,     0,     0,
       0,     0,     0,     0,     0,    90,     0,    65,    66,     0,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,    72,     0,    73,    63,    64,     0,    74,    75,     0,
       0,     0,     0,     0,     0,     0,   126,     0,    65,    66,
       0,     0,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,    72,     0,    73,     0,     0,     0,    74,    75,
       1,     2,     3,     4,     5,    33,     0,   168,     1,     2,
       3,     4,     5,    33,     0,     0,     0,     0,    15,    16,
      17,    18,    19,     0,     0,     0,    15,    16,    17,    18,
      19,     0,    20,    21,    22,    23,     0,   113,    24,     0,
      20,    21,    22,    23,     0,   154,    24,     1,     2,     3,
       4,     5,    33,     0,     1,     2,     3,     4,     5,    33,
       0,     0,     0,     0,     0,    15,    16,    17,    18,    19,
       0,     0,    15,    16,    17,    18,    19,     0,     0,    20,
      21,    22,    23,     0,   157,    24,    20,    21,    22,    23,
       0,     0,    24,     1,     2,     3,     4,     5,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,     0,    65,    66,    20,    21,    22,    23,     0,
       0,    44,    67,    68,    69,    70,    71,    72,     0,    73,
      63,    64,     0,    74,    75,     0,   123,     0,     0,     0,
       0,     0,     0,     0,    65,    66,     0,    63,    64,     0,
       0,     0,     0,    67,    68,    69,    70,    71,    72,     0,
      73,    65,    66,     0,    74,    75,     0,   127,     0,     0,
      67,    68,    69,    70,    71,    72,     0,    73,    63,    64,
       0,    74,    75,     0,   160,     0,     0,     0,     0,     0,
       0,     0,    65,    66,     0,    63,    64,     0,     0,     0,
       0,    67,    68,    69,    70,    71,    72,     0,    73,    65,
      66,     0,    74,    75,     0,   182,     0,     0,    67,    68,
      69,    70,    71,    72,     0,    73,    63,    64,     0,    74,
      75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    66,     0,    63,    64,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,    73,    65,    66,     0,
      74,    75,     0,     0,     0,    63,    64,     0,     0,    70,
      71,    72,     0,    73,    63,    64,     0,    74,    75,    65,
      66,     0,    63,    64,     0,     0,     0,     0,    65,   -81,
       0,     0,    71,    72,     0,    73,    65,    66,     0,    74,
      75,     0,    72,     0,    73,     0,     0,     0,    74,    75,
      72,     0,    73,     0,     0,     0,    74,    75
};

static const yytype_int16 yycheck[] =
{
       1,     2,    27,    78,    79,    80,   100,    21,     9,    21,
      85,    22,    41,    21,    41,    22,    22,    46,    77,    20,
      21,    22,    23,    24,     3,     4,    22,    40,    21,    40,
      42,    45,    40,    40,    47,    11,    12,   123,    17,    18,
      21,   127,    43,    44,    41,    46,    47,    26,    27,    28,
      29,    30,    31,    40,    33,   141,     3,     4,    37,    38,
      47,    40,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,   132,   160,   134,    77,    41,   164,    46,
     166,    40,    26,    21,    26,    46,    33,   162,    47,   183,
      37,    38,   178,     3,     4,     0,    97,    41,    99,    41,
       3,     4,     3,     4,    40,    40,    39,    17,    18,    39,
      41,    47,    47,   138,    21,    40,    26,    27,    28,    29,
      30,    31,    47,    33,   125,   126,    45,    37,    38,    45,
     131,   132,   133,   134,    37,    38,    37,    38,    48,    45,
      21,    45,    41,    45,     3,     4,     5,     6,     7,     8,
       9,    10,    46,    43,    13,    14,    15,    16,    21,   184,
      19,    20,    21,    22,    23,    24,    25,   168,    40,    21,
      41,    21,    41,    46,    21,    47,    35,    36,    37,    38,
      46,    45,    41,     3,     4,    44,    46,    46,     3,     4,
       5,     6,     7,     8,     9,    10,    22,    17,    13,    14,
      15,    16,    43,   173,    19,    20,    21,    22,    23,    24,
      25,    31,     6,    33,   165,   144,    84,    37,    38,    -1,
      35,    36,    37,    38,    -1,    -1,    41,    -1,    -1,    44,
      -1,    46,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    37,    38,    -1,    -1,
      41,    -1,    -1,    44,    -1,    46,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,     3,     4,    -1,    -1,    -1,    -1,    -1,    35,    36,
      37,    38,    -1,    -1,    41,    17,    18,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    31,
      -1,    33,     3,     4,    -1,    37,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,     3,     4,    -1,    37,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    17,    18,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    31,    -1,    33,     3,     4,    -1,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    17,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    37,    38,
       3,     4,     5,     6,     7,     8,    -1,    46,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    21,    22,    23,    24,
      25,    -1,    35,    36,    37,    38,    -1,    40,    41,    -1,
      35,    36,    37,    38,    -1,    40,    41,     3,     4,     5,
       6,     7,     8,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    21,    22,    23,    24,    25,
      -1,    -1,    21,    22,    23,    24,    25,    -1,    -1,    35,
      36,    37,    38,    -1,    40,    41,    35,    36,    37,    38,
      -1,    -1,    41,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    35,    36,    37,    38,    -1,
      -1,    41,    26,    27,    28,    29,    30,    31,    -1,    33,
       3,     4,    -1,    37,    38,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,     3,     4,    -1,
      -1,    -1,    -1,    26,    27,    28,    29,    30,    31,    -1,
      33,    17,    18,    -1,    37,    38,    -1,    40,    -1,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,     3,     4,
      -1,    37,    38,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,     3,     4,    -1,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    31,    -1,    33,    17,
      18,    -1,    37,    38,    -1,    40,    -1,    -1,    26,    27,
      28,    29,    30,    31,    -1,    33,     3,     4,    -1,    37,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    30,    31,    -1,    33,    17,    18,    -1,
      37,    38,    -1,    -1,    -1,     3,     4,    -1,    -1,    29,
      30,    31,    -1,    33,     3,     4,    -1,    37,    38,    17,
      18,    -1,     3,     4,    -1,    -1,    -1,    -1,    17,    18,
      -1,    -1,    30,    31,    -1,    33,    17,    18,    -1,    37,
      38,    -1,    31,    -1,    33,    -1,    -1,    -1,    37,    38,
      31,    -1,    33,    -1,    -1,    -1,    37,    38
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    13,
      14,    15,    16,    19,    20,    21,    22,    23,    24,    25,
      35,    36,    37,    38,    41,    46,    50,    51,    53,    55,
      56,    59,    60,     8,    59,    59,    22,    22,    21,    21,
      45,    60,    21,    41,    41,    59,    41,    41,    21,    46,
      46,    39,    39,    59,    59,    59,    59,    59,     0,    53,
      11,    12,    57,     3,     4,    17,    18,    26,    27,    28,
      29,    30,    31,    33,    37,    38,    46,    41,    45,    45,
      45,    21,    63,    64,    65,    45,    46,    59,    58,    59,
      46,    59,    59,    41,    46,    21,    21,    42,    40,    41,
      43,    52,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    40,    58,    59,    63,    63,    63,    41,
      21,    64,    63,    40,    40,    47,    46,    40,    21,    40,
      54,    26,    41,    26,    41,    59,    59,    44,    51,    40,
      22,    40,    61,    62,    41,    52,    46,    59,    59,    52,
      46,    40,    47,    59,    40,    58,    59,    40,    58,    48,
      40,    44,    45,    52,    40,    47,    40,    61,    46,    46,
      21,    40,    40,    52,    63,    52,    62,    52,    40,    59,
      57,    52,    40,    43,    51,    44
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (parser, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, parser)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, parser); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, SPerl_yy_parser* parser)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    SPerl_yy_parser* parser;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (parser);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, SPerl_yy_parser* parser)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, parser)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    SPerl_yy_parser* parser;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, SPerl_yy_parser* parser)
#else
static void
yy_reduce_print (yyvsp, yyrule, parser)
    YYSTYPE *yyvsp;
    int yyrule;
    SPerl_yy_parser* parser;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , parser);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, parser); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, SPerl_yy_parser* parser)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, parser)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    SPerl_yy_parser* parser;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (parser);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (SPerl_yy_parser* parser);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (SPerl_yy_parser* parser)
#else
int
yyparse (parser)
    SPerl_yy_parser* parser;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 42 "sperly.y"
    { printf("NULL -> grammar\n"); ;}
    break;

  case 3:
#line 44 "sperly.y"
    { printf("statements -> grammar\n"); ;}
    break;

  case 4:
#line 48 "sperly.y"
    { printf("statement -> statements\n"); ;}
    break;

  case 5:
#line 50 "sperly.y"
    { printf("statements statement -> statements\n"); ;}
    break;

  case 6:
#line 54 "sperly.y"
    { printf("{ } -> block\n"); ;}
    break;

  case 7:
#line 56 "sperly.y"
    { printf("{ statements } -> block\n"); ;}
    break;

  case 8:
#line 60 "sperly.y"
    { printf("SUB subname : modiftype ( ) block -> statement\n"); ;}
    break;

  case 9:
#line 62 "sperly.y"
    { printf("SUB subname : modiftype ( subargs ) block -> statement\n"); ;}
    break;

  case 10:
#line 64 "sperly.y"
    { printf("term ; -> statement\n") ;}
    break;

  case 11:
#line 66 "sperly.y"
    { printf("; -> statement\n") ;}
    break;

  case 12:
#line 68 "sperly.y"
    { printf("if -> statement\n"); ;}
    break;

  case 13:
#line 70 "sperly.y"
    { printf("FOR ( term ; term ; term ) { statements }\n"); ;}
    break;

  case 14:
#line 72 "sperly.y"
    { printf("WHILE ( term ) block\n"); ;}
    break;

  case 15:
#line 74 "sperly.y"
    { printf("PACKAGE WORD ; -> statement\n"); ;}
    break;

  case 16:
#line 76 "sperly.y"
    { printf("LAST ; -> statement\n"); ;}
    break;

  case 17:
#line 78 "sperly.y"
    { printf("CONTINUE ; -> statement\n"); ;}
    break;

  case 18:
#line 80 "sperly.y"
    { printf("RETURN term ; -> statement\n"); ;}
    break;

  case 19:
#line 82 "sperly.y"
    { printf("RETURN ( terms ) ; -> statement\n"); ;}
    break;

  case 20:
#line 84 "sperly.y"
    { printf("USE WORD ; -> statement\n"); ;}
    break;

  case 21:
#line 86 "sperly.y"
    { printf("USE WORD ( ) ; -> statement\n"); ;}
    break;

  case 22:
#line 88 "sperly.y"
    { printf("USE WORD ( words ) ; -> statement\n"); ;}
    break;

  case 25:
#line 96 "sperly.y"
    { printf("MY VAR : modiftype -> declvar\n"); ;}
    break;

  case 26:
#line 98 "sperly.y"
    { printf("OUR VAR : modiftype -> declvar\n"); ;}
    break;

  case 27:
#line 100 "sperly.y"
    { printf("HAS WORD : modiftype -> declvar\n"); ;}
    break;

  case 28:
#line 104 "sperly.y"
    { printf("if ( term ) block\n"); ;}
    break;

  case 29:
#line 106 "sperly.y"
    { printf("if else -> if\n"); ;}
    break;

  case 30:
#line 110 "sperly.y"
    { printf("else block"); ;}
    break;

  case 31:
#line 112 "sperly.y"
    { printf("elsif ( term ) block else\n"); ;}
    break;

  case 32:
#line 116 "sperly.y"
    { printf("term -> terms\n"); ;}
    break;

  case 33:
#line 118 "sperly.y"
    { printf("terms , term -> terms\n"); ;}
    break;

  case 34:
#line 122 "sperly.y"
    {
      (yyval.opval) = (yyvsp[(1) - (1)].opval);
      printf("VAR -> term (%s)\n", ((SPerl_SVOP*)(yyvsp[(1) - (1)].opval))->uv.pv)
    ;}
    break;

  case 35:
#line 127 "sperly.y"
    { printf("NOTOP term -> term\n"); ;}
    break;

  case 36:
#line 129 "sperly.y"
    { printf("BITNOTOP term -> term\n"); ;}
    break;

  case 37:
#line 131 "sperly.y"
    { printf("INCOP term\n") ;}
    break;

  case 38:
#line 133 "sperly.y"
    { printf("term INCOP\n") ;}
    break;

  case 39:
#line 135 "sperly.y"
    { printf("DECOP term\n") ;}
    break;

  case 40:
#line 137 "sperly.y"
    { printf("term DECOP\n") ;}
    break;

  case 41:
#line 139 "sperly.y"
    {printf("+ term -> term\n"); ;}
    break;

  case 42:
#line 141 "sperly.y"
    { printf("- term -> term\n"); ;}
    break;

  case 43:
#line 143 "sperly.y"
    {
      (yyval.opval) = SPerl_newBINOP(SPerl_OP_ADD, 0, (yyvsp[(1) - (3)].opval), (yyvsp[(3) - (3)].opval));
      printf("term + term -> term\n");
    ;}
    break;

  case 44:
#line 148 "sperly.y"
    { printf("term - term -> term\n"); ;}
    break;

  case 45:
#line 150 "sperly.y"
    { printf("term * term -> term\n"); ;}
    break;

  case 46:
#line 152 "sperly.y"
    { printf("BOOL -> term (%d)\n", ((SPerl_SVOP*)(yyvsp[(1) - (1)].opval))->uv.iv); ;}
    break;

  case 47:
#line 154 "sperly.y"
    { printf("subname () -> term\n"); ;}
    break;

  case 48:
#line 156 "sperly.y"
    { printf("subname (terms) -> term\n"); ;}
    break;

  case 49:
#line 158 "sperly.y"
    { printf("term RELOP term -> term %d\n", (yyvsp[(2) - (3)].ival)); ;}
    break;

  case 50:
#line 160 "sperly.y"
    { printf("term EQOP term -> term\n"); ;}
    break;

  case 51:
#line 162 "sperly.y"
    { printf("term ASSIGNOP term -> term\n"); ;}
    break;

  case 52:
#line 164 "sperly.y"
    { printf("term ANDOP term -> term\n"); ;}
    break;

  case 53:
#line 166 "sperly.y"
    { printf("term OROP term -> term\n"); ;}
    break;

  case 54:
#line 168 "sperly.y"
    { printf("term BITANDOP term -> term\n"); ;}
    break;

  case 55:
#line 170 "sperly.y"
    { printf("term BITOROP term -> term\n"); ;}
    break;

  case 56:
#line 172 "sperly.y"
    { printf("term SHIFTOP term\n"); ;}
    break;

  case 57:
#line 174 "sperly.y"
    { printf("SUB : modiftype () block -> term\n"); ;}
    break;

  case 58:
#line 176 "sperly.y"
    { printf("SUB : modiftype ( subargs ) block -> term\n"); ;}
    break;

  case 59:
#line 178 "sperly.y"
    { printf("( term ) -> term\n"); ;}
    break;

  case 60:
#line 180 "sperly.y"
    { printf("VAR ARROW WORD -> term\n"); ;}
    break;

  case 61:
#line 182 "sperly.y"
    { printf("VAR ARROW WORD ASSIGNOP term -> term\n"); ;}
    break;

  case 62:
#line 184 "sperly.y"
    { printf("VAR ARROW WORD ( )\n"); ;}
    break;

  case 63:
#line 186 "sperly.y"
    { printf("VAR ARROW WORD ( terms )\n"); ;}
    break;

  case 64:
#line 188 "sperly.y"
    { printf("VAR ARROW WORD -> term\n"); ;}
    break;

  case 65:
#line 190 "sperly.y"
    { printf("VAR ARROW WORD ASSIGNOP term -> term\n"); ;}
    break;

  case 66:
#line 192 "sperly.y"
    { printf("VAR ARROW WORD ( )\n"); ;}
    break;

  case 67:
#line 194 "sperly.y"
    { printf("VAR ARROW WORD ( terms )\n"); ;}
    break;

  case 68:
#line 196 "sperly.y"
    { printf("VAR ARROW [ term ]\n"); ;}
    break;

  case 69:
#line 198 "sperly.y"
    { printf("declvar -> term\n"); ;}
    break;

  case 70:
#line 200 "sperly.y"
    {
      (yyval.opval) = (yyvsp[(1) - (1)].opval);
      printf("INT -> term (%d)\n", ((SPerl_SVOP*)(yyvsp[(1) - (1)].opval))->uv.iv);
    ;}
    break;

  case 71:
#line 205 "sperly.y"
    {
      (yyval.opval) = (yyvsp[(1) - (1)].opval);
      printf("STRING(%s) -> statement\n", ((SPerl_SVOP*)(yyvsp[(1) - (1)].opval))->uv.pv);
    ;}
    break;

  case 72:
#line 212 "sperly.y"
    { printf("WORD -> subname\n"); ;}
    break;

  case 73:
#line 216 "sperly.y"
    { printf("subarg -> subargs\n"); ;}
    break;

  case 74:
#line 218 "sperly.y"
    { printf("subargs , subarg\n"); ;}
    break;

  case 75:
#line 222 "sperly.y"
    { printf("VAR : modiftype -> subarg (%s)\n", ((SPerl_SVOP*)(yyvsp[(1) - (3)].opval))->uv.pv); ;}
    break;

  case 76:
#line 226 "sperly.y"
    { printf("type -> modiftype\n"); ;}
    break;

  case 77:
#line 228 "sperly.y"
    { printf("modifier type -> modiftype\n"); ;}
    break;

  case 78:
#line 232 "sperly.y"
    { printf("WORD -> type (%s)\n", ((SPerl_SVOP*)(yyvsp[(1) - (1)].opval))->uv.pv); ;}
    break;

  case 79:
#line 236 "sperly.y"
    { printf("WORD -> modifier\n"); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2060 "sperly.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (parser, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (parser, yymsg);
	  }
	else
	  {
	    yyerror (parser, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, parser);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, parser);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (parser, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, parser);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, parser);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 239 "sperly.y"



