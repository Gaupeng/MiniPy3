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
"return"                            {return (RET);}
"in"                                {return (IN);}
"elif"|"is"|"lambda"|"not"          {return (KEYWORD);}
"or"|"and"|"try"|"except"           {return (KEYWORD);}
"range"|"lambda"|"with"|"break"     {return (KEYWORD);}
"int"|"float"|"double"|"str"        {return (KEYWORD);}
"print"                             {return (PRINT);}


\=                                  {return (EQ);}
"    "                              {scope++; return(TAB);}
[ ]                                 {return(SPACE);}
\+                                  {yylval.value = yytext; return (PLUS);}
\-                                  {yylval.value = yytext; return (MINUS);}
\*                                  {yylval.value = yytext; return (MUL);}
\/                                  {yylval.value = yytext; return (DIVIDE);}
\<                                  {return (LT);}
\>                                  {return (GT);}
\>\=                                {return (GTE);}                               
\<\=                                {return (LTE);}                          
\(                                  {return (LBRACKET);}
\)                                  {return (RBRACKET);}
\;                                  {return (SEMICOLON);}
\:                                  {return (COLON);}
\,                                  {return (COMMA);}
"\n"                                {type = "var"; scope = 0; ++yylineno; return (NL);}

{STRING_LIT}                        {yylval.value = yytext; return (STRING_LIT);}
{NUMBER}                            {yylval.value = yytext; return (NUMBER);}
{IDENTIFIER}                        {
                                        cout << "(IDENTIFIER " << yytext << " -> scope: "<< scope << ", type: " <<  type << ")\n";                                         
                                        insertToken(&st, string(yytext), string(type), scope, yylineno);
                                        if (isFunc)
                                        {
                                            isFunc = !isFunc;
                                            type = "var";
                                            scope++;
                                        }
                                        yylval.value = yytext;
                                        return(ID);
                                    }
<<EOF>>                             {return(ENDF);}
.                                   {return (ERROR);}
%%