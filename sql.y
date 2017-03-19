%{
#include<stdio.h>
#include<stdlib.h>
#define YYDEBUG 1
#define max_identifier_length 20
extern char *yytext;
void yyerror(char *s);
typedef struct node{
	int cptr;
	struct node *child[50];
	char s[50];
} node;

node *create(char *s){
	node *nd = (node *)malloc(sizeof(node));
	strcpy(nd->s, s);
	nd->cptr = 0;
	return nd;
}
void print(node *);
%}
%union {char str[100]; struct node *nd_ptr;}
%start program
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
%token <str> COMPARATOR
%token COMMA
%token EQUAL 
%token VALUES 
%token <str> identifier
%token <str> constant_val
%token <str> numeric_val	
%type <nd_ptr> drop 
%type <nd_ptr> constraint
%type <nd_ptr> vb 
%type <nd_ptr> deletion 
 %type <nd_ptr> alter_add 
%type <nd_ptr> alter_drop 
%type <nd_ptr> updation_list
%type <nd_ptr> updation  
%type <nd_ptr> insertion 
%type <nd_ptr> fields
%type <nd_ptr> v_val 
%type <nd_ptr> value_list 
 %type <nd_ptr> schema_list 
 %type <nd_ptr> selection 
 %type <nd_ptr> sel 
 %type <nd_ptr> creation  
 %type <nd_ptr> query 
 %type <nd_ptr> tab_list 
 %type <nd_ptr> colums 
%type <str> WILDCARD
%type <nd_ptr> id_list 
%type <str> CONJUNCTION  
%type <nd_ptr> condition 

%%

program : line program | line
line  : deletion {print($1);}| alter_add {print($1);}| alter_drop {print($1);}| insertion {print($1);}| updation {print($1);}| creation  {		
print($1);} | selection {print($1);}| drop {print($1);}

creation : CREATE TABLE identifier BO schema_list BC COL  { 
	 			node *nd = create("STATEMENT");
				nd->child[(nd->cptr++)] = create("CREATE");
			  	nd->child[(nd->cptr++)] = create("TABLE");
				nd->child[(nd->cptr++)] = create($3);
				nd->child[(nd->cptr++)] = create("(");
				nd->child[(nd->cptr++)] = $5;
				nd->child[(nd->cptr++)] = create(")");	
				//	printf("\n*****************  %d   %s  **********\n\n", nd->child[4]->child[2]->cptr, nd->child[4]->child[2]->s);
		
		$$ = nd;				
		}

schema_list : identifier identifier constraint COMMA schema_list {
			node *nd = create("SCHEMA_LIST");
			nd->child[(nd->cptr++)] = create($1);
			nd->child[(nd->cptr++)] = create($1);
			nd->child[(nd->cptr++)] = $3;
			nd->child[(nd->cptr++)] = create(",");
			nd->child[(nd->cptr++)] = $5;
			$$ = nd;	   
} 
	    | identifier identifier constraint {
			node *nd = create("SCHEMA_LIST");
			nd->child[(nd->cptr++)] = create($1);
			nd->child[(nd->cptr++)] = create($2);
			nd->child[(nd->cptr++)] = $3;
					$$ = nd;
}

selection : sel WHERE constraint COL{
	  	node *nd = create("SELECTION");
		nd->child[(nd->cptr++)] = $1;
		nd->child[(nd->cptr++)] = create("WHERE");
		nd->child[(nd->cptr++)] = $3;
		$$ = nd;
} |  sel WHERE constraint {
	  	node *nd = create("SELECTION");
		nd->child[(nd->cptr++)] = $1;
		nd->child[(nd->cptr++)] = create("WHERE");
		nd->child[(nd->cptr++)] = $3;
		$$ = nd;
} 
| sel COL {
	node *nd = create("SELECTION");
	nd->child[(nd->cptr++)] = $1;
	$$ = nd;
} | 
sel {
	node *nd = create("SELECTION");
	nd->child[(nd->cptr++)] = $1;
	$$ = nd;
}
sel : SELECT query {
	node *nd = create("SELECTION SUBCLASS");
	nd->child[(nd->cptr++)] = create("SELECT");   
	nd->child[(nd->cptr++)] = $2;
	$$ = nd;
} 

query : colums FROM tab_list  {
	node *nd = create("QUERY");
	nd->child[(nd->cptr++)] = $1;
	nd->child[(nd->cptr++)] = create("FROM");
	nd->child[(nd->cptr++)] = $3;
	$$ = nd;
}

tab_list : id_list {
	node *nd = create("TAB_LIST");
	nd->child[(nd->cptr++)] = $1;
	$$ = nd;	 
}
	 | BO selection BC {
	node *nd = create("TAB_LIST");
	nd->child[(nd->cptr++)] = create("(");
	nd->child[(nd->cptr++)] = $2;
	nd->child[(nd->cptr++)] = create(")");
	$$ = nd;
}

colums : WILDCARD {
	node *nd = create("COLUMNS");
	nd->child[(nd->cptr++)] = create("*");
	$$ = nd;      
} | id_list {
	node *nd = create("COLUMNS");
	nd->child[(nd->cptr++)] = $1;
	$$ = nd;
}

insertion : INSERT INTO identifier vb BO value_list BC COL {
	node *nd = create("INSERT STATEMENT");
	nd->child[(nd->cptr++)] = create("INSERT");
	nd->child[(nd->cptr++)] = create("INTO");
	nd->child[(nd->cptr++)] = create($3);
	nd->child[(nd->cptr++)] = $4;
	nd->child[(nd->cptr++)] = create("(");
	nd->child[(nd->cptr++)] = $6;
	nd->child[(nd->cptr++)] = create(")");
	$$ = nd; 
}

vb: fields VALUES{
	node *nd = create("FIELDS/VALUE");
	nd->child[(nd->cptr++)] = $1;
	nd->child[(nd->cptr++)] = create("VALUES");
	$$ = nd;	
}
 | VALUES {
	$$ = create("VALUES");
}


fields :  BO id_list BC {
	node *nd = create("FIELDS");
	nd->child[(nd->cptr++)] = create("(");
	nd->child[(nd->cptr++)] = $2;
	nd->child[(nd->cptr++)] = create(")");
	$$ = nd;       
}

value_list : v_val COMMA value_list {
	node *nd = create("VALUE_LIST");
	nd->child[(nd->cptr++)] = $1;
	nd->child[(nd->cptr++)] = create(",");
	nd->child[(nd->cptr++)] = $3;
	$$ = nd;	   
} 
	| v_val{
	$$ = create("VALUES_LIST");
	$$->child[($$->cptr++)] = $1;
} 


updation : UPDATE identifier SET updation_list WHERE constraint COL{
	node *nd = create("UPDATE STATEMENT");
	nd->child[(nd->cptr++)] = create("UPDATE");
	nd->child[(nd->cptr++)] = create($2);
	nd->child[(nd->cptr++)] = create("SET");
	nd->child[(nd->cptr++)] = $4;
	nd->child[(nd->cptr++)] = create("WHERE");
	nd->child[(nd->cptr++)] = $6;
	$$ = nd;	 
}

updation_list : identifier EQUAL constant_val COMMA updation_list {
//	printf("UUUUUUUUUUUUUUUUUUUUUU\n");
	node *nd = create("UPDATION_LIST");
	nd->child[(nd->cptr++)] = create($1);
	nd->child[(nd->cptr++)] = create("=");
	nd->child[(nd->cptr++)] = create($3);
	nd->child[(nd->cptr++)] = create(",");
	nd->child[(nd->cptr++)] = $5;
	$$ = nd;	
}
| identifier EQUAL constant_val {
//	printf("DONEEEEEEEE\n");
	node *nd = create("UPDATION_LIST");
	nd->child[(nd->cptr++)] = create($1);
	nd->child[(nd->cptr++)] = create("=");
	nd->child[(nd->cptr++)] = create($3);
	$$ = nd;
}

deletion : DELETE FROM identifier WHERE constraint COL {
	node *nd = create("DELETION");
	nd->child[(nd->cptr++)] = create("DELETE");
	nd->child[(nd->cptr++)] = create("FROM");
	nd->child[(nd->cptr++)] = create($3);
	nd->child[(nd->cptr++)] = create("WHERE");
	nd->child[(nd->cptr++)] = $5;
	$$ = nd;
}

alter_drop : ALTER TABLE identifier DROP COLUMN identifier COL {
	node *nd = create("ALTER_DROP");
	nd->child[(nd->cptr++)] = create("ALTER");
	nd->child[(nd->cptr++)] = create("TABLE");
	nd->child[(nd->cptr++)] = create($3);
	nd->child[(nd->cptr++)] = create("DROP");
	nd->child[(nd->cptr++)] = create("COLUMN");
	nd->child[(nd->cptr++)] = create($6);
	$$ = nd;	   
}

alter_add : ALTER TABLE identifier ADD identifier identifier COL  {
	node *nd = create("ALTER_ADD");
	nd->child[(nd->cptr++)] = create("ALTER");
	nd->child[(nd->cptr++)] = create("TABLE");
	nd->child[(nd->cptr++)] = create($3);
	nd->child[(nd->cptr++)] = create("ADD");
	nd->child[(nd->cptr++)] = create($5);
	nd->child[(nd->cptr++)] = create($6);
	$$ = nd;
}	  

constraint : condition CONJUNCTION constraint {
		node *nd = create("CONSTRAINT");
		nd->child[(nd->cptr++)] = $1;
		nd->child[(nd->cptr++)] = create($2);
		nd->child[(nd->cptr++)] = $3;
		$$ = nd;	
   }
	| condition {node *nd = create("CONSTRAINT"); nd->child[(nd->cptr++)] = $1;
$$ = nd;}
id_list : identifier COMMA id_list {
	node *nd = create("ID_LIST");
	nd->child[(nd->cptr++)] = create($1);
	nd->child[(nd->cptr++)] = create(",");
	nd->child[(nd->cptr++)] = $3;
$$ = nd;
} | identifier {$$ = create($1);} 
drop : DROP TABLE identifier COL {
		node *nd = create("drop production"); 
		nd->child[(nd->cptr++)] = create("DROP");
		nd->child[(nd->cptr++)] = create("TABLE");
		nd->child[(nd->cptr++)] = create($3);
		$$ = nd;		
}

condition : identifier COMPARATOR identifier {
	node *nd = create("CONDITION");
	nd->child[(nd->cptr++)] = create($1);
	 nd->child[(nd->cptr++)] = create($2);
	nd->child[(nd->cptr++)] = create($3);
 	$$ = nd;
		  
}| v_val COMPARATOR v_val {
	node *nd = create("CONDITION");
	nd->child[(nd->cptr++)] =$1;
	 nd->child[(nd->cptr++)] = create($2);
	nd->child[(nd->cptr++)] =$3;
 	$$ = nd;
		  
}| v_val COMPARATOR identifier {
	node *nd = create("CONDITION");
	nd->child[(nd->cptr++)] = $1;
	 nd->child[(nd->cptr++)] = create($2);
	nd->child[(nd->cptr++)] = create($3);
 	$$ = nd;
		  
}| identifier COMPARATOR v_val {
	node *nd = create("CONDITION");
	nd->child[(nd->cptr++)] = create($1);
	 nd->child[(nd->cptr++)] = create($2);
	//printf(" 11111111111  %s\n", $2);
	nd->child[(nd->cptr++)] = $3;
// 	printf(" 22222222222  %s\n", $3->s);
	$$ = nd;
		  
}


v_val : constant_val {
node *nd = create($1); 
                $$ = nd; 
			}
      | numeric_val {	node *nd = create($1); 
                   	$$ = nd;
			} 


%%

int main (void) {
	/* init symbol table */
//	yydebug = 1;
	return yyparse ( );
}

void print(node *nd){
//printf("\n*****************  %d   %s  ******  PLEASE PLEASE ****\n\n", nd->child[4]->child[2]->cptr, nd->child[4]->child[2]->s);

//	printf("|| %s ||\n", nd->s);
	node *q[1000];
	int parent[1000];
	node *mark = (node *)malloc(sizeof(node));
	int s = 0, e = 0;
	q[e++] = nd;
	parent[e-1] = -1;
	q[e++] = NULL;
	parent[e-1] = -2;
	while(e!=s){
		if(q[s] == NULL){
			s++;
			printf("\n\n");
			if(e!=s)
			q[e++] = NULL;
			if(q[s]!=NULL)
				printf("%s -> ", q[parent[s]]->s);
			parent[e-1] = -2;			
			continue;
		}
		if(q[s] == mark){
			s++;
			printf("       ||");
			if(q[s]!=NULL)
			printf("%s -> ", q[parent[s]]->s);
			continue;
		}
		printf("  %s  ", q[s]->s);
		for(int i=0;i<q[s]->cptr;++i){
			q[e++] = q[s]->child[i];
			parent[e-1] = s;
			//printf("Child %d --> %s\n", i, q[s]->child[i]->s);
		}
		q[e++] = mark;
		parent[e-1] = s;
		s++;
	}
	printf("\n\n\n\n");
}

void yyerror (char *s) {
  printf("SYNTAX ERROR NEAR : '%s'\n", yytext);

}
