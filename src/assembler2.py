# open output file
# extract ICG from file
import sys
sys.stdout = open('../test/output.txt', 'a')

outputList = []
registerCount = 0
goto = ""

variableList = []
registerList = []
variableCount = {}


def extractICG(filename):
    extractList = []
    copy = False
    fh = open(filename, "r")
    for line in fh:
        if line.strip() == "Intermediate Code Generation:":
            copy = True
        elif line.strip() == "End of Intermediate Code Generation":
            copy = False
        elif copy:
            extractList.append(line.strip())
    extractList.pop()
    extractList.pop(0)
    return extractList

ICG = extractICG('../test/output.txt')
# print(ICG)

def findBinOp(op):
    if op == "+":
        return "ADD"
    elif op == "*":
        return "MUL"
    elif op == "/":
        return "DIV"
    else:
        return "SUB"

def findRelOp(op):
    if op == "<":
        return "GE"
    elif op == ">":
        return "LE"
    elif op == ">=":
        return "LT"
    elif op == "<=":
        return "GT"
    return "NEQ"
    
def assignOp(ele,ICG,ind):
    global registerCount, goto
    spaceSplit = ele.split()
    if len(spaceSplit) == 3:
        if spaceSplit[2].isdigit():

            reg = getRegister(str(spaceSplit[2]))
            lookahead(ICG,ind,spaceSplit[2])
            # registerCount += 1
            print("MOV", reg + ",", "#" + spaceSplit[2])
        else:
            if "\"" in spaceSplit[2]:
                print("str: .asciiz", spaceSplit[2])
                reg1 = getRegister("str")
                
                # registerCount += 1
                print("LDR", reg1 + ",", "str")
                reg2 = getRegister(spaceSplit[0])
                
                lookahead(ICG,ind,"str")
                lookahead(ICG,ind,spaceSplit[0])
                print("LDR", reg2 + ",",spaceSplit[0])

                # registerCount += 1
                print("LDR","R" + str(registerCount) + ",["+ reg1+"]")
                print("STR","R" + str(registerCount) + ",["+ reg2+"]")
                # registerCount += 1
            else:
                reg1= getRegister(spaceSplit[2])
                
                # registerCount += 1
                print("LDR", reg1 + ",", spaceSplit[2])

                reg2 = getRegister(spaceSplit[0])
                
                lookahead(ICG,ind,spaceSplit[2])
                lookahead(ICG,ind,spaceSplit[0])
                print("LDR", reg2 + ",",spaceSplit[0])
                # registerCount += 1
                print("LDR","R" + str(registerCount) + ",["+ reg1+"]")
                print("STR","R" + str(registerCount) + ",["+ reg2+"]")
                # registerCount += 1
        # print("ST", spaceSplit[0] + ",", reg)
    elif len(spaceSplit) != 3:
        op = findBinOp(spaceSplit[3])
        reg1 = getRegister(spaceSplit[2])
       
        # registerCount += 1
        print("LDR", reg1 + ",", spaceSplit[2])
        reg2 = getRegister(spaceSplit[0])
        
        # registerCount += 1
        print("LDR", reg2 + ",", spaceSplit[0])
        reg3 = getRegister("["+ reg1+"]")
        
        registerCount += 1
        lookahead(ICG, ind, spaceSplit[2])
        lookahead(ICG, ind, spaceSplit[0])
        lookahead(ICG, ind, "["+ reg1+"]")
        print("LDR",reg3 + ", "+"["+ reg1+"]")
        print(op, reg3 + ",", reg3 + ",", spaceSplit[4])
        print("STR",reg3 + ", ["+ reg2+"]")
    if goto:
        print("B", goto)
        goto = ""
    registerCount = 0
    print()


def getRegister(variable):
    global variableList,variableCount,registerList
    flag = 1

    if(variable in variableList):
        variableCount[variable] = 0
        return registerList[variableList.index(variable)]
    else:
        
        variableList.append(variable)
      
        for i in range(14):
            register = "R"+str(i);
            if ("R"+str(i)) not in registerList:
                
                break;
        
        registerList.append(register)
        
        variableCount[variable] = 0
        
    return register

def lookahead(ICG,ind,variable):
    global variableList,registerList,variableCount
    count = 1
    for i in range(ind,len(ICG)):
        if(variable in ICG[i]):
            break
        else:
            count += 1
        if(count > 9):
            del variableCount[variable]
            registerToBeRemoved = registerList[variableList.index(variable)]
            variableList.remove(variable)
            registerList.remove(registerToBeRemoved)
            break

def incVariableCount():
    removed = 0
    global variableList,variableCount,registerList
    keysToBeRemoved = []
    for key in variableCount:
        variableCount[key] += 1
        if(variableCount[key] > 3):
          
            removed = 1
            keysToBeRemoved.append(key)
    
    for key in keysToBeRemoved:
        del variableCount[key]
        registerToBeRemoved = registerList[variableList.index(key)]
        variableList.remove(key)
        registerList.remove(registerToBeRemoved)
    
    return removed


def assembler(ICG):
    print("Assembly Code:\n")
    global registerCount, goto
    for ind in range(len(ICG)):
        incVariableCount()
        ele = ICG[ind]
        if ':' in ele and '=' in ele:
            spaceSplit = ele.split()
            print(spaceSplit[0])
            spaceSplit.pop(0)
            newEle = ' '.join(spaceSplit)
            assignOp(newEle,ICG,ind)
        elif '=' in ele:
            assignOp(ele,ICG,ind)
        # we've entered either an if, for or while
        elif 'ifFalse' in ele:
            forReg = ""
            # for or while
            if 'in' in ele:
                forReg = "R" + str(registerCount)
                registerCount += 1
                print("LDR", forReg + ",","#0")
            if ":" in ele:
                spaceSplit = ele.split()
                print(spaceSplit[0])
                goto = spaceSplit.pop(0)[:-1]
                ele = ' '.join(spaceSplit)
            spaceSplit = ele.split()
            left = spaceSplit[1]
            op = spaceSplit[2]
            right = spaceSplit[3]
            bnum = spaceSplit[-1]
            if 'in' not in ele:
                reg1 = "R" + str(registerCount)
                print("LDR", reg1 + ",", left)
                registerCount += 1
                reg2 = "R" + str(registerCount)
                print("LDR", reg2 + ",", right)
                registerCount += 1
                print("CMP", reg1 + ",", reg2)
                oppOp = findRelOp(op)
                print("B" + oppOp, bnum)
            else:
                reg2 = "R" + str(registerCount)
                print("LDR", reg2 + ",", right + "(" + forReg + ")")
                print("ADD", forReg + ",", forReg + ",", "#1")
                registerCount += 1
                print("CMP", reg2 + ",", "0x00")
                print("BEQ", bnum)

assembler(ICG)