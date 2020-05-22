%{
    #include "customModels.hpp"
    #include "y.tab.h"
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

"def"                               {isFunc = true; type = string("function"); return (DEF); }
"class"                             {type = string("class"); return (CLASS);}
"if"                                {return (IF);}
"else"                              {return (ELSE);}
"for"                               {return (FOR);}
"while"                             {return (WHILE);}
"import"                            {return (IMPORT);}
"return"                            {return (RET);}
"in"                                {yylval.type = "binOp"; yylval.value = "in";return (IN);}
"elif"|"is"|"lambda"|"not"          {return (KEYWORD);}
"or"|"and"|"try"|"except"           {return (KEYWORD);}
"range"|"lambda"|"with"|"break"     {return (KEYWORD);}
"int"|"float"|"double"|"str"        {return (KEYWORD);}
"print"                             {return (PRINT);}


\=                                  {return (EQ);}
"    "                              {scope++; return(TAB);}
[ ]                                 {return(SPACE);}
\+                                  {yylval.type = "binOp"; yylval.value = "+"; return (PLUS);}
\-                                  {yylval.type = "binOp"; yylval.value = "-"; return (MINUS);}
\*                                  {yylval.type = "binOp"; yylval.value = "*"; return (MUL);}
\/                                  {yylval.type = "binOp"; yylval.value = "/"; return (DIVIDE);}
\<                                  {yylval.type = "relOp"; yylval.value = "<"; return (LT);}
\>                                  {yylval.type = "relOp"; yylval.value = ">"; return (GT);}
\>\=                                {yylval.type = "relOp"; yylval.value = ">="; return (GTE);}                               
\<\=                                {yylval.type = "relOp"; yylval.value = "<="; return (LTE);}                          
\(                                  {yylval.type = "sym"; yylval.value = "("; return (LBRACKET);}
\)                                  {yylval.type = "sym"; yylval.value = ")"; return (RBRACKET);}
\;                                  {yylval.type = "sym"; yylval.value = ";"; return (SEMICOLON);}
\:                                  {yylval.type = "sym"; yylval.value = ":"; return (COLON);}
\,                                  {yylval.type = "sym"; yylval.value = ","; return (COMMA);}
"\n"                                {type = "var"; scope = 0; ++yylineno; return (NL);}

{STRING_LIT}                        {yylval.type = "strConst"; yylval.value = string(yytext); return (STRING_LIT);}
{NUMBER}                            {yylval.type = "numConst"; yylval.value = string(yytext);  return (NUMBER);}
{IDENTIFIER}                        {       
                                        insertToken(&st, string(yytext), string(type), scope, yylineno);
                                        if (isFunc)
                                        {
                                            isFunc = !isFunc;
                                            type = "var";
                                            scope++;
                                        }
                                        yylval.type = "ID";
                                        yylval.value = yytext;
                                        return(ID);
                                    }
<<EOF>>                             {return(ENDF);}
.                                   {return (ERROR);}
%%