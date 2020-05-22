# this is a test comment, to be removed by our compiler
import numpy

a = 10
b = 20
c = 5
s = "string!"

"""
this is a test
multiline comment
to be removed
"""

if a < b:
    a = a + b * c
    c = b

d = 10

for char in s:
    print(char)

e = a + b * c / d