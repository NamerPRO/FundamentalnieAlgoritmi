#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <ctype.h>

#include "overflow.h"

#define NO_FLAG_GIVEN 1
#define WRONG_ARGUMENTS_EXCEPTION 2
#define WRONG_NUMBER_EXCEPTION 3
#define UNKNOWN_FLAG_EXCEPTION 4
#define VARIABLE_OVERFLOW_EXCEPTION 5
#define ONE_ROOT 6
#define TWO_ROOTS 7
#define ANY_NUMBER 8
#define IDENTICALLY_FALSE 9

typedef enum FLAG_TYPE {
  Q,
  M,
  T,
  UNKNOWN
} FLAG_TYPE;

typedef struct roots {
  double x1, x2;
} roots;

int char_to_digit(char x) {
  return x - '0';
}

double my_strtod(char* str, int* is_number) {
  int is_negative = 1;
  int i = 0;
  if (*str == '-') {
    is_negative = -1;
    i = 1;
  }
  double dbl_number = 0;
  if (!isdigit(str[i])) {
    *is_number = 0;
    return 0;
  } else {
     dbl_number = char_to_digit(str[i]);
    ++i;
  }
  int is_frac = 0;
  int frac_pos = 10;
	int in_for = 1;
  int is_overflowed = 0;
  for (; i < strlen(str) && in_for; ++i) {
    if (str[i] == '.' && !is_frac) {
      is_frac = 1;
      continue;
    }
    if (!isdigit(str[i])) {
      *is_number = 0;
      return 0;
    }
    switch (is_frac) {
      case 0:
        // dbl_number = dbl_number * 10 + char_to_digit(str[i]);
        dbl_number = safe_mult_dbl(dbl_number, 10, &is_overflowed);
        if (is_overflowed) {
          *is_number = 0;
          return 0;
        }
        dbl_number = safe_sum_dbl(dbl_number, char_to_digit(str[i]), &is_overflowed);
        if (is_overflowed) {
          *is_number = 0;
          return 0;
        }
        break;
      case 1:
        // dbl_number = dbl_number + (double)char_to_digit(str[i]) / frac_pos;
        dbl_number = safe_sum_dbl(dbl_number, (double)char_to_digit(str[i]) / frac_pos, &is_overflowed);
        if (is_overflowed) {
          *is_number = 0;
          return 0;
        }
        frac_pos *= 10;
				if (frac_pos == (int)1e9) {
					in_for = 0;
				}
        break;
    }
  }
  *is_number = 1;
  return dbl_number * is_negative;
}

int my_strtoi(char* str, int* is_number) {
  int is_negative = 1;
  int i = 0;
  if (*str == '-') {
    is_negative = -1;
    i = 1;
  }
  int int_number = 0;
  int is_overflowed;
  for (; i < strlen(str); ++i) {
    if (!isdigit(str[i])) {
      *is_number = 0;
      return 0;
    }
    int_number = safe_mult(int_number, 10, &is_overflowed);
    if (is_overflowed) {
      *is_number = 0;
      return 0;
    }
    int_number = safe_sum(int_number, char_to_digit(str[i]), &is_overflowed);
    if (is_overflowed) {
      *is_number = 0;
      return 0;
    }
  }
  *is_number = 1;
  return int_number * is_negative;
}

FLAG_TYPE get_flag_type(char* str) {
  if (!strcmp(str, "-q") || !strcmp(str, "/q")) {
    return Q;
  }
  if (!strcmp(str, "-m") || !strcmp(str, "/m")) {
    return M;
  }
  if (!strcmp(str, "-t") || !strcmp(str, "/t")) {
    return T;
  }
  return UNKNOWN;
}

double max(double a, double b) {
  return (a > b) ? a : b;
}

int solve_quadratic_equotion(double a, double b, double c, roots* eq_roots) {
  if (fabs(a) > DBL_EPSILON) {
    int is_overflowed_equation = 0;
    double d = safe_mult(b, b, &is_overflowed_equation);
    if (is_overflowed_equation) {
      return VARIABLE_OVERFLOW_EXCEPTION;
    }
    double p = safe_mult(4, a, &is_overflowed_equation);
    if (is_overflowed_equation) {
      return VARIABLE_OVERFLOW_EXCEPTION;
    }
    p = safe_mult(p, c, &is_overflowed_equation);
    if (is_overflowed_equation) {
      return VARIABLE_OVERFLOW_EXCEPTION;
    }
    d = safe_min(d, p, &is_overflowed_equation);
    if (is_overflowed_equation) {
      return VARIABLE_OVERFLOW_EXCEPTION;
    }
    //double d = b * b - 4 * a * c;

    if (fabs(d) < DBL_EPSILON) {
      // printf("x=%lf\n", -b / (2 * a));
      eq_roots->x1 = -b / (2 * a);
      eq_roots->x2 = -b / (2 * a);
      return TWO_ROOTS;
    } else if (d > 0) {
      // printf("x=%lf\n", (-b + sqrt(d)) / (2 * a));
      // printf("x=%lf\n", (-b - sqrt(d)) / (2 * a));
      eq_roots->x1 = (-b + sqrt(d)) / (2 * a);
      eq_roots->x2 = (-b - sqrt(d)) / (2 * a);
      return TWO_ROOTS;
    } else {
      // printf("Equotion has no real roots :(\n");
      return IDENTICALLY_FALSE;
    }
  } else if (fabs(b) > DBL_EPSILON) {
    // printf("x=%lf\n", -c / b);
    eq_roots->x1 = -c / b;
    return ONE_ROOT;
  } else if (fabs(c) < DBL_EPSILON) {
    // printf("x=any number\n");
    return ANY_NUMBER;
  } else {
    // printf("Given identically false equotion!\n");
    return IDENTICALLY_FALSE;
  }
}

void solve_text_output(double a, double b, double c, roots* eq_roots) {
  printf("Solution of %lfx^2+(%lf)x+(%lf)=0 is:\n", a, b, c);
  int code = solve_quadratic_equotion(a, b, c, eq_roots);
  switch (code) {
    case ONE_ROOT:
      printf("x=%lf\n", eq_roots->x1);
      break;
    case TWO_ROOTS:
      printf("x1=%lf\nx2=%lf\n", eq_roots->x1, eq_roots->x2);
      break;
    case ANY_NUMBER:
      printf("Any number is a solution.\n");
      break;
    case IDENTICALLY_FALSE:
      printf("Equation is identically false.\n");
      break;
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    //printf("NO_FLAG_GIVEN\n");
    return NO_FLAG_GIVEN;
  }

  FLAG_TYPE flag = get_flag_type(argv[1]);
  int is_number;
  switch (flag) {
  case Q:
    if (argc != 5) {
      //printf("WRONG_ARGUMENTS_EXCEPTION\n");
      return WRONG_ARGUMENTS_EXCEPTION;
    }
    double a = my_strtod(argv[2], &is_number);
    if (!is_number) {
      //printf("WRONG_NUMBER_EXCEPTION\n");
      return WRONG_NUMBER_EXCEPTION;
    }
    double b = my_strtod(argv[3], &is_number);
    if (!is_number) {
      //printf("WRONG_NUMBER_EXCEPTION\n");
      return WRONG_NUMBER_EXCEPTION;
    }
    double c = my_strtod(argv[4], &is_number);
    if (!is_number) {
      //printf("WRONG_NUMBER_EXCEPTION\n");
      return WRONG_NUMBER_EXCEPTION;
    }
    roots eq_roots;
    solve_text_output(a, b, c, &eq_roots);
    solve_text_output(a, c, b, &eq_roots);
    solve_text_output(b, a, c, &eq_roots);
    solve_text_output(b, c, a, &eq_roots);
    solve_text_output(c, a, b, &eq_roots);
    solve_text_output(c, b, a, &eq_roots);
    break;
  case M:
    if (argc != 4) {
      //printf("WRONG_ARGUMENTS_EXCEPTION\n");
      return WRONG_ARGUMENTS_EXCEPTION;
    }
    int number1 = my_strtoi(argv[2], &is_number);
    if (!is_number || number1 == 0) {
      //printf("WRONG_NUMBER_EXCEPTION\n");
      return WRONG_NUMBER_EXCEPTION;
    }
    int number2 = my_strtoi(argv[3], &is_number);
    if (!is_number || number2 == 0) {
      //printf("WRONG_NUMBER_EXCEPTION\n");
      return WRONG_NUMBER_EXCEPTION;
    }
    if (number1 % number2 == 0) {
      printf("First number is multiple to second one!\n");
    } else {
      printf("First number is NOT multiple to second one!\n");
    }
    break;
  case T:
    if (argc != 5) {
      //printf("WRONG_ARGUMENTS_EXCEPTION\n");
      return WRONG_ARGUMENTS_EXCEPTION;
    }
    double side1 = my_strtod(argv[2], &is_number);
    if (!is_number || fabs(side1) < DBL_EPSILON) {
      //printf("WRONG_NUMBER_EXCEPTION\n");
      return WRONG_NUMBER_EXCEPTION;
    }
    double side2 = my_strtod(argv[3], &is_number);
    if (!is_number || fabs(side2) < DBL_EPSILON) {
      //printf("WRONG_NUMBER_EXCEPTION\n");
      return WRONG_NUMBER_EXCEPTION;
    }
    double side3 = my_strtod(argv[4], &is_number);
    if (!is_number || fabs(side3) < DBL_EPSILON) {
      //printf("WRONG_NUMBER_EXCEPTION\n");
      return WRONG_NUMBER_EXCEPTION;
    }
    if (side1 > 0 && side2 > 0 && side3 > 0) {
      double hypotenuse = max(max(side1, side2), side3);
      double leg1;
      double leg2;
      if (fabs(hypotenuse - side3) < DBL_EPSILON) {
        leg1 = side1;
        leg2 = side2;
      } else if (fabs(hypotenuse - side2) < DBL_EPSILON) {
        leg1 = side1;
        leg2 = side3;
      } else if (fabs(hypotenuse - side1) < DBL_EPSILON) {
        leg1 = side2;
        leg2 = side3;
      }
      if (fabs(leg1 * leg1 + leg2 * leg2 - hypotenuse * hypotenuse) < DBL_EPSILON) {
        printf("Given sides form a rectangular triangle :)\n");
      } else {
        printf("Given sides cannot form a rectangular triangle :(\n");
      }
    } else {
      printf("Given sides cannot form a rectangular triangle due to negative numbers were found :(\n");
    }
    break;
  case UNKNOWN:
    //printf("UNKNOWN_FLAG_EXCEPTION\n");
    return UNKNOWN_FLAG_EXCEPTION;
  }
  return 0;
}
