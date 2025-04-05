/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "2005031.y"

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<cmath>
#include<vector>
#include "2005031.h"
// #define YYSTYPE SymbolInfo*

using namespace std;
int yyparse(void);
int yylex(void);
extern FILE *yyin;
extern int line_count;

SymbolTable *table;
SymbolInfo* parseTree;

FILE* logout;
FILE* errorout;
FILE* treeout;

int errcount = 0;

void yyerror(char *s)
{
	//write your code
	fprintf(logout,"Error at line no %d : syntax error\n",line_count);
}

class FuncList {
	public:
	string name;
	string type;
	FuncList* next;
	SymbolInfo* args;
	bool defined;
	FuncList(string name="", string type="", FuncList* next=NULL, bool defined=false) {
		this->name = name;
		this->type = type;
		this->next = next;
		this->args = NULL;
		this->defined = defined;
	}
	void addArg (string type) {
		this->args = new SymbolInfo(type,type,this->args);
	}
	FuncList* findFunc(string name){
		for (FuncList* x=this; x!=NULL; x=x->next)
			if(x->name==name)
				return x;
		return NULL;
	}
	bool checkEqArgs (SymbolInfo* args2) {
		SymbolInfo* args1 = this->args;
		while(args1!=NULL && args2!=NULL) {
			if(args1->getName()!=args2->getType()){
				fprintf(errorout,"Line# 45: Type mismatch for argument 1 of 'func'\n");
				errcount++;
			}
			args1=args1->getNextSymbolInfo();
			args2=args2->getNextSymbolInfo();
		}
		if(args1!=NULL || args2!=NULL)
			return false;
		return true;
	}
	bool checkEqNumArgs (SymbolInfo* args2) {
		SymbolInfo* args1 = this->args;
		while(args1!=NULL && args2!=NULL) {
			args1=args1->getNextSymbolInfo();
			args2=args2->getNextSymbolInfo();
		}
		if(args1!=NULL && args2!=NULL)
			return true;
		return false;
	}
	void setDefined() {
		defined = true;
	}
};
FuncList* funcList;
FuncList* funcDefList;


#line 157 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    LPAREN = 259,
    RPAREN = 260,
    SEMICOLON = 261,
    COMMA = 262,
    LCURL = 263,
    RCURL = 264,
    INT = 265,
    FLOAT = 266,
    VOID = 267,
    LSQUARE = 268,
    CONST_INT = 269,
    RSQUARE = 270,
    FOR = 271,
    IF = 272,
    ELSE = 273,
    WHILE = 274,
    PRINTLN = 275,
    RETURN = 276,
    ASSIGNOP = 277,
    LOGICOP = 278,
    RELOP = 279,
    ADDOP = 280,
    MULOP = 281,
    NOT = 282,
    CONST_FLOAT = 283,
    INCOP = 284,
    DECOP = 285,
    BREAK = 286,
    CASE = 287,
    COLON = 288,
    SWITCH = 289,
    DEFAULT = 290,
    LOWER_THAN_ELSE = 291
  };
#endif
/* Tokens.  */
#define ID 258
#define LPAREN 259
#define RPAREN 260
#define SEMICOLON 261
#define COMMA 262
#define LCURL 263
#define RCURL 264
#define INT 265
#define FLOAT 266
#define VOID 267
#define LSQUARE 268
#define CONST_INT 269
#define RSQUARE 270
#define FOR 271
#define IF 272
#define ELSE 273
#define WHILE 274
#define PRINTLN 275
#define RETURN 276
#define ASSIGNOP 277
#define LOGICOP 278
#define RELOP 279
#define ADDOP 280
#define MULOP 281
#define NOT 282
#define CONST_FLOAT 283
#define INCOP 284
#define DECOP 285
#define BREAK 286
#define CASE 287
#define COLON 288
#define SWITCH 289
#define DEFAULT 290
#define LOWER_THAN_ELSE 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 88 "2005031.y"

	SymbolInfo* symbolInfo;

#line 285 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   181

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  129

#define YYUNDEFTOK  2
#define YYMAXUTOK   291


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   102,   102,   110,   116,   123,   128,   134,   141,   198,
     221,   221,   378,   378,   405,   405,   412,   420,   427,   433,
     441,   450,   459,   506,   511,   516,   521,   528,   535,   545,
     550,   560,   565,   573,   578,   583,   583,   591,   602,   611,
     622,   631,   644,   665,   670,   676,   681,   696,   726,   731,
     751,   756,   771,   776,   791,   796,   814,   819,   853,   866,
     878,   885,   890,   950,   957,   962,   967,   973,   981,   986,
     993,  1000
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "LPAREN", "RPAREN", "SEMICOLON",
  "COMMA", "LCURL", "RCURL", "INT", "FLOAT", "VOID", "LSQUARE",
  "CONST_INT", "RSQUARE", "FOR", "IF", "ELSE", "WHILE", "PRINTLN",
  "RETURN", "ASSIGNOP", "LOGICOP", "RELOP", "ADDOP", "MULOP", "NOT",
  "CONST_FLOAT", "INCOP", "DECOP", "BREAK", "CASE", "COLON", "SWITCH",
  "DEFAULT", "LOWER_THAN_ELSE", "$accept", "start", "program", "unit",
  "func_declaration", "func_definition", "$@1", "$@2", "$@3",
  "parameter_list", "compound_statement", "var_declaration",
  "type_specifier", "declaration_list", "statements", "statement", "$@4",
  "expression_statement", "variable", "expression", "logic_expression",
  "rel_expression", "simple_expression", "term", "unary_expression",
  "factor", "argument_list", "arguments", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291
};
# endif

#define YYPACT_NINF (-69)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-36)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      45,   -69,   -69,   -69,    30,    45,   -69,   -69,   -69,   -69,
      16,   -69,   -69,    10,     8,    85,   -69,    53,     6,   -69,
      34,    28,    55,    20,    48,    51,    56,   -69,   -69,    74,
      78,    45,   -69,   -69,    79,    74,    69,   -69,   -69,    74,
      84,    80,   -69,    93,    11,    25,   -69,   -69,   -69,    98,
     100,   106,   108,    25,    25,    25,   -69,   -69,    73,    97,
     -69,    74,   -69,    19,   109,   -69,   104,    95,   112,   -69,
     -69,   -69,   -69,   -69,   -69,    25,    25,   116,   153,    25,
      25,   120,   124,   118,   -69,   -69,   119,   -69,   -69,   -69,
      25,   -69,   -69,   -69,    25,    25,    25,    25,   -69,   129,
     133,   128,   -69,   153,   144,   146,   150,   -69,   -69,   -69,
     135,   112,   -69,   -69,    25,   -69,    25,   125,   125,   152,
     -69,   156,   145,   -69,   -69,   125,   125,   -69,   -69
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    24,    25,    26,     0,     2,     4,     6,     7,     5,
       0,     1,     3,     0,    29,     0,    23,     0,     0,    22,
       0,     0,    12,     0,    19,     0,    27,    14,     9,     0,
      10,     0,    18,    30,     0,     0,     0,    13,     8,     0,
      17,     0,    15,     0,    46,     0,    43,    21,    64,     0,
       0,     0,     0,     0,     0,     0,    65,    33,     0,     0,
      31,     0,    34,    61,     0,    48,    50,    52,    54,    56,
      60,    11,    16,    28,    45,    69,     0,     0,     0,     0,
       0,     0,     0,    61,    58,    59,    29,    20,    32,    36,
       0,    66,    67,    44,     0,     0,     0,     0,    71,     0,
      68,     0,    63,     0,     0,     0,     0,    42,    49,    51,
      53,    55,    57,    62,     0,    47,     0,     0,     0,     0,
      70,     0,    38,    40,    41,     0,     0,    37,    39
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -69,   -69,   -69,   157,   -69,   -69,   -69,   -69,   -69,   -69,
     -29,    13,     9,   -69,   -69,   -58,   -69,   -67,   -52,   -45,
     -68,    70,    71,    72,   -50,   -69,   -69,   -69
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,     8,    39,    29,    35,    23,
      37,    57,    58,    15,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    99,   100
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      77,    88,    83,    83,    84,    85,    42,    98,    82,    10,
      71,   103,    17,     9,    10,    75,    16,    13,     9,    14,
      25,    18,   108,    83,    76,    30,    24,    31,    44,    45,
      11,   101,    89,    27,   104,   105,   116,    26,    83,    48,
      40,    90,    83,    83,    83,    83,   120,   112,    91,    92,
      54,    32,    55,    56,    21,     1,     2,     3,    22,   122,
     123,    28,    83,     1,     2,     3,    33,   127,   128,    34,
      43,   121,    44,    45,    13,    46,    86,   -35,    47,     1,
       2,     3,    36,    48,    38,    49,    50,    72,    51,    52,
      53,    19,    20,    41,    54,    73,    55,    56,    43,    74,
      44,    45,    78,    46,    79,   -35,    87,     1,     2,     3,
      80,    48,    81,    49,    50,    93,    51,    52,    53,    95,
      96,   102,    54,   106,    55,    56,    43,    94,    44,    45,
     107,    46,    18,   -35,   113,     1,     2,     3,    97,    48,
     114,    49,    50,   115,    51,    52,    53,    91,    92,   117,
      54,   118,    55,    56,    43,   119,    44,    45,   124,    46,
      96,   125,    12,   126,   109,     0,   110,    48,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,     0,
      55,    56
};

static const yytype_int8 yycheck[] =
{
      45,    59,    54,    55,    54,    55,    35,    75,    53,     0,
      39,    78,     4,     0,     5,     4,     6,     1,     5,     3,
      14,    13,    90,    75,    13,     5,    17,     7,     3,     4,
       0,    76,    61,     5,    79,    80,   103,     3,    90,    14,
      31,    22,    94,    95,    96,    97,   114,    97,    29,    30,
      25,     3,    27,    28,     1,    10,    11,    12,     5,   117,
     118,     6,   114,    10,    11,    12,    15,   125,   126,    13,
       1,   116,     3,     4,     1,     6,     3,     8,     9,    10,
      11,    12,     8,    14,     6,    16,    17,     3,    19,    20,
      21,     6,     7,    14,    25,    15,    27,    28,     1,     6,
       3,     4,     4,     6,     4,     8,     9,    10,    11,    12,
       4,    14,     4,    16,    17,     6,    19,    20,    21,    24,
      25,     5,    25,     3,    27,    28,     1,    23,     3,     4,
       6,     6,    13,     8,     5,    10,    11,    12,    26,    14,
       7,    16,    17,    15,    19,    20,    21,    29,    30,     5,
      25,     5,    27,    28,     1,     5,     3,     4,     6,     6,
      25,     5,     5,    18,    94,    -1,    95,    14,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    28
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    10,    11,    12,    38,    39,    40,    41,    42,    48,
      49,     0,    40,     1,     3,    50,     6,     4,    13,     6,
       7,     1,     5,    46,    49,    14,     3,     5,     6,    44,
       5,     7,     3,    15,    13,    45,     8,    47,     6,    43,
      49,    14,    47,     1,     3,     4,     6,     9,    14,    16,
      17,    19,    20,    21,    25,    27,    28,    48,    49,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    47,     3,    15,     6,     4,    13,    56,     4,     4,
       4,     4,    56,    55,    61,    61,     3,     9,    52,    47,
      22,    29,    30,     6,    23,    24,    25,    26,    57,    63,
      64,    56,     5,    54,    56,    56,     3,     6,    57,    58,
      59,    60,    61,     5,     7,    15,    54,     5,     5,     5,
      57,    56,    52,    52,     6,     5,    18,    52,    52
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    39,    39,    40,    40,    40,    41,    41,
      43,    42,    44,    42,    45,    42,    46,    46,    46,    46,
      47,    47,    48,    48,    49,    49,    49,    50,    50,    50,
      50,    51,    51,    52,    52,    53,    52,    52,    52,    52,
      52,    52,    52,    54,    54,    54,    55,    55,    56,    56,
      57,    57,    58,    58,    59,    59,    60,    60,    61,    61,
      61,    62,    62,    62,    62,    62,    62,    62,    63,    63,
      64,    64
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       0,     7,     0,     6,     0,     7,     4,     3,     2,     1,
       3,     2,     3,     3,     1,     1,     1,     3,     6,     1,
       4,     1,     2,     1,     1,     0,     2,     7,     5,     7,
       5,     5,     3,     1,     2,     2,     1,     4,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     2,     2,
       1,     1,     4,     3,     1,     1,     2,     2,     1,     0,
       3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

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
#line 102 "2005031.y"
                    {
						fprintf(logout, "start : program \n");
						(yyval.symbolInfo) = new SymbolInfo("start", "program");
						(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
						parseTree = (yyval.symbolInfo);
					}
#line 1566 "y.tab.c"
    break;

  case 3:
#line 110 "2005031.y"
                        {
							fprintf(logout, "program : program unit \n");
							(yyval.symbolInfo) = new SymbolInfo("program", "program unit");
							(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
							(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
						}
#line 1577 "y.tab.c"
    break;

  case 4:
#line 116 "2005031.y"
                            {
							fprintf(logout, "program : unit \n");
							(yyval.symbolInfo) = new SymbolInfo("program", "unit");
							(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
						}
#line 1587 "y.tab.c"
    break;

  case 5:
#line 123 "2005031.y"
                            {
								fprintf(logout, "unit : var_declaration  \n");
								(yyval.symbolInfo) = new SymbolInfo("unit", "var_declaration");
								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
							}
#line 1597 "y.tab.c"
    break;

  case 6:
#line 128 "2005031.y"
                            {
								fprintf(logout, "unit : func_declaration \n");
								(yyval.symbolInfo) = new SymbolInfo("unit", "func_declaration");
								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
								
							}
#line 1608 "y.tab.c"
    break;

  case 7:
#line 134 "2005031.y"
                            {
								fprintf(logout, "unit : func_definition  \n");
								(yyval.symbolInfo) = new SymbolInfo("unit", "func_definition");
								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
							}
#line 1618 "y.tab.c"
    break;

  case 8:
#line 141 "2005031.y"
                                                                            {
																				string type = "";
																				SymbolInfo* tempStack = new SymbolInfo("","");
																				for (SymbolInfo* x = (yyvsp[-2].symbolInfo)->getChildren(); x!=NULL; x=x->getChildren())
																					if(x->getName()=="type_specifier"){
																						tempStack = new SymbolInfo(x->getType(),x->getType(),tempStack);
																						break;
																					}
																					else {
																						string type = x->getNextSymbolInfo()->getNextSymbolInfo()->getType();
																						tempStack = new SymbolInfo(type,type,tempStack);
																					}
																				// for (SymbolInfo* x = $4->getChildren(); x!=NULL; x=x->getChildren())
																				// 	for(SymbolInfo* y=x; y!=NULL; y=y->getNextSymbolInfo())
																				// 		if(y->getName() == "ID") 
																				// 			tempStack = new SymbolInfo(type,type,tempStack);
																				// 		else if (y->getName() == "type_specifier")
																				// 			type = y->getChildren()->getName();

																				funcList = new FuncList((yyvsp[-4].symbolInfo)->getType(), (yyvsp[-5].symbolInfo)->getChildren()->getName(), funcList);
																				for (SymbolInfo* x=tempStack;x->getName()!="";x=x->getNextSymbolInfo())
																					funcList->addArg(x->getName());
																				
																				for(FuncList* x=funcList->next;x!=NULL;x=x->next){
																					if(x->name==funcList->name){
																						SymbolInfo* args1 = funcList->args;
																						SymbolInfo* args2 = x->args;
																						while(args1!=NULL && args2!=NULL) {
																							if(args1->getName()!=args2->getName())
																								break;
																							args1=args1->getNextSymbolInfo();
																							args2=args2->getNextSymbolInfo();
																						}
																						if(args1==NULL && args2==NULL){
																							fprintf(errorout,"Function redeclared\n");
																							FuncList* temp = funcList;
																							funcList = funcList->next;
																							delete temp->args;
																							delete temp;
																							break;
																						}
																					}
																				}
																				delete tempStack;

																				// symboltable insersion of function name
																				table->insert((yyvsp[-4].symbolInfo)->getType(), (yyvsp[-5].symbolInfo)->getChildren()->getName(), true);

																				fprintf(logout, "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON \n");
																				(yyval.symbolInfo) = new SymbolInfo("func_declaration", "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
																				(yyval.symbolInfo)->addChildren((yyvsp[-5].symbolInfo));
																				(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																				(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																				(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																				(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																				(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																			}
#line 1680 "y.tab.c"
    break;

  case 9:
#line 198 "2005031.y"
                                                                                    {
																				funcList = new FuncList((yyvsp[-3].symbolInfo)->getType(), (yyvsp[-4].symbolInfo)->getChildren()->getName(), funcList);
																				for(FuncList* x=funcList->next;x!=NULL;x=x->next)
																					if(x->name==funcList->name && x->args==NULL){
																						fprintf(errorout,"Function redeclared\n");
																							FuncList* temp = funcList;
																							funcList = funcList->next;
																							delete temp;
																							break;
																						}
																				// symboltable insersion of function name
																				table->insert((yyvsp[-3].symbolInfo)->getType(), (yyvsp[-4].symbolInfo)->getChildren()->getName(), true);

																				fprintf(logout, "func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
																				(yyval.symbolInfo) = new SymbolInfo("func_declaration", "type_specifier ID LPAREN RPAREN SEMICOLON");
																				(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																				(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																				(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																				(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																				(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																			}
#line 1706 "y.tab.c"
    break;

  case 10:
#line 221 "2005031.y"
                                                                                                                {
																							// symboltable insersion of arguments
																							table->enterScope();
																							string type = "";
																							SymbolInfo* tempStack = NULL;
																							for (SymbolInfo* x = (yyvsp[-1].symbolInfo)->getChildren(); x!=NULL; x=x->getChildren())
																								if(x->getName()=="type_specifier" && x->getNextSymbolInfo()!=NULL){
																									SymbolInfo* y =x->getNextSymbolInfo();
																									tempStack = new SymbolInfo(y->getType(),x->getType(),tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction());
																									break;
																								}
																								else if(x->getName()=="type_specifier" && x->getNextSymbolInfo()==NULL){
																									SymbolInfo* y =x->getNextSymbolInfo();
																									tempStack = new SymbolInfo("",x->getType(),tempStack,x->getStartLine(),x->getEndLine(),x->getIsFunction());
																									break;
																								}
																								else if(x->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()!=NULL){
																									SymbolInfo* z = x->getNextSymbolInfo()->getNextSymbolInfo();
																									SymbolInfo* y = z->getNextSymbolInfo();
																									tempStack = new SymbolInfo(y->getType(),z->getType(),tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction());
																								}
																								else {
																									SymbolInfo* z = x->getNextSymbolInfo()->getNextSymbolInfo();
																									tempStack = new SymbolInfo("",z->getType(),tempStack,z->getStartLine(),z->getEndLine(),z->getIsFunction());
																									break;
																								}
																							// string type = "";
																							// SymbolInfo* tempStack = NULL;
																							// for (SymbolInfo* x = $4->getChildren(); x!=NULL; x=x->getChildren())
																							// 	for(SymbolInfo* y=x; y!=NULL; y=y->getNextSymbolInfo())
																							// 		if(y->getName() == "ID") 
																							// 			tempStack = new SymbolInfo(y->getType(),type,tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction());
																							// 		else if (y->getName() == "type_specifier")
																							// 			type = y->getChildren()->getName();
																							for (SymbolInfo* x=tempStack;x!=NULL;x=x->getNextSymbolInfo())
																								if(x->getName()!="")
																									if(!table->insert(x->getName(),x->getType())) {
																										fprintf(errorout,"Line# %d: Redefinition of parameter \'%s\'\n",x->getStartLine(),x->getName().c_str());
																										errcount++;
																										break;
																									}
																							
																							// FuncList* f = funcList->findFunc($2->getType());
																							// if(f)
																							// 	if(!f->checkEqArgs(tempStack) || f->type!=$1->getChildren()->getName())
																							// 		fprintf(logout,"FUNCTION %s\n",$1->getChildren()->getName());
																							delete tempStack;
																						}
#line 1759 "y.tab.c"
    break;

  case 11:
#line 270 "2005031.y"
                                                                                                                                                        {
																							table->printAll();
																							table->exitScope();

																							fprintf(logout, "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
																							(yyval.symbolInfo) = new SymbolInfo("func_definition", "type_specifier ID LPAREN parameter_list RPAREN compound_statement");
																							(yyval.symbolInfo)->addChildren((yyvsp[-6].symbolInfo));
																							(yyval.symbolInfo)->addChildren((yyvsp[-5].symbolInfo));
																							(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																							(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																							(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																							(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

																							// check function prototype
																							string type = "";
																							SymbolInfo* tempStack = NULL;
																							for (SymbolInfo* x = (yyvsp[-3].symbolInfo)->getChildren(); x!=NULL; x=x->getChildren())
																								if(x->getName()=="type_specifier" && x->getNextSymbolInfo()!=NULL){
																									SymbolInfo* y =x->getNextSymbolInfo();
																									tempStack = new SymbolInfo(y->getType(),x->getType(),tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction());
																									break;
																								}
																								else if(x->getName()=="type_specifier" && x->getNextSymbolInfo()==NULL){
																									SymbolInfo* y =x->getNextSymbolInfo();
																									tempStack = new SymbolInfo("",x->getType(),tempStack,x->getStartLine(),x->getEndLine(),x->getIsFunction());
																									break;
																								}
																								else if(x->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()!=NULL){
																									SymbolInfo* z = x->getNextSymbolInfo()->getNextSymbolInfo();
																									SymbolInfo* y = z->getNextSymbolInfo();
																									tempStack = new SymbolInfo(y->getType(),z->getType(),tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction());
																								}
																								else {
																									SymbolInfo* z = x->getNextSymbolInfo()->getNextSymbolInfo();
																									tempStack = new SymbolInfo("",z->getType(),tempStack,z->getStartLine(),z->getEndLine(),z->getIsFunction());
																									break;
																								}
																							


																							// funcList = new FuncList($2->getType(), $1->getChildren()->getName(), funcList);
																							// for (SymbolInfo* x=tempStack;x!=NULL;x=x->getNextSymbolInfo())
																							// 	funcList->addArg(x->getName());
																							
																							// FuncList* f=NULL;
																							// for(FuncList* x = funcList; x!=NULL;x=x->next){
																							// 	if(x->name==funcList->name){
																							// 		SymbolInfo* args1 = funcList->args;
																							// 		SymbolInfo* args2 = x->args;
																							// 		while(args1!=NULL && args2!=NULL) {
																							// 			if(args1->getName()!=args2->getName())
																							// 				break;
																							// 			args1=args1->getNextSymbolInfo();
																							// 			args2=args2->getNextSymbolInfo();
																							// 		}
																							// 		if(args1==NULL && args2==NULL){
																							// 			if ($1->getType()==x->type);
																							// 			else if(x->defined){
																							// 				fprintf(errorout,"Function redefined\n");
																							// 				FuncList* temp = funcList;
																							// 				funcList = funcList->next;
																							// 				delete temp->args;
																							// 				delete temp;
																							// 			}
																							// 			else{
																							// 				x->setDefined();
																							// 			}
																							// 			break;
																							// 		}
																							// 	}
																							// }

																							FuncList* f = funcList->findFunc((yyvsp[-5].symbolInfo)->getType());



																							
																							if (f && f->defined==false)
																								f->setDefined();
																							else if (f && f->defined==true)
																								fprintf(errorout,"Redefined a function\n");
																							else {
																								funcList = new FuncList((yyvsp[-5].symbolInfo)->getType(), (yyvsp[-6].symbolInfo)->getChildren()->getName(), funcList, true);
																								for (SymbolInfo* x=tempStack;x!=NULL;x=x->getNextSymbolInfo())
																									funcList->addArg(x->getName());
																							}
																							// symboltable insersion of function name

																							int num_of_args =0;

																							if(!table->insert((yyvsp[-5].symbolInfo)->getType(), (yyvsp[-6].symbolInfo)->getChildren()->getName(), true)){
																								if(f && (!f->checkEqNumArgs(tempStack) || f->type!=(yyvsp[-6].symbolInfo)->getChildren()->getName()))
																									fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",(yyvsp[-5].symbolInfo)->getStartLine(),(yyvsp[-5].symbolInfo)->getType().c_str());
																								else
																									fprintf(errorout,"Line# %d: '%s' redeclared as different kind of symbol\n",(yyvsp[-5].symbolInfo)->getStartLine(),(yyvsp[-5].symbolInfo)->getType().c_str());
																								errcount++;
																							}
																							// if(f)
																							// 	f->checkEqArgs(tempStack);
																							// if(f)
																							// 	if(f->checkEqArgs(tempStack)==false || f->type!=$1->getChildren()->getName())
																							// 		fprintf(logout,"FUNCTION %s %s %s\n",$1->getChildren()->getName().c_str(),f->type.c_str(),$1->getChildren()->getName().c_str());
																							if(f!=NULL && f->type!=(yyvsp[-6].symbolInfo)->getChildren()->getName()) {
																								fprintf(logout,"FUNCTION %s\n",(yyvsp[-6].symbolInfo)->getChildren()->getName().c_str());
																								// fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",$1->getChildren()->getStartLine(),f->name.c_str());
																							}
																							delete tempStack;
																						}
#line 1872 "y.tab.c"
    break;

  case 12:
#line 378 "2005031.y"
                                                  {table->enterScope();}
#line 1878 "y.tab.c"
    break;

  case 13:
#line 378 "2005031.y"
                                                                                                                       {
																							table->printAll();
																							table->exitScope();

																							FuncList* f = funcList->findFunc((yyvsp[-4].symbolInfo)->getType());

																							// symboltable insersion of function name
																							// table->insert($2->getType(), $1->getChildren()->getName(), true);
																							if (f && f->defined==false)
																								f->setDefined();
																							else if (f && f->defined==true)
																								fprintf(errorout,"Redefined a function\n");
																							else 
																								funcList = new FuncList((yyvsp[-4].symbolInfo)->getType(), (yyvsp[-5].symbolInfo)->getChildren()->getName(), funcList, true);

																							fprintf(logout, "func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");
																							(yyval.symbolInfo) = new SymbolInfo("func_definition", "type_specifier ID LPAREN RPAREN compound_statement");
																							(yyval.symbolInfo)->addChildren((yyvsp[-5].symbolInfo));
																							(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																							(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																							(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																							(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

																							// FuncList* f = funcList->findFunc($2->getType());
																							if(f!=NULL && f->type!=(yyvsp[-5].symbolInfo)->getChildren()->getName())
																								fprintf(logout,"FUNCTION %s\n",(yyvsp[-5].symbolInfo)->getChildren()->getName());
																						}
#line 1910 "y.tab.c"
    break;

  case 14:
#line 405 "2005031.y"
                                                                                                                                        {
																							fprintf(errorout,"Line# %d: Syntax error at parameter list of function definition\n",line_count);
																						}
#line 1918 "y.tab.c"
    break;

  case 16:
#line 412 "2005031.y"
                                                            {
																fprintf(logout, "parameter_list  : parameter_list COMMA type_specifier ID\n");
																(yyval.symbolInfo) = new SymbolInfo("parameter_list", "parameter_list COMMA type_specifier ID");
																(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
															}
#line 1931 "y.tab.c"
    break;

  case 17:
#line 420 "2005031.y"
                                                                    {
																fprintf(logout, "parameter_list  : parameter_list COMMA type_specifier\n");
																(yyval.symbolInfo) = new SymbolInfo("parameter_list", "parameter_list COMMA type_specifier");
																(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
															}
#line 1943 "y.tab.c"
    break;

  case 18:
#line 427 "2005031.y"
                                                                    {
																fprintf(logout, "parameter_list  : type_specifier ID\n");
																(yyval.symbolInfo) = new SymbolInfo("parameter_list", "type_specifier ID");
																(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
															}
#line 1954 "y.tab.c"
    break;

  case 19:
#line 433 "2005031.y"
                                                                    {
																fprintf(logout, "parameter_list  : type_specifier\n");
																(yyval.symbolInfo) = new SymbolInfo("parameter_list", "type_specifier");
																(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
															}
#line 1964 "y.tab.c"
    break;

  case 20:
#line 441 "2005031.y"
                                                            {
																fprintf(logout, "compound_statement : LCURL statements RCURL  \n");
																// table->printAll();
																(yyval.symbolInfo) = new SymbolInfo("compound_statement", "LCURL statements RCURL");
																(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																// table->exitScope();
															}
#line 1978 "y.tab.c"
    break;

  case 21:
#line 450 "2005031.y"
                                                                    {
																fprintf(logout, "compound_statement : LCURL RCURL\n");
																// table->printAll();
																(yyval.symbolInfo) = new SymbolInfo("compound_statement", "LCURL RCURL");
																(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
															}
#line 1990 "y.tab.c"
    break;

  case 22:
#line 459 "2005031.y"
                                                            {
																fprintf(logout, "var_declaration : type_specifier declaration_list SEMICOLON  \n");
																(yyval.symbolInfo) = new SymbolInfo("var_declaration", "type_specifier declaration_list SEMICOLON");
																(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

																SymbolInfo* tempStack = NULL;

																// symboltable insersion of variables
																for (SymbolInfo* x = (yyvsp[-1].symbolInfo)->getChildren(); x!=NULL; x=x->getChildren())
																	for(SymbolInfo* y=x; y!=NULL; y=y->getNextSymbolInfo())
																		if(y->getName() == "ID") {
																			if (y->getNextSymbolInfo()!=NULL && y->getNextSymbolInfo()->getName()=="LSQUARE")
																				tempStack = new SymbolInfo(y->getType(),"ARRAY",tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction(), y->getType());
																			else
																				tempStack = new SymbolInfo(y->getType(),(yyvsp[-2].symbolInfo)->getChildren()->getName(),tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction());
																		}
																
																for (SymbolInfo* y = tempStack;y!=NULL; y=y->getNextSymbolInfo()){
																	if((yyvsp[-2].symbolInfo)->getChildren()->getName()=="VOID"){
																		fprintf(errorout,"Line# %d: Variable or field '%s' declared void\n",y->getStartLine(),y->getName().c_str());
																		errcount++;
																	}
																	else if(!table->insert(y->getName(), y->getType())){
																		fprintf(errorout,"Line# %d: Conflicting types for'%s'\n",y->getStartLine(),y->getName().c_str());
																		errcount++;
																	}
																}
																delete tempStack;

																// symboltable insersion of variables
																// for (SymbolInfo* x = $2->getChildren(); x!=NULL; x=x->getChildren())
																// 	for(SymbolInfo* y=x; y!=NULL; y=y->getNextSymbolInfo())
																// 		if(y->getName() == "ID") {
																// 			if($1->getChildren()->getName()=="VOID")
																// 				fprintf(errorout,"Line# %d: Variable or field '%s' declared void\n",y->getStartLine(),y->getType().c_str());
																// 			if (y->getNextSymbolInfo()!=NULL && y->getNextSymbolInfo()->getName()=="LSQUARE"){
																// 				if(!table->insert(y->getType(), "ARRAY"))
																// 					fprintf(errorout,"Line# %d: Conflicting types for'%s'\n",y->getStartLine(),y->getType().c_str());
																// 			}
																// 			else{
																// 				if(!table->insert(y->getType(), $1->getChildren()->getName()))
																// 					fprintf(errorout,"Line# %d: Conflicting types for'%s'\n",y->getStartLine(),y->getType().c_str());
																// 			}
																// 		}
															}
#line 2042 "y.tab.c"
    break;

  case 23:
#line 506 "2005031.y"
                                                                                        {
																fprintf(errorout,"Line# %d: Syntax error at declaration list of variable declaration\n",line_count);
															}
#line 2050 "y.tab.c"
    break;

  case 24:
#line 511 "2005031.y"
                        {
							fprintf(logout, "type_specifier	: INT \n");
							(yyval.symbolInfo) = new SymbolInfo("type_specifier", "INT");
							(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
						}
#line 2060 "y.tab.c"
    break;

  case 25:
#line 516 "2005031.y"
                                        {
							fprintf(logout, "type_specifier	: FLOAT \n"); (yyval.symbolInfo) = (yyvsp[0].symbolInfo);
							(yyval.symbolInfo) = new SymbolInfo("type_specifier", "FLOAT");
							(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
						}
#line 2070 "y.tab.c"
    break;

  case 26:
#line 521 "2005031.y"
                                        {
							fprintf(logout, "type_specifier	: VOID\n"); (yyval.symbolInfo) = (yyvsp[0].symbolInfo);
							(yyval.symbolInfo) = new SymbolInfo("type_specifier", "VOID");
							(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
						}
#line 2080 "y.tab.c"
    break;

  case 27:
#line 528 "2005031.y"
                                                                {
																	fprintf(logout, "declaration_list : declaration_list COMMA ID  \n");
																	(yyval.symbolInfo) = new SymbolInfo("declaration_list", "declaration_list COMMA ID");
																	(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																}
#line 2092 "y.tab.c"
    break;

  case 28:
#line 535 "2005031.y"
                                                                        {
																	fprintf(logout, "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
																	(yyval.symbolInfo) = new SymbolInfo("declaration_list", "declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
																	(yyval.symbolInfo)->addChildren((yyvsp[-5].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																}
#line 2107 "y.tab.c"
    break;

  case 29:
#line 545 "2005031.y"
                                                                        {
																	fprintf(logout, "declaration_list : ID \n");
																	(yyval.symbolInfo) = new SymbolInfo("declaration_list", "ID");
																	(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																}
#line 2117 "y.tab.c"
    break;

  case 30:
#line 550 "2005031.y"
                                                                        {
																	fprintf(logout, "declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
																	(yyval.symbolInfo) = new SymbolInfo("declaration_list", "ID LSQUARE CONST_INT RSQUARE");
																	(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																}
#line 2130 "y.tab.c"
    break;

  case 31:
#line 560 "2005031.y"
                                                                                                {
																	fprintf(logout, "statements : statement  \n");
																	(yyval.symbolInfo) = new SymbolInfo("statements", "statement");
																	(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																}
#line 2140 "y.tab.c"
    break;

  case 32:
#line 565 "2005031.y"
                                                                                                        {
																	fprintf(logout, "statements : statements statement  \n");
																	(yyval.symbolInfo) = new SymbolInfo("statements", "statements statement");
																	(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																	(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																}
#line 2151 "y.tab.c"
    break;

  case 33:
#line 573 "2005031.y"
                                                                                            {
																								fprintf(logout, "statement : var_declaration \n");
																								(yyval.symbolInfo) = new SymbolInfo("statement", "var_declaration");
																								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																							}
#line 2161 "y.tab.c"
    break;

  case 34:
#line 578 "2005031.y"
                                                                                                {
																								fprintf(logout, "statement : expression_statement  \n");
																								(yyval.symbolInfo) = new SymbolInfo("statement", "expression_statement");
																								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																							}
#line 2171 "y.tab.c"
    break;

  case 35:
#line 583 "2005031.y"
            {table->enterScope();}
#line 2177 "y.tab.c"
    break;

  case 36:
#line 584 "2005031.y"
                                                                                                    {
																								fprintf(logout, "statement : compound_statement\n");
																								(yyval.symbolInfo) = new SymbolInfo("statement", "compound_statement");
																								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																								table->printAll();
																								table->exitScope();
																							}
#line 2189 "y.tab.c"
    break;

  case 37:
#line 591 "2005031.y"
                                                                                                {
																								fprintf(logout, "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement\n");
																								(yyval.symbolInfo) = new SymbolInfo("statement", "FOR LPAREN expression_statement expression_statement expression RPAREN statement");
																								(yyval.symbolInfo)->addChildren((yyvsp[-6].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-5].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																							}
#line 2205 "y.tab.c"
    break;

  case 38:
#line 602 "2005031.y"
                                                                                                {
																								fprintf(logout, "statement : IF LPAREN expression RPAREN statement\n");
																								(yyval.symbolInfo) = new SymbolInfo("statement", "IF LPAREN expression RPAREN statement");
																								(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																							}
#line 2219 "y.tab.c"
    break;

  case 39:
#line 611 "2005031.y"
                                                                                                {
																								fprintf(logout, "statement : IF LPAREN expression RPAREN statement ELSE statement\n");
																								(yyval.symbolInfo) = new SymbolInfo("statement", "IF LPAREN expression RPAREN statement ELSE statement");
																								(yyval.symbolInfo)->addChildren((yyvsp[-6].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-5].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																							}
#line 2235 "y.tab.c"
    break;

  case 40:
#line 622 "2005031.y"
                                                                                                {
																								fprintf(logout, "statement : WHILE LPAREN expression RPAREN statement\n");
																								(yyval.symbolInfo) = new SymbolInfo("statement", "WHILE LPAREN expression RPAREN statement");
																								(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																							}
#line 2249 "y.tab.c"
    break;

  case 41:
#line 631 "2005031.y"
                                                                                                {
																								fprintf(logout, "statement : PRINTLN LPAREN ID RPAREN SEMICOLON\n");
																								(yyval.symbolInfo) = new SymbolInfo("statement", "PRINTLN LPAREN ID RPAREN SEMICOLON");
																								(yyval.symbolInfo)->addChildren((yyvsp[-4].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																								if(table->lookupInfo((yyvsp[-2].symbolInfo)->getType())==NULL){
																									fprintf(errorout,"Line# %d: Undeclared variable '%s'\n",(yyvsp[-2].symbolInfo)->getStartLine(),(yyvsp[-2].symbolInfo)->getType().c_str());
																									errcount++;
																								}
																							}
#line 2267 "y.tab.c"
    break;

  case 42:
#line 644 "2005031.y"
                                                                                                {
																								fprintf(logout, "statement : RETURN expression SEMICOLON\n");
																								(yyval.symbolInfo) = new SymbolInfo("statement", "RETURN expression SEMICOLON");
																								(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
																								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
																							}
#line 2279 "y.tab.c"
    break;

  case 43:
#line 665 "2005031.y"
                                                        {
												fprintf(logout, "expression_statement : SEMICOLON			\n");
												(yyval.symbolInfo) = new SymbolInfo("expression_statement", "SEMICOLON");
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
											}
#line 2289 "y.tab.c"
    break;

  case 44:
#line 670 "2005031.y"
                                                        {
												fprintf(logout, "expression_statement : expression SEMICOLON 		 \n");
												(yyval.symbolInfo) = new SymbolInfo("expression_statement", "expression SEMICOLON");
												(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
											}
#line 2300 "y.tab.c"
    break;

  case 45:
#line 676 "2005031.y"
                                                                        {
												fprintf(errorout,"Line# %d: Syntax error at expression of expression statement\n",line_count);
											}
#line 2308 "y.tab.c"
    break;

  case 46:
#line 681 "2005031.y"
                                                {
												fprintf(logout, "variable : ID 	 \n");
												SymbolInfo* symbolInfo = table->lookupInfo((yyvsp[0].symbolInfo)->getType());
												string type = "INT";
												if(symbolInfo)
													type = symbolInfo->getType();
												(yyval.symbolInfo) = new SymbolInfo("variable", "ID",NULL,INT_MAX,-1,false,type);
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

												// error
												if(table->lookupInfo((yyvsp[0].symbolInfo)->getType())==NULL){
													fprintf(errorout,"Line# %d: Undeclared variable '%s'\n",(yyvsp[0].symbolInfo)->getStartLine(),(yyvsp[0].symbolInfo)->getType().c_str());
													errcount++;
												}
											}
#line 2328 "y.tab.c"
    break;

  case 47:
#line 696 "2005031.y"
                                                {
												SymbolInfo* symbolInfo = table->lookupInfo((yyvsp[-3].symbolInfo)->getType());
												string type = "INT";
												if(symbolInfo)
													type = symbolInfo->getExpType();

												fprintf(logout, "variable : ID LSQUARE expression RSQUARE  	 \n");
												(yyval.symbolInfo) = new SymbolInfo("variable", "ID LSQUARE expression RSQUARE",NULL,INT_MAX,-1,false,type);
												(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
												(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
												(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

												// error
												SymbolInfo* symbolInfo2 = table->lookupInfo((yyvsp[-3].symbolInfo)->getType());
												if(symbolInfo2==NULL){
													fprintf(errorout,"Line# %d: Undeclared variable '%s'\n",(yyvsp[-3].symbolInfo)->getStartLine(),(yyvsp[-3].symbolInfo)->getType().c_str());
													errcount++;
												}
												else if (symbolInfo2->getType()!="ARRAY"){
													fprintf(errorout,"Line# %d: '%s' is not an array\n",(yyvsp[-3].symbolInfo)->getStartLine(),(yyvsp[-3].symbolInfo)->getType().c_str());
													errcount++;
												}
												if ((yyvsp[-1].symbolInfo)->getExpType()!="INT"){
													fprintf(errorout,"Line# %d: Array subscript is not an integer\n",(yyvsp[-1].symbolInfo)->getStartLine());
													errcount++;
												}
											}
#line 2361 "y.tab.c"
    break;

  case 48:
#line 726 "2005031.y"
                                            {
												fprintf(logout, "expression 	: logic_expression	 \n");
												(yyval.symbolInfo) = new SymbolInfo("expression", "logic_expression",NULL,INT_MAX,-1,false,(yyvsp[0].symbolInfo)->getExpType());
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
											}
#line 2371 "y.tab.c"
    break;

  case 49:
#line 731 "2005031.y"
                                                        {
													fprintf(logout, "expression 	: variable ASSIGNOP logic_expression 		 \n");
													(yyval.symbolInfo) = new SymbolInfo("expression", "variable ASSIGNOP logic_expression",NULL,INT_MAX,-1,false,(yyvsp[-2].symbolInfo)->getExpType());
													(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
													(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
													(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

													// error
													if((yyvsp[-2].symbolInfo)->getExpType()=="VOID"||(yyvsp[0].symbolInfo)->getExpType()=="VOID"){
														fprintf(errorout,"Line# %d: Void cannot be used in expression \n",(yyvsp[-1].symbolInfo)->getStartLine());
														errcount++;
													}
													
													if(((yyvsp[-2].symbolInfo)->getExpType()=="FLOAT"&&(yyvsp[0].symbolInfo)->getExpType()=="INT")||((yyvsp[-2].symbolInfo)->getExpType()=="INT"&&(yyvsp[0].symbolInfo)->getExpType()=="FLOAT")){
														fprintf(errorout,"Line# %d: Warning: possible loss of data in assignment of FLOAT to INT\n",(yyvsp[-1].symbolInfo)->getStartLine());
														errcount++;
													}
												}
#line 2394 "y.tab.c"
    break;

  case 50:
#line 751 "2005031.y"
                                                        {
														fprintf(logout, "logic_expression : rel_expression 	 \n");
														(yyval.symbolInfo) = new SymbolInfo("logic_expression", "rel_expression",NULL,INT_MAX,-1,false,(yyvsp[0].symbolInfo)->getExpType());
														(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
													}
#line 2404 "y.tab.c"
    break;

  case 51:
#line 756 "2005031.y"
                                                                {
														fprintf(logout, "logic_expression : rel_expression LOGICOP rel_expression 	 	 \n");
														(yyval.symbolInfo) = new SymbolInfo("logic_expression", "rel_expression LOGICOP rel_expression",NULL,INT_MAX,-1,false,"INT");
														(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
														(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
														(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

														// error
														if((yyvsp[-2].symbolInfo)->getExpType()=="VOID"||(yyvsp[0].symbolInfo)->getExpType()=="VOID"){
															fprintf(errorout,"Line# %d: Void cannot be used in expression \n",(yyvsp[-1].symbolInfo)->getStartLine());
															errcount++;
														}
													}
#line 2422 "y.tab.c"
    break;

  case 52:
#line 771 "2005031.y"
                                                        {
															fprintf(logout, "rel_expression	: simple_expression \n");
															(yyval.symbolInfo) = new SymbolInfo("rel_expression", "simple_expression",NULL,INT_MAX,-1,false,(yyvsp[0].symbolInfo)->getExpType());
															(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
														}
#line 2432 "y.tab.c"
    break;

  case 53:
#line 776 "2005031.y"
                                                                    {
															fprintf(logout, "rel_expression	: simple_expression RELOP simple_expression	  \n");
															(yyval.symbolInfo) = new SymbolInfo("rel_expression", "simple_expression RELOP simple_expression",NULL,INT_MAX,-1,false,"INT");
															(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
															(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
															(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

															// error
															if((yyvsp[-2].symbolInfo)->getExpType()=="VOID"||(yyvsp[0].symbolInfo)->getExpType()=="VOID"){
																fprintf(errorout,"Line# %d: Void cannot be used in expression \n",(yyvsp[-1].symbolInfo)->getStartLine());
																errcount++;
															}
														}
#line 2450 "y.tab.c"
    break;

  case 54:
#line 791 "2005031.y"
                                            {
												fprintf(logout, "simple_expression : term \n");
												(yyval.symbolInfo) = new SymbolInfo("simple_expression", "term",NULL,INT_MAX,-1,false,(yyvsp[0].symbolInfo)->getExpType());
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
											}
#line 2460 "y.tab.c"
    break;

  case 55:
#line 796 "2005031.y"
                                                    {
												string type = "INT";
												if((yyvsp[-2].symbolInfo)->getExpType()=="FLOAT" || (yyvsp[0].symbolInfo)->getExpType()=="FLOAT")
													type = "FLOAT";
												fprintf(logout, "simple_expression : simple_expression ADDOP term  \n");
												(yyval.symbolInfo) = new SymbolInfo("simple_expression", "simple_expression ADDOP term",NULL,INT_MAX,-1,false,type);
												(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
												(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

												// error
												if((yyvsp[-2].symbolInfo)->getExpType()=="VOID"||(yyvsp[0].symbolInfo)->getExpType()=="VOID"){
													fprintf(errorout,"Line# %d: Void cannot be used in expression \n",(yyvsp[-1].symbolInfo)->getStartLine());
													errcount++;
												}
											}
#line 2481 "y.tab.c"
    break;

  case 56:
#line 814 "2005031.y"
                                        {
											fprintf(logout, "term :	unary_expression \n");
											(yyval.symbolInfo) = new SymbolInfo("term", "unary_expression",NULL,INT_MAX,-1,false,(yyvsp[0].symbolInfo)->getExpType());
											(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
										}
#line 2491 "y.tab.c"
    break;

  case 57:
#line 819 "2005031.y"
                                        {
											fprintf(logout, "term :	term MULOP unary_expression \n");
											string type = "INT";
											if((yyvsp[-2].symbolInfo)->getExpType()=="FLOAT" || (yyvsp[0].symbolInfo)->getExpType()=="FLOAT")
												type = "FLOAT";
											if((yyvsp[-1].symbolInfo)->getType()=="%")
												type = "INT";
											(yyval.symbolInfo) = new SymbolInfo("term", "term MULOP unary_expression",NULL,INT_MAX,-1,false,type);
											(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
											(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
											(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

											// error
											if((yyvsp[-2].symbolInfo)->getExpType()=="VOID"||(yyvsp[0].symbolInfo)->getExpType()=="VOID"){
												fprintf(errorout,"Line# %d: Void cannot be used in expression \n",(yyvsp[-1].symbolInfo)->getStartLine());
												errcount++;
											}
											
											else if((yyvsp[-1].symbolInfo)->getType()=="%"&&((yyvsp[-2].symbolInfo)->getExpType()!="INT"||(yyvsp[0].symbolInfo)->getExpType()!="INT")){
												fprintf(errorout,"Line# %d: Operands of modulus must be integers \n",(yyvsp[-1].symbolInfo)->getStartLine());
												errcount++;
											}

											SymbolInfo* zornot = (yyvsp[0].symbolInfo)->getChildren();
											if(zornot->getName()=="factor"){
												zornot = zornot->getChildren();
												if(zornot->getName()=="CONST_INT"&&stoi(zornot->getType())==0){
													fprintf(errorout,"Line# %d: Warning: division by zero i=0f=1Const=0\n",(yyvsp[-1].symbolInfo)->getStartLine());
													errcount++;
												}
											}
										}
#line 2528 "y.tab.c"
    break;

  case 58:
#line 853 "2005031.y"
                                            {
												// error
												if((yyvsp[0].symbolInfo)->getExpType()=="VOID"){
													fprintf(errorout,"Line# %d: Void cannot be used in expression \n",(yyvsp[0].symbolInfo)->getStartLine());
													errcount++;
												}


												fprintf(logout, "unary_expression : ADDOP unary_expression  \n");
												(yyval.symbolInfo) = new SymbolInfo("unary_expression", "ADDOP unary_expression",NULL,INT_MAX,-1,false,(yyvsp[0].symbolInfo)->getExpType());
												(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
											}
#line 2546 "y.tab.c"
    break;

  case 59:
#line 866 "2005031.y"
                                                    {
												fprintf(logout, "unary_expression : NOT unary_expression \n");
												(yyval.symbolInfo) = new SymbolInfo("unary_expression", "NOT unary_expression",NULL,INT_MAX,-1,false,"INT");
												(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

												// error
												if((yyvsp[0].symbolInfo)->getExpType()=="VOID"){
													fprintf(errorout,"Line# %d: Void cannot be used in expression \n",(yyvsp[0].symbolInfo)->getStartLine());
													errcount++;
												}
											}
#line 2563 "y.tab.c"
    break;

  case 60:
#line 878 "2005031.y"
                                                    {
												fprintf(logout, "unary_expression : factor \n");
												(yyval.symbolInfo) = new SymbolInfo("unary_expression", "factor",NULL,INT_MAX,-1,false,(yyvsp[0].symbolInfo)->getExpType());
												(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
											}
#line 2573 "y.tab.c"
    break;

  case 61:
#line 885 "2005031.y"
                                        {
											fprintf(logout, "factor	: variable \n");
											(yyval.symbolInfo) = new SymbolInfo("factor", "variable",NULL,INT_MAX,-1,false,(yyvsp[0].symbolInfo)->getExpType());
											(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
										}
#line 2583 "y.tab.c"
    break;

  case 62:
#line 890 "2005031.y"
                                            {
											SymbolInfo* symbolInfo = table->lookupInfo((yyvsp[-3].symbolInfo)->getType());
											string type = "INT";
											if(symbolInfo)
												type = symbolInfo->getType();

											fprintf(logout, "factor	: ID LPAREN argument_list RPAREN  \n");
											(yyval.symbolInfo) = new SymbolInfo("factor", "ID LPAREN argument_list RPAREN",NULL,INT_MAX,-1,false,type);
											(yyval.symbolInfo)->addChildren((yyvsp[-3].symbolInfo));
											(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
											(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
											(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));

											// error
											if(table->lookupInfo((yyvsp[-3].symbolInfo)->getType())==NULL){
												fprintf(errorout,"Line# %d: Undeclared function '%s'\n",(yyvsp[-3].symbolInfo)->getStartLine(),(yyvsp[-3].symbolInfo)->getType().c_str());
												errcount++;
											}

											// error
											FuncList* func = funcList->findFunc((yyvsp[-3].symbolInfo)->getType());
											if (func) {

												FuncList* func = funcList->findFunc((yyvsp[-3].symbolInfo)->getType());
												SymbolInfo* tempStack = NULL;
												for (SymbolInfo* x = (yyvsp[-1].symbolInfo)->getChildren(); x!=NULL; x=x->getChildren()) {
													if(x->getName()=="arguments"&&x->getChildren()!=NULL&&x->getChildren()->getNextSymbolInfo()==NULL) {
														SymbolInfo* y = x->getChildren();
														tempStack = new SymbolInfo(y->getType(),y->getType(),tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction(),y->getExpType());
													}
													else if(x->getName()=="arguments"&&x->getChildren()!=NULL&&x->getChildren()->getNextSymbolInfo()!=NULL&&x->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()!=NULL) {
														SymbolInfo* y = x->getChildren()->getNextSymbolInfo()->getNextSymbolInfo();
														tempStack = new SymbolInfo(y->getType(),y->getType(),tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction(),y->getExpType());
													}
												}
												
												SymbolInfo* arg1 = func->args;
												SymbolInfo* arg2 = tempStack;
												int count = 1;
												while(arg1 != NULL && arg2 != NULL) {
													if(arg1->getExpType()!=arg2->getExpType()){
														fprintf(errorout,"Line# %d: Type mismatch for argument %d of '%s'\n",arg2->getStartLine(),count,(yyvsp[-3].symbolInfo)->getType().c_str());
														errcount++;
													}
													arg1 = arg1->getNextSymbolInfo();
													arg2 = arg2->getNextSymbolInfo();
													count++;
												}
												if(arg1!=NULL){
													fprintf(errorout,"Line# %d: Too few arguments to function '%s'\n",(yyvsp[-3].symbolInfo)->getStartLine(),(yyvsp[-3].symbolInfo)->getType().c_str());
													errcount++;
												}
												else if(arg2!=NULL){
													fprintf(errorout,"Line# %d: Too many arguments to function '%s'\n",(yyvsp[-3].symbolInfo)->getStartLine(),(yyvsp[-3].symbolInfo)->getType().c_str());
													errcount++;
												}
												delete tempStack;

											}
										}
#line 2648 "y.tab.c"
    break;

  case 63:
#line 950 "2005031.y"
                                            {
											fprintf(logout, "factor	: LPAREN expression RPAREN   \n");
											(yyval.symbolInfo) = new SymbolInfo("factor", "LPAREN expression RPAREN",NULL,INT_MAX,-1,false,(yyvsp[-1].symbolInfo)->getExpType());
											(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
											(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
											(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
										}
#line 2660 "y.tab.c"
    break;

  case 64:
#line 957 "2005031.y"
                                            {
											fprintf(logout, "factor	: CONST_INT   \n");
											(yyval.symbolInfo) = new SymbolInfo("factor", "CONST_INT",NULL,INT_MAX,-1,false,"INT");
											(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
										}
#line 2670 "y.tab.c"
    break;

  case 65:
#line 962 "2005031.y"
                                            {
											fprintf(logout, "factor	: CONST_FLOAT   \n");
											(yyval.symbolInfo) = new SymbolInfo("factor", "CONST_FLOAT",NULL,INT_MAX,-1,false,"FLOAT");
											(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
										}
#line 2680 "y.tab.c"
    break;

  case 66:
#line 967 "2005031.y"
                                            {
											fprintf(logout, "factor	: variable INCOP \n");
											(yyval.symbolInfo) = new SymbolInfo("factor", "variable INCOP",NULL,INT_MAX,-1,false,(yyvsp[-1].symbolInfo)->getExpType());
											(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
											(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
										}
#line 2691 "y.tab.c"
    break;

  case 67:
#line 973 "2005031.y"
                                            {
											fprintf(logout, "factor	: variable DECOP\n");
											(yyval.symbolInfo) = new SymbolInfo("factor", "variable DECOP",NULL,INT_MAX,-1,false,(yyvsp[-1].symbolInfo)->getExpType());
											(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
											(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
										}
#line 2702 "y.tab.c"
    break;

  case 68:
#line 981 "2005031.y"
                            {
								fprintf(logout, "argument_list : arguments  \n");
								(yyval.symbolInfo) = new SymbolInfo("argument_list", "arguments");
								(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
							}
#line 2712 "y.tab.c"
    break;

  case 69:
#line 986 "2005031.y"
                                        {
								fprintf(logout, "argument_list : \n");
								(yyval.symbolInfo) = new SymbolInfo("argument_list", "");
								// $$->addChildren(new SymbolInfo ("", ""));
							}
#line 2722 "y.tab.c"
    break;

  case 70:
#line 993 "2005031.y"
                                                {
													fprintf(logout, "arguments : arguments COMMA logic_expression \n");
													(yyval.symbolInfo) = new SymbolInfo("arguments", "arguments COMMA logic_expression");
													(yyval.symbolInfo)->addChildren((yyvsp[-2].symbolInfo));
													(yyval.symbolInfo)->addChildren((yyvsp[-1].symbolInfo));
													(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
												}
#line 2734 "y.tab.c"
    break;

  case 71:
#line 1000 "2005031.y"
                                                    {
													fprintf(logout, "arguments : logic_expression\n");
													(yyval.symbolInfo) = new SymbolInfo("arguments", "logic_expression");
													(yyval.symbolInfo)->addChildren((yyvsp[0].symbolInfo));
												}
#line 2744 "y.tab.c"
    break;


#line 2748 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
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
  return yyresult;
}
#line 1008 "2005031.y"

int main(int argc,char *argv[])
{
    FILE *fp, *fp2, *fp3, *fp4;

	if((fp=fopen("input.txt","r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	/* fp2= fopen(argv[2],"w");
	fclose(fp2);
	fp3= fopen(argv[3],"w");
	fclose(fp3); */
	
	fp2= fopen("log.txt","w");
	fp3= fopen("error.txt","w");
	fp4= fopen("parsetree.txt","w");
	table = new SymbolTable(11,fp2);
	
    logout = fp2;
	errorout = fp3;
	treeout = fp4;

	yyin=fp;
	yyparse();
	fprintf(logout,"Total Lines: %d\n", line_count);
	fprintf(logout,"Total Errors: %d\n", errcount);
	parseTree->print(treeout);
	

	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	delete table;
	
	return 0;
}

