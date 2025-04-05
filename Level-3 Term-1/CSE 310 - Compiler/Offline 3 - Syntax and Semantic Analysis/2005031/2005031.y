%{
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

%}

%union {
	SymbolInfo* symbolInfo;
}

%token <symbolInfo> ID LPAREN RPAREN SEMICOLON COMMA LCURL RCURL INT FLOAT VOID LSQUARE CONST_INT RSQUARE FOR IF ELSE WHILE PRINTLN RETURN ASSIGNOP LOGICOP RELOP ADDOP MULOP NOT CONST_FLOAT INCOP DECOP
%type  <symbolInfo> start program unit func_declaration func_definition parameter_list compound_statement var_declaration type_specifier declaration_list statements statement expression_statement variable expression logic_expression rel_expression simple_expression term unary_expression factor argument_list arguments
%left 
%right

%nonassoc 
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

start : program     {
						fprintf(logout, "start : program \n");
						$$ = new SymbolInfo("start", "program");
						$$->addChildren($1);
						parseTree = $$;
					}
	;

program : program unit  {
							fprintf(logout, "program : program unit \n");
							$$ = new SymbolInfo("program", "program unit");
							$$->addChildren($1);
							$$->addChildren($2);
						}
	| unit              {
							fprintf(logout, "program : unit \n");
							$$ = new SymbolInfo("program", "unit");
							$$->addChildren($1);
						}
	;
	
unit : var_declaration      {
								fprintf(logout, "unit : var_declaration  \n");
								$$ = new SymbolInfo("unit", "var_declaration");
								$$->addChildren($1);
							}
     | func_declaration     {
								fprintf(logout, "unit : func_declaration \n");
								$$ = new SymbolInfo("unit", "func_declaration");
								$$->addChildren($1);
								
							}
     | func_definition      {
								fprintf(logout, "unit : func_definition  \n");
								$$ = new SymbolInfo("unit", "func_definition");
								$$->addChildren($1);
							}
     ;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON {
																				string type = "";
																				SymbolInfo* tempStack = new SymbolInfo("","");
																				for (SymbolInfo* x = $4->getChildren(); x!=NULL; x=x->getChildren())
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

																				funcList = new FuncList($2->getType(), $1->getChildren()->getName(), funcList);
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
																				table->insert($2->getType(), $1->getChildren()->getName(), true);

																				fprintf(logout, "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON \n");
																				$$ = new SymbolInfo("func_declaration", "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
																				$$->addChildren($1);
																				$$->addChildren($2);
																				$$->addChildren($3);
																				$$->addChildren($4);
																				$$->addChildren($5);
																				$$->addChildren($6);
																			}
		| type_specifier ID LPAREN RPAREN SEMICOLON                         {
																				funcList = new FuncList($2->getType(), $1->getChildren()->getName(), funcList);
																				for(FuncList* x=funcList->next;x!=NULL;x=x->next)
																					if(x->name==funcList->name && x->args==NULL){
																						fprintf(errorout,"Function redeclared\n");
																							FuncList* temp = funcList;
																							funcList = funcList->next;
																							delete temp;
																							break;
																						}
																				// symboltable insersion of function name
																				table->insert($2->getType(), $1->getChildren()->getName(), true);

																				fprintf(logout, "func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON \n");
																				$$ = new SymbolInfo("func_declaration", "type_specifier ID LPAREN RPAREN SEMICOLON");
																				$$->addChildren($1);
																				$$->addChildren($2);
																				$$->addChildren($3);
																				$$->addChildren($4);
																				$$->addChildren($5);
																			}
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN						{
																							// symboltable insersion of arguments
																							table->enterScope();
																							string type = "";
																							SymbolInfo* tempStack = NULL;
																							for (SymbolInfo* x = $4->getChildren(); x!=NULL; x=x->getChildren())
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

				compound_statement     													{
																							table->printAll();
																							table->exitScope();

																							fprintf(logout, "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement \n");
																							$$ = new SymbolInfo("func_definition", "type_specifier ID LPAREN parameter_list RPAREN compound_statement");
																							$$->addChildren($1);
																							$$->addChildren($2);
																							$$->addChildren($3);
																							$$->addChildren($4);
																							$$->addChildren($5);
																							$$->addChildren($7);

																							// check function prototype
																							string type = "";
																							SymbolInfo* tempStack = NULL;
																							for (SymbolInfo* x = $4->getChildren(); x!=NULL; x=x->getChildren())
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

																							FuncList* f = funcList->findFunc($2->getType());



																							
																							if (f && f->defined==false)
																								f->setDefined();
																							else if (f && f->defined==true)
																								fprintf(errorout,"Redefined a function\n");
																							else {
																								funcList = new FuncList($2->getType(), $1->getChildren()->getName(), funcList, true);
																								for (SymbolInfo* x=tempStack;x!=NULL;x=x->getNextSymbolInfo())
																									funcList->addArg(x->getName());
																							}
																							// symboltable insersion of function name

																							int num_of_args =0;

																							if(!table->insert($2->getType(), $1->getChildren()->getName(), true)){
																								if(f && (!f->checkEqNumArgs(tempStack) || f->type!=$1->getChildren()->getName()))
																									fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",$2->getStartLine(),$2->getType().c_str());
																								else
																									fprintf(errorout,"Line# %d: '%s' redeclared as different kind of symbol\n",$2->getStartLine(),$2->getType().c_str());
																								errcount++;
																							}
																							// if(f)
																							// 	f->checkEqArgs(tempStack);
																							// if(f)
																							// 	if(f->checkEqArgs(tempStack)==false || f->type!=$1->getChildren()->getName())
																							// 		fprintf(logout,"FUNCTION %s %s %s\n",$1->getChildren()->getName().c_str(),f->type.c_str(),$1->getChildren()->getName().c_str());
																							if(f!=NULL && f->type!=$1->getChildren()->getName()) {
																								fprintf(logout,"FUNCTION %s\n",$1->getChildren()->getName().c_str());
																								// fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",$1->getChildren()->getStartLine(),f->name.c_str());
																							}
																							delete tempStack;
																						}
		| type_specifier ID LPAREN RPAREN {table->enterScope();} compound_statement                            {
																							table->printAll();
																							table->exitScope();

																							FuncList* f = funcList->findFunc($2->getType());

																							// symboltable insersion of function name
																							// table->insert($2->getType(), $1->getChildren()->getName(), true);
																							if (f && f->defined==false)
																								f->setDefined();
																							else if (f && f->defined==true)
																								fprintf(errorout,"Redefined a function\n");
																							else 
																								funcList = new FuncList($2->getType(), $1->getChildren()->getName(), funcList, true);

																							fprintf(logout, "func_definition : type_specifier ID LPAREN RPAREN compound_statement\n");
																							$$ = new SymbolInfo("func_definition", "type_specifier ID LPAREN RPAREN compound_statement");
																							$$->addChildren($1);
																							$$->addChildren($2);
																							$$->addChildren($3);
																							$$->addChildren($4);
																							$$->addChildren($6);

																							// FuncList* f = funcList->findFunc($2->getType());
																							if(f!=NULL && f->type!=$1->getChildren()->getName())
																								fprintf(logout,"FUNCTION %s\n",$1->getChildren()->getName());
																						}
		| type_specifier ID LPAREN error RPAREN											{
																							fprintf(errorout,"Line# %d: Syntax error at parameter list of function definition\n",line_count);
																						}
		  compound_statement
 		;				


parameter_list  : parameter_list COMMA type_specifier ID    {
																fprintf(logout, "parameter_list  : parameter_list COMMA type_specifier ID\n");
																$$ = new SymbolInfo("parameter_list", "parameter_list COMMA type_specifier ID");
																$$->addChildren($1);
																$$->addChildren($2);
																$$->addChildren($3);
																$$->addChildren($4);
															}
		| parameter_list COMMA type_specifier               {
																fprintf(logout, "parameter_list  : parameter_list COMMA type_specifier\n");
																$$ = new SymbolInfo("parameter_list", "parameter_list COMMA type_specifier");
																$$->addChildren($1);
																$$->addChildren($2);
																$$->addChildren($3);
															}
 		| type_specifier ID                                 {
																fprintf(logout, "parameter_list  : type_specifier ID\n");
																$$ = new SymbolInfo("parameter_list", "type_specifier ID");
																$$->addChildren($1);
																$$->addChildren($2);
															}
		| type_specifier                                    {
																fprintf(logout, "parameter_list  : type_specifier\n");
																$$ = new SymbolInfo("parameter_list", "type_specifier");
																$$->addChildren($1);
															}
 		;

 		
compound_statement : LCURL statements RCURL                 {
																fprintf(logout, "compound_statement : LCURL statements RCURL  \n");
																// table->printAll();
																$$ = new SymbolInfo("compound_statement", "LCURL statements RCURL");
																$$->addChildren($1);
																$$->addChildren($2);
																$$->addChildren($3);
																// table->exitScope();
															}
 		    | LCURL RCURL                                   {
																fprintf(logout, "compound_statement : LCURL RCURL\n");
																// table->printAll();
																$$ = new SymbolInfo("compound_statement", "LCURL RCURL");
																$$->addChildren($1);
																$$->addChildren($2);
															}
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON {
																fprintf(logout, "var_declaration : type_specifier declaration_list SEMICOLON  \n");
																$$ = new SymbolInfo("var_declaration", "type_specifier declaration_list SEMICOLON");
																$$->addChildren($1);
																$$->addChildren($2);
																$$->addChildren($3);

																SymbolInfo* tempStack = NULL;

																// symboltable insersion of variables
																for (SymbolInfo* x = $2->getChildren(); x!=NULL; x=x->getChildren())
																	for(SymbolInfo* y=x; y!=NULL; y=y->getNextSymbolInfo())
																		if(y->getName() == "ID") {
																			if (y->getNextSymbolInfo()!=NULL && y->getNextSymbolInfo()->getName()=="LSQUARE")
																				tempStack = new SymbolInfo(y->getType(),"ARRAY",tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction(), y->getType());
																			else
																				tempStack = new SymbolInfo(y->getType(),$1->getChildren()->getName(),tempStack,y->getStartLine(),y->getEndLine(),y->getIsFunction());
																		}
																
																for (SymbolInfo* y = tempStack;y!=NULL; y=y->getNextSymbolInfo()){
																	if($1->getChildren()->getName()=="VOID"){
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
		| type_specifier error SEMICOLON 					{
																fprintf(errorout,"Line# %d: Syntax error at declaration list of variable declaration\n",line_count);
															}
		;
 		 
type_specifier	: INT   {
							fprintf(logout, "type_specifier	: INT \n");
							$$ = new SymbolInfo("type_specifier", "INT");
							$$->addChildren($1);
						}
				| FLOAT {
							fprintf(logout, "type_specifier	: FLOAT \n"); $$ = $1;
							$$ = new SymbolInfo("type_specifier", "FLOAT");
							$$->addChildren($1);
						}
				| VOID  {
							fprintf(logout, "type_specifier	: VOID\n"); $$ = $1;
							$$ = new SymbolInfo("type_specifier", "VOID");
							$$->addChildren($1);
						}
				;
 		
declaration_list : declaration_list COMMA ID                    {
																	fprintf(logout, "declaration_list : declaration_list COMMA ID  \n");
																	$$ = new SymbolInfo("declaration_list", "declaration_list COMMA ID");
																	$$->addChildren($1);
																	$$->addChildren($2);
																	$$->addChildren($3);
																}
 		  | declaration_list COMMA ID LSQUARE CONST_INT RSQUARE {
																	fprintf(logout, "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE \n");
																	$$ = new SymbolInfo("declaration_list", "declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
																	$$->addChildren($1);
																	$$->addChildren($2);
																	$$->addChildren($3);
																	$$->addChildren($4);
																	$$->addChildren($5);
																	$$->addChildren($6);
																}
 		  | ID                                                  {
																	fprintf(logout, "declaration_list : ID \n");
																	$$ = new SymbolInfo("declaration_list", "ID");
																	$$->addChildren($1);
																}
 		  | ID LSQUARE CONST_INT RSQUARE                        {
																	fprintf(logout, "declaration_list : ID LSQUARE CONST_INT RSQUARE \n");
																	$$ = new SymbolInfo("declaration_list", "ID LSQUARE CONST_INT RSQUARE");
																	$$->addChildren($1);
																	$$->addChildren($2);
																	$$->addChildren($3);
																	$$->addChildren($4);
																}
 		  ;
 		  
statements : statement          								{
																	fprintf(logout, "statements : statement  \n");
																	$$ = new SymbolInfo("statements", "statement");
																	$$->addChildren($1);
																}
	| statements statement   									{
																	fprintf(logout, "statements : statements statement  \n");
																	$$ = new SymbolInfo("statements", "statements statement");
																	$$->addChildren($1);
																	$$->addChildren($2);
																}
	   ;
	   
statement : var_declaration                                                                 {
																								fprintf(logout, "statement : var_declaration \n");
																								$$ = new SymbolInfo("statement", "var_declaration");
																								$$->addChildren($1);
																							}
	  | expression_statement                                                                {
																								fprintf(logout, "statement : expression_statement  \n");
																								$$ = new SymbolInfo("statement", "expression_statement");
																								$$->addChildren($1);
																							}
	  | {table->enterScope();}
	  	compound_statement                                                                  {
																								fprintf(logout, "statement : compound_statement\n");
																								$$ = new SymbolInfo("statement", "compound_statement");
																								$$->addChildren($2);
																								table->printAll();
																								table->exitScope();
																							}
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement    {
																								fprintf(logout, "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement\n");
																								$$ = new SymbolInfo("statement", "FOR LPAREN expression_statement expression_statement expression RPAREN statement");
																								$$->addChildren($1);
																								$$->addChildren($2);
																								$$->addChildren($3);
																								$$->addChildren($4);
																								$$->addChildren($5);
																								$$->addChildren($6);
																								$$->addChildren($7);
																							}
	  | IF LPAREN expression RPAREN statement    %prec LOWER_THAN_ELSE                      {
																								fprintf(logout, "statement : IF LPAREN expression RPAREN statement\n");
																								$$ = new SymbolInfo("statement", "IF LPAREN expression RPAREN statement");
																								$$->addChildren($1);
																								$$->addChildren($2);
																								$$->addChildren($3);
																								$$->addChildren($4);
																								$$->addChildren($5);
																							}
	  | IF LPAREN expression RPAREN statement ELSE statement                                {
																								fprintf(logout, "statement : IF LPAREN expression RPAREN statement ELSE statement\n");
																								$$ = new SymbolInfo("statement", "IF LPAREN expression RPAREN statement ELSE statement");
																								$$->addChildren($1);
																								$$->addChildren($2);
																								$$->addChildren($3);
																								$$->addChildren($4);
																								$$->addChildren($5);
																								$$->addChildren($6);
																								$$->addChildren($7);
																							}
	  | WHILE LPAREN expression RPAREN statement                                            {
																								fprintf(logout, "statement : WHILE LPAREN expression RPAREN statement\n");
																								$$ = new SymbolInfo("statement", "WHILE LPAREN expression RPAREN statement");
																								$$->addChildren($1);
																								$$->addChildren($2);
																								$$->addChildren($3);
																								$$->addChildren($4);
																								$$->addChildren($5);
																							}
	  | PRINTLN LPAREN ID RPAREN SEMICOLON                                                  {
																								fprintf(logout, "statement : PRINTLN LPAREN ID RPAREN SEMICOLON\n");
																								$$ = new SymbolInfo("statement", "PRINTLN LPAREN ID RPAREN SEMICOLON");
																								$$->addChildren($1);
																								$$->addChildren($2);
																								$$->addChildren($3);
																								$$->addChildren($4);
																								$$->addChildren($5);
																								if(table->lookupInfo($3->getType())==NULL){
																									fprintf(errorout,"Line# %d: Undeclared variable '%s'\n",$3->getStartLine(),$3->getType().c_str());
																									errcount++;
																								}
																							}
	  | RETURN expression SEMICOLON                                                         {
																								fprintf(logout, "statement : RETURN expression SEMICOLON\n");
																								$$ = new SymbolInfo("statement", "RETURN expression SEMICOLON");
																								$$->addChildren($1);
																								$$->addChildren($2);
																								$$->addChildren($3);
																							}
	  ;
	  
expression_statement 	: SEMICOLON			{
												fprintf(logout, "expression_statement : SEMICOLON			\n");
												$$ = new SymbolInfo("expression_statement", "SEMICOLON");
												$$->addChildren($1);
											}
			| expression SEMICOLON          {
												fprintf(logout, "expression_statement : expression SEMICOLON 		 \n");
												$$ = new SymbolInfo("expression_statement", "expression SEMICOLON");
												$$->addChildren($1);
												$$->addChildren($2);
											}
			| error SEMICOLON				{
												fprintf(errorout,"Line# %d: Syntax error at expression of expression statement\n",line_count);
											}
			;
	  
variable : ID 		                        {
												fprintf(logout, "variable : ID 	 \n");
												SymbolInfo* symbolInfo = table->lookupInfo($1->getType());
												string type = "INT";
												if(symbolInfo)
													type = symbolInfo->getType();
												$$ = new SymbolInfo("variable", "ID",NULL,INT_MAX,-1,false,type);
												$$->addChildren($1);

												// error
												if(table->lookupInfo($1->getType())==NULL){
													fprintf(errorout,"Line# %d: Undeclared variable '%s'\n",$1->getStartLine(),$1->getType().c_str());
													errcount++;
												}
											}
	 | ID LSQUARE expression RSQUARE        {
												SymbolInfo* symbolInfo = table->lookupInfo($1->getType());
												string type = "INT";
												if(symbolInfo)
													type = symbolInfo->getExpType();

												fprintf(logout, "variable : ID LSQUARE expression RSQUARE  	 \n");
												$$ = new SymbolInfo("variable", "ID LSQUARE expression RSQUARE",NULL,INT_MAX,-1,false,type);
												$$->addChildren($1);
												$$->addChildren($2);
												$$->addChildren($3);
												$$->addChildren($4);

												// error
												SymbolInfo* symbolInfo2 = table->lookupInfo($1->getType());
												if(symbolInfo2==NULL){
													fprintf(errorout,"Line# %d: Undeclared variable '%s'\n",$1->getStartLine(),$1->getType().c_str());
													errcount++;
												}
												else if (symbolInfo2->getType()!="ARRAY"){
													fprintf(errorout,"Line# %d: '%s' is not an array\n",$1->getStartLine(),$1->getType().c_str());
													errcount++;
												}
												if ($3->getExpType()!="INT"){
													fprintf(errorout,"Line# %d: Array subscript is not an integer\n",$3->getStartLine());
													errcount++;
												}
											}
	;
	 
expression : logic_expression	            {
												fprintf(logout, "expression 	: logic_expression	 \n");
												$$ = new SymbolInfo("expression", "logic_expression",NULL,INT_MAX,-1,false,$1->getExpType());
												$$->addChildren($1);
											}
	   | variable ASSIGNOP logic_expression 	{
													fprintf(logout, "expression 	: variable ASSIGNOP logic_expression 		 \n");
													$$ = new SymbolInfo("expression", "variable ASSIGNOP logic_expression",NULL,INT_MAX,-1,false,$1->getExpType());
													$$->addChildren($1);
													$$->addChildren($2);
													$$->addChildren($3);

													// error
													if($1->getExpType()=="VOID"||$3->getExpType()=="VOID"){
														fprintf(errorout,"Line# %d: Void cannot be used in expression \n",$2->getStartLine());
														errcount++;
													}
													
													if(($1->getExpType()=="FLOAT"&&$3->getExpType()=="INT")||($1->getExpType()=="INT"&&$3->getExpType()=="FLOAT")){
														fprintf(errorout,"Line# %d: Warning: possible loss of data in assignment of FLOAT to INT\n",$2->getStartLine());
														errcount++;
													}
												}
	   ;
			
logic_expression : rel_expression 	                {
														fprintf(logout, "logic_expression : rel_expression 	 \n");
														$$ = new SymbolInfo("logic_expression", "rel_expression",NULL,INT_MAX,-1,false,$1->getExpType());
														$$->addChildren($1);
													}
		 | rel_expression LOGICOP rel_expression 	{
														fprintf(logout, "logic_expression : rel_expression LOGICOP rel_expression 	 	 \n");
														$$ = new SymbolInfo("logic_expression", "rel_expression LOGICOP rel_expression",NULL,INT_MAX,-1,false,"INT");
														$$->addChildren($1);
														$$->addChildren($2);
														$$->addChildren($3);

														// error
														if($1->getExpType()=="VOID"||$3->getExpType()=="VOID"){
															fprintf(errorout,"Line# %d: Void cannot be used in expression \n",$2->getStartLine());
															errcount++;
														}
													}
		 ;
			
rel_expression	: simple_expression                     {
															fprintf(logout, "rel_expression	: simple_expression \n");
															$$ = new SymbolInfo("rel_expression", "simple_expression",NULL,INT_MAX,-1,false,$1->getExpType());
															$$->addChildren($1);
														}
		| simple_expression RELOP simple_expression	    {
															fprintf(logout, "rel_expression	: simple_expression RELOP simple_expression	  \n");
															$$ = new SymbolInfo("rel_expression", "simple_expression RELOP simple_expression",NULL,INT_MAX,-1,false,"INT");
															$$->addChildren($1);
															$$->addChildren($2);
															$$->addChildren($3);

															// error
															if($1->getExpType()=="VOID"||$3->getExpType()=="VOID"){
																fprintf(errorout,"Line# %d: Void cannot be used in expression \n",$2->getStartLine());
																errcount++;
															}
														}
		;
				
simple_expression : term                    {
												fprintf(logout, "simple_expression : term \n");
												$$ = new SymbolInfo("simple_expression", "term",NULL,INT_MAX,-1,false,$1->getExpType());
												$$->addChildren($1);
											}
		  | simple_expression ADDOP term    {
												string type = "INT";
												if($1->getExpType()=="FLOAT" || $3->getExpType()=="FLOAT")
													type = "FLOAT";
												fprintf(logout, "simple_expression : simple_expression ADDOP term  \n");
												$$ = new SymbolInfo("simple_expression", "simple_expression ADDOP term",NULL,INT_MAX,-1,false,type);
												$$->addChildren($1);
												$$->addChildren($2);
												$$->addChildren($3);

												// error
												if($1->getExpType()=="VOID"||$3->getExpType()=="VOID"){
													fprintf(errorout,"Line# %d: Void cannot be used in expression \n",$2->getStartLine());
													errcount++;
												}
											}
		  ;
					
term :	unary_expression                {
											fprintf(logout, "term :	unary_expression \n");
											$$ = new SymbolInfo("term", "unary_expression",NULL,INT_MAX,-1,false,$1->getExpType());
											$$->addChildren($1);
										}
     |  term MULOP unary_expression     {
											fprintf(logout, "term :	term MULOP unary_expression \n");
											string type = "INT";
											if($1->getExpType()=="FLOAT" || $3->getExpType()=="FLOAT")
												type = "FLOAT";
											if($2->getType()=="%")
												type = "INT";
											$$ = new SymbolInfo("term", "term MULOP unary_expression",NULL,INT_MAX,-1,false,type);
											$$->addChildren($1);
											$$->addChildren($2);
											$$->addChildren($3);

											// error
											if($1->getExpType()=="VOID"||$3->getExpType()=="VOID"){
												fprintf(errorout,"Line# %d: Void cannot be used in expression \n",$2->getStartLine());
												errcount++;
											}
											
											else if($2->getType()=="%"&&($1->getExpType()!="INT"||$3->getExpType()!="INT")){
												fprintf(errorout,"Line# %d: Operands of modulus must be integers \n",$2->getStartLine());
												errcount++;
											}

											SymbolInfo* zornot = $3->getChildren();
											if(zornot->getName()=="factor"){
												zornot = zornot->getChildren();
												if(zornot->getName()=="CONST_INT"&&stoi(zornot->getType())==0){
													fprintf(errorout,"Line# %d: Warning: division by zero i=0f=1Const=0\n",$2->getStartLine());
													errcount++;
												}
											}
										}
     ;

unary_expression : ADDOP unary_expression   {
												// error
												if($2->getExpType()=="VOID"){
													fprintf(errorout,"Line# %d: Void cannot be used in expression \n",$2->getStartLine());
													errcount++;
												}


												fprintf(logout, "unary_expression : ADDOP unary_expression  \n");
												$$ = new SymbolInfo("unary_expression", "ADDOP unary_expression",NULL,INT_MAX,-1,false,$2->getExpType());
												$$->addChildren($1);
												$$->addChildren($2);
											}
		 | NOT unary_expression             {
												fprintf(logout, "unary_expression : NOT unary_expression \n");
												$$ = new SymbolInfo("unary_expression", "NOT unary_expression",NULL,INT_MAX,-1,false,"INT");
												$$->addChildren($1);
												$$->addChildren($2);

												// error
												if($2->getExpType()=="VOID"){
													fprintf(errorout,"Line# %d: Void cannot be used in expression \n",$2->getStartLine());
													errcount++;
												}
											}
		 | factor                           {
												fprintf(logout, "unary_expression : factor \n");
												$$ = new SymbolInfo("unary_expression", "factor",NULL,INT_MAX,-1,false,$1->getExpType());
												$$->addChildren($1);
											}
		 ;
	
factor	: variable                      {
											fprintf(logout, "factor	: variable \n");
											$$ = new SymbolInfo("factor", "variable",NULL,INT_MAX,-1,false,$1->getExpType());
											$$->addChildren($1);
										}
	| ID LPAREN argument_list RPAREN    {
											SymbolInfo* symbolInfo = table->lookupInfo($1->getType());
											string type = "INT";
											if(symbolInfo)
												type = symbolInfo->getType();

											fprintf(logout, "factor	: ID LPAREN argument_list RPAREN  \n");
											$$ = new SymbolInfo("factor", "ID LPAREN argument_list RPAREN",NULL,INT_MAX,-1,false,type);
											$$->addChildren($1);
											$$->addChildren($2);
											$$->addChildren($3);
											$$->addChildren($4);

											// error
											if(table->lookupInfo($1->getType())==NULL){
												fprintf(errorout,"Line# %d: Undeclared function '%s'\n",$1->getStartLine(),$1->getType().c_str());
												errcount++;
											}

											// error
											FuncList* func = funcList->findFunc($1->getType());
											if (func) {

												FuncList* func = funcList->findFunc($1->getType());
												SymbolInfo* tempStack = NULL;
												for (SymbolInfo* x = $3->getChildren(); x!=NULL; x=x->getChildren()) {
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
														fprintf(errorout,"Line# %d: Type mismatch for argument %d of '%s'\n",arg2->getStartLine(),count,$1->getType().c_str());
														errcount++;
													}
													arg1 = arg1->getNextSymbolInfo();
													arg2 = arg2->getNextSymbolInfo();
													count++;
												}
												if(arg1!=NULL){
													fprintf(errorout,"Line# %d: Too few arguments to function '%s'\n",$1->getStartLine(),$1->getType().c_str());
													errcount++;
												}
												else if(arg2!=NULL){
													fprintf(errorout,"Line# %d: Too many arguments to function '%s'\n",$1->getStartLine(),$1->getType().c_str());
													errcount++;
												}
												delete tempStack;

											}
										}
	| LPAREN expression RPAREN          {
											fprintf(logout, "factor	: LPAREN expression RPAREN   \n");
											$$ = new SymbolInfo("factor", "LPAREN expression RPAREN",NULL,INT_MAX,-1,false,$2->getExpType());
											$$->addChildren($1);
											$$->addChildren($2);
											$$->addChildren($3);
										}
	| CONST_INT                         {
											fprintf(logout, "factor	: CONST_INT   \n");
											$$ = new SymbolInfo("factor", "CONST_INT",NULL,INT_MAX,-1,false,"INT");
											$$->addChildren($1);
										}
	| CONST_FLOAT                       {
											fprintf(logout, "factor	: CONST_FLOAT   \n");
											$$ = new SymbolInfo("factor", "CONST_FLOAT",NULL,INT_MAX,-1,false,"FLOAT");
											$$->addChildren($1);
										}
	| variable INCOP                    {
											fprintf(logout, "factor	: variable INCOP \n");
											$$ = new SymbolInfo("factor", "variable INCOP",NULL,INT_MAX,-1,false,$1->getExpType());
											$$->addChildren($1);
											$$->addChildren($2);
										}
	| variable DECOP                    {
											fprintf(logout, "factor	: variable DECOP\n");
											$$ = new SymbolInfo("factor", "variable DECOP",NULL,INT_MAX,-1,false,$1->getExpType());
											$$->addChildren($1);
											$$->addChildren($2);
										}
	;
	
argument_list : arguments   {
								fprintf(logout, "argument_list : arguments  \n");
								$$ = new SymbolInfo("argument_list", "arguments");
								$$->addChildren($1);
							}
			  |             {
								fprintf(logout, "argument_list : \n");
								$$ = new SymbolInfo("argument_list", "");
								// $$->addChildren(new SymbolInfo ("", ""));
							}
			  ;
	
arguments : arguments COMMA logic_expression    {
													fprintf(logout, "arguments : arguments COMMA logic_expression \n");
													$$ = new SymbolInfo("arguments", "arguments COMMA logic_expression");
													$$->addChildren($1);
													$$->addChildren($2);
													$$->addChildren($3);
												}
	      | logic_expression                    {
													fprintf(logout, "arguments : logic_expression\n");
													$$ = new SymbolInfo("arguments", "logic_expression");
													$$->addChildren($1);
												}
	      ;
 

%%
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

