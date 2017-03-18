%{
#include "y.tab.h"
%}

%%

"print"  {return print;}
"exit"  {return exit_cmd;}
[a-zA-Z] {yylval.token_class = yytext[0]; return identifier; }
[0-9]+ {yylval.num = atoi(yytext); return number;}
[ \t\n] ;
[-+=;]  {return yytext[0];}
. {return yyerror("unexpected error!!\n");}

%%

int yywrap(void){
return 1;
}
