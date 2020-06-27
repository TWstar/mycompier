#pragma once

#include <stdio.h>
#define Iden		300

#define IntNo		301
#define	RealNo		302

/*Define  keywords here:*/
#define	Program		400
#define	Begin		401
#define	End			402
#define	Var			403
#define Integer		404
#define	Real		405
#define	While		406
#define	Do			407
#define	If			408
#define	Then		409
#define	Else		410
#define	Or			411
#define	And			412
#define	Not			413
#define	To			414
#define	For			415

/*Define double_character terminates: */
#define	LE			416
#define	GE			417
#define	NE			418

struct Vartab {
	int id = 0;
	char var[100] = { '\0' };
};
struct astnode {
	char name[200] = { '\0' };
	int id = 0;
	int name1 = 0;
	astnode* father = NULL;
	astnode* bother;//下一个兄弟节点
	astnode* pro;//前驱节点
	astnode* children[10] = { NULL };
};

struct gent {
	int No = 0;
	char op[50] = { "\0" };
	char opNo1[100] = { "\0" };
	char opNo2[100] = { "\0" };
	char result[100] = { "\0" };
};

void error(int t);
astnode* Prog();
void semant(astnode* root);
void Expr(astnode* father);
void BoolExpr(astnode* father);
void init2(gent gen[200]);
