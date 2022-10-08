unsigned int safe_sum(const unsigned int a, const unsigned int b, int* is_overflowed) {
  unsigned int sum = a + b;
  *is_overflowed = sum < a || sum < b;
  return sum;
}

unsigned int safe_mult(const unsigned int a, const unsigned int b, int* is_overflowed) {
  if (!a || !b) {
    *is_overflowed = 0;
    return 0;
  }
  unsigned int multiple = a * b;
  if (multiple / b == a) {
    *is_overflowed = 0;
  } else {
    *is_overflowed = 1;
  }
  return multiple;
}
