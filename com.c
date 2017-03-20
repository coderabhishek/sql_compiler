#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include "com.h"
#define true 1
#define false 0

int ch = 0;
int maxp = 0;
node *memory[100];
int mem_ptr = 0;

typedef short int bool;

char input[1000000];
int ptr = 0;



int up(){
	ptr++;
//	printf("MX --> %d\n", maxp);
	maxp = (maxp>ptr?maxp:ptr);
	return ptr-1;
}
void sanitize(){
	int r=0, w=0;
	while(input[r]!='\0'){
		if(input[r] == ' ' || input[r] == '\n')
			r++;
		else input[w++] = input[r++];
	}
	input[w] = '\0';
}

node *match(char *s, int i){
	if(ch) printf("==================================\n\n%s\n\n", s);
	while(input[ptr]==' ' || input[ptr] == '\t')
		up();
	char test[i];
	int j;
	for(j=0;j<i && input[ptr]!='\0' && input[ptr]!=' ';++j){
		if(ch) printf("&& %c &&\n", input[ptr]);
		test[j] = input[up()];
	}
	test[j++] = '\0';	
	if(ch) printf("\n\n==============================================\n\n");
	if(strcmp(test, s)==0)
		return create(s);
	else return NULL;
}

node *constant_val(){
	char val[100];
	int i=0;
	while(input[ptr]==' ' || input[ptr] == '\t')
		up();
	if(input[up()]!='"')
		return NULL;
	while(input[ptr]!='"'){
		if(ch) printf("Constant val char-> %c\n", input[ptr]);
		if(input[ptr] == '\0' || input[ptr] == ' ')
			return NULL;
		val[i++] = input[ptr];
		up();
	}
	val[i] = '\0';
	up();
	return create(val);
}

node * numeric_val(){
	char val[100];
	int i=0;
	while(input[ptr]==' ' || input[ptr] == '\t')
		up();
	if(!isdigit(input[ptr])) return NULL;
	while(isdigit(input[ptr])){
	//	if(ch) printf("digit %c\n", input[ptr]);
		val[i++] = input[ptr];
		up();
	}
	val[i] = '\0';
	return create(val);
}

node * v_val(){
	int save = ptr;
	node *fl = constant_val();
	if(fl)
		return fl;
	else {
		ptr = save;
		node *temp = numeric_val();
	//	if(ch) printf("050505005050#$%c %c\n", input[ptr-1], input[ptr]);
		return temp;
	}
}

node * COL(){
	while(input[ptr]==' ' || input[ptr] == '\t')
		up();
	if(input[up()] == ';')
		return create(";");
	else return NULL;
	
}


node * identifier(){
	char val[100];
	int i=0;
	while(input[ptr]==' ' || input[ptr] == '\t')
		up();
	if(ch) printf("IDENTIFIER  %c\n\n", input[ptr]);

	int check = ptr;
	if(!isalpha(input[up()]))
		return NULL;

	while(isalnum(input[ptr])){
		if(ch) printf(" ** %c **\n", input[ptr]);
		val[i++] = input[ptr];
		up();
	}
	val[i] = '\0';
	return create(val);
}

node * COMMA(){
	while(input[ptr]==' ' || input[ptr] == '\t')
		up();
	if(input[up()] == ',')
		return create(",");
	else NULL;
}


node * BO(){
	while(input[ptr]==' ' || input[ptr] == '\t')
		up();
	if(ch) printf("BO  %c\n", input[ptr]);
	if(input[up()] == '(')
		return create("(");
	else NULL;
	
}
node * BC(){
	//	if(ch) printf("BBBBBBCCCCCCCCCCCCCCC!!\n");
	while(input[ptr]==' ' || input[ptr] == '\t')
		up();
	if(input[up()] == ')')
		return create(")");
	else NULL;
}

node * WILDCARD(){

	while(input[ptr]==' ' || input[ptr] == '\t')
		up();	
	if(input[up()] == '*')
		return create("*");
	else NULL;
}

node * id_list(){
	int save = ptr;
	node *n1, *n2, *n3;
	if((n1 = identifier()) &&  (n2 = COMMA()) && (n3 = id_list())){
		node *nd = create("ID_LIST");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3; 
		return nd;
	}
	ptr = save;
	if(n1 = identifier()){
		node *nd = create("ID_LIST");
		nd->child[(nd->cptr++)] = n1; 
		return nd;
	}
	return NULL;
}

node * tab_list(){
	//if(ch) printf("&&\n\n%s\n\n&&", input);
	int save = ptr;
	node *n1, *n2, *n3;
	if(n1 = id_list()) return n1;	
	
	ptr = save;
	if((n1 = BO()) && (n2 = selection2()) && ( n3 = BC()))
	{
		node *nd = create("TAB_LIST");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
	}	
	ptr = save;
	if((n1=BO()) && (n2=sel()) && (n3=BC())){
		node *nd = create("TAB_LIST");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
	}
	return NULL;
}

node * columns(){
	int save = ptr;
	node *n1, *n2;
	if((n1 =  WILDCARD())){
		return 	create("*");
	}
	ptr = save;
	if((n1 = id_list())){
		node *nd = create("COLUMNS");
		nd->child[(nd->cptr++)] = n1;
		return nd;	
	}	
	return NULL;
}



node * query(){
	//if(ch) printf("++    %c %c %c\n", input[ptr], input[ptr+1], input[ptr+2]);
	node *n1, *n2, *n3;
	if((n1=columns()) && (n2=match("FROM", 4)) && (n3=tab_list())){
		node *nd = create("QUERY");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
	}
	return NULL;
}

node * sel(){
	node *n1, *n2;
	if((n1=match("SELECT", 6)) && (n2=query())){
		node *nd = create("TAB_LIST");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;	
		return nd;	
	}
	return NULL;
}


node * updation(){
	node *n1, *n2, *n3, *n4, *n5, *n6, *n7;
	if((n1=match("UPDATE", 6)) && (n2=identifier()) && (n3=match("SET", 3)) && (n4=updation_list()) && (n5=match("WHERE", 5)) && (n6=constraint()) && (n7=COL())){
		node *nd = create("UPDATE");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		nd->child[(nd->cptr++)] = n5;
		nd->child[(nd->cptr++)] = n6;
		nd->child[(nd->cptr++)] = n7;
		return nd;
	}
	return NULL;
}

node * updation_list(){
	int save = ptr;
	node *n1, *n2, *n3, *n4, *n5;
	if((n1=identifier()) && (n2=match("=", 1)) && (n3=v_val()) && (n4=COMMA()) && (n5=updation_list())){
		node *nd = create("UPDATE_LIST");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		nd->child[(nd->cptr++)] = n5;
		return nd;
	}	
       ptr = save;
       if((n1 = identifier()) && (n2 = match("=", 1)) && (n3 = v_val())){
       	node *nd = create("UPDATE_LIST");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
       }
       return NULL;
}

node *n1, *n2, *n3, *n4, *n5, *n6, *n7, *n8;
node * deletion(){
	if((n1=match("DELETE", 8)) && (n2=match("FROM", 4)) && (n3=identifier()) && (n4=match("WHERE", 5)) && (n5=constraint()) && (n6=COL())){
		node *nd = create("DELETION");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		nd->child[(nd->cptr++)] = n5;
		nd->child[(nd->cptr++)] = n6;
		return nd;
	}
	return NULL;
}

node * alter_drop(){
	if((n1= match("ALTER", 5)) && (n2=match("TABLE", 5)) && (n3=identifier()) && (n4=match("DROP", 4)) && (n5=match("COLUMN", 6)) && (n6=identifier()) && (n7=COL()))
	{
node *nd = create("ALTER_DROP");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		nd->child[(nd->cptr++)] = n5;
		nd->child[(nd->cptr++)] = n6;
		nd->child[(nd->cptr++)] = n7;
		return nd;
	}
	return NULL;
}

node * alter_add(){
	node *n9;
	if((n1=match("ALTER", 5)) && (n2=match("TABLE", 5)) && (n3=identifier()) && (n4=match("ADD", 3)) && (n5=match("COLUMN", 6)) && (n6=identifier()) && (n7=identifier()) && (n8=constraint()) && (n9=COL())){
		node *nd = create("ALTER_ADD");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		nd->child[(nd->cptr++)] = n5;
		nd->child[(nd->cptr++)] = n6;
		nd->child[(nd->cptr++)] = n7;
		nd->child[(nd->cptr++)] = n8;
		nd->child[(nd->cptr++)] = n9;
		return nd;	
	}
	return NULL;
}


node * value_list(){
	int save = ptr;
	if((n1=v_val()) && (n2=COMMA()) && (n3=value_list())){
		node *nd = create("VALUE_LIST");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
	}	
	ptr = save;
	if(n1 = v_val()){
		node *nd = create("ALTER_ADD");
		nd->child[(nd->cptr++)] = n1;
		return nd;
	}
	return NULL;

}

node * fields(){
	if((n1=BO()) && (n2=id_list()) && (n3=BC())){
		node *nd = create("FIELDS");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
	}
	return NULL;
}


node * vb(){
	int save = ptr;
	if((n1=fields()) && (n2=match("VALUES", 6))){
		node *nd = create("VB");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		return nd;
	}	 
	ptr = save;
	if((n1 = match("VALUES", 6))){
		return n1;
	}
	return NULL;
}


node * insertion(){
	if((n1=match("INSERT", 6)) && (n2=match("INTO", 4)) && (n3=identifier()) && (n4=vb()) && (n5=BO()) && (n6=value_list()) && (n7=BC()) && (n8=COL())){
		node *nd = create("INSERTION");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		nd->child[(nd->cptr++)] = n5;
		nd->child[(nd->cptr++)] = n6;
		nd->child[(nd->cptr++)] = n7;
		nd->child[(nd->cptr++)] = n8;
		return nd;
	}
	return NULL;
}




node * schema_list(){
//	if(ch) printf("SCHEMA LIST %c%c%c%c\n", input[ptr], input[ptr+1], input[ptr+2], input[ptr+3]);
	
	int save = ptr;
	if((n1=identifier()) && (n2=identifier()) && (n3=constraint()) && (n4=COMMA()) && (n5=schema_list())){
		node *nd = create("SCHEMA_LIST");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		nd->child[(nd->cptr++)] = n5;
		return nd;
		
	}	
	ptr = save;
	if((n1=identifier()) && (n2=identifier()) && (n3=constraint())){
		node *nd = create("SCHEMA_LIST");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
	}
	return NULL;
}


node * creation(){
	if((n1=match("CREATE", 6)) && (n2=match("TABLE", 5)) && (n3=identifier()) && (n4=BO()) && (n5=schema_list()) && (n6=BC()) && (n7=COL())){
		node *nd = create("CREATION");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		nd->child[(nd->cptr++)] = n5;
		nd->child[(nd->cptr++)] = n6;
		nd->child[(nd->cptr++)] = n7;
		return nd;
	}
	return NULL;
}

node * selection(){
	int save = ptr;
	if(n1=selection1()){
		return n1;
	}	
	ptr = save;
	if(n1 = selection2()){
		return n1;
	}
	ptr = save;
       if(n1=selection3()) return n1;
	return NULL;
}

node * selection1(){
	if((n1=sel()) && (n2=match("WHERE", 5)) && (n3=constraint()) && (n4=COL())){
		node *nd = create("SELECTION1");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		return nd;	
	}
	return false;
}

node * selection2(){
	if((n1=sel()) && (n2=match("WHERE", 5)) && (n3=constraint())){
		node *nd = create("SELECTION2");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;			
	}
	return NULL;
}

node * selection3(){
	if((n1=sel()) && (n2=COL())){
		node *nd = create("SELECTION3");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		return nd;	
	}
	return NULL;
}


node * drop(){
	if((n1=match("DROP", 4)) && (n2=match("TABLE", 5)) && (n3=identifier()) && (n4=COL())){
		node *nd = create("SELECTION2");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		nd->child[(nd->cptr++)] = n4;
		return nd;	
	}
	return NULL;
}

short int line(){
	int save = ptr;
	return((ptr = 0, selection()) || 
			(ptr = 0, drop()) ||
			(ptr = 0, deletion()) ||
			(ptr = 0, updation()) ||
			(ptr = 0, creation()) ||
			(ptr = 0, alter_add()) ||
			(ptr = 0, alter_drop()) ||
			(ptr = 0, insertion())
	  );
}

node * CONJUNCTION(){
	int save = ptr;
	node *f = match("AND", 3);
	if(f) return f;
	ptr = save;
	return match("OR", 2);
}

node * COMPARATOR(){
	node *f = NULL;
	if(input[ptr] == '>' || input[ptr] == '<' || input[ptr]=='='){
		char val[5];
		val[0] = input[ptr];
		val[1] = '\0';
		f = create(val);
	}
	up();
	//	if(ch) printf("%c 9090  %d\n", input[ptr], f);
	return f;
}

node * condition(){
	int save = ptr;
	if((n1=identifier()) && (n2=COMPARATOR()) && (n3=identifier())){
		node *nd = create("SELECTION2");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
	}	
	ptr = save;
       if((n1=v_val()) && (n2=COMPARATOR()) && (n3=v_val())){
       	node *nd = create("SELECTION2");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
       } 
       ptr = save;
       if((n1=v_val()) && (n2=COMPARATOR()) && (n3=identifier())){
       	node *nd = create("SELECTION2");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
       } 
       ptr = save;
       if((n1=identifier()) && (n2=COMPARATOR()) && (n3=v_val())){
       	node *nd = create("SELECTION2");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
       }
       return NULL;
}

node * constraint(){
//	if(ch) printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOoo            %c\n\n", input[ptr+1]);

	int save = ptr;
	if((n1=condition()) && (n2=CONJUNCTION()) && (n3=constraint())){
		node *nd = create("SELECTION2");
		nd->child[(nd->cptr++)] = n1;
		nd->child[(nd->cptr++)] = n2;
		nd->child[(nd->cptr++)] = n3;
		return nd;
	}	
	ptr = save;
	if(n1=condition()){
		return condition();
	}	
	return NULL;
}

int main(){
	int l_no = 0;
	while(gets(input)){
		maxp = 0;
		ptr = 0;
///		gets(input);
		//sanitize();
		if(ch) printf("PARSING:--> %s\n\n", input);
		int save = 0;
		int res = line();
		printf("%d ", res);
		l_no++;
		if(res == 0)
			printf("SYNTAX ERROR near:  \"%c%c%c\"  line no. : %d  col: %d ", input[maxp-1], input[maxp], input[maxp+1], l_no, maxp);
	       	
//		if(ch) printf("--  %c %c\n", input[ptr-1], input[ptr]);
		printf("\n\n");
	}
	return 0;
}
