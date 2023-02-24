#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#define EPSILON 1e-6

typedef struct point {
  double x;
  double y;
} point;

point get_vecotor(point a, point b) {
  return (point) { .x = b.x - a.x, .y = b.y - a.y };
}

int is_covex(int amount_of_dots, ...) {
  va_list dots;
  va_start(dots, amount_of_dots);
  point point1 = va_arg(dots, point);
  point point2 = va_arg(dots, point);
  point previous_vector = get_vecotor(point1, point2);
  point current_vector;
  int is_line_flag = 1;
  double product;
  for (int i = 2; i < amount_of_dots; ++i) {
    point point3 = va_arg(dots, point);
    current_vector = get_vecotor(point2, point3);
    product = -previous_vector.x * current_vector.y + previous_vector.y * current_vector.x;
    if (product < 0) {
      va_end(dots);
      return 0;
    }
    if (fabs(product) > EPSILON) {
      is_line_flag = 0;
    }
    point2 = point3;
    previous_vector = current_vector;
  }
  va_end(dots);
  current_vector = get_vecotor(point2, point1);
  product = -previous_vector.x * current_vector.y + previous_vector.y * current_vector.x;
  if (product < 0 || (fabs(product) < EPSILON && is_line_flag)) {
    return 0;
  }
  return 1;
}

int main() {
  if (is_covex(4, (point) {.x = 1, .y = 1}, (point) {.x = 1, .y = 3}, (point) {.x = 1.5, .y = 2}, (point) {.x = 4, .y = 3.77})) {
    printf("Given polygon is regular.\n");
  } else {
    printf("Given polygom is irregular or a line was entered.\n");
  }

  return 0;
}
