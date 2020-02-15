#include <string>
#include <vector>
#include <iostream>
using namespace std;

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

void insertToken(symbolTable *st, string name, string type, int scope, int lineNo);
void printSymTable(symbolTable *st);