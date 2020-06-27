#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"

extern char Text[20000];
extern struct Vartab Tab[100];
int p = 0;//token指针,遇到终结符++
int cont = 1;

astnode* queue[200] = { NULL };
int pop = 10;
int push = 10;

struct symtable {
	char var[100];
	char type[100];
}symt[200];
int symid = 0;
void initsymt() {
	for (int i = 0; i < 50; i++) {
		strcpy(symt[i].type, "\0");
		strcpy(symt[i].var,"\0");
	}
}
astnode* init(astnode* node){
	strcpy(node->name , "\0");
	node->id = 0;
	node->name1 = 0;
	node->father = NULL;
	node->bother = NULL;
	node->pro = NULL;
	node->name1 = 0;
	memset(node->children, NULL, 10*sizeof(astnode*));
	return node;
}

astnode* notfinal(astnode* father, char sym[]) {
	struct astnode* node;
	node = (astnode*)malloc(sizeof(astnode));
	node = init(node);
	strcpy(node->name, sym);
	node->id = -1;
	node->name1 = cont;
	cont++;
	node->father = father;
	int i = 0;
	for (i = 0; i < 10; i++) {
		if (father->children[i] == NULL) {
			father->children[i] = node;
			
			if (i > 0) {
				node->pro = father->children[i - 1];
				father->children[i - 1]->bother = node;
			}
			break;
		}
	}
	return node;
}//遇到非终结符节点处理

void isfinal(astnode* father, char token[]) {
	astnode* node;
	node = (astnode*)malloc(sizeof(astnode));
	node = init(node);
	if (strcmp(Tab[p].var, token) == 0) {
		strcpy(node->name, Tab[p].var);
		node->id = Tab[p].id;
		node->name1= cont;
		cont++;
		node->father = father;
		int i = 0;
		for (i = 0; i < 10; i++) {
			if (father->children[i] == NULL) {
				father->children[i] = node;
				
				if (i > 0) {
					node->pro = father->children[i - 1];
					father->children[i - 1]->bother = node;
				}
				break;
			}
		}
	}
	else {
		error(1);
	}
}//遇到终结符节点处理

bool scansymtable(char token[100]) {
	int i = 0;
	for (i = 0; i < 20; i++) {
		if (strcmp(token, symt[i].var) == 0) {

			return true;
		}
			
	}
	if (i == 20) {
		return false;
	}
}

void isID(astnode* father, int id) {
	if (Tab[p].id == id) {
		struct astnode* node;
		node = (astnode*)malloc(sizeof(astnode));
		node = init(node);
		strcpy(node->name, Tab[p].var);
		node->id = Tab[p].id;
		node->name1 = cont;
		cont++;
		node->father = father;
		int i = 0;
		for (i = 0; i < 10; i++) {
			if (father->children[i] == NULL) {
				father->children[i] = node;
				if (i > 0) {
					node->pro = father->children[i - 1];
					father->children[i - 1]->bother = node;
				}
				break;
			}
		}
	}
	else {
		error(1);
	}
}//变量操作，符号表


void iden(astnode* father) {
	p++;
	int i = 0;
	for (i = 0; i < 20; i++) {
		if (strcmp(Tab[p].var, symt[i].var) != 0)
			continue;
		else
			break;
	}
	if (i == 20) {
		strcpy(symt[symid].var, Tab[p].var);

		symid++;
	}
	isID(father, Iden);
}

void intno(astnode* father) {
	p++;
	strcpy(symt[symid].type, "IntNo");
	strcpy(symt[symid].var, Tab[p].var);
	symid++;
	isID(father, IntNo);
}

void realno(astnode* father) {
	p++;
	strcpy(symt[symid].type, "RealNo");
	strcpy(symt[symid].var, Tab[p].var);
	symid++;
	isID(father, RealNo);
}

void Type(astnode* father) {
	if (Tab[p + 1].id == Integer) {
		p++;
		char token1[10] = "integer";
		for (int i = 0; i < symid; i++) {
			if (strcmp(symt[i].var, "\0") == 1 && strcmp(symt[i].type, "\0") == 0) {
				strcpy(symt[i].type, "integer");
			}
		}
			
		isfinal(father, token1);
	}
	else if (Tab[p + 1].id == Real) {
		p++;
		char token1[10] = "real";
		for (int i = 0; i < symid; i++) {
			if (strcmp(symt[i].var, "\0") == 1 && strcmp(symt[i].type, "\0") == 0) {
				strcpy(symt[i].type, "real");
			}
		}
		isfinal(father, token1);
	}
	else error(1);
}
void AssignState(astnode* father);
void Rop(astnode* father) {
	if (Tab[p + 1].id == '<') {
		p++;
		char token1[10] = "<";
		isfinal(father, token1);
	}
	else if (Tab[p + 1].id == '>') {
		p++;
		char token1[10] = ">";
		isfinal(father, token1);
	}
	else if (Tab[p + 1].id == '=') {
		p++;
		char token1[10] = "=";
		isfinal(father, token1);
	}
	else if (Tab[p + 1].id == GE) {
		p++;
		char token1[10] = "GE";
		isfinal(father, token1);
	}
	else if (Tab[p + 1].id == NE) {
		p++;
		char token1[10] = "NE";
		isfinal(father, token1);
	}
	else if (Tab[p + 1].id == LE) {
		p++;
		char token1[10] = "LE";
		isfinal(father, token1);
	}
	else error(1);
}

void ForLoop1(astnode* father) {
	p++;
	char token2[20] = "for";
	isfinal(father, token2);

	struct astnode* node1;
	node1 = (astnode*)malloc(sizeof(astnode));
	node1 = init(node1);
	char token1[20] = "AsignState";
	node1 = notfinal(father, token1);
	AssignState(node1);
}

void ForLoop2(astnode* father) {
	struct astnode* node1;
	node1 = (astnode*)malloc(sizeof(astnode));
	node1 = init(node1);
	char token1[200] = "ForLoop1";
	node1 = notfinal(father, token1);
	ForLoop1(node1);

	p++;
	char token3[200] = "to";
	isfinal(father, token3);

	struct astnode* node2;
	node2 = (astnode*)malloc(sizeof(astnode));
	node2 = init(node2);
	char token2[200] = "Expr";
	node2 = notfinal(father, token2);
	Expr(node2);
}

void BoolTerm(astnode* father) {
	if (Tab[p + 1].id == '(') {
		p++;
		char token1[50] = "(";
		isfinal(father, token1);

		struct astnode* node2;
		node2 = (astnode*)malloc(sizeof(astnode));
		node2 = init(node2);
		char token2[20] = "BoolExpr";
		node2 = notfinal(father, token2);
		BoolExpr(node2);

		p++;
		char token3[5] = ")";
		isfinal(father, token3);
	}
	else if (Tab[p + 1].id == Not) {
		p++;
		char token4[5] = "not";
		isfinal(father, token4);

		struct astnode* node5;
		node5 = (astnode*)malloc(sizeof(astnode));
		node5 = init(node5);
		char token5[20] = "BoolTerm";
		node5 = notfinal(father, token5);
		BoolTerm(node5);
	}
	else {
		struct astnode* node6;
		node6 = (astnode*)malloc(sizeof(astnode));
		node6 = init(node6);
		char token6[20] = "Expr";
		node6 = notfinal(father, token6);
		Expr(node6);

		struct astnode* node7;
		node7 = (astnode*)malloc(sizeof(astnode));
		node7 = init(node7);
		char token7[20] = "Rop";
		node7 = notfinal(father, token7);
		Rop(node7);

		struct astnode* node8;
		node8 = (astnode*)malloc(sizeof(astnode));
		node8 = init(node8);
		char token8[20] = "Expr";
		node8 = notfinal(father, token8);
		Expr(node8);
	}
}

void BoolExpr(astnode* father) {
	if (Tab[p + 1].id == Not) {
		p++;
		char token1[5] = "not";
		isfinal(father, token1);

		struct astnode* node2;
		node2 = (astnode*)malloc(sizeof(astnode));
		node2 = init(node2);
		char token2[20] = "BoolExpr";
		node2 = notfinal(father, token2);
		BoolExpr(node2);
	}
	else if (Tab[p + 1].id == '(' || Tab[p + 1].id == IntNo || Tab[p + 1].id == RealNo || Tab[p + 1].id == Iden) {
		struct astnode* node3;
		node3 = (astnode*)malloc(sizeof(astnode));
		node3 = init(node3);
		char token3[20] = "BootTerm";
		node3 = notfinal(father, token3);
		BoolTerm(node3);

		if (Tab[p + 1].id == And) {
			p++;
			char token4[5] = "and";
			isfinal(father, token4);
		}
		else if (Tab[p + 1].id == Or) {
			p++;
			char token4[5] = "or";
			isfinal(father, token4);
		}
		struct astnode* node5;
		node5 = (astnode*)malloc(sizeof(astnode));
		node5 = init(node5);
		char token5[20] = "BoolExpr";
		node5 = notfinal(father, token5);
		BoolExpr(node5);
	}
	else if (Tab[p + 1].id == Then|| Tab[p + 1].id == Do|| Tab[p + 1].id == ')') {}
	else error(1);
}

void Factor(astnode* father) {
	if (Tab[p + 1].id == '(') {
		p++;
		char token1[100] = "(";
		isfinal(father, token1);

		struct astnode* node2;
		node2 = (astnode*)malloc(sizeof(astnode));
		node2 = init(node2);
		char token2[200] = "Expr";
		node2 = notfinal(father, token2);
		Expr(node2);

		p++;
		char token3[100] = ")";
		isfinal(father, token3);
	}
	else if (Tab[p + 1].id == IntNo) {
		struct astnode* node5;
		node5 = (astnode*)malloc(sizeof(astnode));
		node5 = init(node5);
		char token5[200] = "IntNo";
		node5=notfinal(father, token5);
		intno(node5);
	}
	else if (Tab[p + 1].id == RealNo) {
		struct astnode* node5;
		node5 = (astnode*)malloc(sizeof(astnode));
		node5 = init(node5);
		char token5[200] = "RealNo";
		node5 = notfinal(father, token5);
		realno(node5);
	}
	else if (Tab[p + 1].id == Iden) {
		if (!scansymtable(Tab[p + 1].var)) {
			printf("变量未定义\n");
			exit(0);
		}
		struct astnode* node5;
		node5 = (astnode*)malloc(sizeof(astnode));
		node5 = init(node5);
		char token5[200] = "Iden";
		node5 = notfinal(father, token5);
		iden(node5);
	}
	else if (Tab[p + 1].id == '-') {
		p++;
		char token6[500] = "-";
		isfinal(father, token6);

		struct astnode* node7;
		node7 = (astnode*)malloc(sizeof(astnode));
		node7 = init(node7);
		char token7[200] = "Factor";
		node7 = notfinal(father, token7);
		Factor(node7);
	}
	else error(1);
}

void Term(astnode* father) {
	if (Tab[p + 1].id == '(' || Tab[p + 1].id == IntNo || Tab[p + 1].id == RealNo || Tab[p + 1].id == Iden) {
		struct astnode* node1;
		node1 = (astnode*)malloc(sizeof(astnode));
		node1 = init(node1);
		char token1[20] = "Factor";
		node1 = notfinal(father, token1);
		Factor(node1);

		if (Tab[p + 1].id == '*') {
			p++;
			char token2[5] = "*";
			isfinal(father, token2);

			struct astnode* node6;
			node6 = (astnode*)malloc(sizeof(astnode));
			node6 = init(node6);
			char token6[20] = "Factor";
			node6 = notfinal(father, token6);
			Factor(node6);
		}
		else if (Tab[p + 1].id == '/') {
			p++;
			char token3[5] = "/";
			isfinal(father, token3);

			struct astnode* node7;
			node7 = (astnode*)malloc(sizeof(astnode));
			node7 = init(node7);
			char token7[20] = "Factor";
			node7 = notfinal(father, token7);
			Factor(node7);
		}

	}

	else if (Tab[p + 1].id == '-') {
		p++;
		char token5[20] = "-";
		isfinal(father, token5);

		struct astnode* node4;
		node4 = (astnode*)malloc(sizeof(astnode));
		node4 = init(node4);
		char token4[20] = "Term";
		node4 = notfinal(father, token4);
		Term(node4);
	}
	else error(1);
}

void Expr(astnode* father) {
	if (Tab[p + 1].id == '('|| Tab[p + 1].id == IntNo || Tab[p + 1].id == RealNo || Tab[p + 1].id == Iden) {
		struct astnode* node1;
		node1 = (astnode*)malloc(sizeof(astnode));
		node1 = init(node1);
		char token1[20] = "Term";
		node1 = notfinal(father, token1);
		Term(node1);

		if (Tab[p + 1].id == '+') {
			p++;
			char token2[20] = "+";
			isfinal(father, token2);

			struct astnode* node6;
			node6 = (astnode*)malloc(sizeof(astnode));
			node6 = init(node6);
			char token6[20] = "Term";
			node6 = notfinal(father, token6);
			Term(node6);
		}
		else if (Tab[p + 1].id == '-') {
			p++;
			char token3[20] = "-";
			isfinal(father, token3);

			struct astnode* node7;
			node7 = (astnode*)malloc(sizeof(astnode));
			node7 = init(node7);
			char token7[20] = "Term";
			node7 = notfinal(father, token7);
			Term(node7);
		}
	}
	else if (Tab[p + 1].id == '-') {
		p++;
		char token5[20] = "-";
		isfinal(father, token5);

		struct astnode* node8;
		node8 = (astnode*)malloc(sizeof(astnode));
		node8 = init(node8);
		char token8[20] = "Expr";
		node8 = notfinal(father, token8);
		Expr(node8);
	}
	else error(1);
}

void AssignState(astnode* father) {
	if (Tab[p + 1].id == Iden) {
		if (!scansymtable(Tab[p + 1].var)) {
			printf("变量未定义\n");
			exit(0);
		}
		struct astnode* node1;
		node1 = (astnode*)malloc(sizeof(astnode));
		node1 = init(node1);
		char token1[200] = "Iden";
		node1 = notfinal(father, token1);
		iden(node1);
	}
	else error(1);
	 
	p++;
	char token2[100] = ":";
	isfinal(father, token2);

	p++;
	char token4[100] = "=";
	isfinal(father, token4);

	struct astnode* node3;
	node3 = (astnode*)malloc(sizeof(astnode));
	node3 = init(node3);
	char token3[200] = "Expr";
	node3 = notfinal(father, token3);
	Expr(node3);
}

void WBD(astnode* father) {
	p++;
	char token2[20] = "while";
	isfinal(father, token2);

	struct astnode* node1;
	node1 = (astnode*)malloc(sizeof(astnode));
	node1 = init(node1);
	char token1[20] = "BoolExpr";
	node1 = notfinal(father, token1);
	BoolExpr(node1);

	p++;
	char token3[20] = "do";
	isfinal(father, token3);
}

void IBT(astnode* father) {
	p++;
	char token2[20] = "if";
	isfinal(father, token2);

	struct astnode* node1;
	node1 = (astnode*)malloc(sizeof(astnode));
	node1 = init(node1);
	char token1[20] = "BoolExpr";
	node1 = notfinal(father, token1);
	BoolExpr(node1);

	p++;
	char token3[20] = "then";
	isfinal(father, token3);
}
void StateList(astnode* father);
void CompState(astnode* father) {
	p++;
	char token1[10] = "begin";
	isfinal(father, token1);

	if (Tab[p + 1].id != End) {
		struct astnode* node2;
		node2 = (astnode*)malloc(sizeof(astnode));
		node2 = init(node2);
		char token2[20] = "StateList";
		node2 = notfinal(father, token2);
		StateList(node2);
	}

	p++;
	char token3[20] = "end";
	isfinal(father, token3);
	if (Tab[p + 1].id == '.') {
		p++;
		char token4[20] = ".";
		isfinal(father, token4);
	}
}//或许需要考虑error

void Statement(astnode* father) {
	if (Tab[p + 1].id == Iden) {
		if (!scansymtable(Tab[p + 1].var)) {
			printf("变量未定义\n");
			exit(0);
		}
		struct astnode* node1;
		node1 = (astnode*)malloc(sizeof(astnode));
		node1 = init(node1);
		char token1[200] = "AssignState";
		node1 = notfinal(father, token1);
		AssignState(node1);
	}
	else if (Tab[p + 1].id == If) {
		struct astnode* node2;
		node2 = (astnode*)malloc(sizeof(astnode));
		node2 = init(node2);
		char token2[200] = "IBT";
		node2 = notfinal(father, token2);
		IBT(node2);

		struct astnode* node3;
		node3 = (astnode*)malloc(sizeof(astnode));
		node3 = init(node3);
		char token3[200] = "Statement";
		node3 = notfinal(father, token3);
		Statement(node3);

		if (Tab[p + 1].id == Else) {
			p++;
			char token4[200] = "else";
			isfinal(father, token4);

			struct astnode* node5;
			node5 = (astnode*)malloc(sizeof(astnode));
			node5 = init(node5);
			char token5[200] = "Statement";
			node5 = notfinal(father, token5);
			Statement(node5);
		}
	}
	else if (Tab[p + 1].id == While) {
		struct astnode* node6;
		node6 = (astnode*)malloc(sizeof(astnode));
		node6 = init(node6);
		char token6[200] = "WBD";
		node6 = notfinal(father, token6);
		WBD(node6);

		struct astnode* node7;
		node7 = (astnode*)malloc(sizeof(astnode));
		node7 = init(node7);
		char token7[200] = "Statement";
		node7 = notfinal(father, token7);
		Statement(node7);
	}
	else if (Tab[p + 1].id == Begin) {
		struct astnode* node8;
		node8 = (astnode*)malloc(sizeof(astnode));
		node8 = init(node8);
		char token8[200] = "CompState";
		node8 = notfinal(father, token8);
		CompState(node8);
	}
	else if (Tab[p + 1].id == For) {
		struct astnode* node9;
		node9 = (astnode*)malloc(sizeof(astnode));
		node9 = init(node9);
		char token9[200] = "ForLoop2";
		node9 = notfinal(father, token9);
		ForLoop2(node9);

		p++;
		char token10[200] = "do";
		isfinal(father, token10);

		struct astnode* node11;
		node11 = (astnode*)malloc(sizeof(astnode));
		node11 = init(node11);
		char token11[200] = "Statement";
		node11 = notfinal(father, token11);
		Statement(node11);
	}
	else if (Tab[p + 1].id == End || Tab[p + 1].id == ';') {}
	else error(1);
}

void StateList(astnode* father) {
	if (Tab[p + 1].id == For||Tab[p + 1].id == Iden||Tab[p+1].id==If||Tab[p+1].id==While||Tab[p+1].id==Begin||Tab[p+1].id==';') {
		if (Tab[p + 1].id != ';') {
			struct astnode* node1;
			node1 = (astnode*)malloc(sizeof(astnode));
			node1 = init(node1);
			char token1[20] = "Statement";
			node1 = notfinal(father, token1);
			Statement(node1);
		}

		if (Tab[p + 1].id == ';') {
			p++;
			char token2[4] = ";";
			isfinal(father, token2);

			struct astnode* node3;
			node3 = (astnode*)malloc(sizeof(astnode));
			node3 = init(node3);
			char token3[20] = "StateList";
			node3 = notfinal(father, token3);
			StateList(node3);
		}
		else
			error(1);
	}
	else if (Tab[p + 1].id == End) {}
	else error(1);
}

void VarList(astnode* father) {
	if (Tab[p + 1].id == Iden) {
		if (scansymtable(Tab[p + 1].var)) {
			printf("变量重复定义\n");
			exit(0);
		}
		struct astnode* node1;
		node1 = (astnode*)malloc(sizeof(astnode));
		node1 = init(node1);
		char token1[20] = "Iden";
		node1 = notfinal(father, token1);

		iden(node1);

		if (Tab[p + 1].id == ',') {
			p++;
			char token2[3] = ",";
			isfinal(father, token2);

			astnode* node3;
			node3 = (astnode*)malloc(sizeof(astnode));
			node3 = init(node3);
			char token3[20] = "VarList";
			node3 = notfinal(father, token3);
			VarList(node3);
		}
		else if (Tab[p + 1].id == ':') {}
		else error(1);
	}
	else if (Tab[p + 1].id == ':') {}
	else error(1);
}

void VarDefState(astnode* father) {
	if (Tab[p + 1].id == Iden || Tab[p + 1].id == ':') {
		if (Tab[p + 1].id != ':') {
			astnode* node1;
			node1 = (astnode*)malloc(sizeof(astnode));
			node1 = init(node1);
			char token1[20] = "VarList";
			node1 = notfinal(father, token1);
			VarList(node1);
		}
		if (Tab[p + 1].id == ':') {
			p++;
			char token2[3] = ":";
			isfinal(father, token2);
		}

		if (strcmp(Tab[p + 1].var, "integer") == 0|| strcmp(Tab[p + 1].var, "real") == 0) {
			astnode* node3;
			node3 = (astnode*)malloc(sizeof(astnode));
			node3 = init(node3);
			char token3[20] = "Type";
			node3 = notfinal(father, token3);
			Type(node3);

		}
		else
			error(1);
	}
	else if (Tab[p + 1].id == ';') {}
	else
		error(1);
}

void VarDefList(astnode* father) {
	if (Tab[p + 1].id == Iden || Tab[p + 1].id == ':') {
		if (Tab[p + 1].id != ';') {
			astnode* node1;
			node1 = (astnode*)malloc(sizeof(astnode));
			node1 = init(node1);
			char token1[20] = "VarDefState";
			node1 = notfinal(father, token1);
			VarDefState(node1);
		}
		if (Tab[p + 1].id == ';') {
			p++;
			char token2[3] = ";";
			isfinal(father, token2);
		}
		else
			error(1);

		if (Tab[p + 1].id == Iden || Tab[p + 1].id == ':' || Tab[p + 1].id == ';') {
			astnode* node2;
			node2 = (astnode*)malloc(sizeof(astnode));
			node2 = init(node2);
			char token2[20] = "VarDefList";
			node2 = notfinal(father, token2);
			VarDefList(node2);
		}
		else if (Tab[p + 1].id == Begin) {

		}
		else
			error(1);
	}

	else if (Tab[p + 1].id == Begin) {}
	
	else
		error(1);
}

void VarDef(astnode* father) {
	p++;
	char token1[10] = "var";
	isfinal(father, token1);

	if (Tab[p + 1].id != Begin) {
		astnode* node2;
		node2 = (astnode*)malloc(sizeof(astnode));
		node2 = init(node2);
		char token2[20] = "VarDefList";
		node2 = notfinal(father, token2);
		VarDefList(node2);
	}
}

void SubProg(astnode* father) {
	astnode* node1;
	node1 = (astnode*)malloc(sizeof(astnode));
	node1 = init(node1);
	char token[20] = "VarDef";
	node1=notfinal(father,token);
	VarDef(node1);

	struct astnode* node2;
	node2 = (astnode*)malloc(sizeof(astnode));
	node2 = init(node2);
	char token1[20] = "CompState";
	node2 = notfinal(father, token1);
	CompState(node2);
}

void ProgDef(astnode* father) {
	char token1[20] = "program";
	isfinal(father, token1);

	struct astnode* node2;
	node2 = (astnode*)malloc(sizeof(astnode));
	node2 = init(node2);
	char token[20] = "Iden";
	node2 = notfinal(father, token);
	iden(node2);
	initsymt();
	symid--;
	p++;
	char token3[3] = ";";
	isfinal(father, token3);

	struct astnode* node4;
	node4 = (astnode*)malloc(sizeof(astnode));
	node4 = init(node4);
	char token4[20] = "SubProg";
	node4 = notfinal(father, token4);
	SubProg(node4);
}

void Printsym() {
	for (int i = 0; i < symid; i++) {
		if (strcmp(symt[i].type, "IntNo") == 0 || strcmp(symt[i].type, "RealNo") == 0) {
			continue;
			//printf("type:%s value:%s\n", symt[i].type, symt[i].var);
		}
		else
			printf("type:%s name:%s\n", symt[i].type, symt[i].var);
	}
}

void dot(astnode* node) {
	FILE* fp = NULL;
	int i = 0;
	astnode* root;
	root = (astnode*)malloc(sizeof(astnode));
	int tmproot = 0;
	fp = fopen("D://graphz//ast.dot", "w");
	fprintf(fp, "strict digraph action {\nrankdir = n\nnode[shape = box color = blue]\nedge[color = red]\n\n");
	root = node;
	while(root != NULL){
		while (root->children[i] != NULL) {
			if (root->children[i]->id < 0) {
				queue[push] = root->children[i];
				push++;
			}
			fprintf(fp, "%d [label=\"%s\"]\n", root->name1,root->name);
			tmproot++;
			fprintf(fp, "%d [label=\"%s\"]\n", root->children[i]->name1,root->children[i]->name);
			fprintf(fp, "%d -> %d\n", root->name1, root->children[i]->name1);
			i++;
		}
		root = queue[pop];
		pop++;
		i = 0;
	}
	fputs("}\n", fp);
	fclose(fp);  //完关闭文件
}

void Printast(astnode* root) {
	int i = 0;
	while (root != NULL) {
		while (root->children[i] != NULL) {
			if (root->children[i]->id <0) {
				queue[push] = root->children[i];
				push++;
			}
			printf("%s ", root->children[i]->name);
			i++;
		}
		printf("\n");
		root = queue[pop];
		pop++;
		i = 0;
	}
}

astnode* Prog() {
	astnode* root;
	root = (astnode*)malloc(sizeof(astnode));
	root = init(root);
	strcpy(root->name, "root");
	root->name1 = 0;
	for (int i = 0; i < 20; i++) {
		strcpy(symt[i].type, "\0");
		strcpy(symt[i].var, "\0");
	}
	
	memset(queue, NULL, 200 * sizeof(astnode*));

	ProgDef(root);
	printf("\n\n*****symoltable****\n\n");
	Printsym();
	Printast(root);
	pop = 0;
	push = 0;
	dot(root);
	return root;
}