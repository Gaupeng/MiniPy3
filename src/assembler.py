# open output file
# extract ICG from file
import sys
sys.stdout = open('../test/output2.txt', 'a')

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
    
def assignOp(ele, indexele):
    global registerCount, goto
    spaceSplit = ele.split()
    if len(spaceSplit) == 3:
        if spaceSplit[2].isdigit():

            reg = searchregister(str(spaceSplit[0]), indexele)
            reg =str(reg)
            # registerCount += 1
            print("MOV", reg + ",", "#" + spaceSplit[2])
            deleteregister(str(spaceSplit[0]),indexele)
        else:
            if "\"" in spaceSplit[2]:
                print("str: .asciiz", spaceSplit[2])
                reg1 = searchregister("str", indexele)
                reg1 = str(reg1)
                # registerCount += 1
                print("LDR", reg1 + ",", "str")
                reg2 = searchregister(spaceSplit[0], indexele)
                reg2=str(reg2)
                print("LDR", reg2 + ",",spaceSplit[0])
                # registerCount += 1
                reg3 = newregister()
                reg3 = str(reg3)
                print("LDR", reg3 + ", ["+ reg1+"]")
                print("STR", reg3 + ", ["+ reg2+"]")
                deleteregister("str",indexele)
                deleteregister(str(spaceSplit[0]),indexele)
                # registerCount += 1
            else:
                reg1= searchregister(spaceSplit[2], indexele)
                reg1 = str(reg1)
                #reg1 = str(reg1)
                # registerCount += 1
                print("LDR", reg1 + ",", spaceSplit[2])
                reg2 = searchregister(spaceSplit[0], indexele)
                reg2 = str(reg2)
                print("LDR", reg2 + ",",spaceSplit[0])
                # registerCount += 1
                reg3= newregister()
                reg3 = str(reg3)
                print("LDR", reg3 + ", ["+ reg1+"]")
                print("STR", reg3 + ", ["+ reg2+"]")
                deleteregister(str(spaceSplit[2]),indexele)
                if(spaceSplit[2] != spaceSplit[0]):
                	deleteregister(str(spaceSplit[0]),indexele)
                # registerCount += 1
        # print("ST", spaceSplit[0] + ",", reg)
    elif len(spaceSplit) != 3:
        op = findBinOp(spaceSplit[3])
        reg1 = searchregister(spaceSplit[2], indexele)
        reg1 = str(reg1)
        # registerCount += 1
        print("LDR", reg1 + ",", spaceSplit[2])
        reg2 = searchregister(spaceSplit[0], indexele)
        reg2=str(reg2)
        # registerCount += 1
        print("LDR", reg2 + ",", spaceSplit[0])
        reg3 = newregister()
        reg3 = str(reg3)
        registerCount += 1
        print("LDR",reg3 + ", "+"["+ reg1+"]")
        print(op, reg3 + ",", reg3 + ",", spaceSplit[4])
        print("STR",reg3 + ", ["+ reg2+"]")
        deleteregister(str(spaceSplit[2]),indexele)
        if(spaceSplit[2] != spaceSplit[0]):
        	deleteregister(str(spaceSplit[0]),indexele)
    if goto:
        print("B", goto)
        goto = ""
    registerCount = 0
    print()


def searchregister(variable, index):
	global variableCount
	if len(variableCount) == 13:
		reg = "R13"
		var = list(variableCount.keys())[0]
		reg2 = variableCount[var]
		print("LDR", reg, var)
		print("STR", reg2 + ", ["+ reg+"]"  )
		del variableCount[var]

	if(variable in variableCount):
		reg = variableCount[variable]
		variableCount.pop(variable)
		variableCount[variable] = reg
		return variableCount[variable]
	else:
		for i in range(13):
			register = "R"+str(i);
			if register not in variableCount.values():
				reg = "R" +str(i)
				variableCount[variable] = reg
				return str(reg)


def newregister():
	global variableCount
	if len(variableCount) == 13:
		reg = "R13"
		var = list(variableCount.keys())[0]
		reg2 = variableCount[var]
		print("LDR", reg, var)
		print("STR", reg2 + ", ["+ reg+"]"  )
		del variableCount[var]
	for i in range(13):
			if ("R"+str(i)) not in variableCount.values():
				reg = "R" +str(i)
				return str(reg)


def deleteregister(variable,index):
	
	index = index +1
	newindex = len(ICG)

	newlist = list(ICG[index:newindex])

	if str(variable) in str(newlist):
		return
	variableCount.pop(variable)


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
    print(ICG)
    for ind in range(len(ICG)):
        #incVariableCount()
        ele = ICG[ind]
        if ':' in ele and '=' in ele:
            spaceSplit = ele.split()
            print(spaceSplit[0])
            spaceSplit.pop(0)
            newEle = ' '.join(spaceSplit)
            assignOp(newEle,ind)
        elif '=' in ele:
            assignOp(ele,ind)
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
