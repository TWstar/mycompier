#pragma warning(disable:4996)
#include<stdio.h>
#include <string.h>
#include <memory.h>
#include "lex.h"
#include <algorithm>
using namespace std;

struct Vartab Tab[100];
int cnt = 0;//结构体数组下标
char Text[100000] = { '/0' };//文本
int len;//文本长度
extern int p;

void error(int t) {
	if (t == 0)
		printf("词法错误: %s %s\n",Tab[p-1].var,Tab[p].var);
	else if (t == 1) {
		printf("语法错误: %s %s\n",Tab[p-1].var,Tab[p].var);
	}
	else
		printf("其他错误\n");
	//exit(0);
}

void delete_nono(int p) {
	char Txtmp[100000] = { '/0' };
	int k = 0;
	int i = 0;
	for (i = 0, k = 0; i < p; i++) {
		if (Text[i] == '/' && Text[i + 1] == '/') {
			for (int j = i; j < p; j++) {
				if (Text[j] == '\n') {
					i = j;//检测到匹配的注释，删除这一行，从注释后开始赋值
					break;
				}
			}
		}
		else if (Text[i] == '/' && Text[i + 1] == '*') {
			for (int j = i; j < p; j++) {
				if (Text[j] == '*' && Text[j + 1] == '/') {
					i = j + 2;//检测到匹配的注释，从注释后开始赋值
					break;
				}
			}
		}
		else if (Text[i] == '\n' || Text[i] == '\t' || Text[i] == '\v' || Text[i] == '\r' || Text[i] == ' ') {
			while (Text[i] == '\n' || Text[i] == '\t' || Text[i] == '\v' || Text[i] == '\r' || Text[i + 1] == ' ') {
				if (Text[i + 1] != '\n' && Text[i + 1] != '\t' && Text[i + 1] != '\v' && Text[i + 1] != '\r' && Text[i + 1] != ' ')
					break;
				i++;
			}//消除多个tab或者换行等多余符号
			if (k > 0) {
				if (Txtmp[k - 1] != ' ') {
					Txtmp[k] = ' ';
					k++;
				}
			}//消除多个空格
		}
		else {
			if (Text[i] >= 'A' && Text[i] <= 'Z')
				Txtmp[k] = Text[i] + 32;
			else
				Txtmp[k] = Text[i];
			k++;
		}
	}
	Txtmp[k-1] = '\0';
	len = strlen(Txtmp);
	strcpy(Text, Txtmp);
	printf("过滤之后的程序:\n%s\n", Text);
}
bool isletter(char ch) {
	if (ch >= 'a' && ch <= 'z')
		return true;
	else
		return false;
}
//判断是否是关键字或者变量

bool digit(char ch) {
	if (ch <= '9' && ch >= '0')
		return true;
	else
		return false;
}

bool IDs(char ch) {
	if (digit(ch) || isletter(ch)||ch=='_')
		return true;
	else
		return false;
}

void judgeIDorkey(char var[]) {
	if (strcmp(var, "program")==0) {
		strcpy(Tab[cnt].var, "program");
		Tab[cnt].id = Program;
		cnt++;
	}
	else if (strcmp(var, "begin")==0) {
		strcpy(Tab[cnt].var, "begin");
		Tab[cnt].id = Begin;
		cnt++;
	}
	else if (strcmp(var, "end")==0) {
		strcpy(Tab[cnt].var, "end");
		Tab[cnt].id = End;
		cnt++;
	}
	else if (strcmp(var, "var")==0) {
		strcpy(Tab[cnt].var, "var");
		Tab[cnt].id = Var;
		cnt++;
	}
	else if (strcmp(var, "integer")==0) {
		strcpy(Tab[cnt].var, "integer");
		Tab[cnt].id = Integer;
		cnt++;
	}
	else if (strcmp(var, "real")==0) {
		strcpy(Tab[cnt].var, "real");
		Tab[cnt].id = Real;
		cnt++;
	}
	else if (strcmp(var, "while")==0) {
		strcpy(Tab[cnt].var, "while");
		Tab[cnt].id = While;
		cnt++;
	}
	else if (strcmp(var, "do")==0) {
		strcpy(Tab[cnt].var, "do");
		Tab[cnt].id = Do;
		cnt++;
	}
	else if (strcmp(var, "if")==0) {

		strcpy(Tab[cnt].var, "if");
		Tab[cnt].id = If;
		cnt++;
	}
	else if (strcmp(var, "then")==0) {
		strcpy(Tab[cnt].var, "then");
		Tab[cnt].id = Then;
		cnt++;
	}
	else if (strcmp(var, "else")==0) {
		strcpy(Tab[cnt].var, "else");
		Tab[cnt].id = Else;
		cnt++;
	}
	else if (strcmp(var, "or")==0) {
		strcpy(Tab[cnt].var, "or");
		Tab[cnt].id = Or;
		cnt++;
	}
	else if (strcmp(var, "and")==0) {
		strcpy(Tab[cnt].var, "and");
		Tab[cnt].id = And;
		cnt++;
	}
	else if (strcmp(var, "not")==0) {
		strcpy(Tab[cnt].var, "not");
		Tab[cnt].id = Not;
		cnt++;
	}
	else if (strcmp(var, "for") == 0) {
		strcpy(Tab[cnt].var, "for");
		Tab[cnt].id = For;
		cnt++;
	}
	else if (strcmp(var, "to") == 0) {
		strcpy(Tab[cnt].var, "to");
		Tab[cnt].id = To;
		cnt++;
	}
	else {
		strcpy(Tab[cnt].var, var);
		Tab[cnt].id = Iden;
		cnt++;
	}
}

void tokenanalyze(char token[], int k) {
	char var[200] = { '\0' };
	int j = 0;
	int dot = 0;

	for (int i = 0; i < k; i++) {
 		if (isletter(token[i])||token[i]=='_') {
			while (IDs(token[i])) {
				var[j] = token[i];
				i++;
				j++;
			}
			judgeIDorkey(var);
		}

		else if (digit(token[i])) {
			while (digit(token[i]) || token[i] == '.' && digit(token[i + 1])) {
				var[j] = token[i];
				if (token[i] == '.')
					dot = 1;
				i++;
				j++;
			}
			if (dot == 1) {
				strcpy(Tab[cnt].var, var);
				Tab[cnt].id = RealNo;
				cnt++;
			}
			else
			{
				strcpy(Tab[cnt].var, var);
				Tab[cnt].id = IntNo;
				cnt++;
			}
		}
		memset(var, '\0', 200);
		j = 0;
		switch (token[i])
		{
		case '.':
			strcpy(Tab[cnt].var, ".");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case ',':
			strcpy(Tab[cnt].var, ",");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case '<':
			if (token[i + 1] == '=') {
				strcpy(Tab[cnt].var, "LE");
				Tab[cnt].id = LE;
				i++;
				cnt++;
			}
			else {
				strcpy(Tab[cnt].var, "<");
				Tab[cnt].id = (int)token[i];
				cnt++;
			}
			break;
		case '=':
			strcpy(Tab[cnt].var, "=");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case '>':
			if (token[i + 1] == '=') {
				strcpy(Tab[cnt].var, "GE");
				Tab[cnt].id = GE;
				i++;
				cnt++;
			}
			else {
				strcpy(Tab[cnt].var, ">");
				Tab[cnt].id = (int)token[i];
				cnt++;
			}
			break;
		case '+':
			strcpy(Tab[cnt].var, "+");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case '-':
			strcpy(Tab[cnt].var, "-");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case '*':
			strcpy(Tab[cnt].var, "*");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case '/':
			strcpy(Tab[cnt].var, "/");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case '(':
			strcpy(Tab[cnt].var, "(");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case ')':
			strcpy(Tab[cnt].var, ")");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case ';':
			strcpy(Tab[cnt].var, ";");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case ':':
			strcpy(Tab[cnt].var, ":");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case '!':

			if (token[i + 1] == '=') {
				strcpy(Tab[cnt].var, "NE");
				Tab[cnt].id = NE;
				i++;
				cnt++;
			}
			else {
				strcpy(Tab[cnt].var, "Not");
				Tab[cnt].id = Not;
				cnt++;
			}
			break;
		case '[':
			strcpy(Tab[cnt].var, "[");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case ']':
			strcpy(Tab[cnt].var, "]");
			Tab[cnt].id = (int)token[i];
			cnt++;
			break;
		case ' ':
			continue;
		case '\0':
			break;
		default:
			error(0);
			break;
		}
	}
}

void gettoken() {

	char token[200] = { '\0' };
	int k = 0;

	for (int i = 0; i < len; i++) {
		if (Text[i] == ' ')
			continue;
		else {
			while (Text[i] != ' '&&i<len) {
				token[k] = Text[i];
				k++;
				i++;
			}
			tokenanalyze(token, k);
			
		}
		memset(token, '\0', 200);
		k = 0;
	}
}

int compare(gent x, gent y) {
	if (x.No < y.No)
		return 1;
	else
		return 0;
}

int main()
{
	extern int pop;
	extern int push;
	//打开测试文件
	int p = 0;//文件读入指针
	astnode* root;
	extern gent gen[200];
	extern int genoder;
	FILE* fp = NULL;
	fp = fopen("C:\\Users\\Administrator\\Documents\\GitHub\\miniPascal-parser\\test\\test_if_then_else.txt", "r");
	while (!feof(fp))
		Text[p++] = fgetc(fp);
	Text[p] = '\0';
	fclose(fp);
	len = strlen(Text);
	printf("%s\n", Text);
	//删除注释和实现不区分大小写
	delete_nono(p);
	gettoken();
	printf("\n\n*****二元式*****\n\n");
	for (int i = 0; i < cnt; i++) {
		printf("(%s,%d)\n", Tab[i].var, Tab[i].id);
	}
	root=Prog();

	init2(gen);
	pop = 10;
	push = 10;
	semant(root);
	sort(gen, gen+genoder, compare);
	
	strcpy(gen[genoder].op, "end");
	strcpy(gen[genoder].opNo1, "-1");
	strcpy(gen[genoder].opNo2, "-1");
	strcpy(gen[genoder].result, "end");
	int i = 0;
	while (strcmp(gen[i].op, "0") != 0) {

		printf("(%d) (%s, %s, %s, %s)\n",i, gen[i].op, gen[i].opNo1, gen[i].opNo2, gen[i].result);
		i++;
	}
}