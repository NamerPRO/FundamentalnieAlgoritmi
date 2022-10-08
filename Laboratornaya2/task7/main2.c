#include <stdio.h>
#include <stdarg.h>
#include <math.h>

double get_polynom_value(double dot, int power, ...) {
  va_list coefficients;
  va_start(coefficients, power);
  double value = 0.0;
  for (int i = power; i >= 0; --i) {
    value += va_arg(coefficients, double) * pow(dot, i);
  }
  va_end(coefficients);
  return value;
}

int main() {
  printf("Polynom value: %lf\n", get_polynom_value(-2.987, 5, 1.7, 8.0, 13.2, 0.0, 1.0, 10.0));

  return 0;
}
