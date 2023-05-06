valid_result = 0
pre_calculated_factorial = 1

for i in range(1, 10001):
    pre_calculated_factorial *= i
    valid_result = valid_result + pre_calculated_factorial

try:
    with open('output', 'r') as file:
        test_number = 1
        for line in file:
            calculated_data = 0
            try:
                calculated_data = int(line.rstrip().split()[-1])
            except ValueError:
                print("Test №" + str(test_number) + ": Wrong input!!!")
                continue
            if calculated_data == valid_result:
                print("Test №" + str(test_number) + ": Passed!")
            else:
                print("Test №" + str(test_number) + ": Failed :(")
            test_number = test_number + 1
except EnvironmentError:
    print("File not found! Must be named 'output'.")
