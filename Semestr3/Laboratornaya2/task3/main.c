#include <stdio.h>
#include <math.h>
#include <limits.h>

double fact(int n) {
  double factorial = 1;
  for (int i = 2; i <= n; ++i) {
    factorial *= i;
  }
  return factorial;
}

//---Вычисление через предел---
double e_lim(double* x) {
  *x += 100000;
  return pow(1 + 1.0 / *x, *x);
}

double pi_lim(double* x) {
  *x += 100000;
  double product = 1;
  for (int i = 2; i <= *x; i += 2) {
    product *= i / (i - 1.0);
  }
  return pow(product, 2) * 4 / (2 * *x + 1);
}

double ln2_lim(double* x) {
  *x += 100000;
  return *x * (pow(2, 1.0 / *x) - 1);
}

double sqrt2_lim(double* x) {
  *x = *x - *x * *x / 2 + 1;
  return *x;
}

double gamma_lim(double* x) {
  *x += 1;
  static double fraction_sum = 0;
  fraction_sum += 1 / *x;
  return fraction_sum - log(*x);
}

double calculate_limit(double (*function)(double* x), double start, const double epsilon) {
  double x = start;
  double previous_iteration = function(&x);
  double current_iteration = function(&x);
  while (fabs(current_iteration - previous_iteration) > epsilon) {
    previous_iteration = current_iteration;
    current_iteration = function(&x);
  }
  return current_iteration;
}
//------

//---Вычисление через ряд/произведение---
double sum(double a, double b) {
  return a + b;
}

double multiply(double a, double b) {
  return a * b;
}

double none(double a, double b) {
  return b;
}

double e_ser(int* x) {
  return 1.0 / fact((*x)++);
}

double pi_ser(int* x) {
  double step = (*x % 2) ? 1.0 / (2 * *x - 1) : -1.0 / (2 * *x - 1);
  ++*x;
  step += (*x % 2) ? 1.0 / (2 * *x - 1) : -1.0 / (2 * *x - 1);
  ++*x;
  return step;
}

double ln2_ser(int* x) {
  double step = (*x % 2) ? 1.0 / *x : -1.0 / *x;
  ++*x;
  step += (*x % 2) ? 1.0 / *x : -1.0 / *x;
  ++*x;
  return step;
}

double sqrt2_ser(int* x) {
  return pow(2, pow(2, -(*x)++));
}

double gamma_ser(int* x) {
  static double fraction_sum = 0;
  fraction_sum += 1 / pow(floor(sqrt(*x)), 2) - 1.0 / *x;
  ++*x;
  fraction_sum += 1 / pow(floor(sqrt(*x)), 2) - 1.0 / *x;
  ++*x;
  return -pow(acos(-1), 2) / 6 + fraction_sum;
}

double calculate_series(double (*function)(int* x), int start, const double epsilon, double (*include)(double a, double b)) {
  int* x = &start;
  double previous_iteration = function(x);
  double current_iteration = include(previous_iteration, function(x));
  while (fabs(current_iteration - previous_iteration) > epsilon) {
    previous_iteration = current_iteration;
    current_iteration = include(current_iteration, function(x));
  }
  return current_iteration;
}
//------

//---Вычисление через корень уравнения---
double e_eqo() {
  return exp(1);
}

double pi_eqo() {
  return acos(-1);
}

double ln2_eqo() {
  return log(2);
}

double sqrt2_eqo() {
  return sqrt(2);
}

int is_prime(int x) {
  for (int i = 2; i <= (int)sqrt(x); ++i) {
    if (x % i == 0) {
      return 0;
    }
  }
  return 1;
}

double get_prime_mult(double* x) {
  static double prime_mult = 0.5;
  static double last_prime = 2;
  if (last_prime + 1 > *x) {
    return prime_mult;
  }
  for (++last_prime; last_prime <= *x; ++last_prime) {
    if (is_prime(last_prime)) {
      prime_mult *= (last_prime - 1) / last_prime;
    }
  }
  --last_prime;
  return prime_mult;
}

double gamma_func(double* x) {
  double step = log(*x) * get_prime_mult(x);
  *x += 1000;
  return step;
}

double gamma_eqo(double epsilon) {
  return -log(calculate_limit(gamma_func, 2, epsilon));
}

//------

int main() {

  int l;
  printf("Enter accuracy of calculations: ");
  scanf("%d", &l);

  const double epsilon = pow(10, -l - 1);

  printf("--------------------\nCalculations via limit:\n");
  printf("e = %.*lf\n", l, calculate_limit(e_lim, 0, epsilon));
  printf("pi = %.*lf\n", l, calculate_limit(pi_lim, 0, epsilon));
  printf("ln2 = %.*lf\n", l, calculate_limit(ln2_lim, 0, epsilon));
  printf("sqrt2 = %.*lf\n", l, calculate_limit(sqrt2_lim, 0.5, epsilon));
  printf("gamma = %.*lf\n", l, calculate_limit(gamma_lim, 0, epsilon));
  printf("--------------------\nCalculations via series:\n");
  printf("e = %.*lf\n", l, calculate_series(e_ser, 0, epsilon, sum));
  printf("pi = %.*lf\n", l, 4 * calculate_series(pi_ser, 1, epsilon, sum));
  printf("ln2 = %.*lf\n", l, calculate_series(ln2_ser, 1, epsilon, sum));
  printf("sqrt2 = %.*lf\n", l, calculate_series(sqrt2_ser, 2, epsilon, multiply));
  printf("gamma = %.*lf\n", l, calculate_series(gamma_ser, 2, epsilon, none));
  printf("--------------------\nCalculations via solving equotion:\n");
  printf("e = %.*lf\n", l, e_eqo());
  printf("pi = %.*lf\n", l, pi_eqo());
  printf("ln2 = %.*lf\n", l, ln2_eqo());
  printf("sqrt2 = %.*lf\n", l, sqrt2_eqo());
  printf("gamma = %.*lf\n", l, gamma_eqo(epsilon));
  printf("--------------------\n");

  return 0;
}
