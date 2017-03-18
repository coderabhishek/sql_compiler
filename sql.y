%{
#include<stdio.h>
#include<stdlib.h>
#define max_identifier_length 20
void yyerror(char *s);
int tab[52];
void update(char sym, int t);
%}

%union {int num; char token_class;}
%start line
%token print
%token exit_cmd
%token <token_class> identifier
%token <num> number
%type <num> line exp term
%type <token_class> assignment

	
%%

line  : assignment ';' {;}
      | exit_cmd ';'	{exit(0);}
      | print exp ';' {printf("Printing %d\n", $2);}
      | assignment ';' line  {;}
      | print exp  ';' line  {printf("Printing %d\n", $2);} 


assignment : identifier '=' exp {update($1, $3);}

exp : term {$$ = $1;}
    | exp '+' term {$$ = $1 + $3;}
    | exp '-' term {$$ = $1 - $3;}

term : number 	{$$ = $1;}
     | identifier {$$ = tab[$1];}

%%

void update(char sym, int t){
	tab[sym] = t;
}

int main (void) {
	/* init symbol table */
	int i;
	for(i=0; i<52; i++) {
		tab[i] = 0;
	}

	return yyparse ( );
}

void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 

