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
	sprintf(t,"t_%d",next_temp);
	next_temp++;
	return t;
}
char * newlabel()
{
	char * t;
	t = (char *) malloc(sizeof(char)*64);
	sprintf(t,"L%d",next_label);
	next_label++;
	return t;
}
char * gen_expr(char * first_code, char * second_code, char ** lval, char * expr1, char * op, char * expr2)
{
	char * result = concat(first_code,second_code);
	char * new_code;
	* lval = newtemp();
	new_code = malloc((strlen(*lval) + strlen(expr1) + strlen(op) + strlen(expr2) + 7) * sizeof(char));
	sprintf(new_code,"\t%s = %s %s %s\n",*lval,expr1,op,expr2);
	result = concat(result,new_code);
	return result;
}

char *gen_assgnmnt(char *code, char *lval, char *rval)
{
	char *result;
	if(code)
	{
		result = (char*)malloc((strlen(code) + strlen(lval) + strlen(rval) + 5)*(sizeof(char)));
		sprintf(result,"%s\t%s = %s\n",code,lval,rval);
	}
	else
	{
		result = (char*)malloc((strlen(lval) + strlen(rval) + 5)*(sizeof(char)));
		sprintf(result, "\t%s = %s\n",lval,rval);
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
	char * result = expr_code;
	char * label1 = newlabel();
	char * label2 = newlabel();
	result = concat(result, gen_cond_goto(label1,expr));
	result = concat(result,else_statements);
	result = concat(result,gen_goto(label2));
	result = concat(result,concat(label1,":"));
	result = concat(result,if_statements);
	result = concat(result,concat(label2,":"));
	return result;
}

char *gen_while(char* expr_code, char* expr, char* statements)
{
	char * result;
	char * label1 = newlabel();
	char * label2 = newlabel();
	char * label3 = newlabel();
	result = (char *)malloc((strlen(label1) +1)* (sizeof(char)));
	sprintf(result,"%s:",label1);
	result = concat(result, expr_code);
	result = concat(result,gen_cond_goto(label2,expr));
	result = concat(result,gen_goto(label3));
	result = concat(result,label2);
	result = concat(result,":");
	result = concat(result,statements);
	result = concat(result, gen_goto(label1));
	result = concat(result,label3);
	result = concat(result,":");
	return result;
}

char *gen_goto(char * label)
{
	char * result = (char *) malloc( (strlen(label)+8) * (sizeof(char)));
	sprintf(result,"\tGOTO %s\n",label);
	return result;
}
char *gen_cond_goto(char * label, char * expr)
{ 
	char * result = (char *) malloc( (strlen(label) + strlen(expr)+16) * (sizeof(char)));
	sprintf(result,"\tif ( %s ) GOTO %s\n",expr,label);
	return result;
}
