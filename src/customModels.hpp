#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#define YYSTYPE yylval_t

/* Symbol Table */

/* lineNo specifies last line when symbol was used */

typedef struct token{
    std::string name;
    std::string type;
    std::string value;
    int scope;
    int lineNo;
}token;

typedef struct symbolTable{
    std::vector<token> symTab;
} symbolTable;

/* Abstract Syntax Tree (AST) */

typedef struct node{
    token * record;
    int numNodes;
    std::string value;
    std::string type;
	std::vector<struct node *> ptrVec;
} node;

/* ICG - Quads representation */
typedef struct quad{
    token * arg1;
    token * arg2;
    token * result;
    std::string op;
} quad;

typedef struct varCount{
    std::string name;
    string value;
    int lhsCount;
    int rhsCount;
} varCount;

/* General Functions */
typedef struct 
{
    std::string type;
    std::string value;
    node * nodePtr;
} yylval_t;

int yylex(void);
int yyerror(const char *s);

/* Symbol Table Functions */

void insertToken(symbolTable *st, string name, string type, int scope, int lineNo);
void printSymTable(symbolTable *st);
void modifyID(symbolTable * st, string left, node * right);
token * getRecord(symbolTable *st, string key);

/* AST Functions */ 

node * createNode(symbolTable *st, std::string type, std::string value, std::vector<node *> &vec, int len);
void printNode(std::string& prefix, node * currNode, int isLeft);
void printArray(std::vector<node *> &ASTArray);

/* ICG Functions */

int isKey(node * currNode);
int isBinOp(node * thisNode);
int isNum(node * thisNode);
int isAssign(node * thisNode);
void addToVarTable(node * currNode, std::vector<varCount *> &countTable);
node * createNodeICG(symbolTable *st, node * currNode, int * tCount,
 std::vector<quad *> &quadTable, std::vector<varCount *> &countTable, int * bCount);
void printICG(symbolTable *st, std::vector<node *> &ASTArray,
 std::vector<quad *> &quadTable, std::vector<varCount *> &countTable);
void printQuad(quad * resQuad, int * bCount);
void printCount(std::vector<varCount *> &countTable);