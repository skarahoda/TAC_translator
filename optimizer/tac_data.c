#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tac_data.h"
int next_temp_id=0;
tac_node_t * list = NULL;

void print_if(FILE *out,char * expr, char * label)
{
	int id;
	char temp[16];
	if(is_temp(expr))
	{
		sscanf(expr,"t_%d",&id);
		sprintf(temp,"t_%d",list[id].new_id);
		print_temp_assignment(out, id, temp);
		fprintf(out,"\tif ( %s ) GOTO %s\n",temp,label);
		next_temp_id--;
	}
	else
	{
		fprintf(out,"\tif ( %s ) GOTO %s\n",expr,label);
	}
}

void optimize_assignment1(FILE *out,char * lhs, char * rhs)
{
	int id, rhs1_id,rhs2_id;
	if(is_temp(rhs))
	{
		sscanf(rhs,"t_%d",&id);
		print_temp_assignment(out,id,lhs);
		next_temp_id--;
	}
	else
	{
		fprintf(out,"\t%s = %s\n",lhs,rhs);
	}
}

void optimize_assignmemt2(FILE *out,char * lhs, char * rhs1, char * op, char * rhs2)
{
	int rhs1_id, rhs2_id, lhs_id;
	if(is_temp(lhs))
	{
		sscanf(lhs,"t_%d",&lhs_id);
		list = (tac_node_t *)realloc(list,(sizeof(tac_node_t)*(lhs_id + 1)));
		list[lhs_id].op = strdup(op);
		list[lhs_id].rhs1 = strdup(rhs1);
		if(is_temp(rhs1))
		{
			next_temp_id--;
		}
		list[lhs_id].rhs2 = strdup(rhs2);
		if(is_temp(rhs2))
		{
			next_temp_id--;
		}
		list[lhs_id].new_id = next_temp_id;
		next_temp_id++;
	}
	else
	{
		if(is_temp(rhs1))
		{
			sscanf(rhs1,"t_%d",&rhs1_id);
			free(rhs1);
			rhs1 = (char *)malloc(sizeof(char)*16);
			sprintf(rhs1,"t_%d",list[rhs1_id].new_id);
			print_temp_assignment(out,rhs1_id,rhs1);
			next_temp_id--;
		}
		if(is_temp(rhs2))
		{
			sscanf(rhs2,"t_%d",&rhs2_id);
			free(rhs2);
			rhs2 = (char *)malloc(sizeof(char)*16);
			sprintf(rhs2,"t_%d",list[rhs2_id].new_id);
			print_temp_assignment(out,rhs2_id,rhs2);
			next_temp_id--;
		}
		fprintf(out,"\t%s = %s %s %s", lhs, rhs1, op, rhs2);
	}
}

int is_temp(char *id)
{
	return ((id[0] == 't') && (id[1] == '_'));
}

void print_temp_assignment(FILE *out, int id, char * new_val)
{
	int rhs1, rhs2;
	char string_rhs1[16];
	char string_rhs2[16];
	strcpy(string_rhs1,list[id].rhs1);
	strcpy(string_rhs2,list[id].rhs2);
	if(is_temp(string_rhs1))
	{
		sscanf(string_rhs1,"t_%d",&rhs1);
		sprintf(string_rhs1,"t_%d",list[rhs1].new_id);
		print_temp_assignment(out, rhs1, string_rhs1);
	}
	if(is_temp(string_rhs2))
	{
		sscanf(string_rhs2,"t_%d",&rhs2);
		sprintf(string_rhs2,"t_%d",list[rhs2].new_id);
		print_temp_assignment(out, rhs2, string_rhs2);
	}
	fprintf(out,"\t%s = %s %s %s\n",new_val,string_rhs1,list[id].op,string_rhs2);
}
