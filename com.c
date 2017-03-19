#include<stdio.h>
#include<stdlib.h>
#include "com.h"
#define true 1
#define false 0

int ch = 0;
int maxp = 0;

int up(){
	printf("MX --> %d\n", maxp);
	maxp = (maxp>ptr?maxp:ptr);
}

typedef short int bool;

char input[1000000];
int ptr = 0;

void sanitize(){
	int r=0, w=0;
	while(input[r]!='\0'){
		if(input[r] == ' ' || input[r] == '\n')
			r++;
		else input[w++] = input[r++];
	}
	input[w] = '\0';
}

bool match(char *s, int i){
	if(ch) printf("==================================\n\n%s\n\n", s);
	while(input[ptr]==' ' || input[ptr] == '\t')
		ptr++;
	char test[i];
	int j;
	for(j=0;j<i && input[ptr]!='\0' && input[ptr]!=' ';++j){
		if(ch) printf("&& %c &&\n", input[ptr]);
		test[j] = input[ptr++];
	}
	test[j++] = '\0';	
	if(ch) printf("\n\n==============================================\n\n");
	return (strcmp(test, s)==0);
}

bool constant_val(){
	while(input[ptr]==' ' || input[ptr] == '\t')
		ptr++;
	if(input[ptr++]!='"')
		return false;
	while(input[ptr]!='"'){
		if(ch) printf("Constant val char-> %c\n", input[ptr]);
		if(input[ptr] == '\0' || input[ptr] == ' ')
			return false;
		ptr++;
	}
	ptr++;
	return true;
}

bool numeric_val(){
	while(input[ptr]==' ' || input[ptr] == '\t')
		ptr++;
	if(!isdigit(input[ptr])) return false;
	while(isdigit(input[ptr])){
	//	if(ch) printf("digit %c\n", input[ptr]);
		ptr++;
	}
	return true;
}

bool v_val(){
	int save = ptr;
	int fl = constant_val();
	if(fl)
		return true;
	else {
		ptr = save;
		int temp = numeric_val();
	//	if(ch) printf("050505005050#$%c %c\n", input[ptr-1], input[ptr]);
		return temp;
	}
}

bool COL(){
	while(input[ptr]==' ' || input[ptr] == '\t')
		ptr++;
	return (input[ptr++] == ';');
	
}


bool identifier(){
	while(input[ptr]==' ' || input[ptr] == '\t')
		ptr++;
	if(ch) printf("IDENTIFIER  %c\n\n", input[ptr]);

	int check = ptr;
	if(!isalpha(input[ptr++]))
		return false;

	while(isalnum(input[ptr])){
		if(ch) printf(" ** %c **\n", input[ptr]);
		ptr++;
	}
	return true;
}

bool COMMA(){
	while(input[ptr]==' ' || input[ptr] == '\t')
		ptr++;
	return (input[ptr++] == ',');
}


bool BO(){
	while(input[ptr]==' ' || input[ptr] == '\t')
		ptr++;
	if(ch) printf("BO  %c\n", input[ptr]);
	return (input[ptr++] == '(');
	
}
bool BC(){
	//	if(ch) printf("BBBBBBCCCCCCCCCCCCCCC!!\n");
	while(input[ptr]==' ' || input[ptr] == '\t')
		ptr++;
	return (input[ptr++] == ')');
}

bool WILDCARD(){

	while(input[ptr]==' ' || input[ptr] == '\t')
		ptr++;	
	return (input[ptr++] == '*');
}

bool id_list(){
	int save = ptr;
	return ((ptr = save, (identifier() && COMMA() && id_list())) || ((ptr = save, identifier())));
}

bool tab_list(){
	//if(ch) printf("&&\n\n%s\n\n&&", input);
	int save = ptr;
	return ((ptr = save, id_list()) || (ptr = save, (BO() && selection2() && BC())) || (ptr = save, (BO() && sel() && BC())));
}

bool columns(){
	int save = ptr;
	return ((ptr = save, WILDCARD()) || (ptr = save, id_list())); 
}



bool query(){
	//if(ch) printf("++    %c %c %c\n", input[ptr], input[ptr+1], input[ptr+2]);
	return (columns() && match("FROM", 4) && tab_list());
}

bool sel(){
	return (match("SELECT", 6) && query());
}


bool updation(){
	return(match("UPDATE", 6) && identifier() && match("SET", 3) && updation_list() && match("WHERE", 5) && constraint() && COL());
}

bool updation_list(){
	int save = ptr;
	return((ptr = save, identifier() && match("=", 1) && v_val() && COMMA() && updation_list()) || (ptr = save, identifier() && match("=", 1) && v_val()));
}


bool deletion(){
	return(match("DELETE", 8) && match("FROM", 4) && identifier() && match("WHERE", 5) && constraint() && COL());
}

bool alter_drop(){
	return(match("ALTER", 5) && match("TABLE", 5) && identifier() && match("DROP", 4) && match("COLUMN", 6) && identifier() && COL());
}

bool alter_add(){
	return(match("ALTER", 5) && match("TABLE", 5) && identifier() && match("ADD", 3) && match("COLUMN", 6) && identifier() && identifier() && constraint() && COL());
}


bool value_list(){
	int save = ptr;
	return((ptr = save, v_val() && COMMA() && value_list()) || (ptr = save, v_val()));
}

bool fields(){
	return(BO() && id_list() && BC());
}


bool vb(){
	int save = ptr;
	return ((ptr = save, fields() && match("VALUES", 6)) || (ptr = save, match("VALUES", 6)));
}


bool insertion(){
	return(match("INSERT", 6) && match("INTO", 4) && identifier() && vb() && BO() && value_list() && BC() && COL());
}




bool schema_list(){
//	if(ch) printf("SCHEMA LIST %c%c%c%c\n", input[ptr], input[ptr+1], input[ptr+2], input[ptr+3]);
	
	int save = ptr;
	return((ptr = save, identifier() && identifier() && constraint() && COMMA() && schema_list()) || (ptr = save, identifier() && identifier() && constraint()));
}


bool creation(){
	return(match("CREATE", 6) && match("TABLE", 5) && identifier() && BO() && schema_list() && BC() && COL());
}

bool selection(){
	int save = ptr;
	return ((ptr = save, selection1()) || (ptr = save, selection2()) || (ptr = save, selection3()));
}

bool selection1(){
	return (sel() && match("WHERE", 5) && constraint() && COL());
}

bool selection2(){
	return (sel() && match("WHERE", 5) && constraint());
}

bool selection3(){
	return (sel() && COL());
}


bool drop(){
	return(match("DROP", 4) && match("TABLE", 5) && identifier() && COL());
}

bool line(){
	int save = ptr;
	return((up(), ptr = 0, selection()) || 
			(up(), ptr = 0, drop()) ||
			(up(), ptr = 0, deletion()) ||
			(up(), ptr = 0, updation()) ||
			(up(), ptr = 0, creation()) ||
			(up(), ptr = 0, alter_add()) ||
			(up(), ptr = 0, alter_drop()) ||
			(up(), ptr = 0, insertion())
	  );
}

bool CONJUNCTION(){
	int save = ptr;
	bool f = match("AND", 3);
	if(f) return true;
	ptr = save;
	return match("OR", 2);
}

bool COMPARATOR(){
	bool f = (input[ptr] == '>' || input[ptr] == '<' || input[ptr]=='=');
	ptr++;
	//	if(ch) printf("%c 9090  %d\n", input[ptr], f);
	return f;
}

bool condition(){
	int save = ptr;
	return ((ptr = save, (identifier() && COMPARATOR() && identifier())) || (ptr = save, (v_val() && COMPARATOR() && v_val())) || (ptr = save, (v_val() && COMPARATOR() && identifier())) || (ptr = save, (identifier() && COMPARATOR() && v_val())));
}

bool constraint(){
//	if(ch) printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOoo            %c\n\n", input[ptr+1]);

	int save = ptr;
	return ((ptr = save, (condition() && CONJUNCTION() && constraint())) || (ptr = save, condition()));	
}

int main(){
	while(1){
		ptr = 0;
		gets(input);
		//sanitize();
		if(ch) printf("PARSING:--> %s\n\n", input);
		int save = 0;
		int res = line();
		printf("%d ", res);
		if(res == 0)
			printf("  %c%c%c", input[ptr-1], input[ptr], input[ptr+1]); 
//		if(ch) printf("--  %c %c\n", input[ptr-1], input[ptr]);
	}
	return 0;
}
