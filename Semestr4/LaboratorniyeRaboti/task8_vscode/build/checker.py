a = 1
b = 1

res = 0

for i in range(1, 10001):
    if i % 2 == 0:
        a *= i
        res += a
    else:
        b *= i
        res += b

with open('output', 'r') as file:
    answ = int(file.readline().split()[-1])
    if answ == res:
        print("Test PASSED!")
    else:
        print("Test FAILED!")
