# Run ./a.out < ../test/testInput.py > ../test/output.txt

a.out : lex.yy.c y.tab.c customModels.hpp functions.cpp
	g++ lex.yy.c y.tab.c functions.cpp -w -ll

lex.yy.c : py3Lex.l py3Yacc.y customModels.hpp
		yacc -d -v py3Yacc.y
		lex py3Lex.l

clean :
		rm -f a.out lex.yy.c y.tab.c y.tab.h y.output