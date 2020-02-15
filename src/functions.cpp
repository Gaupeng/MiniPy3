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
    printf("%-5s %-5s %-5s %-5s %-5s\n", "Name", "Type", "Scope", "Line No.", "Value");

    for(int i = 0; i < (st->symTab).size(); i++)
    {
        cout << '[' << st->symTab[i].name<< "\t\t"<< st->symTab[i].type<< "\t\t" << st->symTab[i].scope << "\t\t"
        << st->symTab[i].lineNo << "\t\t" << st->symTab[i].value << ']' << endl;
    }
    return;
}

void modifyID(symbolTable *st, string left, string right)
{
    for(int i = 0; i < (st->symTab).size(); i++)
    {
        if(st->symTab[i].name == left)
        {
            st->symTab[i].value = right;
        }
    }
}

void searchAndOp(symbolTable *st, string dest, string left, string op, string right)
{
    int temp;
    for(int i = 0; i < (st->symTab).size(); i++)
    {
        if(st->symTab[i].name == left)
        {
            temp = stoi(st->symTab[i].value);
            if(op == "+")
            {
                temp += stoi(right);
            }
            else if(op == "-")
            {
                temp -= stoi(right);
            }
            else if(op == "*")
            {
                temp *= stoi(right);
            }
            else
            {
                temp /= stoi(right);
            }
        }
    }
    for(int i = 0; i < (st->symTab).size(); i++)
    {
        if(st->symTab[i].name == dest)
        {
            st->symTab[i].value = to_string(temp);
            return;
        }
    }
}

node * createNode(string value, node * left, node * right)
{
    node * newNode = (node *)malloc(sizeof(node));
    newNode->value = value;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

/* Pre-order print */

void printPreorder(node * root)
{
    if(root == NULL)
    {
        return;
    }
    printf("%s ", root->value);
    printPreorder(root->left);
    printPreorder(root->right);
}

void printPreorderWrap(AST * tree)
{
    printPreorder(tree->root);
}

/* Post-order Print */

void printPostorder(node * root)
{
    if(root == NULL)
    {
        return;
    }
    printPostorder(root->left);
    printPostorder(root->right);
    printf("%s ", root->value);
}

void printPostorderWrap(AST * tree)
{
    printPostorder(tree->root);
}

/* Inorder Print */ 
void printInorder(node * root)
{
    if(root == NULL)
    {
        return;
    }
    printInorder(root->left);
    printf("%s ", root->value);
    printInorder(root->right);
}

void printInorderWrap(AST * tree)
{
    printInorder(tree->root);
}

