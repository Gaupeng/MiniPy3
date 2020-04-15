%define parse.error verbose

%{
    #include "customModels.hpp"
    #include <vector>
    extern symbolTable st;
    extern char * yytext;
    int valid = 1;
    std::vector<node *> nullVec;
    std::vector<node *> newVec;
    std::vector<node *> ASTArray;
%}

%token KEYWORD STRING_LIT NUMBER ID ENDF
%token PRINT NL COLON GTE LTE GT LT TAB
%token FOR WHILE IF ELSE IN DEF CLASS RETURN
%token RET SPACE COMMA SEMICOLON ERROR
%left PLUS MINUS MUL DIVIDE LBRACKET RBRACKET
%right EQ

%%
S :     stmt S {$$  = $1;}
        | NL S {$$ = $2;}
        | ENDF {printSymTable(&st); cout << "\nAST Nodes " << endl; printArray(ASTArray); exit(1);}
    ;

cond_lit :      ID
                {
                $1.nodePtr = createNode(&st, "ID", $1.value, nullVec, 0);
                $$ = $1;
                }
                |       NUMBER  
                        {
                        $1.nodePtr = createNode(&st, "numConst", $1.value, nullVec, 0);
                        $$ = $1;
                        }
                |       STRING_LIT      
                        {
                        $1.nodePtr = createNode(&st, "numConst", $1.value, nullVec, 0);
                        $$ = $1;
                        }
            ;


stmt :  expre   {$$ = $1;}
        |       loops   {$$ = $1;}
        |       arith_expr      {$$ = $1;}
        ;

repeat_stmt :   %empty
                | stmt repeat_stmt {$$ = $1;}
                ;
    
expre : ID EQ arith_expr        {
                                modifyID(&st, $1.value, $3.nodePtr);
                                $1.nodePtr = createNode(&st, "ID", $1.value, nullVec, 0);
                                newVec.clear();
                                newVec.push_back($1.nodePtr);
                                newVec.push_back($3.nodePtr);
                                $$.nodePtr = createNode(&st, "Symbol", "=", newVec, 2);
                                ASTArray.push_back($$.nodePtr);
                                }
        | ID SPACE EQ SPACE arith_expr  {
                                        modifyID(&st, $1.value, $5.nodePtr);
                                        $1.nodePtr = createNode(&st, "ID", $1.value, nullVec, 0);
                                        newVec.clear();
                                        newVec.push_back($1.nodePtr);
                                        newVec.push_back($5.nodePtr);
                                        $$.nodePtr = createNode(&st, "Symbol", "=", newVec, 2);
                                        ASTArray.push_back($$.nodePtr);
                                        }
        ;

arith_expr :    cond_lit bin_op arith_expr      {
                                                newVec.clear();
                                                newVec.push_back($1.nodePtr);
                                                newVec.push_back($3.nodePtr);
                                                $$.nodePtr = createNode(&st, "Symbol", $2.value, newVec, 2);
                                                ASTArray.push_back($$.nodePtr);
                                                }
                | cond_lit SPACE bin_op SPACE arith_expr        {
                                                                newVec.clear();
                                                                newVec.push_back($1.nodePtr);
                                                                newVec.push_back($5.nodePtr);
                                                                $$.nodePtr = createNode(&st, "Symbol", $3.value, newVec, 2);
                                                                ASTArray.push_back($$.nodePtr);
                                                                }
                | cond_lit      {
                                $$ = $1;
                                }
                ;

loops :  FOR SPACE conditions COLON body
        | FOR '(' conditions ')' COLON body
        | WHILE SPACE conditions COLON body
        | WHILE '(' conditions ')' COLON body
        | IF SPACE conditions COLON body
        ;

body :  NL TAB stmt repeat_stmt S
        | NL SPACE stmt repeat_stmt S
        ;

conditions :    cond_lit SPACE relop SPACE cond_lit {
                                        newVec.clear();
                                        newVec.push_back($1.nodePtr);
                                        newVec.push_back($3.nodePtr);
                                        $$.nodePtr = createNode(&st, " ", $2.value, newVec, 2);
                                        ASTArray.push_back($$.nodePtr);
                                        }
                | cond_lit {$$ = $1;}
                ;

relop : GT {$$ = $1;}
        | LT {$$ = $1;} 
        | GTE {$$ = $1;}
        | LTE {$$ = $1;}
        ;

bin_op : PLUS {$$ = $1;}
         | DIVIDE {$$ = $1;}
         | MINUS {$$ = $1;}
         | MUL {$$ = $1;}
         ;
%%

#include <ctype.h>
int yyerror(const char *s)
{
    printf("Invalid Program.\n");
    valid = 0;
    extern int yylineno;
    printf("%d::%s\n", yylineno, s);
    yyparse();
    return 1;
}

int main()
{
    yyparse();

	if(valid)
	{
		printf("Valid Program.\n");
	}

}