%{
#include<stdio.h>
#include<stdlib.h>
#define YYDEBUG 1
#define max_identifier_length 20
void yyerror(char *s);
%}

%union {char token_class[100];}
%start line
%token type
%token SELECT
%token INTO
%token UPDATE
%token FROM 
%token CONJUNCTION
%token SET
%token DROP
%token ALTER
%token COLUMN
%token ADD
%token INT
%token VARCHAR
%token CHAR
%token NOT
%token COL
%token NUL
%token UNIQUE
%token WILDCARD
%token CREATE
%token TABLE
%token INSERT
%token DELETE
%token WHERE
%token COLON
%token BO
%token BC
%token COMPARATOR
%token COMMA
%token EQUAL 
%token VALUES 
%token <token_class> identifier
%token <token_class> constant_val
%token <token_class> numeric_val	

%%

line  : deletion | alter_add | alter_drop | insertion | updation | creation  | selection | drop {printf("DROPPING!!\n");}

creation : CREATE TABLE identifier BO schema_list BC COL

schema_list : identifier identifier constraint COMMA schema_list 
	    | identifier identifier constraint

selection : sel WHERE constraint COL | sel COL
sel : SELECT query 

query : colums FROM tab_list  

tab_list : identifier | BO selection BC 

colums : WILDCARD | id_list

insertion : INSERT INTO identifier vb BO value_list BC COL

vb: fields VALUES | VALUES
fields :  BO id_list BC

value_list : v_val COMMA value_list | v_val


updation : UPDATE identifier SET updation_list WHERE constraint COL

updation_list : identifier EQUAL constant_val COMMA updation_list | identifier EQUAL constant_val 

deletion : DELETE FROM identifier WHERE constraint COL

alter_drop : ALTER TABLE identifier DROP COLUMN identifier COL

alter_add : ALTER TABLE identifier ADD identifier identifier COL 


constraint : condition CONJUNCTION constraint | condition
id_list : identifier COMMA id_list | identifier 
drop : DROP TABLE identifier COL

condition : identifier COMPARATOR identifier | v_val COMPARATOR v_val | v_val COMPARATOR identifier | identifier COMPARATOR v_val
v_val : constant_val | numeric_val 


%%

int main (void) {
	/* init symbol table */
	yydebug = 1;
	return yyparse ( );
}

void yyerror (char *s) {printf("%s\n", s);} 

