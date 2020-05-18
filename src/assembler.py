# open output file
# extract ICG from file
import sys
sys.stdout = open('../test/output.txt', 'a')

outputList = []
registerCount = 0
goto = ""

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
    
def assignOp(ele):
    global registerCount, goto
    spaceSplit = ele.split()
    if len(spaceSplit) == 3:
        if spaceSplit[2].isdigit():
            reg = "R" + str(registerCount)
            registerCount += 1
            print("LD", reg + ",", "#" + spaceSplit[2])
        else:
            if "\"" in spaceSplit[2]:
                print("str: .asciiz", spaceSplit[2])
                reg = "R" + str(registerCount)
                print("LD", reg + ",", "str")
            else:
                reg = "R" + str(registerCount)
                registerCount += 1
                print("LD", reg + ",", spaceSplit[2])
        print("ST", spaceSplit[0] + ",", reg)
    elif len(spaceSplit) != 3:
        op = findBinOp(spaceSplit[3])
        reg = "R" + str(registerCount)
        registerCount += 1
        print("LD", reg + ",", spaceSplit[2])
        print(op, reg + ",", reg + ",", spaceSplit[4])
        print("ST", spaceSplit[0] + ",", reg)
    if goto:
        print("GOTO", goto)
        goto = ""
    print()


def assembler(ICG):
    print("Assembly Code:\n")
    global registerCount, goto
    for ind in range(len(ICG)):
        ele = ICG[ind]
        if ':' in ele and '=' in ele:
            spaceSplit = ele.split()
            print(spaceSplit[0])
            spaceSplit.pop(0)
            newEle = ' '.join(spaceSplit)
            assignOp(newEle)
        elif '=' in ele:
            assignOp(ele)
        # we've entered either an if, for or while
        elif 'ifFalse' in ele:
            forReg = ""
            # for or while
            if 'in' in ele:
                forReg = "R" + str(registerCount)
                registerCount += 1
                print("LD", forReg + ",", "#0")
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
                print("LD ", reg1 + ",", left)
                registerCount += 1
                reg2 = "R" + str(registerCount)
                print("LD ", reg2 + ",", right)
                registerCount += 1
                print("CMP ", reg1 + ",", reg2)
                oppOp = findRelOp(op)
                print("B" + oppOp, bnum)
            else:
                reg2 = "R" + str(registerCount)
                print("LD ", reg2 + ",", right + "(" + forReg + ")")
                print("ADD", forReg + ",", forReg + ",", "#1")
                registerCount += 1
                print("CMP", reg2 + ",", "0x00")
                print("BEQ", bnum)

assembler(ICG)