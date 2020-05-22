# open output file
# extract ICG from file
import sys
sys.stdout = open('../test/output2.txt', 'a')
goto = ""

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


ICG = extractICG('../test/output2.txt')


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


def assignOp(ele, indexele):
    global goto
    spaceSplit = ele.split()
    if len(spaceSplit) == 3:
        if spaceSplit[2].isdigit():

            reg = searchregister(str(spaceSplit[0]), indexele)
            reg = str(reg)
            print("MOV", reg + ",", "#" + spaceSplit[2])
            deleteregister(str(spaceSplit[0]), indexele)
        else:
            if "\"" in spaceSplit[2]:
                print("str: .asciiz", spaceSplit[2])
                reg1 = searchregister("str", indexele)
                reg1 = str(reg1)
                print("LDR", reg1 + ",", "str")
                reg2 = searchregister(spaceSplit[0], indexele)
                reg2 = str(reg2)
                print("LDR", reg2 + ",", spaceSplit[0])
                reg3 = newregister()
                reg3 = str(reg3)
                print("LDR", reg3 + ", [" + reg1+"]")
                print("STR", reg3 + ", [" + reg2+"]")
                deleteregister("str", indexele)
                deleteregister(str(spaceSplit[0]), indexele)
            else:
                reg1 = searchregister(spaceSplit[2], indexele)
                reg1 = str(reg1)
                print("LDR", reg1 + ",", spaceSplit[2])
                reg2 = searchregister(spaceSplit[0], indexele)
                reg2 = str(reg2)
                print("LDR", reg2 + ",", spaceSplit[0])
                reg3 = newregister()
                reg3 = str(reg3)
                print("LDR", reg3 + ", [" + reg1+"]")
                print("STR", reg3 + ", [" + reg2+"]")
                deleteregister(str(spaceSplit[2]), indexele)
                if(spaceSplit[2] != spaceSplit[0]):
                    deleteregister(str(spaceSplit[0]), indexele)

    elif len(spaceSplit) != 3:
        op = findBinOp(spaceSplit[3])
        reg1 = searchregister(spaceSplit[2], indexele)
        reg1 = str(reg1)
        print("LDR", reg1 + ",", spaceSplit[2])
        reg2 = searchregister(spaceSplit[0], indexele)
        reg2 = str(reg2)
        print("LDR", reg2 + ",", spaceSplit[0])
        reg3 = newregister()
        reg3 = str(reg3)
        print("LDR", reg3 + ", "+"[" + reg1+"]")
        print(op, reg3 + ",", reg3 + ",", spaceSplit[4])
        print("STR", reg3 + ", [" + reg2+"]")
        deleteregister(str(spaceSplit[2]), indexele)
        if(spaceSplit[2] != spaceSplit[0]):
            deleteregister(str(spaceSplit[0]), indexele)
    if goto:
        print("B", goto)
        goto = ""
        deleteregister("forCount", indexele)
        deleteregister("forChar", indexele)
    print()


def searchregister(variable, index):
    global variableCount
    if len(variableCount) == 13:
        reg = "R13"
        var = list(variableCount.keys())[0]
        reg2 = variableCount[var]
        print("LDR", reg, var)
        print("STR", reg2 + ", [" + reg+"]")
        del variableCount[var]

    if(variable in variableCount):
        reg = variableCount[variable]
        variableCount.pop(variable)
        variableCount[variable] = reg
        return str(variableCount[variable])
    else:
        for i in range(13):
            register = "R"+str(i)
            if register not in variableCount.values():
                reg = "R" + str(i)
                variableCount[variable] = reg
                return str(reg)


def newregister():
    global variableCount
    if len(variableCount) == 13:
        reg = "R13"
        var = list(variableCount.keys())[0]
        reg2 = variableCount[var]
        print("LDR", reg, var)
        print("STR", reg2 + ", [" + reg+"]")
        del variableCount[var]
    for i in range(13):
        if ("R"+str(i)) not in variableCount.values():
            reg = "R" + str(i)
            return str(reg)


def deleteregister(variable, index):
    if variable not in variableCount:
        return
    index = index + 1
    newindex = len(ICG)
    newlist = list(ICG[index:newindex])
    if str(variable) in str(newlist):
        return
    variableCount.pop(variable)


def assembler(ICG):
    global goto
    for ind in range(len(ICG)):
        # incVariableCount()
        ele = ICG[ind]
        if ':' in ele and '=' in ele:
            # if/for/while exit branch
            spaceSplit = ele.split()
            print(spaceSplit[0])
            spaceSplit.pop(0)
            newEle = ' '.join(spaceSplit)
            assignOp(newEle, ind)
        elif '=' in ele:
            assignOp(ele, ind)
        # we've entered either an if, for or while
        elif 'ifFalse' in ele:
            forCount = ""
            # for or while
            if 'in' in ele:
                # for loop
                forCount = searchregister("forCount", ind)
                print("LDR", forCount + ",", "#0")
            if ":" in ele:
                # for or while
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
                # while
                reg1 = searchregister(left, ind)
                print("LDR", reg1 + ",", left)
                reg2 = searchregister(right, ind)
                print("LDR", reg2 + ",", right)
                print("CMP", reg1 + ",", reg2)
                oppOp = findRelOp(op)
                print("B" + oppOp, bnum)
                print()
            else:
                reg2 = searchregister("forChar", ind)
                print("LDR", reg2 + ",", right + "(" + forCount + ")")
                print("ADD", forCount + ",", forCount + ",", "#1")
                print("CMP", reg2 + ",", "0x00")
                print("BEQ", bnum)
                print()


def store():
    global variableCount
    keys = list(variableCount.keys())
    while variableCount:
        var = keys[0]
        reg2 = variableCount[var]
        reg = "R13"
        print("LDR", reg, var)
        print("STR", reg2 + ", [" + reg+"]")
        del keys[0]
        del variableCount[var]


print("Target Code:\n")
assembler(ICG)
store()
print("\nEnd of Target Code!")
