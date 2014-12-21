#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_


extern int next_temp;
extern int next_label;

char *newtemp();
char *gen_expr(char * first_code, char * second_code, char ** lval, char * expr1, char * op, char * expr2);
char *gen_assgnmnt(char *code, char *lval, char *rval);
char *gen_if(char* expr_code, char* expr, char* if_statements, char* else_statements);
char *gen_while(char* expr_code, char* expr, char* statements);
char *concat(char* first_code, char *second_code);
char *gen_goto(char * label);
char *gen_cond_goto(char * label, char * expr);
#endif
