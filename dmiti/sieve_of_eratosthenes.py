import math

n = int(input())
list = list(range(2, n+1))

x = 2
while (x < math.sqrt(n)):
    for y in range(2*x, n+1, x):
        if y in list:
            list.remove(y)
    x = list[list.index(x)+1]
print(list)