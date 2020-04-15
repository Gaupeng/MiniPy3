#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#define YYSTYPE yylval_t

/* Symbol Table */

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
void printNode(node * currNode);
void printChildren(node * currNode);
void printArray(std::vector<node *> &ASTArray);