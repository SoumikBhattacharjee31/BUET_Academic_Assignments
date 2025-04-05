%{
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include "2005031.h"
#include "y.tab.h"
using namespace std;

extern YYSTYPE yylval;
extern SymbolTable *table;
extern FILE *logout;
void yyerror(char *);

// SymbolTable* symbolTable;
int line_count = 1;
int error = 0;
int totalWarning = 0;
int scopeCount = 0;
int lineStart = 1;
int messedTab = 0;
// FILE *tokenout;
string comment;
string replaceEscape (string s) {
    string res = "";
    for(int i=0;i<s.size();i++) {
        char c = s[i];
        if (s[i]=='\\') {
            switch (s[i+1]) {
                case '\'':
                    c = '\'';
                    break;
                case '\"':
                    c = '\"';
                    break;
                case 'n':
                    c = '\n';
                    break;
                case 't':
                    c = '\t';
                    break;
                case '\\':
                    c = '\\';
                    break;
                case '/':
                    c = '/';
                    break;
                case 'a':
                    c = '\a';
                    break;
                case 'f':
                    c = '\f';
                    break;
                case 'r':
                    c = '\r';
                    break;
                case 'b':
                    c = '\b';
                    break;
                case 'v':
                    c = '\v';
                    break;
                case '0':
                    c = '\0';
                    break;
                default:
                    c = s[i];
                    break;
            }
            i++;
        }
        res += c;
    }
    return res;
    }
void checkGlobal () {
    if(lineStart==1 && scopeCount!=0){
        totalWarning++;
        fprintf(logout, "Line# %d: warning, %d of tabs needed but got %d tabs.\n",line_count, scopeCount, 0);
    }
}
%}

Exp [E]
Point [.]
Quote ["]
Digit [0-9]
Apostrophe [\']
Underscore [_]
Letter [a-zA-Z]
Digits {Digit}+
Digit_s {Digit}*
Plus_Minus_ [+-]?
Letters {Letter}+
AlphaNumeric {Letter}|{Digit}
Exp_Term {Exp}{Plus_Minus_}{Digits}
Identifier ({Letter}|{Underscore})({AlphaNumeric}|{Underscore})*
Integer {Digits}
Float ({Digit_s}{Point}{Digits}{Exp_Term}?)|({Digits}{Exp_Term})
CharLiteral {Apostrophe}([^\'\"\n\t\\\/\a\f\r\b\v\0]|(\\['"nt\\\/afrbv0])){Apostrophe}
StrLiteral {Quote}([^\"\r\n]|(\\["]))*{Quote}
StrMulti {Quote}[^\"\r\n]*([\\][\r]?[\n][^\"\r\n]*)+{Quote}
SingleComment [/][/][^\r\n]*([\\][\r]?[\n][^\r\n]*)*
FloatErr {Digit_s}[\.]{Digit_s}([\.]{Digit_s})+{Exp_Term}?
ExpErr {Digit_s}({Point}{Digit_s})?{Exp}{Plus_Minus_}{Digit_s}({Point}{Digit_s})+
IllSuffix {Digits}{Letters}
MultiChar {Apostrophe}({AlphaNumeric}|[ ])({AlphaNumeric}|[ ])+{Apostrophe}
UnfinishedChar ({Apostrophe}([^\'\"\n\t\\\/\a\f\r\b\v\0]|(\\['"nt\\\/afrbv0]))?)|\'
UnknownChar [^a-zA-Z0-9\n\r\t'"]|(\'\\[^'"nt\\\/afrbv0]\')
EmptyChar ''
UnfinishedStr {Quote}[^\"\r\n]*([\\][\r]?[\n][^\"\r\n]*)*
%x MULTICOMMENT
%s COUNTTAB


%%
<COUNTTAB>[\t ]*[}]   {
                // symbolTable->exitScope();
                // table->exitScope();
                scopeCount--;
                int tabCount = 0;
                for(int i=0;yytext[i]!=0;i++)
                    if(yytext[i]=='\t')
                        tabCount++;
                    else if (yytext[i]==' ')
                        messedTab=1;
                if(lineStart)
                if (tabCount != scopeCount) {
                    fprintf(logout, "Line# %d: warning, %d of tabs needed but got %d tabs.\n",line_count, scopeCount, tabCount);
                    totalWarning++;
                }
                else if (messedTab) {
                    fprintf(logout, "Line# %d: Warning, tab requrired but got space.\n",line_count);
                    totalWarning++;
                }
                // fprintf(tokenout,"<RCURL, }>\n");
                fprintf(logout,"Line# %d: Token <RCURL> Lexeme } found\n",line_count);
                tabCount = 0;
                messedTab = 0;
                BEGIN INITIAL;
                lineStart = 0;
                // table->printAll();
                SymbolInfo* symbolInfo =new SymbolInfo("RCURL", "}", NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return RCURL;
            }
<COUNTTAB>[\t ]*  {
                int tabCount = 0;
                for(int i=0;yytext[i]!=0;i++)
                    if(yytext[i]=='\t')
                        tabCount++;
                    else if (yytext[i]==' ')
                        messedTab=1;
                if(lineStart)
                if (tabCount != scopeCount) {
                    fprintf(logout, "Line# %d: warning, %d of tabs needed but got %d tabs.\n",line_count, scopeCount, tabCount);
                    totalWarning++;
                }
                else if (messedTab) {
                    fprintf(logout, "Line# %d: Warning, tab requrired but got space.\n",line_count);
                    totalWarning++;
                }
                tabCount = 0;
                messedTab = 0;
                BEGIN INITIAL;
                lineStart = 0;
}
[\r]?[\n]  {
                    line_count++;
                    lineStart=1;
                    BEGIN COUNTTAB;
            }
"/*"        {
                BEGIN MULTICOMMENT;
                comment = "/*";
                checkGlobal();
                lineStart = 0;
            }
<MULTICOMMENT>"*/"        {
                BEGIN INITIAL;
                comment += "*/";
                fprintf(logout,"Line# %d: Token <MULTI LINE COMMENT> Lexeme %s found\n",line_count,comment.c_str());
                for (char x:comment)
                    if (x=='\n')
                        line_count++;
                comment = "";
                lineStart = 0;
            }
<MULTICOMMENT>[^0]|[0]    {
                comment += yytext[0];
                lineStart = 0;
            }
<MULTICOMMENT><<EOF>> {
                BEGIN INITIAL;
                for (char x:comment)
                    if (x=='\n')
                        line_count++;
                fprintf(logout,"Error at line# %d: UNFINISHED_COMMENT %s\n",line_count,comment.c_str());
                comment = "";
                error++;
                lineStart = 0;
}

"case"      {return CASE;}
":"         {return COLON;}
"break"     {return BREAK;}
"switch"    {return SWITCH;}
"default"    {return DEFAULT;}

"if"       {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                checkGlobal();
                fprintf(logout,"Line# %d: Token <IF> Lexeme if found\n",line_count);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("IF", "if", NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return IF;
            }
"for"       {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                checkGlobal();
                fprintf(logout,"Line# %d: Token <FOR> Lexeme for found\n",line_count);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("FOR", "for", NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return FOR;
            }
"int"       {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                checkGlobal();
                fprintf(logout,"Line# %d: Token <INT> Lexeme int found\n",line_count);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("INT", "int", NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return INT;
            }
"float"       {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                checkGlobal();
                fprintf(logout,"Line# %d: Token <FLOAT> Lexeme float found\n",line_count);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("FLOAT", "float", NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return FLOAT;
            }
"void"       {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                checkGlobal();
                fprintf(logout,"Line# %d: Token <VOID> Lexeme void found\n",line_count);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("VOID", "void", NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return VOID;
            }
"else"       {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                checkGlobal();
                fprintf(logout,"Line# %d: Token <ELSE> Lexeme else found\n",line_count);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("ELSE", "else", NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return ELSE;
            }
"while"       {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                checkGlobal();
                fprintf(logout,"Line# %d: Token <WHILE> Lexeme while found\n",line_count);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("WHILE", "while", NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return WHILE;
            }
"return"       {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                checkGlobal();
                fprintf(logout,"Line# %d: Token <RETURN> Lexeme return found\n",line_count);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("RETURN", "return", NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return RETURN;
            }

{Integer}   {
                checkGlobal();
                // fprintf(tokenout,"<CONST_INT, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <CONST_INT> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                // SymbolInfo *s = new SymbolInfo(yytext, (char*)"CONST_INT");
                // yylval = (YYSTYPE)s;
                SymbolInfo* symbolInfo =new SymbolInfo("CONST_INT", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return CONST_INT;
            }
{Float}     {
                checkGlobal();
                // fprintf(tokenout,"<CONST_FLOAT, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <CONST_FLOAT> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                // SymbolInfo *s= new  SymbolInfo(yytext, (char *)"CONST_FLOAT");
				// yylval = (YYSTYPE)s;
                SymbolInfo* symbolInfo =new SymbolInfo("CONST_FLOAT", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return CONST_FLOAT;
            }
{CharLiteral}     {
    // \'\"\n\t\\\/\a\f\r\b\v\0
                string ss = replaceEscape(yytext);
                checkGlobal();
                // fprintf(tokenout,"<CONST_CHAR, %c>\n",s[1]);
                fprintf(logout,"Line# %d: Token <CONST_CHAR> Lexeme %c found\n",line_count,ss[1]);
                lineStart = 0;
                // SymbolInfo *s= new  SymbolInfo(yytext, (char *)"RELOP");
                // yylval = (YYSTYPE)s;
                // return CONST_CHAR;
            }
[+-]        {
                checkGlobal();
                // fprintf(tokenout,"<ADDOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <ADDOP> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                // SymbolInfo *s= new  SymbolInfo(yytext, (char *)"ADDOP");
                // yylval = (YYSTYPE)s;
                SymbolInfo* symbolInfo =new SymbolInfo("ADDOP", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return ADDOP;
            }
[*/%]       {
                checkGlobal();
                // fprintf(tokenout,"<MULOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <MULOP> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                // SymbolInfo *s= new  SymbolInfo(yytext, (char *)"MULOP");
                // yylval = (YYSTYPE)s;
                SymbolInfo* symbolInfo =new SymbolInfo("MULOP", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return MULOP;
            }
"++"        {
                checkGlobal();
                // fprintf(tokenout,"<INCOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <INCOP> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("INCOP", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return INCOP;
            }
"--"        {
                checkGlobal();
                // fprintf(tokenout,"<INCOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <INCOP> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("INCOP", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return DECOP;
            }
"<"|"<="|">"|">="|"=="|"!=" {
                checkGlobal();
                // fprintf(tokenout,"<RELOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <RELOP> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                // SymbolInfo *s= new  SymbolInfo(yytext, (char *)"RELOP");
                // yylval = (YYSTYPE)s;
                SymbolInfo* symbolInfo =new SymbolInfo("RELOP", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return RELOP;
            }
"="         {
                checkGlobal();
                // fprintf(tokenout,"<ASSIGNOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <ASSIGNOP> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("ASSIGNOP", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return ASSIGNOP;
            }
"&&"|"||"   {
                checkGlobal();
                // fprintf(tokenout,"<LOGICOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <LOGICOP> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                // SymbolInfo *s= new  SymbolInfo(yytext, (char *)"LOGICOP");
                // yylval = (YYSTYPE)s;
                SymbolInfo* symbolInfo =new SymbolInfo("LOGICOP", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return LOGICOP;
            }
"&"|"|"|"^"|"<<"|">>"   {
                checkGlobal();
                // fprintf(tokenout,"<BITOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <BITOP> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                // SymbolInfo *s= new  SymbolInfo(yytext, (char *)"BITOP");
                // yylval = (YYSTYPE)s;
                // return BITOP;
            }
"!"         {
                checkGlobal();
                // fprintf(tokenout,"<NOT, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <NOT> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("NOT", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return NOT;
            }
"\("         {
                checkGlobal();
                // fprintf(tokenout,"<LPAREN, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <LPAREN> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("LPAREN", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return LPAREN;
            }
"\)"         {
                checkGlobal();
                // fprintf(tokenout,"<RPAREN, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <RPAREN> Lexeme %s found\n",line_count,yytext);
                SymbolInfo* symbolInfo =new SymbolInfo("RPAREN", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                lineStart = 0;
                return RPAREN;
            }
"\{"         {
                checkGlobal();
                // fprintf(tokenout,"<LCURL, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <LCURL> Lexeme %s found\n",line_count,yytext);
                // symbolTable->enterScope();
                // table->enterScope();
                scopeCount++;
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("LCURL", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return LCURL;
            }
[}]         {
                // symbolTable->exitScope();
                // table->exitScope();
                scopeCount--;
                checkGlobal();
                // fprintf(tokenout,"<RCURL, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <RCURL> Lexeme %s found\n",line_count,yytext);
                // cout<<"yes";
                lineStart = 0;
                // table->printAll();
                SymbolInfo* symbolInfo =new SymbolInfo("RCURL", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return RCURL;
            }
"\["         {
                checkGlobal();
                // fprintf(tokenout,"<LSQUARE, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <LSQUARE> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("LSQUARE", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return LSQUARE;
            }
"\]"         {
                checkGlobal();
                // fprintf(tokenout,"<RSQUARE, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <RSQUARE> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("RSQUARE", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return RSQUARE;
            }
","         {
                checkGlobal();
                // fprintf(tokenout,"<COMMA, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <COMMA> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("COMMA", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return COMMA;
            }
";"         {
                checkGlobal();
                // fprintf(tokenout,"<SEMICOLON, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <SEMICOLON> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                SymbolInfo* symbolInfo =new SymbolInfo("SEMICOLON", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return SEMICOLON;
            }
{Identifier}    {
                checkGlobal();
                // fprintf(tokenout,"<ID, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <ID> Lexeme %s found\n",line_count,yytext);
                // bool inserted = table->insert(yytext,"ID");
                // if (inserted)
                //     symbolTable->printAll();
                lineStart = 0;
                // SymbolInfo *s= table->lookupInfo(yytext);
				// if(s==NULL)
				// {					
					// s= new SymbolInfo(yytext, (char *)"ID");
				// }
				// yylval = (YYSTYPE)s;
                SymbolInfo* symbolInfo =new SymbolInfo("ID", yytext, NULL, line_count, line_count);
                yylval.symbolInfo = symbolInfo;
                return ID;
            }
{StrLiteral}    {
                string ss = replaceEscape(yytext);
                ss = ss.substr(1,ss.size()-2);
                // fprintf(tokenout,"<SINGLE LINE STRING, %s>\n",s.c_str());
                checkGlobal();
                fprintf(logout,"Line# %d: Token <SINGLE LINE STRING> Lexeme %s found\n",line_count,yytext);
                lineStart = 0;
                // SymbolInfo *s= new  SymbolInfo(yytext, (char *)"SINGLE LINE STRING");
				// yylval = (YYSTYPE)s;
                // return SINGLE_LINE_STRING;
            }
{StrMulti}    {
                int ln = line_count;
                string ss = yytext;
                string res = "";
                for (int i=1;i<ss.size()-1;i++) {
                    if(i<ss.size()-3 && ss[i]=='\\' && (ss[i+1]=='\r') && (ss[i+2]=='\n'))
                        i+=2;
                    else if(i<ss.size()-2 && ss[i]=='\\' && (ss[i+1]=='\n' || ss[i+1]=='\r'))
                        i+=1;
                    else
                        res += ss[i];
                }
                // fprintf(tokenout,"<MULTI LINE STRING, %s>\n",res.c_str());
                checkGlobal();
                fprintf(logout,"Line# %d: Token <MULTI LINE STRING> Lexeme %s found\n",ln,yytext);
                for(int i = 0; yytext[i] != 0; i++)
                    if(yytext[i] == '\n')
                        line_count++;
                lineStart = 0;
                // SymbolInfo *s= new  SymbolInfo(yytext, (char *)"MULTI LINE STRING");
				// yylval = (YYSTYPE)s;
                // return MULTI_LINE_STRING;
            }
{SingleComment} {
                checkGlobal();
                fprintf(logout,"Line# %d: Token <SINGLE LINE COMMENT> Lexeme %s found\n",line_count,yytext);
                for (int i=0;yytext[i]!=0;i++)
                    if(yytext[i] == '\n')
                        line_count++;
                lineStart = 0;
            } 
[ \t]*     {lineStart = 0;}
[\t ]*[\r]?[\n]        {
                line_count++;
                BEGIN COUNTTAB;
                lineStart = 1;
            }
{FloatErr}  {
                checkGlobal();
                fprintf(logout,"Error at line# %d: TOO_MANY_DECIMAL_POINTS %s\n",line_count,yytext);
                error++;
                lineStart = 0;
            }
{ExpErr}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: ILLFORMED_NUMBER %s\n",line_count,yytext);
                error++;
                lineStart = 0;
            }
{IllSuffix}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: INVALID_ID_SUFFIX_NUM_PREFIX %s\n",line_count,yytext);
                error++;
                lineStart = 0;
            }
{UnfinishedStr}    {
                checkGlobal();
                for(int i=0;yytext[i]!=0;i++)
                    if(yytext[i]=='\n')
                        line_count++;
                fprintf(logout,"Error at line# %d: UNFINISHED_STRING %s\n",line_count,yytext);
                error++;
                lineStart = 0;
            }
{UnknownChar}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: UNRECOGNIZED_CHAR %s\n",line_count,yytext);
                error++;
                lineStart = 0;
            }
{MultiChar}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: MULTICHAR_CONST_CHAR %s\n",line_count,yytext);
                error++;
                lineStart = 0;
            }
{UnfinishedChar}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: UNFINISHED_CONST_CHAR %s\n",line_count,yytext);
                error++;
                lineStart = 0;
            }
{EmptyChar}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: EMPTY_CONST_CHAR %s\n",line_count,yytext);
                error++;
                lineStart = 0;
            }



%%

int yywrap(void)
{
	return 1;
}



/* int main (int argc, const char **argv) {
    if (argc != 2) {
        cout << "please provide filename";
        return 1;
    }
    FILE* fp = fopen(argv[1],"r");
    if (fp == NULL) {
        cout << "file could not be opened";
        return 2;
    }
    yyin = fp;
    logout = fopen ("2005031_log.txt","w");
    tokenout = fopen ("2005031_token.txt","w");
    symbolTable = new SymbolTable(10, logout);
    
    BEGIN COUNTTAB;
    yylex();

    symbolTable->printAll();
    fprintf(logout, "Total lines: %d\n", line_count);
    fprintf(logout, "Total errors: %d\n", error);
    fprintf(logout, "Total warnings: %d\n", totalWarning);
    
    delete symbolTable;
    fclose(yyin);
    fclose(logout);
    fclose(tokenout);
    return 0;
} */