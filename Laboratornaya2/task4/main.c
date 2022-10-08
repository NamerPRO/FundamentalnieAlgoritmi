#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#define UNKNOWN_SPECIFICATOR_EXCEPTION 1
#define WRONG_FORMAT_STRING_EXCEPTION 2
#define SUCCESS_GEOMETRIC_MEAN 0
#define UNEXPECTED_RESULT 3

int geometric_mean(char* format, double* answer, ...) {
  int count = 0;
  double result = 1;
  va_list args;
  va_start(args, answer);
  for (int i = 0; format[i]; ++i) {
    if (format[i] != '%') {
      if (i > 0 && format[i - 1] == '%') {
        switch (format[i]) {
          case 'd':
            result *= va_arg(args, int);
            ++count;
            break;
          case 'f':
            result *= va_arg(args, double);
            ++count;
            break;
          default:
            return UNKNOWN_SPECIFICATOR_EXCEPTION;
        }
      } else {
        return WRONG_FORMAT_STRING_EXCEPTION;
      }
    }
  }
  va_end(args);
  if (count % 2 == 0 && result < 0) {
    return UNEXPECTED_RESULT;
  }
  int negative_flag = (result < 0) ? -1 : 1;
  *answer = negative_flag * pow(negative_flag * result, 1.0 / count);
  return SUCCESS_GEOMETRIC_MEAN;
}

double get_power(double x, int y) {
  if (!y) {
    return 1;
  }
  return x * get_power(x, y - 1);
}

double power(double x, int y) {
  return (y < 0) ? 1 / get_power(x, -y) : get_power(x, y);
}

int main() {
  printf("PART 1:\n");
  double geom_mean = 0;
  //Надо приводить (int)1???
  if (!geometric_mean("%d%d%f", &geom_mean, (int)1, (int)5, (double)-123.7)) {
    printf("Geometric mean is: %lf\n", geom_mean);
  } else {
    printf("An error occured while using geometric_mean function!\n");
  }
  printf("------\n");

  printf("PART 2:\n");
  printf("Enter number and a power: ");
  double number;
  int number_power;
  scanf("%lf%d", &number, &number_power);
  printf("%lf^%d is %lf\n", number, number_power, power(number, number_power));
  printf("------\n");

  return 0;
}
