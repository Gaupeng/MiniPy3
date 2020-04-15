#include "customModels.hpp"

void insertToken(symbolTable *st, string name, string type, int scope, int lineNo)
{
    for(int i = 0; i < (st->symTab).size(); i++)
    {
        if(st->symTab[i].name == name)
        {
            st->symTab[i].lineNo = lineNo;
            st->symTab[i].scope = scope;
            return;
        }
    }
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
    printf("\nSYMBOL TABLE: \n");
    printf("%-6s %-6s %-7s %-6s %-5s\n", "Name", "Type", "Scope", "Line No.", "Value");

    for(int i = 0; i < (st->symTab).size(); i++)
    {
        cout << '[' << st->symTab[i].name<< "\t\t"<< st->symTab[i].type<< "\t\t" << st->symTab[i].scope << "\t\t"
        << st->symTab[i].lineNo << "\t\t" << st->symTab[i].value << ']' << endl;
    }
    return;
}

void modifyID(symbolTable *st, string left, node * right)
{
    token * foundRec = getRecord(st, left);
    foundRec->value = right->value;
    return;
}

token * getRecord(symbolTable *st, string key)
{
    for(int i = 0; i < (st->symTab).size(); i++)
    {
        if(st->symTab[i].name == key)
        {
            return &st->symTab[i];
        }
    }
    return NULL;
}


node * createNode(symbolTable *st, std::string type, std::string value, std::vector<node *> &vec, int len)
{
    node * newNode = new node;
    newNode->type.assign(type);
    newNode->value.assign(value);
    cout << "Created AST Node: " << newNode->type << ": " << newNode->value << endl;
    if(type == "ID")
    {
        newNode->record = getRecord(st, value);
    }
    newNode->numNodes = len;
    for(int i = 0; i < len; i++)
    {
        newNode->ptrVec.push_back(vec[i]);
    }
    return newNode;
}


void printNode(node * currNode)
{
    cout << "\nNode Data" << endl;
    cout << "Type: "<< currNode->type << endl;
    cout << "Value: " << currNode->value << endl;
    cout << "Children: " << currNode->numNodes << endl;
    cout << "Data: ";
    if(currNode->type == "numConst")
    {
        cout << stoi(currNode->value) << endl;
        cout << "\n---------------------" << endl;
    }
    else if(currNode->type == "strConst")
    {
        cout << currNode->value << endl;
        cout << "\n---------------------" << endl;
    }
    else if(currNode->type == "ID")
    {
        cout << currNode->record << endl;
        cout << "\n---------------------" << endl;
    }
    else if(currNode->type == "Symbol")
    {
        cout << currNode->value << endl;
        cout << "\n---------------------" << endl;
    }
    
}

void printChildren(node * currNode)
{
    cout << "\n---------------------" << endl;
    if(currNode != NULL)
    {
        printNode(currNode);
        for(int i = 0; i < currNode->numNodes; i++)
        {
            printChildren(currNode->ptrVec[i]);
        }
    }
}


void printArray(std::vector<node *> &ASTArray)
{
    for(int i = 0; i < ASTArray.size(); i++)
    {
        printChildren(ASTArray[i]);
    }
}