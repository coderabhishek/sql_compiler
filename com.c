#include<stdio.h>
#include<stdlib.h>
#include "com.h"
#define true 1
#define false 0

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
	printf("9999   %c %c\n", input[ptr], input[ptr+1]);
	char test[i];
	for(int j=0;j<i && input[j]!='\0';++j){
		test[j] = input[ptr++];
	}
	
	return (strcmp(test, s)==0);
}

bool constant_val(){
	if(input[ptr++]!='"')
		return false;
	while(input[ptr]!='"'){
		if(input[ptr] == '\0')
			return false;
		ptr++;
	}
	return true;
}

bool numeric_val(){
	if(!isdigit(input[ptr])) return false;
	while(isdigit(input[ptr++]));
	return true;
}

bool v_val(){
	int save = ptr;
	int fl = constant_val();
	if(fl)
		return true;
	else {
		ptr = save;
		return numeric_val();
	}
}

bool COL(){
	if(input[ptr++] = ';')
		return true;
	else false;
}

bool identifier(){
	int check = ptr;
	if(!isalpha(input[ptr++]))
		return false;
	while(isalnum(input[ptr]))
			ptr++;
	return true;
}

bool COMMA(){
	if(input[ptr++] == ',')
	return true;
	else false;
}


bool BO(){
	if(input[ptr++] == '(')
	return true;
	else false;
}
bool BC(){
	if(input[ptr++] == ')')
	return true;
	else false;
}

bool WILDCARD(){
	if(input[ptr++] == '*')
	return true;
	else false;
}

bool id_list(){
	int save = ptr;
	return ((ptr = save, (identifier() && COMMA() && id_list())) || ((ptr = save, identifier())));
}

bool tab_list(){
	//printf("&&\n\n%s\n\n&&", input);
	int save = ptr;
	return ((ptr = save, identifier()) || (ptr = save, (BO() && selection() && BC())));
}

bool columns(){
	int save = ptr;
	return ((ptr = save, WILDCARD()) || (ptr = save, id_list())); 
}



bool query(){
	printf("++    %c %c %c\n", input[ptr], input[ptr+1], input[ptr+2]);
	return (columns() && match("FROM", 4) && tab_list());
}

bool sel(){
 return (match("SELECT", 6) && query());
}



bool selection(){
int save = ptr;
return ((ptr = save, selection1()) || (ptr = save, selection2()) || (ptr = save, selection3()) || (ptr = save, selection4()));
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

bool selection4(){
	return sel();
}

bool drop(){
	return false;
}

bool line(){
	int save = ptr;
	if((ptr = save, selection()) || 
	   (ptr = save, drop()))
		return true;
	else return false;
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
//	printf("%c 9090  %d\n", input[ptr], f);
	return f;
}

bool condition(){
	int save = ptr;
	return ((ptr = save, (identifier() && COMPARATOR() && identifier())) || (ptr = save, (v_val() && COMPARATOR() && v_val())) || (ptr = save, (v_val() && COMPARATOR() && identifier())) || (ptr = save, (identifier() && COMPARATOR() && v_val())));
}

bool constraint(){
	int save = ptr;
	return ((ptr = save, (condition() && CONJUNCTION() && constraint())) || (ptr = save, condition()));
}

int main(){
	gets(input);
	sanitize();
	printf("PARSING:--> %s\n\n", input);
	int save = 0;
	printf("%d", line());

	return 0;
}
