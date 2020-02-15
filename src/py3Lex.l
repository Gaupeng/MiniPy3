%{
    #include <iostream>
    using namespace std;
    #include "customModels.hpp"
    symbolTable st;
    bool isFunc = false;
    int scope = 0;
    string type = "var"; /* default to var; change to - function on def; class on class; */
%}
    /* Regular Definitions */
NUMBER [0-9]+
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]* 
STRING_LIT \"([^\"\n])*\"|\'([^\'\n])*\'

%%
    /* Comment Removal */
#(.*)                               ++yylineno;					
"\'\'\'"(.*\n)*"\'\'\'"             ; /* Need to handle line count for multi-line comments */			
"\"\"\""(.*\n)*"\"\"\""             ;

"def"                               {isFunc = true; type = string("function"); printf("%s\n", "(KEYWORD)"); /* return (KEYWORD); */}
"class"                             {type = string("class"); printf("%s\n", "(KEYWORD)"); /*return (KEYWORD);*/}
"if"|"else"|"for"|"while"           {printf("%s\n", "(KEYWORD)"); /*return (KEYWORD);*/}
"import"|"from"|"global"|"return"   {printf("%s\n", "(KEYWORD)"); /*return (KEYWORD);*/}
"elif"|"in"|"is"|"lambda"|"not"     {printf("%s\n", "(KEYWORD)"); /*return (KEYWORD);*/}
"or"|"and"|"try"|"except"           {printf("%s\n", "(KEYWORD)"); /*return (KEYWORD);*/}
"range"|"lambda"|"with"|"break"     {printf("%s\n", "(KEYWORD)"); /*return (KEYWORD);*/}
"int"|"float"|"double"|"str"        {printf("%s\n", "(KEYWORD)"); /*return (KEYWORD);*/}
"print"                             {printf("%s\n", "(Print)"); /*return (PRINT);*/}


\=                                  {printf("%s\n", "(EQ)"); /*return (EQ);*/}
[ ]                                 {printf("%s\n", "(SPACE)");}
[\t]                                {scope++; printf("%s\n", "(TAB)");}
\+                                  {printf("%s\n", "(PLUS)"); /*return (PLUS);*/}
\-                                  {printf("%s\n", "(MINUS)"); /*return (MINUS);*/}
\*                                  {printf("%s\n", "(MUL)"); /*return (MUL);*/}
\/                                  {printf("%s\n", "(DIVIDE)"); /*return (DIVIDE);*/}
\(                                  {printf("%s\n", "(LBRACKET)"); /*return (LBRACKET);*/}
\)                                  {printf("%s\n", "(RBRACKET)"); /*return (RBRACKET);*/}
\;                                  {printf("%s\n", "(SEMICOLON)"); /*return (SEMICOLON);*/}
\:                                  {printf("%s\n", "(COLON)"); /*return (COLON);*/}
\,                                  {printf("%s\n", "(COMMA)"); /*return (COMMA);*/}
"\n"                                {type = "var"; scope = 0; ++yylineno; printf("%s\n", "(NL)"); /*return (NL);*/}

{STRING_LIT}                        {printf("%s\n", "(STRING)"); /*return (STRING_LIT);*/}
{NUMBER}                            {printf("%s\n", "(NUMBER)"); /*return (NUMBER);*/}
{IDENTIFIER}                        {
                                        cout << "(IDENTIFIER " << yytext << " -> scope: "<< scope << ", type: " <<  type << ")\n";                                         
                                        insertToken(&st, string(yytext), string(type), scope, yylineno);
                                        if (isFunc)
                                        {
                                            isFunc = !isFunc;a
                                            type = "var";
                                            scope++;
                                        }
                                    }
<<EOF>>                             {return printf("\nReached EOF");}
.                                   {printf("%s\n", "(ERROR)"); /*return (ERROR);*/}
%%

int yywrap()
{
    /*return 1; */
}

int main()
{
    yylex();
    printf("\nSymbol Table: Name, Type, Scope, Line No.\n\n");
    printSymTable(&st);
    /*return 0; */
}