lex.yy.c: 20074033.l
	flex 20074033.l

y.tab.c: 20074033.y
	yacc -d 20074033.y

parser: lex.yy.c y.tab.c
	cc lex.yy.c y.tab.c -g -o parser
