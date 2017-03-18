all: 
	lex sql.l
	yacc sql.y -d
	gcc y.tab.c lex.yy.c -o calc

clean:
	rm y.tab.c y.tab.h lex.yy.c calc
