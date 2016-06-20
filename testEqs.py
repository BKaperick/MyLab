ops = ['*','-','+']

def op(in1,in2,s):
    c1 = int(in1)
    c2 = int(in2)
    if s == '*':
        return c1*c2
    elif s == '-':
        return c1-c2
    elif s == '+':
        return c1+c2

def compare(c1, c2):
    if c1 == '*':
        if c2 == '*':
            return 0
        else:
            return 1
    if c2 == '*':
        return -1
    return 0

def postfix(eq,debug=False):
    deck = ''
    output = ''
    skipTo = -1
    for i,c in enumerate(eq):
        if skipTo >= i:
            print c,skipTo,i
        elif c in ops:
            deck += c
        elif c == '(':
            depth = 1
            for j,c in enumerate(eq[i+1:]):
                if c == '(':
                    depth += 1
                elif c == ')':
                    depth -= 1
                if depth == 0:
                    interiorEnd = i+1+j
                    break
            interior = eq[i+1:interiorEnd]
            output += postfix(interior)

            #while deck is nonempty and either we are at the last element, or next op is less pressing
            while (len(deck) > 0) and ( (interiorEnd == len(eq)-1) or (compare(deck[-1], eq[interiorEnd+1]) > 0) ):
                output += deck[-1]
                deck = deck[:-1]
            
            skipTo = interiorEnd
        else:
            output += c
            while (len(deck) > 0) and ( (i == len(eq)-1) or (compare(deck[-1], eq[i+1]) > 0) ):
                output += deck[-1]
                deck = deck[:-1]
    return output

def evaluate(postfix):
    stack = []
    for c in postfix:
        if c in ops:
            temp = op(stack[-2],stack[-1],c)
            stack = stack[:-2]
            stack.append(temp)
        else:
            stack.append(int(c))
    return stack[0]

def ev(eq):
    temp = postfix(eq)
    print temp
    return evaluate(temp)
