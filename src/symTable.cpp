#include "customModels.hpp"

void insertToken(symbolTable *st, string name, string type, int scope, int lineNo)
{
    token * newToken = (token *)malloc(sizeof(token));
    newToken->name = name;
    newToken->type = type;
    newToken->scope = scope;
    newToken->lineNo = lineNo;
    st->symTab.push_back(*newToken);
    return;
}

void printSymTable(symbolTable *st)
{
    for(int i = 0; i < (st->symTab).size(); i++)
    {
        cout << st->symTab[i].name<< ' ' << st->symTab[i].type<< ' ' << st->symTab[i].scope << ' ' << st->symTab[i].lineNo<< endl;
    }
    return;
}