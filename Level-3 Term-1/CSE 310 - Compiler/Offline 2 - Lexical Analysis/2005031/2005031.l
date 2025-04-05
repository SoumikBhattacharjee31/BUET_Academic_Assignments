%option noyywrap

%{
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include "2005031.h"
SymbolTable* symbolTable;
int linecount = 1;
int totalError = 0;
int totalWarning = 0;
int scopeCount = 0;
int lineStart = 1;
int messedTab = 0;
FILE *logout;
FILE *tokenout;
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
        fprintf(logout, "Line# %d: warning, %d of tabs needed but got %d tabs.\n",linecount, scopeCount, 0);
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
KeyWord if|for|do|int|float|void|switch|default|else|while|break|char|double|return|case|continue
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
                symbolTable->exitScope();
                scopeCount--;
                int tabCount = 0;
                for(int i=0;yytext[i]!=0;i++)
                    if(yytext[i]=='\t')
                        tabCount++;
                    else if (yytext[i]==' ')
                        messedTab=1;
                if(lineStart)
                if (tabCount != scopeCount) {
                    fprintf(logout, "Line# %d: warning, %d of tabs needed but got %d tabs.\n",linecount, scopeCount, tabCount);
                    totalWarning++;
                }
                else if (messedTab) {
                    fprintf(logout, "Line# %d: Warning, tab requrired but got space.\n",linecount);
                    totalWarning++;
                }
                fprintf(tokenout,"<RCURL, }>\n");
                fprintf(logout,"Line# %d: Token <RCURL> Lexeme } found\n",linecount);
                tabCount = 0;
                messedTab = 0;
                BEGIN INITIAL;
                lineStart = 0;
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
                    fprintf(logout, "Line# %d: warning, %d of tabs needed but got %d tabs.\n",linecount, scopeCount, tabCount);
                    totalWarning++;
                }
                else if (messedTab) {
                    fprintf(logout, "Line# %d: Warning, tab requrired but got space.\n",linecount);
                    totalWarning++;
                }
                tabCount = 0;
                messedTab = 0;
                BEGIN INITIAL;
                lineStart = 0;
}
[\r]?[\n]  {
                    linecount++;
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
                fprintf(logout,"Line# %d: Token <MULTI LINE COMMENT> Lexeme %s found\n",linecount,comment.c_str());
                for (char x:comment)
                    if (x=='\n')
                        linecount++;
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
                        linecount++;
                fprintf(logout,"Error at line# %d: UNFINISHED_COMMENT %s\n",linecount,comment.c_str());
                comment = "";
                totalError++;
                lineStart = 0;
}

{KeyWord}   {
                string s = yytext;
                for (int i=0;i<s.size();i++)
                    if(yytext[i]>=97 && yytext[i]<=122)
                        yytext[i]-=32;
                checkGlobal();
                fprintf(tokenout,"<%s, %s>\n",yytext, s.c_str());
                fprintf(logout,"Line# %d: Token <%s> Lexeme %s found\n",linecount,yytext,s.c_str());
                lineStart = 0;
            }
{Integer}   {
                checkGlobal();
                fprintf(tokenout,"<CONST_INT, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <CONST_INT> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
{Float}     {
                checkGlobal();
                fprintf(tokenout,"<CONST_FLOAT, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <CONST_FLOAT> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
{CharLiteral}     {
    // \'\"\n\t\\\/\a\f\r\b\v\0
                string s = replaceEscape(yytext);
                checkGlobal();
                fprintf(tokenout,"<CONST_CHAR, %c>\n",s[1]);
                fprintf(logout,"Line# %d: Token <CONST_CHAR> Lexeme %c found\n",linecount,s[1]);
                lineStart = 0;
            }
[+-]        {
                checkGlobal();
                fprintf(tokenout,"<ADDOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <ADDOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
[*/%]       {
                checkGlobal();
                fprintf(tokenout,"<MULOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <MULOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"++"|"--"        {
                checkGlobal();
                fprintf(tokenout,"<INCOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <INCOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"<"|"<="|">"|">="|"=="|"!=" {
                checkGlobal();
                fprintf(tokenout,"<RELOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <RELOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"="         {
                checkGlobal();
                fprintf(tokenout,"<ASSIGNOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <ASSIGNOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"&&"|"||"   {
                checkGlobal();
                fprintf(tokenout,"<LOGICOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <LOGICOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"&"|"|"|"^"|"<<"|">>"   {
                checkGlobal();
                fprintf(tokenout,"<BITOP, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <BITOP> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"!"         {
                checkGlobal();
                fprintf(tokenout,"<NOT, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <NOT> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"\("         {
                checkGlobal();
                fprintf(tokenout,"<LPAREN, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <LPAREN> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"\)"         {
                checkGlobal();
                fprintf(tokenout,"<RPAREN, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <RPAREN> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"\{"         {
                checkGlobal();
                fprintf(tokenout,"<LCURL, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <LCURL> Lexeme %s found\n",linecount,yytext);
                symbolTable->enterScope();
                scopeCount++;
                lineStart = 0;
            }
[}]         {
                symbolTable->exitScope();
                scopeCount--;
                checkGlobal();
                fprintf(tokenout,"<RCURL, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <RCURL> Lexeme %s found\n",linecount,yytext);
                // cout<<"yes";
                lineStart = 0;
            }
"\["         {
                checkGlobal();
                fprintf(tokenout,"<LSQUARE, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <LSQUARE> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
"\]"         {
                checkGlobal();
                fprintf(tokenout,"<RSQUARE, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <RSQUARE> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
","         {
                checkGlobal();
                fprintf(tokenout,"<COMMA, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <COMMA> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
";"         {
                checkGlobal();
                fprintf(tokenout,"<SEMICOLON, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <SEMICOLON> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
{Identifier}    {
                checkGlobal();
                fprintf(tokenout,"<ID, %s>\n",yytext);
                fprintf(logout,"Line# %d: Token <ID> Lexeme %s found\n",linecount,yytext);
                bool inserted = symbolTable->insert(yytext,"ID");
                if (inserted)
                    symbolTable->printAll();
                lineStart = 0;
            }
{StrLiteral}    {
                string s = replaceEscape(yytext);
                s = s.substr(1,s.size()-2);
                fprintf(tokenout,"<SINGLE LINE STRING, %s>\n",s.c_str());
                checkGlobal();
                fprintf(logout,"Line# %d: Token <SINGLE LINE STRING> Lexeme %s found\n",linecount,yytext);
                lineStart = 0;
            }
{StrMulti}    {
                int ln = linecount;
                string s = yytext;
                string res = "";
                for (int i=1;i<s.size()-1;i++) {
                    if(i<s.size()-3 && s[i]=='\\' && (s[i+1]=='\r') && (s[i+2]=='\n'))
                        i+=2;
                    else if(i<s.size()-2 && s[i]=='\\' && (s[i+1]=='\n' || s[i+1]=='\r'))
                        i+=1;
                    else
                        res += s[i];
                }
                fprintf(tokenout,"<MULTI LINE STRING, %s>\n",res.c_str());
                checkGlobal();
                fprintf(logout,"Line# %d: Token <MULTI LINE STRING> Lexeme %s found\n",ln,yytext);
                for(int i = 0; yytext[i] != 0; i++)
                    if(yytext[i] == '\n')
                        linecount++;
                lineStart = 0;
            }
{SingleComment} {
                checkGlobal();
                fprintf(logout,"Line# %d: Token <SINGLE LINE COMMENT> Lexeme %s found\n",linecount,yytext);
                for (int i=0;yytext[i]!=0;i++)
                    if(yytext[i] == '\n')
                        linecount++;
                lineStart = 0;
            } 
[ \t]*     {lineStart = 0;}
[\t ]*[\r]?[\n]        {
                linecount++;
                BEGIN COUNTTAB;
                lineStart = 1;
            }
{FloatErr}  {
                checkGlobal();
                fprintf(logout,"Error at line# %d: TOO_MANY_DECIMAL_POINTS %s\n",linecount,yytext);
                totalError++;
                lineStart = 0;
            }
{ExpErr}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: ILLFORMED_NUMBER %s\n",linecount,yytext);
                totalError++;
                lineStart = 0;
            }
{IllSuffix}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: INVALID_ID_SUFFIX_NUM_PREFIX %s\n",linecount,yytext);
                totalError++;
                lineStart = 0;
            }
{UnfinishedStr}    {
                checkGlobal();
                for(int i=0;yytext[i]!=0;i++)
                    if(yytext[i]=='\n')
                        linecount++;
                fprintf(logout,"Error at line# %d: UNFINISHED_STRING %s\n",linecount,yytext);
                totalError++;
                lineStart = 0;
            }
{UnknownChar}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: UNRECOGNIZED_CHAR %s\n",linecount,yytext);
                totalError++;
                lineStart = 0;
            }
{MultiChar}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: MULTICHAR_CONST_CHAR %s\n",linecount,yytext);
                totalError++;
                lineStart = 0;
            }
{UnfinishedChar}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: UNFINISHED_CONST_CHAR %s\n",linecount,yytext);
                totalError++;
                lineStart = 0;
            }
{EmptyChar}    {
                checkGlobal();
                fprintf(logout,"Error at line# %d: EMPTY_CONST_CHAR %s\n",linecount,yytext);
                totalError++;
                lineStart = 0;
            }



%%

int main (int argc, const char **argv) {
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
    fprintf(logout, "Total lines: %d\n", linecount);
    fprintf(logout, "Total errors: %d\n", totalError);
    fprintf(logout, "Total warnings: %d\n", totalWarning);
    
    delete symbolTable;
    fclose(yyin);
    fclose(logout);
    fclose(tokenout);
    return 0;
}