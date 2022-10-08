#include <stdio.h>
#include <math.h>

double input_equotion(double x) {
  return cos(x) - exp(-x * x / 2) + x - 1;
}

// double get_root_via_dichotomy_method(double a, double b, double epsilon, double (*function)(double x)) {
//   double c = (a + b) / 2;
//   if (fabs(function(c)) <= epsilon) { //|| fabs(b - c) <= epsilon
//     return c;
//   }
//   if (function(c) * function(a) < 0) {
//     return get_root_via_dichotomy_method(a, c, epsilon, function);
//   }
//   return get_root_via_dichotomy_method(c, b, epsilon, function);
// }

double get_root_via_dichotomy_method(double a, double b, double epsilon, double (*function)(double x)) {
  double c = 0.0;
  while (fabs(a - b) > epsilon) {
    c = (a + b) / 2;
    if (function(a) * function(c) < 0) {
      b = c;
    } else {
      a = c;
    }
  }
  return c;
}

int main() {
  double root = get_root_via_dichotomy_method(1, 2, 1e-9, input_equotion);
  printf("Root: %.8lf\n", root);
  return 0;
}
