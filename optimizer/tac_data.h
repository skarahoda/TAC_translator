#ifndef TAC_DATA_H_
#define TAC_DATA_H_

extern int next_temp;

typedef struct{
	int new_id;
	char* rhs1;
	char* rhs2;
	char * op;
	
} tac_node_t; // structure of statement "new_id = rhs1 op rhs2"

extern tac_node_t * list;

char * new_id(char * old_id);
int is_temp(char *id);
void print_if(FILE *out,char * expr, char * label);
void optimize_assignment1(FILE *out,char * lhs, char * rhs);
void optimize_assignmemt2(FILE *out,char * lhs, char * rhs1, char * op, char * rhs2);
void print_temp_assignment(FILE *out, int id, char * new_var);
#endif
