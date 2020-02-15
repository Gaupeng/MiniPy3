%define parse.error verbose

%{
    #include "customModels.hpp"
    extern symbolTable st;

    AST * tree = (AST *)malloc(sizeof(AST));
    tree->root->value = "AST: ";
    tree->root->left = NULL;
    tree->root->right = NULL;

    int valid = 1;
%}

%token KEYWORD STRING_LIT NUMBER ID ENDF
%token PRINT NL COLON GTE LTE GT LT TAB
%token FOR WHILE IF ELSE IN DEF CLASS RETURN
%token RET SPACE COMMA SEMICOLON ERROR
%left PLUS MINUS MUL DIVIDE LBRACKET RBRACKET EQ
%expect 16

%%
S :     stmt S
        | NL S
        | ENDF {printSymTable(&st); exit(1);}
    ;

cond_lit :  ID
            | NUMBER
            | STRING_LIT
            ;


stmt :  expre
        | loops
        | arith_expr
        ;

repeat_stmt :   %empty
                | stmt repeat_stmt
                ;
    
expre : ID EQ ID {modifyID(&st, $1.value, $3.value);}
        | ID SPACE EQ SPACE ID {modifyID(&st, $1.value, $5.value);}
        | ID SPACE EQ SPACE STRING_LIT {modifyID(&st, $1.value, $5.value);}
        | ID EQ NUMBER {modifyID(&st, $1.value, $3.value);}
        | ID SPACE EQ SPACE NUMBER {modifyID(&st, $1.value, $5.value);}
        | ID SPACE EQ SPACE cond_lit bin_op cond_lit {searchAndOp(&st, $1.value, $5.value, $6.value, $7.val);}
        | ID SPACE EQ SPACE cond_lit SPACE bin_op SPACE cond_lit {searchAndOp(st, $1.value, $5.value, $7.value, $9.value);}
        ;

arith_expr :    cond_lit bin_op arith_expr
                | cond_lit SPACE bin_op SPACE arith_expr
                ;

loops :  FOR SPACE conditions COLON body
        | FOR SPACE conditions SPACE COLON body
        | FOR '(' conditions ')' COLON body
        | FOR SPACE '(' conditions SPACE ')' COLON body
        | WHILE SPACE conditions COLON body
        | WHILE SPACE conditions SPACE COLON body
        | WHILE '(' conditions ')' COLON body
        ;

body :  NL TAB stmt repeat_stmt S
        | NL SPACE stmt repeat_stmt S
        ;

conditions :    cond_lit relop cond_lit
                | cond_lit SPACE relop SPACE cond_lit
                | cond_lit relop SPACE cond_lit
                | cond_lit SPACE relop cond_lit
                | cond_lit
                | cond_lit SPACE IN SPACE cond_lit
                ;

relop : GT
        | LT
        | GTE
        | LTE
        ;

bin_op : PLUS
         | DIVIDE
         | MINUS
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