%{
#include<iostream>
#include<sstream>
#include<cstdlib>
#include<cstring>
#include<algorithm>
#include<string>
#include<cmath>
#include<vector>
#include "2005031.h"

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
FILE* codeout;

int errcount = 0;

void yyerror(char *s)
{
	
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

class VarInfo {
	string name;
	string scope;
	string type;
	int pos;
	int size;
	public:
	VarInfo(string name, string scope, string type, int pos=0, int size=1) {
		this->name = name;
		this->scope = scope;
		this->type = type;
		this->pos = pos;
		this->size = size;
	}
	void swap(VarInfo* varInfo){
		string tempstr;
		int tempint;

		tempstr=this->scope;
		this->scope=varInfo->scope;
		varInfo->scope=tempstr;

		tempstr=this->type;
		this->type=varInfo->type;
		varInfo->type=tempstr;

		tempint=this->size;
		this->size=varInfo->size;
		varInfo->size=tempint;

		tempstr=this->name;
		this->name=varInfo->name;
		varInfo->name=tempstr;
	}
	void setPos(int pos) {
		this->pos = pos;
	}
	void setName(string name) {
		this->name=name;
	}
	string getName() {
		return name;
	}
	string getScope() {
		return scope;
	}
	string getType() {
		return type;
	}
	int getPos () {
		return pos;
	}
	int getSize() {
		return size;
	}
	
};
void swapObs(vector<VarInfo*>&vec, int first, int last) {
		for(int i=first;i<=(first+last)/2;i++){
			int j=first+last-i;
			
			vec[i]->swap(vec[j]);
		}
	}


int labelCount = 0;
int globalCount = 0;
vector<VarInfo*> varStack;
string currentScope="global";
string globalInstructions = "";
string code="";
void writeCode(SymbolInfo* node) {
	if (node->getName()=="start" ||
		node->getName()=="program" ||
		node->getName()=="statements") {
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	}
	else if (node->getName()=="expression_statement"){
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
		if(node->getChildren()->getName()=="expression") {
			code+="POP AX		;"+to_string(node->getStartLine())+"\n";
			varStack.pop_back();
		}
	}
	else if (node->getName()=="unit"&&node->getChildren()->getName()=="var_declaration") {
		int varCount=0;
		for (SymbolInfo* param = node->getChildren()->getChildren()->getNextSymbolInfo()->getChildren(); param!=NULL; param=param->getChildren()) {
			if (param->getName()=="declaration_list" ) {
				if (param->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()==NULL){
					globalInstructions+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
					globalCount++;
					varCount++;
					varStack.push_back(new VarInfo(param->getNextSymbolInfo()->getNextSymbolInfo()->getType(),currentScope,"var",varStack.size()));
				}
				else {
					for (int i=0;i<stoi(param->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getType());i++){
						globalInstructions+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
						globalCount++;
					varCount++;
						varStack.push_back(new VarInfo(param->getNextSymbolInfo()->getNextSymbolInfo()->getType(),currentScope,"var",varStack.size()));
					}
				}
			}
			else if (param->getName()=="ID") {
				if (param->getNextSymbolInfo()==NULL){
					globalInstructions+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
					globalCount++;
					varCount++;
					varStack.push_back(new VarInfo(param->getType(),currentScope,"var",varStack.size()));
				}
				else {
					for (int i=0;i<stoi(param->getNextSymbolInfo()->getNextSymbolInfo()->getType());i++){
						globalInstructions+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
						globalCount++;
					varCount++;
						varStack.push_back(new VarInfo(param->getType(),currentScope,"var",varStack.size()));
					}
				}
			}
		}
		swapObs(varStack,varStack.size()-varCount,varStack.size()-1);
	}
	else if (node->getName()=="unit") {
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	}
	else if (node->getName()=="func_definition") {
		string prevScope=currentScope;
		currentScope=node->getChildren()->getNextSymbolInfo()->getType();
		int pres_no_of_vars  = varStack.size();
		code+=currentScope+" PROC		;"+to_string(node->getStartLine())+"\n";
		int num_of_itr=0;
		for (SymbolInfo* param = node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getChildren(); param!=NULL; param=param->getChildren()) {
			if (param->getName()=="parameter_list" && param->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()!=NULL && param->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getName()=="ID") {
				varStack.push_back(new VarInfo(param->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getType(),currentScope,"var",varStack.size()));
				num_of_itr++;
			}
			else if (param->getName()=="type_specifier"&&param->getNextSymbolInfo()!=NULL) {
				varStack.push_back(new VarInfo(param->getNextSymbolInfo()->getType(),currentScope,"var",varStack.size()));
				num_of_itr++;
			}
		}
		pres_no_of_vars-=num_of_itr;
		
		for(int i=0;i<num_of_itr/2;i++){
			
			string temp=varStack[varStack.size()-i-1]->getName();
			varStack[varStack.size()-i-1]->setName(varStack[varStack.size()-num_of_itr+i]->getName());
			varStack[varStack.size()-num_of_itr+i]->setName(temp);
		}
		
		varStack.push_back(new VarInfo(currentScope,currentScope,"func"));
		if(currentScope!="main");
			
		else {
			code+="MOV AX, @DATA		;"+to_string(node->getStartLine())+"\n";
			code+="MOV DS, AX		;"+to_string(node->getStartLine())+"\n";
			code+="MOV DX, SP		;"+to_string(node->getStartLine())+"\n";
			code+=globalInstructions;
			code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
		}
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo()){
			writeCode(child);
		}
		
		VarInfo* temp = NULL;
		
		
		while(varStack.size()){
			
			if(varStack[varStack.size()-1]->getType()=="func"){
				code+="POP BX		;"+to_string(node->getStartLine())+"\n";
				temp=varStack[varStack.size()-1];
				break;
			}
			else
				code+="POP AX		;"+to_string(node->getStartLine())+"\n";
			varStack.pop_back();
		}
		cout<<varStack.size();
		if(currentScope!="main") {
			code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
			code+="PUSH BX		;"+to_string(node->getStartLine())+"\n";
			varStack.push_back(temp);
			code+="RET		;"+to_string(node->getStartLine())+"\n";
			varStack.pop_back();
		}
		else {
			code+="MOV AH, 4CH		;"+to_string(node->getStartLine())+"\n";
			code+="INT 21H		;"+to_string(node->getStartLine())+"\n";
		}
		code+=currentScope+" ENDP		;"+to_string(node->getStartLine())+"\n";
		currentScope=prevScope;
	}
	else if (node->getName()=="compound_statement") {
		
		int pres_no_of_vars  = varStack.size();
		for(SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
		
		while(pres_no_of_vars<varStack.size()) {
			code+="POP AX		;"+to_string(node->getStartLine())+"\n";
			varStack.pop_back();
		}
	}
	else if (node->getName()=="var_declaration") {
		int varCount=0;
		for (SymbolInfo* param = node->getChildren()->getNextSymbolInfo()->getChildren(); param!=NULL; param=param->getChildren()) {
			if (param->getName()=="declaration_list" ) {
				if (param->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()==NULL){
					code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
					varCount++;
					varStack.push_back(new VarInfo(param->getNextSymbolInfo()->getNextSymbolInfo()->getType(),currentScope,"var",varStack.size()));
				}
				else {
					for (int i=0;i<stoi(param->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getType());i++){
						code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
						varCount++;
						varStack.push_back(new VarInfo(param->getNextSymbolInfo()->getNextSymbolInfo()->getType(),currentScope,"var",varStack.size()));
					}
				}
			}
			else if (param->getName()=="ID") {
				if (param->getNextSymbolInfo()==NULL){
					code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
					varCount++;
					varStack.push_back(new VarInfo(param->getType(),currentScope,"var",varStack.size()));
				}
				else {
					for (int i=0;i<stoi(param->getNextSymbolInfo()->getNextSymbolInfo()->getType());i++){
						code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
						varCount++;
						varStack.push_back(new VarInfo(param->getType(),currentScope,"var",varStack.size()));
					}
				}
			}
		}
		swapObs(varStack,varStack.size()-varCount,varStack.size()-1);
	}
	else if (node->getName()=="statement" && node->getChildren()!=NULL && node->getChildren()->getName()=="FOR") {
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		int curLabel = labelCount++;
		int curLabel2 = labelCount++;
		code+="LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getChildren());
		code+= "POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		code+= "CMP AX, 0		;"+to_string(node->getStartLine())+"\n";
		code+= "JE LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo());
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo());
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		code+= "JMP LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
		code+= "LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
	}
	else if (node->getName()=="statement" && node->getChildren()!=NULL && node->getChildren()->getName()=="IF" && node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()==NULL) {
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		int curLabel = labelCount++;
		code+= "POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		code+= "CMP AX, 0		;"+to_string(node->getStartLine())+"\n";
		code+= "JE LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo());
		code+= "LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
	}
	else if (node->getName()=="statement" && node->getChildren()!=NULL && node->getChildren()->getName()=="IF" && node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()!=NULL) {
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		int curLabel = labelCount++;
		code+= "POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		code+= "CMP AX, 0		;"+to_string(node->getStartLine())+"\n";
		code+="JE LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo());
		int curLabel2 = labelCount++;
		code+="JMP LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
		code+="LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo());
		code+="LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
	}
	else if (node->getName()=="statement" && node->getChildren()!=NULL && node->getChildren()->getName()=="WHILE") {
		int curLabel = labelCount++;
		int curLabel2 = labelCount++;
		code+="LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		code+="CMP AX, 0		;"+to_string(node->getStartLine())+"\n";
		code+="JE LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo()->getNextSymbolInfo());
		code+="JMP LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
		code+="LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
	}
	else if (node->getName()=="statement"&&node->getChildren()!=NULL&&node->getChildren()->getName()=="PRINTLN") {
		string id=node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo()->getType();
		bool found=false;
		for(int i=varStack.size()-1;i>=0&&currentScope==varStack[i]->getScope();i--)
			if(varStack[i]->getName()==id) {
				code+="MOV BP, SP		;"+to_string(node->getStartLine())+"\n";
				code+="MOV AX, [BP+"+to_string((varStack.size()-1-i)*2)+"]		;"+to_string(node->getStartLine())+"\n";
				found=true;
				break;
			}
		if(!found)
			for(int i=0;i<varStack.size()&&"global"==varStack[i]->getScope();i++)
				if(varStack[i]->getName()==id) {
					code+="MOV BP, DX		;"+to_string(node->getStartLine())+"\n";
					code+="MOV AX, [BP-"+to_string(2*i+2)+"]		;"+to_string(node->getStartLine())+"\n";
					break;
				}
		code+= "CALL print_output		;"+to_string(node->getStartLine())+"\n";
		code+= "CALL new_line		;"+to_string(node->getStartLine())+"\n";
	}
	else if (node->getName()=="statement"&&node->getChildren()!=NULL&&node->getChildren()->getName()=="RETURN"){
		
		vector<VarInfo*> newvec(varStack);
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
		VarInfo* temp=NULL;
		code+="POP CX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		while(varStack.size()>0 && (currentScope==varStack[varStack.size()-1]->getScope())){
			if(currentScope=="main")
				break;
			
			if(varStack[varStack.size()-1]->getType()=="func"){
				code+="POP BX		;"+to_string(node->getStartLine())+"\n";
				temp=varStack[varStack.size()-1]; 
				varStack.pop_back();
				break;
			}
			else{
				
				code+= "POP AX		;"+to_string(node->getStartLine())+"\n";
				varStack.pop_back();
			}
		}
		if(currentScope!="main") {
			code+="PUSH CX		;"+to_string(node->getStartLine())+"\n";
			code+="PUSH BX		;"+to_string(node->getStartLine())+"\n";
			varStack.push_back(new VarInfo(currentScope,currentScope,"val"));
			varStack.push_back(temp);
			varStack.pop_back();
			code+="RET		;"+to_string(node->getStartLine())+"\n";
		}
		else {
			code+="MOV AH, 4CH		;"+to_string(node->getStartLine())+"\n";
			code+="INT 21H		;"+to_string(node->getStartLine())+"\n";
		}
		varStack.pop_back();
		code+="RET		;"+to_string(node->getStartLine())+"\n";
		varStack=newvec;
		
	}
	else if (node->getName()=="statement") {
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	}
	else if (node->getName()=="expression" && node->getChildren()->getName()=="variable") {
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		if(node->getChildren()->getChildren()->getNextSymbolInfo()!=NULL){
			writeCode(node->getChildren()->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
			code+="POP BX		;"+to_string(node->getStartLine())+"\n";
			varStack.pop_back();
		}
		code+= "POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		string id=node->getChildren()->getChildren()->getType();
		bool found = false;
		for(int i=varStack.size()-1;i>=0&&varStack[i]->getScope()==currentScope;i--){
			if(varStack[i]->getName()==id) {
				code+="MOV BP, SP		;"+to_string(node->getStartLine())+"\n";
				if(node->getChildren()->getChildren()->getNextSymbolInfo()!=NULL){
					code+="ADD BP, BX		;"+to_string(node->getStartLine())+"\n";
					code+="ADD BP, BX		;"+to_string(node->getStartLine())+"\n";
				}
				code+="MOV [BP+"+to_string((varStack.size()-1-i)*2)+"], AX		;"+to_string(node->getStartLine())+"\n";
				
				
				found=true;
				break;
			}
		}
		if(!found)
			for(int i=0;i<varStack.size()&&varStack[i]->getScope()=="global";i++){
				if(varStack[i]->getName()==id) {
					code+="MOV BP, DX		;"+to_string(node->getStartLine())+"\n";
					if(node->getChildren()->getChildren()->getNextSymbolInfo()!=NULL)
						code+="SUB BP, BX		;"+to_string(node->getStartLine())+"\n";
					code+="MOV [BP-"+to_string(2*i+2)+"], AX		;"+to_string(node->getStartLine())+"\n";
					break;
				}
			}
		code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo(currentScope,currentScope,"val"));
	}
	else if (node->getName()=="expression")
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	else if (node->getName()=="variable"&&node->getChildren()->getNextSymbolInfo()==NULL) {
		string id=node->getChildren()->getType();
		bool found=false;
		for(int i=varStack.size()-1;i>=0&&varStack[i]->getScope()==currentScope;i--)
			if(varStack[i]->getName()==id) {
				code+="MOV BP, SP		;"+to_string(node->getStartLine())+"\n";
				
				
				
				
				code+="MOV AX, [BP+"+to_string((varStack.size()-1-i)*2)+"]		;"+to_string(node->getStartLine())+"\n";
				code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
				varStack.push_back(new VarInfo("",currentScope,"val"));
				found=true;
				break;
			}
		if(!found)
			for(int i=0;i<varStack.size()&&varStack[i]->getScope()=="global";i++)
				if(varStack[i]->getName()==id) {
					code+="MOV BP, DX		;"+to_string(node->getStartLine())+"\n";
					code+="MOV AX, [BP-"+to_string(2*i+2)+"]		;"+to_string(node->getStartLine())+"\n";
					code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
					varStack.push_back(new VarInfo("",currentScope,"val"));
					found=true;
					break;
				}
	}
	else if(node->getName()=="variable"){
		string id=node->getChildren()->getType();
		bool found=false;
		for(int i=varStack.size()-1;i>=0&&currentScope==varStack[i]->getScope();i--)
			if(varStack[i]->getName()==id) {
				writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
				code+= "POP AX		;"+to_string(node->getStartLine())+"\n";
				varStack.pop_back();
				code+="MOV BP, SP		;"+to_string(node->getStartLine())+"\n";
				code+="ADD BP, AX		;"+to_string(node->getStartLine())+"\n";
				code+="ADD BP, AX		;"+to_string(node->getStartLine())+"\n";
				code+="MOV AX, [BP+"+to_string((varStack.size()-1-i)*2)+"]		;"+to_string(node->getStartLine())+"\n";
				code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
				varStack.push_back(new VarInfo("",currentScope,"val"));
				found=true;
				break;
			}
		if(!found)
			for(int i=0;i<varStack.size()&&"global"==varStack[i]->getScope();i++)
				if(varStack[i]->getName()==id) {
					writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
					code+= "POP AX		;"+to_string(node->getStartLine())+"\n";
					varStack.pop_back();
					code+="MOV BP, DX		;"+to_string(node->getStartLine())+"\n";
					code+="SUB BP, AX		;"+to_string(node->getStartLine())+"\n";
					code+="MOV AX, [BP-"+to_string(2*i+2)+"]		;"+to_string(node->getStartLine())+"\n";
					code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
					varStack.push_back(new VarInfo("",currentScope,"val"));
					break;
				}
	}
	
	else if (node->getName()=="logic_expression"&&node->getChildren()->getNextSymbolInfo()==NULL)
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);

	else if (node->getName()=="logic_expression") {
		writeCode(node->getChildren());
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		code+="POP BX		;"+to_string(node->getStartLine())+"\n";
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		varStack.pop_back();
		if (node->getChildren()->getNextSymbolInfo()->getType()=="&&"){
			int curLabel=labelCount++;
			int curLabel2=labelCount++;
			code+="CMP AX, 0		;"+to_string(node->getStartLine())+"\n";
			code+="JE LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
			code+="CMP BX, 0		;"+to_string(node->getStartLine())+"\n";
			code+="JE LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n",curLabel;
			code+="PUSH 1		;"+to_string(node->getStartLine())+"\n";
			code+="JMP LABEL"+to_string(curLabel2)+"		;"+to_string(node->getStartLine())+"\n";
			code+="LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
			code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
			varStack.push_back(new VarInfo("",currentScope,"val"));
			code+="LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
		}
		else {
			int curLabel=labelCount++;
			int curLabel2=labelCount++;
			code+="CMP AX, 0		;"+to_string(node->getStartLine())+"\n";
			code+="JNE LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
			code+="CMP BX, 0		;"+to_string(node->getStartLine())+"\n";
			code+="JNE LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
			code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
			code+="JMP LABEL"+to_string(curLabel2)+"		;"+to_string(node->getStartLine())+"\n";
			code+="LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
			code+="PUSH 1		;"+to_string(node->getStartLine())+"\n";
			varStack.push_back(new VarInfo("",currentScope,"val"));
			code+="LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
		}
		
		
	}
	else if (node->getName()=="rel_expression"&&node->getChildren()->getNextSymbolInfo()==NULL)
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	else if (node->getName()=="rel_expression") {
		writeCode(node->getChildren());
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		code+="POP BX		;"+to_string(node->getStartLine())+"\n";
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		varStack.pop_back();
		code+="CMP AX, BX		;"+to_string(node->getStartLine())+"\n";
		int curLabel=labelCount++;
		int curLabel2=labelCount++;
		if (node->getChildren()->getNextSymbolInfo()->getType()==">")
			code+="JG LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
		else if (node->getChildren()->getNextSymbolInfo()->getType()==">=")
			code+="JGE LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
		else if (node->getChildren()->getNextSymbolInfo()->getType()=="<")
			code+="JL LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
		else if (node->getChildren()->getNextSymbolInfo()->getType()=="<=")
			code+="JLE LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
		else if (node->getChildren()->getNextSymbolInfo()->getType()=="==")
			code+="JE LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
		else if (node->getChildren()->getNextSymbolInfo()->getType()=="!=")
			code+="JNE LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
		code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
		code+="JMP LABEL"+to_string(curLabel2)+"		;"+to_string(node->getStartLine())+"\n";
		code+="LABEL"+to_string(curLabel)+":		;"+to_string(node->getStartLine())+"\n";
		code+="PUSH 1		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo("",currentScope,"val"));
		code+="LABEL"+to_string(curLabel2)+":		;"+to_string(node->getStartLine())+"\n";
	}
	else if (node->getName()=="simple_expression"&&node->getChildren()->getNextSymbolInfo()==NULL)
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	else if (node->getName()=="simple_expression") {
		writeCode(node->getChildren());
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		code+="POP BX		;"+to_string(node->getStartLine())+"\n";
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		varStack.pop_back();
		if (node->getChildren()->getNextSymbolInfo()->getType()=="+")
			code+="ADD AX,BX		;"+to_string(node->getStartLine())+"\n";
		else if (node->getChildren()->getNextSymbolInfo()->getType()=="-")
			code+="SUB AX,BX		;"+to_string(node->getStartLine())+"\n";
		code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo("",currentScope,"val"));
	}
	else if (node->getName()=="term"&&node->getChildren()->getNextSymbolInfo()==NULL)
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	else if (node->getName()=="term") {
		writeCode(node->getChildren());
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		code+="POP BX		;"+to_string(node->getStartLine())+"\n";
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		varStack.pop_back();
		code+="PUSH DX		;"+to_string(node->getStartLine())+"\n";
		code+="MOV DX,0		;"+to_string(node->getStartLine())+"\n";
		if (node->getChildren()->getNextSymbolInfo()->getType()=="*"){
			code+="MUL BX		;"+to_string(node->getStartLine())+"\n";
			code+="POP DX		;"+to_string(node->getStartLine())+"\n";
			code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
		}
		else if (node->getChildren()->getNextSymbolInfo()->getType()=="/"){
			code+="DIV BX		;"+to_string(node->getStartLine())+"\n";
			code+="POP DX		;"+to_string(node->getStartLine())+"\n";
			code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
		}
		else if (node->getChildren()->getNextSymbolInfo()->getType()=="%"){
			code+="DIV BX		;"+to_string(node->getStartLine())+"\n";
			code+="MOV AX, DX		;"+to_string(node->getStartLine())+"\n";
			code+="POP DX		;"+to_string(node->getStartLine())+"\n";
			code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
		}
		varStack.push_back(new VarInfo("",currentScope,"val"));
	}
	else if (node->getName()=="unary_expression" && node->getChildren()->getName()=="ADDOP") {
		writeCode(node->getChildren()->getNextSymbolInfo());
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		if (node->getChildren()->getType()=="-") {
			code+="PUSH DX		;"+to_string(node->getStartLine())+"\n";
			code+="MOV DX, 0		;"+to_string(node->getStartLine())+"\n";
			code+="MOV BX, -1		;"+to_string(node->getStartLine())+"\n";
			code+="MUL BX		;"+to_string(node->getStartLine())+"\n";
			code+="POP DX		;"+to_string(node->getStartLine())+"\n";
		}
		code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo("",currentScope,"val"));
	}
	else if (node->getName()=="unary_expression" && node->getChildren()->getName()=="NOT") {
		writeCode(node->getChildren()->getNextSymbolInfo());
		int curLabel=labelCount++;
		int curLabel2=labelCount++;
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		code+="CMP AX,0		;"+to_string(node->getStartLine())+"\n";
		code+="JE LABEL"+to_string(curLabel)+"		;"+to_string(node->getStartLine())+"\n";
		code+="PUSH 0		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo("",currentScope,"val"));
		code+="JMP LABEL"+to_string(curLabel2)+"		;"+to_string(node->getStartLine())+"\n";
		code+="LABEL"+to_string(curLabel)+":";
		code+="PUSH 1		;"+to_string(node->getStartLine())+"\n";
		code+="LABEL"+to_string(curLabel2)+":";
	}
	else if (node->getName()=="unary_expression"&&node->getChildren()->getNextSymbolInfo()==NULL)
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	else if (node->getName()=="factor"&&node->getChildren()!=NULL&&node->getChildren()->getName()=="variable"&&node->getChildren()->getNextSymbolInfo()==NULL)
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo()){
			writeCode(child);
		}
	else if (node->getName()=="factor"&&node->getChildren()->getNextSymbolInfo()!=NULL&&node->getChildren()->getNextSymbolInfo()->getType()=="++"){
		writeCode(node->getChildren());
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		code+="ADD AX,1		;"+to_string(node->getStartLine())+"\n";
		
		string id=node->getChildren()->getChildren()->getType();
		bool found = false;
		for(int i=varStack.size()-1;i>=0&&varStack[i]->getScope()==currentScope;i--){
			if(varStack[i]->getName()==id) {
				code+="MOV BP, SP		;"+to_string(node->getStartLine())+"\n";
				code+="MOV [BP+"+to_string((varStack.size()-1-i)*2)+"], AX		;"+to_string(node->getStartLine())+"\n";
				found=true;
				break;
			}
		}
		if(!found)
			for(int i=0;i<varStack.size()&&varStack[i]->getScope()=="global";i++){
				if(varStack[i]->getName()==id) {
					code+="MOV BP, DX		;"+to_string(node->getStartLine())+"\n";
					code+="MOV [BP-"+to_string(2*i+2)+"], AX		;"+to_string(node->getStartLine())+"\n";
					break;
				}
			}
		code+="SUB AX, 1		;"+to_string(node->getStartLine())+"\n";
		code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo("",currentScope,"val"));
	}
	else if (node->getName()=="factor"&&node->getChildren()->getNextSymbolInfo()!=NULL&&node->getChildren()->getNextSymbolInfo()->getType()=="--"){
		writeCode(node->getChildren());
		code+="POP AX		;"+to_string(node->getStartLine())+"\n";
		varStack.pop_back();
		code+="SUB AX,1		;"+to_string(node->getStartLine())+"\n";
		
		string id=node->getChildren()->getChildren()->getType();
		bool found = false;
		for(int i=varStack.size()-1;i>=0&&varStack[i]->getScope()==currentScope;i--){
			if(varStack[i]->getName()==id) {
				code+="MOV BP, SP		;"+to_string(node->getStartLine())+"\n";
				code+="MOV [BP+"+to_string((varStack.size()-1-i)*2)+"], AX		;"+to_string(node->getStartLine())+"\n";
				found=true;
				break;
			}
		}
		if(!found)
			for(int i=0;i<varStack.size()&&varStack[i]->getScope()=="global";i++){
				if(varStack[i]->getName()==id) {
					code+="MOV BP, DX		;"+to_string(node->getStartLine())+"\n";
					code+="MOV [BP-"+to_string(2*i+2)+"], AX		;"+to_string(node->getStartLine())+"\n";
					break;
				}
			}
		code+="ADD AX, 1		;"+to_string(node->getStartLine())+"\n";
		code+="PUSH AX		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo("",currentScope,"val"));
	}
	else if (node->getName()=="factor"&&node->getChildren()->getName()=="CONST_FLOAT"){
		code+="PUSH "+node->getChildren()->getType()+"		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo("",currentScope,"val"));
	}
	else if (node->getName()=="factor"&&node->getChildren()->getName()=="CONST_INT"){
		code+="PUSH "+node->getChildren()->getType()+"		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo("",currentScope,"val"));
	}
	else if (node->getName()=="factor"&&node->getChildren()->getName()=="LPAREN"){
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	}
	else if (node->getName()=="factor"&&node->getChildren()->getName()=="ID") {
		writeCode(node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo());
		code+="CALL "+node->getChildren()->getType()+"		;"+to_string(node->getStartLine())+"\n";
		
		code+="POP CX		;"+to_string(node->getStartLine())+"\n";
		for(SymbolInfo* arg=node->getChildren()->getNextSymbolInfo()->getNextSymbolInfo();;arg=arg->getChildren()){
			cout<<1<<endl;
			if(arg==NULL)break;
			else if(arg->getName()=="")break;
			else if(arg->getName()=="argument_list")continue;
			else if(arg->getName()=="arguments"){varStack.pop_back();code+="POP AX		;"+to_string(node->getStartLine())+"\n";}
			else if(arg->getName()=="logic_expression"){break;}
		}
		code+="PUSH CX		;"+to_string(node->getStartLine())+"\n";
		varStack.push_back(new VarInfo(currentScope,currentScope,"val"));
	}
	else if (node->getName()=="argument_list" && node->getChildren()->getName()=="");
	else if (node->getName()=="argument_list")
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo())
			writeCode(child);
	else if (node->getName()=="arguments")
		for (SymbolInfo* child = node->getChildren(); child!=NULL; child=child->getNextSymbolInfo()){
			writeCode(child);
			
			
		}
}

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
																							
																							
																							
																							
																							
																							
																							
																							
																							for (SymbolInfo* x=tempStack;x!=NULL;x=x->getNextSymbolInfo())
																								if(x->getName()!="")
																									if(!table->insert(x->getName(),x->getType())) {
																										fprintf(errorout,"Line# %d: Redefinition of parameter \'%s\'\n",x->getStartLine(),x->getName().c_str());
																										errcount++;
																										break;
																									}
																							
																							
																							
																							
																							
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
																							

																							int num_of_args =0;

																							if(!table->insert($2->getType(), $1->getChildren()->getName(), true)){
																								if(f && (!f->checkEqNumArgs(tempStack) || f->type!=$1->getChildren()->getName()))
																									fprintf(errorout,"Line# %d: Conflicting types for '%s'\n",$2->getStartLine(),$2->getType().c_str());
																								else
																									fprintf(errorout,"Line# %d: '%s' redeclared as different kind of symbol\n",$2->getStartLine(),$2->getType().c_str());
																								errcount++;
																							}
																							
																							
																							
																							
																							
																							if(f!=NULL && f->type!=$1->getChildren()->getName()) {
																								fprintf(logout,"FUNCTION %s\n",$1->getChildren()->getName().c_str());
																								
																							}
																							delete tempStack;
																						}
		| type_specifier ID LPAREN RPAREN {table->enterScope();} compound_statement                            {
																							table->printAll();
																							table->exitScope();

																							FuncList* f = funcList->findFunc($2->getType());

																							
																							
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

																							
																							if(f!=NULL && f->type!=$1->getChildren()->getName())
																								fprintf(logout,"FUNCTION %s\n",$1->getChildren()->getName().c_str());
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
																
																$$ = new SymbolInfo("compound_statement", "LCURL statements RCURL");
																$$->addChildren($1);
																$$->addChildren($2);
																$$->addChildren($3);
																
															}
 		    | LCURL RCURL                                   {
																fprintf(logout, "compound_statement : LCURL RCURL\n");
																
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

											
											if(table->lookupInfo($1->getType())==NULL){
												fprintf(errorout,"Line# %d: Undeclared function '%s'\n",$1->getStartLine(),$1->getType().c_str());
												errcount++;
											}

											
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
								$$ = new SymbolInfo("", "");
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
    FILE *fp, *fp2, *fp3, *fp4, *fp5, *fp6;

	fp=fopen(argv[1],"r");
	if(fp==NULL)
	{
		fp=fopen("input.txt","r");
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
	fp5= fopen("code.asm","w");
	fp6= fopen("printproc.txt","r");
	table = new SymbolTable(11,fp2);
	
    logout = fp2;
	errorout = fp3;
	treeout = fp4;
	codeout = fp5;


	yyin=fp;
	yyparse();
	fprintf(logout,"Total Lines: %d\n", line_count);
	fprintf(logout,"Total Errors: %d\n", errcount);
	parseTree->print(treeout);
	
	fprintf(codeout,".MODEL SMALL\n");
	fprintf(codeout,".STACK 100H\n");
	fprintf(codeout,".DATA\n");
	fprintf(codeout,"number DB '00000$'\n");
	fprintf(codeout,"%s\n",globalInstructions.c_str());
	fprintf(codeout,".CODE\n");
	char ch;
	while((ch=fgetc(fp6))!=EOF)
		fputc(ch,codeout);
	fprintf(codeout,"\n");
	int no_of_vars=0;
	writeCode(parseTree);
	fprintf(codeout,"%s",code.c_str());
	/* for(;no_of_vars>0;no_of_vars--){
		fprintf(codeout, "POP AX\n");
		varStack.pop_back();
	} */
	fprintf(codeout,"END MAIN\n");

	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	fclose(fp6);
	delete table;

	string optimized_code="";
	string line;
	istringstream iss(code);
	vector<vector<string>> optcodelist;
	while(getline(iss,line)){
		for(int i=0;i<line.size();i++){
			if(line[i]==',')
				line[i]=' ';
		}
		/* line=line.substr(0,line.find(';')); */
		istringstream iss2(line);
		vector<string> tokens;
		string token;
		while(iss2>>token){
			tokens.push_back(token);
		}
		optcodelist.push_back(tokens);
	}
	for(int i=0;i<optcodelist.size();i++){
		if(i+1<optcodelist.size() && optcodelist[i][0]=="PUSH" && optcodelist[i+1][0]=="POP" && optcodelist[i][1]==optcodelist[i+1][1]) {
			optcodelist.erase(optcodelist.begin()+i--);
		}
		else if(i+1<optcodelist.size() && optcodelist[i][0]=="MOV" && optcodelist[i+1][0]=="MOV" && optcodelist[i][1]==optcodelist[i+1][2] && optcodelist[i][2]==optcodelist[i+1][1]) {
			optcodelist.erase(optcodelist.begin()+i--+1);
		}
		else if(i+1<optcodelist.size() && optcodelist[i][0]=="MOV" && optcodelist[i+1][0]=="MOV" && optcodelist[i][1]==optcodelist[i+1][1] && optcodelist[i][2]==optcodelist[i+1][2]) {
			optcodelist.erase(optcodelist.begin()+i--+1);
		}
		else if(optcodelist[i][0]=="MOV" && optcodelist[i][1]==optcodelist[i][2]) {
			optcodelist.erase(optcodelist.begin()+i--+1);
		}
		else if(optcodelist[i][0]=="ADD" && optcodelist[i][1]=="0") {
			optcodelist.erase(optcodelist.begin()+i--+1);
		}
		else if(optcodelist[i][0]=="ADD" && optcodelist[i][2]=="0") {
			optcodelist.erase(optcodelist.begin()+i--+1);
		}
		else if(optcodelist[i][0]=="SUB" && optcodelist[i][1]=="0") {
			optcodelist.erase(optcodelist.begin()+i--+1);
		}
		else if(optcodelist[i][0]=="SUB" && optcodelist[i][2]=="0") {
			optcodelist.erase(optcodelist.begin()+i--+1);
		}
		else if(optcodelist[i][0]=="MUL" && optcodelist[i][1]=="1") {
			optcodelist.erase(optcodelist.begin()+i--+1);
		}
		else if(optcodelist[i][0]=="DIV" && optcodelist[i][1]=="1") {
			optcodelist.erase(optcodelist.begin()+i--+1);
		}
		else if(i+1<optcodelist.size() && optcodelist[i][0]=="PUSH" && optcodelist[i+1][0]=="POP" && optcodelist[i][1]!=optcodelist[i+1][1]) {
			vector<string> temp;
			temp.push_back("MOV");
			temp.push_back(optcodelist[i+1][1]);
			temp.push_back(optcodelist[i][1]);
			optcodelist.erase(optcodelist.begin()+i);
			optcodelist.erase(optcodelist.begin()+i);
			optcodelist.insert(optcodelist.begin()+i--,temp);
		}
	}

	FILE* optfile=fopen("optimized_code.asm","w");
	fprintf(optfile,".MODEL SMALL\n");
	fprintf(optfile,".STACK 100H\n");
	fprintf(optfile,".DATA\n");
	fprintf(optfile,"number DB '00000$'\n");
	fprintf(optfile,"%s\n",globalInstructions.c_str());
	fprintf(optfile,".CODE\n");
	
	fp6=fopen("printproc.txt","r");
	while((ch=fgetc(fp6))!=EOF)
		fputc(ch,optfile);
	fprintf(optfile,"\n");
	fclose(fp6);

	for(int i=0;i<optcodelist.size();i++){
		int j=0;
		if(optcodelist[i][0]=="MOV"||optcodelist[i][0]=="ADD"||optcodelist[i][0]=="SUB"||optcodelist[i][0]=="CMP"){
			j=2;
			fprintf(optfile,"%s %s, ",optcodelist[i][0].c_str(),optcodelist[i][1].c_str());
		}
		for(;j<optcodelist[i].size();j++){
			fprintf(optfile,"%s ",optcodelist[i][j].c_str());
		}
		fprintf(optfile,"\n");
	}
	fprintf(optfile,"END MAIN\n");
	fclose(optfile);
	return 0;
}

