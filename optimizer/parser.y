%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../optimizer/tac_data.h"
#include "optimizer_parser.tab.h"
extern FILE *yyin;
FILE *out;
void yyerror(const char *s) {/* Called by yyparse on error*/
	printf("%s\n",s);
}

%}
%union
{
	char * string;
}
%token tFALSE tTRUE tIF tCOLON tASSIGN tLPAREN tRPAREN tGOTO tPLUS tSTAR tLESS tEQ tAND
%token <string> tID tINTNUM
%type <string> Op Operand
%start Prgrm

%left tAND
%left tEQ
%left tLESS
%left tPLUS
%left tSTAR
%%
Prgrm:		  StmtLst
StmtLst:	  Stmt StmtLst
		| Stmt
Stmt:		  AssgnStmt
		| IfStmt
		| LblStmt
		| GotoStmt
AssgnStmt:	  tID tASSIGN Operand				{ optimize_assignment1(out,$1,$3);	}
		| tID tASSIGN Operand Op Operand		{ optimize_assignmemt2(out,$1,$3,$4,$5);}
IfStmt:		  tIF tLPAREN Operand tRPAREN tGOTO tID		{ print_if(out,$3,$6);			}
LblStmt:	  tID tCOLON					{ fprintf(out,"%s:\n",$1);		}
GotoStmt:	  tGOTO tID					{ fprintf(out,"\tGOTO %s\n",$2);	}
Operand:	  tINTNUM					{ $$ = strdup($1);			}
		| tID						{ $$ = strdup($1);			}
		| tFALSE					{ $$ = strdup("false");			}
		| tTRUE						{ $$ = strdup("true");			}
Op:		  tPLUS						{ $$ = strdup("+");			}
		| tSTAR						{ $$ = strdup("*");			}
		| tLESS						{ $$ = strdup("<");			}
		| tEQ						{ $$ = strdup("==");			}
		| tAND						{ $$ = strdup("and");			}
%%

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("###INVALID ARGUMENTS###\n");
		printf("please command: <PATH>/pl4 <input_file> <output_file>\n");
		return 1;
	}
	yyin = fopen(argv[1],"r");
	out = fopen(argv[2],"w");
	if(yyparse())
	{
		printf("ERROR\n");
		return 1;
	}
	else
	{
		printf("OK\n");
		return 0;
	}
}
