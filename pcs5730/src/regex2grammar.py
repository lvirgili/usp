from sys import stdin
import sys

def grammarize(regex):
    stack = []
    curr = 'A'
    regex.replace(" ", "")
    for char in regex:
        if char is not ')' and char is not '+' and char is not '*':
            stack.append(char)
        elif char is ')':
            term = ""
            stack.append(')')
            while stack[-1] is not '(':
                term += stack.pop()
            term += stack.pop()
            term = term[::-1]
            stack.append(curr)
            print(curr + " = " + str(term))
            curr = chr(ord(curr) + 1)
        elif char is '+':
            while stack[-1] == ' ':
                stack.pop()
            term = '{' + str(stack.pop()) + '}'
            stack.append(curr)
            print(curr + " = " + str(term))
            curr = chr(ord(curr) + 1)
        elif char is '*':
            while stack[-1] == ' ':
                stack.pop()
            term = '[' + str(stack.pop()) + ']'
            stack.append(curr)
            print(curr + " = " + str(term))
            curr = chr(ord(curr) + 1)




if len(sys.argv) > 1:
    grammarize(sys.argv[1])
else:
    grammarize(stdin.readline())