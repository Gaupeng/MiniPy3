#include <string>
#include <vector>
#include <iostream>
using namespace std;

typedef struct 
{
    string value;
} yylval_t;

#define YYSTYPE yylval_t

/* Symbol Table */

typedef struct token{
    string name;
    string type;
    string value;
    int scope;
    int lineNo;
}token;

typedef struct symbolTable{
    vector<token> symTab;
} symbolTable;

/* Abstract Syntax Tree (AST) */

typedef struct ASTnode{
	string value;
	string result;
	string regName;
	struct node *left;
	struct node *right;
} node;

typedef struct AST{
    node * root;
} AST;


/* General Functions */

int yylex(void);
int yyerror(const char *s);

/* Symbol Table Functions */

void insertToken(symbolTable *st, string name, string type, int scope, int lineNo);
void printSymTable(symbolTable *st);
void modifyID(symbolTable * st, string left, string right);
void searchAndOp(symbolTable *st, string dest, string left, string op, string right);

/* AST Functions */ 

node * createNode(string value, node * left, node * right);
void printPreorderWrap(AST * tree);
void printPostorderWrap(AST * tree);
void printInorderWrap(AST * tree);
void printPreorder(node * root);
void printPostorder(node * root);
void printInorder(node * root);