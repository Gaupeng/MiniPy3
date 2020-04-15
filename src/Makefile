a.out : lex.yy.c y.tab.c customModels.hpp 
	g++ lex.yy.c y.tab.c functions.cpp -w -ll

lex.yy.c : py3Lex.l py3yacc.y customModels.hpp
		yacc -d -v py3yacc.y
		lex py3Lex.l

clean :
		rm -f a.out lex.yy.c y.tab.c y.tab.h