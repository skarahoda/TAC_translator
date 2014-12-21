#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "translator.h"
#define eos(s)((s) + strlen(s))
int next_temp=0;
int next_label=0;

char * newtemp()
{
	char * t;
	t = (char *) malloc(sizeof(char)*64);
	sprintf(t,"temp_%d",next_temp);
	next_temp++;
	return t;
}

char * gen_expr(char * first_code, char * second_code, char ** lval, char * expr1, char * op, char * expr2)
{
	char * result = concat(first_code,second_code);
	char * new_code;
	* lval = newtemp();
	new_code = malloc((strlen(*lval) + strlen(expr1) + strlen(op) + strlen(expr2) + 7) * sizeof(char));
	sprintf(new_code,"%s = %s %s %s\n",*lval,expr1,op,expr2);
	result = concat(result,new_code);
	return result;
}

char *gen_assgnmnt(char *code, char *lval, char *rval)
{
	char *result;
	if(code)
	{
		result = (char*)malloc((strlen(code) + strlen(lval) + strlen(rval) + 5)*(sizeof(char)));
		sprintf(result,"%s%s = %s\n",code,lval,rval);
	}
	else
	{
		result = (char*)malloc((strlen(lval) + strlen(rval) + 5)*(sizeof(char)));
		sprintf(result, "%s = %s\n",lval,rval);
	}
	return result;
}

char *concat(char* first_code, char *second_code)
{
	char * result = NULL;
	if(first_code)
	{
		result = (char*)realloc(first_code, (strlen(first_code) + 1) * sizeof(char));
	}
	if(second_code && result)
	{
		result = (char*)realloc(result, (strlen(result) + strlen(second_code) + 1) * sizeof(char));
		strcat(result,second_code);
	}
	else if(second_code)
	{
		result = (char*)realloc(second_code, (strlen(second_code) + 1) * sizeof(char));
	}
	return result;
}

char *gen_if(char* expr_code, char* expr, char* if_statements, char* else_statements)
{
	char * result = concat(concat(expr_code,if_statements),else_statements);
	return result;
}
char *gen_while(char* expr_code, char* expr, char* statements)
{
	char * result = concat(expr_code,statements);
	return result;
}
