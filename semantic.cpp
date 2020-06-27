#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"

gent gen[200];
char Res[20];
gent nogen[20];

int notflag = 0;
int Notmp = -1;
int genoder = 0;
int t = 1;
char T[10];
char tmp[5] = "T";

void init2(gent gen[200]) {
	for (int i = 0; i < 200; i++) {
		gen[i].No = 0;
		strcpy(gen[i].op, "0");
		strcpy(gen[i].opNo1, "0");
		strcpy(gen[i].opNo2, "0");
		strcpy(gen[i].result, "0");
	}
}

void semant(astnode* root) {

	int i = 0;
	astnode* queuet[200];
	int popt = 0;
	int pusht = 0;
	memset(queuet, NULL, 200*sizeof(astnode*));
	strcpy(Res, "0");
	while (root != NULL) {
		i = 0;
		while (root->children[i] != NULL) {
			queuet[pusht] = root->children[i];
			pusht++;
			i++;
		}
		if (strcmp(root->name, "SubProg") == 0) {
			semant(root->children[1]);
			return;
		}
		else if (strcmp(root->name, "CompState") == 0) {
			semant(root->children[1]);
			return;
		}
		else if (strcmp(root->name, "StateList") == 0) {
			semant(root->children[0]);
			if(root->children[1]!=NULL)
				if (strcmp(root->children[1]->name, ";") == 0) {
					semant(root->children[2]);
				}
			return;
		}
		else if (strcmp(root->name, "AssignState") == 0) {
			if (root->children[1]->id == ':' && root->children[2]->id == '=') {
				semant(root->children[1]);
			}
			return;
		}
		else if (strcmp(root->name, "IBT") == 0) {
			char res[20];
			int oder = 0;
			int notoder = 0;
			char tc[20] = { "0" };
			char fc[20] = { "0" };
			notoder = genoder;
			semant(root->children[0]);
			itoa(genoder, res, 10);
			oder = genoder;
			strcpy(gen[genoder - 2].result, res);
			semant(root->bother);
			if (root->bother->bother != NULL)
				itoa(genoder + 1, res, 10);
			else
				itoa(genoder, res, 10);
			strcpy(gen[oder - 1].result, res);

			if (notflag == 1) {
				strcpy(tc, gen[notoder].result);
				strcpy(fc, gen[notoder + 1].result);
				strcpy(gen[notoder].result, fc);
				strcpy(gen[notoder + 1].result, tc);
				notflag = 0;
			}
			
			if (root->bother->bother != NULL)
			{
				gen[genoder].No = genoder;
				strcpy(gen[genoder].op, "j");
				strcpy(gen[genoder].opNo1, "-1");
				strcpy(gen[genoder].opNo2, "-1");
				strcpy(gen[genoder].result, "0");
				oder = genoder;
				genoder++;
				root = root->bother->bother->bother;
				Notmp++;
				semant(root);
				itoa(genoder, gen[oder].result, 10);
			}
				return;
		}
		else if (strcmp(root->name, "ForLoop2") == 0) {
			int oder;
			oder = genoder;
			semant(root->children[0]);
			gen[genoder].No = genoder;
			strcpy(gen[genoder].op, "j<");
			strcpy(gen[genoder].opNo1, gen[oder].result);
			semant(root->children[2]);
			strcpy(gen[genoder].opNo2, Res);
			itoa(genoder+2, gen[genoder].result, 10);
			Notmp++;
			genoder++;
			gen[genoder].No = genoder;
			strcpy(gen[genoder].op, "j");
			strcpy(gen[genoder].opNo1, "-1");
			strcpy(gen[genoder].opNo2, "-1");
			strcpy(gen[genoder].result, "0");
			Notmp++;
			genoder++;
			semant(root->bother->bother);
			gen[genoder].No = genoder;
			strcpy(gen[genoder].op, "j");
			strcpy(gen[genoder].opNo1, "-1");
			strcpy(gen[genoder].opNo2, "-1");
			itoa(oder + 1, gen[genoder].result, 10);
			Notmp++;
			genoder++;
			itoa(genoder, gen[oder + 2].result, 10);
			return;
		}
		else if (strcmp(root->name, "WBD") == 0) {
			char res[20];
			int oder = 0;
			int notoder = 0;
			char tc[20] = { "0" };
			char fc[20] = { "0" };
			notoder = genoder;
			semant(root->children[1]);
			itoa(genoder, res, 10);
			oder = genoder;
			strcpy(gen[genoder - 2].result, res);
			
			if (notflag == 1) {
				strcpy(tc, gen[notoder].result);
				strcpy(fc, gen[notoder + 1].result);
				strcpy(gen[notoder].result, fc);
				strcpy(gen[notoder + 1].result, tc);
				notflag = 0;
			}
			semant(root->bother);
			gen[genoder].No = genoder;
			strcpy(gen[genoder].op, "j");
			strcpy(gen[genoder].opNo1, "-1");
			strcpy(gen[genoder].opNo2, "-1");
			itoa(oder-2, gen[genoder].result, 10);
			Notmp++;
			genoder++;	
			itoa(genoder, res, 10);
 			strcpy(gen[oder - 1].result, res);
			return;
		}
		else if (strcmp(root->name, "if") == 0) {
			semant(root->bother);
			return;
		}
		else if (strcmp(root->name, "BootTerm") == 0) {
			semant(root->children[1]);
			if (strcmp(root->bother->name, "and") == 0 || strcmp(root->bother->name, "or") == 0)
				semant(root->bother);
			return;
		}
		else if (strcmp(root->name, "Expr") == 0) {
			if (root->children[1] != NULL) {
				if (root->children[1]->id == '+' || root->children[1]->id == '-') {
					semant(root->children[1]);
					return;
				}
			}
		}
		else if (strcmp(root->name, "and") == 0 || strcmp(root->name, "or") == 0) {
			int oder;
			oder = genoder;
			semant(root->bother);
			if (strcmp(root->name, "and") == 0) {
				itoa(oder, gen[oder - 2].result, 10);
				itoa(oder + 1, gen[oder - 1].result, 10);
			}
			else if (strcmp(root->name, "or") == 0) {

				itoa(genoder, gen[oder - 2].result, 10);
				itoa(oder, gen[oder - 1].result, 10);
			}
			return;
		}
		else if (strcmp(root->name, "not") == 0) {
			notflag = 1;
			semant(root->bother);
			return;
		}
		else if (strcmp(root->name, "Rop") == 0) {
			int no;
			char op[30] = { "j" };
			Notmp++;
			no = Notmp;
			strcat(op, root->children[0]->name);
			strcpy(gen[Notmp].op, op);
			semant(root->pro);
			strcpy(gen[no].opNo1, Res);
			semant(root->bother);
			strcpy(gen[no].opNo2, Res);
			gen[no].No = genoder;
			itoa(genoder, op, 10);
			strcpy(Res, op);
			genoder++;
			Notmp++;
			strcpy(gen[no + 1].op, "j");
			strcpy(gen[no + 1].opNo1, "-1");
			strcpy(gen[no + 1].opNo2, "-1");
			gen[no + 1].No = genoder;
			genoder++;
			return;
		}
		else if (root->id == '+') {
			int no;
			Notmp++;
			no = Notmp;
			strcpy(gen[Notmp].op, "+");
			semant(root->pro);
			strcpy(gen[no].opNo1, Res);
			semant(root->bother);
			strcpy(gen[no].opNo2, Res);
			itoa(t, T, 10);
			strcpy(tmp, "T");
			strcat(tmp, T);
			t++;
			strcpy(gen[no].result, tmp);
			strcpy(Res, tmp);
			gen[no].No = genoder;
			genoder++;
			return;
		}
		else if (root->id == ':' && root->bother->id == '=') {
			if (strcmp(root->pro->name, "Iden") == 0) {
				int no;
				Notmp++;
				no = Notmp;

				strcpy(gen[Notmp].op, ":=");
				strcpy(gen[Notmp].result, root->pro->children[0]->name);
				semant(root->bother->bother);
				strcpy(gen[no].opNo1, Res);
				gen[no].No = genoder;
				genoder++;
				return;
			}
		}
		else if (root->id == '-') {
			int no;
			Notmp++;
			no = Notmp;
			strcpy(gen[Notmp].op, "-");
			semant(root->pro);
			strcpy(gen[no].opNo1, Res);
			semant(root->bother);
			strcpy(gen[no].opNo2, Res);
			strcpy(tmp, "T");
			itoa(t, T, 10);
			strcat(tmp, T);
			t++;
			strcpy(gen[no].result, tmp);
			strcpy(Res, tmp);
			gen[no].No = genoder;
			genoder++;
			return;
		}
		else if (root->id == '*') {
			int no;
			Notmp++;
			no = Notmp;
			strcpy(gen[Notmp].op, "*");
			semant(root->pro);
			strcpy(gen[no].opNo1, Res);
			semant(root->bother);
			strcpy(gen[no].opNo2, Res);
			strcpy(tmp, "T");
			itoa(t, T, 10);
			strcat(tmp, T);
			t++;
			strcpy(gen[no].result, tmp);
			strcpy(Res, tmp);
			gen[no].No = genoder;
			genoder++;
			return;

		}
		else if (root->id == '/') {
			int no;
			Notmp++;
			no = Notmp;
			strcpy(gen[Notmp].op, "/");
			semant(root->pro);
			strcpy(gen[no].opNo1, Res);
			semant(root->bother);
			strcpy(gen[no].opNo2, Res);
			strcpy(tmp, "T");
			itoa(t, T, 10);
			strcat(tmp, T);
			t++;
			strcpy(gen[no].result, tmp);
			strcpy(Res, tmp);
			gen[no].No = genoder;
			genoder++;
			return;
		}
		
		else if (strcmp(root->name, "Iden") == 0) {
			strcpy(Res, root->children[0]->name);
			//return;
		}
		else if (strcmp(root->name, "IntNo") == 0) {
			strcpy(Res, root->children[0]->name);
			//return;
		}
		else if (strcmp(root->name, "RealNo") == 0) {
			strcpy(Res, root->children[0]->name);
			//return;
		}

		root = queuet[popt];
		popt++;
	}

}