%{
//#include<scanner.h>
#include "y.tab.h"
%}
%%

";"			{return COL;}
"CREATE"	{ return CREATE;}
"("             {return BO;}
","		{return COMMA;}
")"		{return BC;}
\"[a-zA-Z0-9]+\" {strcpy(yylval.str, yytext); return constant_val;}
"SELECT"		{return SELECT;}
"INSERT"	{return INSERT;}
"INTO"	{return INTO;}
"VALUES" {return VALUES;}
"UPDATE"		{return UPDATE;}
"FROM"	{return FROM;}
"DELETE"		{return DELETE;}
"WHERE" {return WHERE;}
"SET"	{return SET;}
"DROP" {return DROP;}
"TABLE"	{return TABLE;}
"ALTER"	{return ALTER;}
"COLUMN"	{return COLUMN;}
"ADD" 	{return ADD;}
"AND"|"OR"	{return CONJUNCTION;}
"INT"|"VARCHAR"|"CHAR" {return type;}
"NOT" {return NOT;}
"NULL" 	{return NUL;}
"=" {return EQUAL;}
"<"|">"|">="|"<="|"=="|"<>" {strcpy(yylval.str, yytext); return COMPARATOR;}
"UNIQUE" 		{return UNIQUE;}
"*"				{strcpy(yylval.str, yytext); return WILDCARD;}
[a-zA-Z]+[a-zA-Z0-9]*	{strcpy(yylval.str, yytext);   return identifier;}
[0-9]+			{strcpy(yylval.str, yytext); return numeric_val;}
[ \t\n]			{;}
.				{printf("SYNTAX ROR!!\n\n");exit(0);}

%%

int yywrap(){
	return 1;
}
