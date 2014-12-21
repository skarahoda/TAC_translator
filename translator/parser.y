%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../translator/translator.h"
#include "translator_parser.tab.h"

extern FILE *yyin;
FILE *out;
void yyerror(const char *s) {/* Called by yyparse on error*/
	printf("%s\n",s);
}

%}
%union {
char		*string;
struct
{
	char	*val;
	char	*code;
}		tac;
}
%token tFALSE tTRUE tBEGIN tEND tIF tTHEN tELSE tSEMI tASSIGN tLPAREN tRPAREN tPLUS tSTAR tLESS tEQ tAND tWHILE
%token <string> tID tINTNUM
%type <tac> Expr 
%type <string> BinOp AssgnStmt Stmt StmtLst StmtBlk IfStmt WhlStmt
%start Prgrm

%left tAND
%left tEQ
%left tLESS
%left tPLUS
%left tSTAR
%%
Prgrm:		  StmtBlk						{ if($1)fprintf(out,"%s",$1);			}
StmtBlk:	  tBEGIN StmtLst tEND					{ $$ = $2;					}
StmtLst:	  Stmt StmtLst						{ $$ = concat($1,$2);				}
		| Stmt							{ $$ = $1;					}
Stmt:		  AssgnStmt						{ $$ = $1;					}
		| IfStmt						{ $$ = $1;					}
		| WhlStmt						{ $$ = $1;					}
AssgnStmt:	  tID tASSIGN Expr tSEMI				{ $$ = gen_assgnmnt($3.code, $1, $3.val);	}
IfStmt:		  tIF tLPAREN Expr tRPAREN tTHEN StmtBlk tELSE StmtBlk	{ $$ = gen_if($3.code,$3.val,$6,$8);		}
WhlStmt:	  tWHILE tLPAREN Expr tRPAREN StmtBlk			{ $$ = gen_while($3.code,$3.val,$5);		}
Expr:		  tINTNUM						{ $$.val = $1;		$$.code= NULL;		}
		| tFALSE						{ $$.val = "false";	$$.code= NULL;		}
		| tTRUE							{ $$.val = "true";	$$.code= NULL;		}
		| tID							{ $$.val = $1;		$$.code= NULL;		}
		| tLPAREN Expr tRPAREN					{ $$.val = $2.val;	$$.code = $2.code;	}
		| Expr BinOp Expr					{ $$.code = gen_expr($1.code,$3.code,&($$.val),$1.val,$2,$3.val);	}
BinOp:		  tPLUS							{ $$ = "+";					}
		| tSTAR							{ $$ = "*";					}
		| tLESS							{ $$ = "<";					}
		| tEQ							{ $$ = "==";					}
		| tAND							{ $$ = "and";					}
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
