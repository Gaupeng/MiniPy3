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
    token * newToken = new token;
    newToken->name = name;
    newToken->type = type;
    newToken->scope = scope;
    newToken->lineNo = lineNo;
    st->symTab.push_back(*newToken);
    return;
}

void printSymTable(symbolTable *st)
{
    /* 
    Note for reader:
    Our Symbol Table print does not present an ideal view of what the table has,
    as we only print the value of the first node that any Identifier points to.

    Taking an example:
        d = a + b + 10

    If the AST created a node for the RHS it would look like the following:

             +   ---------------> d looks at this
            / \
            a  +
              / \
             b  10

    Thus, in this case the symbol table value for d, would indicate +, telling us,
    that the value of the first node d points to, is a +.
    */

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
        token * tempRec = getRecord(st, value);
        newNode->record = new token;
        // Can't do this - manually copy each field
        // newNode->record = getRecord(st, value);
        newNode->record->name = tempRec->name;
        newNode->record->value = tempRec->value;
        newNode->record->lineNo = tempRec->lineNo;
        newNode->record->scope = tempRec->scope;

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
        cout << currNode->record  << endl;
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


/* ICG Functions */

int isKey(node * currNode)
{
    std::string key = currNode->value;
    if(key == "if" || key == "for" || key == "while" || key == "def")
    {
        return 1;
    }
    return 0;
}

int isBinOp(node * thisNode)
{
    std::string key = thisNode->value;
    if(key == "+" || key == "-" || key == "*" || key == "/")
    {
        return 1;
    }
    return 0;
}

int isNum(node * thisNode)
{
    if(thisNode->type == "numConst")
    {
        return 1;
    }
    return 0;
}

int isAssign(node * thisNode)
{
    if(thisNode->ptrVec[1]->type == "numConst")
    {
        return 1;
    }
    return 0;
}

void addToVarTable(node * currNode, std::vector<varCount *> &countTable)
{
    if(currNode->value == "=")
    {
        for(int i = 0; i < countTable.size(); i++)
        {
            if(countTable[i]->name == currNode->ptrVec[0]->value)
            {
                countTable[i]->value = stoi(currNode->ptrVec[1]->value);
                countTable[i]->lhsCount++;
                return;
            }
        }   
        varCount * temp = new varCount;
        temp->lhsCount = 1;
        temp->value = stoi(currNode->ptrVec[1]->value);
        temp->rhsCount = 0;
        temp->name = currNode->ptrVec[0]->value;
        countTable.push_back(temp);
        return;
    }
    else if(currNode->numNodes == 2)
    {
        for(int i = 0; i < countTable.size(); i++)
        {
            for(int j = 0; j < 2; j++)
            {
                if(countTable[i]->name == currNode->ptrVec[j]->value)
                {
                    countTable[i]->rhsCount++;
                    return;
                }
            }
            
        }   
    }
    
}

node * createNodeICG(symbolTable *st, node * currNode, int * tempUsed,
 std::vector<quad *> &quadTable, std::vector<varCount *> &countTable)
{
        // cout << currNode->ptrVec[0]->value << endl;
        // cout << "CurrNode: " << currNode->value << endl;
        // cout << currNode->ptrVec[1]->value << endl;

        /* 
        Quads logic:
        a = b * c, should have:
        res: a; op: *; arg1: b; arg2: c

        however a = b * c + d, should have:
        t1 = b * c --> res: t1; op: *; arg1: b; arg2: c
        a = t1 + d --> res: a, opp: +; arg1: t1; arg2: d
        */
    if(currNode->value == "=")
    {
        if(isAssign(currNode))
        {
            cout << currNode->ptrVec[0]->value << " = " << currNode->ptrVec[1]->value << endl;
            addToVarTable(currNode, countTable);
            return NULL;
        }
        else
        {
            addToVarTable(currNode->ptrVec[1], countTable);
            quad * newQuad = new quad;
            node * rightChild = currNode->ptrVec[1];
            node * parent = currNode;
            if(isBinOp(rightChild))
            {                
                parent = rightChild;
                rightChild = parent->ptrVec[1];
                if(isBinOp(rightChild))
                {
                    parent = createNodeICG(st, parent, tempUsed, quadTable, countTable);
                    cout << currNode->ptrVec[0]->value << " = " << parent->value << endl;
                    return parent;
                }
                newQuad->arg1 = parent->ptrVec[0]->record;
                newQuad->arg2 = parent->ptrVec[1]->record;
                newQuad->op = parent->value;
                newQuad->result = new token;
                newQuad->result = currNode->ptrVec[0]->record;
                printQuad(newQuad); 
                return parent;
            }
            newQuad->arg1 = rightChild->record;
            newQuad->arg2 = NULL;
            newQuad->result = new token;
            newQuad->result = currNode->ptrVec[0]->record;
            newQuad->op = currNode->value;
            printQuad(newQuad); 
            return parent;
        }
    }
      
    else if(isBinOp(currNode))
    {
        addToVarTable(currNode, countTable);
        if(isBinOp(currNode->ptrVec[1]))
        {
            currNode->ptrVec[1] = createNodeICG(st, currNode->ptrVec[1], tempUsed, quadTable, countTable);
        }
        quad * newQuad = new quad;
        newQuad->op = currNode->value;
        newQuad->arg1 = currNode->ptrVec[0]->record;
        newQuad->arg2 = currNode->ptrVec[1]->record;
        std::string newTemp = "T" + to_string((*tempUsed)++);
        insertToken(st, newTemp, "var", 0, -1);
        newQuad->result = new token;
        token * tempRec = getRecord(st, newTemp);
        newQuad->result = tempRec;
        printQuad(newQuad);
        node * newNode = new node;
        newNode->ptrVec.push_back(currNode);
        newNode->value = newTemp;
        newNode->numNodes = 1;
        newNode->record = tempRec;
        return newNode;      
    }

    else if(isKey(currNode))
    {
        if(currNode->value == "if")
        {
            cout << "if " << currNode->ptrVec[0]->ptrVec[0]->value << " " << currNode->ptrVec[0]->value
                << " " << currNode->ptrVec[0]->ptrVec[1]->value << " GOTO " << currNode->ptrVec[1] << endl;
            cout << "ifFalse " << currNode->ptrVec[0]->ptrVec[0]->value << " " << currNode->ptrVec[0]->value
                << " " << currNode->ptrVec[0]->ptrVec[1]->value << " GOTO " << "Next Node" << endl;
        }
    }
}

void printQuad(quad * resQuad)
{
        if(resQuad->arg2 == NULL)
        {
            cout << resQuad->result->name << " " << resQuad->op << " " << resQuad->arg1->name << endl;
        }
        else
        {
            cout << resQuad->result->name << " = " << resQuad->arg1->name << " " << resQuad->op << " " << resQuad->arg2->name << endl;
        }
        return;
        
}

void printICG(symbolTable *st, std::vector<node *> &ASTArray,
std::vector<quad *> &quadTable, std::vector<varCount *> &countTable)
{
    cout << "\n---------------------" << endl;
    cout << "\nIntermediate Code Generation: " << endl;
    int temp = 0;
    int * tempUsed = &temp;
    for(int node = 0; node < ASTArray.size(); node++)
    {
        createNodeICG(st, ASTArray[node], tempUsed, quadTable, countTable);
    }
    cout << "\n---------------------" << endl;
}

void printCount(std::vector<varCount *> &countTable)
{
    // cout << "COUNTS: " << countTable.size() << endl;
    // for(int i = 0; i < countTable.size(); i++)
    // {
    //     cout << countTable[i]->name << endl;
    //     cout << countTable[i]->value << endl;
    //     cout << countTable[i]->rhsCount << endl;
    //     cout << countTable[i]->lhsCount << endl;
    //     cout << endl;
    // }
    cout << "Performing Dead Code removal" << endl;
    cout << "Performing constant propogation" << endl;
    for(int i = 0; i < countTable.size(); i++)
    {
        if(countTable[i]->rhsCount == 0)
        {
            cout << "Variable: " << countTable[i]->name << ", is never used, " << "we can remove it." << endl;
        }
        else
        {
            cout << "Substitute: " << countTable[i]->name << " with its value: " << countTable[i]->value << endl;
        }
    }
}