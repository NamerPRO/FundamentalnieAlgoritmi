#include "standart_commands_lib.h"

void load(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status == WRONG_VARIABLE_NAME || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name \"%s\" passed in load command.\n", arg);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  FILE* file = fopen(get_raw_string(arg), "r");
  if (file == NULL) {
    print_string(stdout, "ERROR! No file \"%s\" found.\n", arg);
    free_heap_string(arg);
    return;
  }
  int current_array_length = 0;
  int maximum_array_length = 0;
  while ((execute_status = read_string(file, arg, isspace_s)) != EOF) {
    if (execute_status != SUCCESS_FUNCTION_RETURN) {
      fclose(file);
      free_heap_string(arg);
      return;
    }
    if (is_string_empty(arg)) {
      continue;
    }
    int possible_element = get_integer_from_string(arg, &execute_status);
    if (execute_status != NUMBER) {
      print_string(stdout, "ERROR! Wrong not integer element \"%s\" found. Skipping it.\n", arg);
      continue;
    }
    if (current_array_length == maximum_array_length) {
      if (maximum_array_length == 0) {
        maximum_array_length = 1;
      }
      int* possible_array = (int*)realloc(array, sizeof(int) * maximum_array_length * 2);
      if (possible_array == NULL) {
        free_heap_string(arg);
        fclose(file);
        return;
      }
      array = possible_array;
      maximum_array_length *= 2;
    }
    array[current_array_length++] = possible_element;
  }
  int* possible_array = (int*)realloc(array, sizeof(int) * current_array_length);
  if (possible_array == NULL) {
    free_heap_string(arg);
    fclose(file);
    return;
  }
  array = possible_array;
  update_variable(variable_name, array, current_array_length);
  free_heap_string(arg);
  fclose(file);
}

void save(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in save command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  FILE* file = fopen(get_raw_string(arg), "w");
  if (file == NULL) {
    print_string(stdout, "ERROR! File \"%s\" could not be accessed.\n", arg);
    free_heap_string(arg);
    return;
  }
  int array_length = get_array_size(variable_name, &execute_status);
  for (int i = 0; i < array_length; ++i) {
    print_string(file, "%d ", array[i]);
  }
  fclose(file);
  free_heap_string(arg);
}

void _rand(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status == WRONG_VARIABLE_NAME || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name \"%s\" passed in rand command.\n", arg);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  int count = get_integer_from_string(arg, &execute_status);
  if (execute_status != NUMBER) {
    print_string(stdout, "ERROR! Wrong elements in array amount \"%s\" passed!\n", arg);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  int from = get_integer_from_string(arg, &execute_status);
  if (execute_status != NUMBER) {
    print_string(stdout, "ERROR! Wrong number \"%s\" as minimum number in random range passed!\n", arg);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  int to = get_integer_from_string(arg, &execute_status);
  if (execute_status != NUMBER) {
    print_string(stdout, "ERROR! Wrong number \"%s\" as maximum number in random range passed!\n", arg);
    free_heap_string(arg);
    return;
  }
  int maximum_array_length = 0;
  srand(time(NULL));
  for (int i = 0; i < count; ++i) {
    if (i == maximum_array_length) {
      if (maximum_array_length == 0) {
        maximum_array_length = 1;
      }
      int* possible_array = (int*)realloc(array, sizeof(int) * maximum_array_length * 2);
      if (possible_array == NULL) {
        free_heap_string(arg);
        return;
      }
      array = possible_array;
      maximum_array_length *= 2;
    }
    array[i] = rand() % (to + 1 - from) + from;
  }
  int* possible_array = (int*)realloc(array, sizeof(int) * count);
  if (possible_array == NULL) {
    free_heap_string(arg);
    return;
  }
  array = possible_array;
  update_variable(variable_name, array, count);
  free_heap_string(arg);
}

void concat(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in concat command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  char variable_name2 = toupper(_char(arg));
  int* array2;
  get_variable_or_null(variable_name2, &array2, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in concat command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  int array_length = get_array_size(variable_name, &execute_status);
  int array_length2 = get_array_size(variable_name2, &execute_status);
  int* possible_array = (int*)realloc(array, sizeof(int) * (array_length + array_length2));
  if (possible_array == NULL) {
    print_string(stdout, "ERROR! Not enought memory to store data.\n");
    free_heap_string(arg);
    return;
  }
  if (array == array2) {
    array2 = possible_array;
  }
  array = possible_array;
  for (int i = 0; i < array_length2; ++i) {
    array[array_length + i] = array2[i];
  }
  update_variable(variable_name, array, array_length + array_length2);
  free_heap_string(arg);
}

void _free(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in free command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  free(array);
  array = NULL;
  update_variable(variable_name, array, 0);
  free_heap_string(arg);
}

void _remove(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in remove command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  int from = get_integer_from_string(arg, &execute_status);
  if (execute_status != NUMBER) {
    print_string(stdout, "ERROR! Wrong number \"%s\" as start index passed!\n", arg);
    free_heap_string(arg);
    return;
  }
  int array_length = get_array_size(variable_name, &execute_status);
  if (from < 0 || from >= array_length) {
    print_string(stdout, "ERROR! Index \"%d\" is out of array bounds!\n", from);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  int count = get_integer_from_string(arg, &execute_status);
  if (execute_status != NUMBER) {
    print_string(stdout, "ERROR! Wrong number \"%s\" as amount of numbers to delete passed!\n", arg);
    free_heap_string(arg);
    return;
  }
  if (count < 0 || from + count > array_length) {
    print_string(stdout, "ERROR! Too many elements to delete requested or this number is negative!\n");
    free_heap_string(arg);
    return;
  }
  for (int i = from + count; i < array_length; ++i) {
    array[i - count] = array[i];
  }
  int* possible_array = (int*)realloc(array, sizeof(int) * (array_length - count + 1));
  if (possible_array == NULL) {
    free_heap_string(arg);
    return;
  }
  array = possible_array;
  update_variable(variable_name, array, array_length - count);
  free_heap_string(arg);
}

void copy(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in copy command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  int from = get_integer_from_string(arg, &execute_status);
  if (execute_status != NUMBER) {
    print_string(stdout, "ERROR! Wrong number \"%s\" as start index passed!\n", arg);
    free_heap_string(arg);
    return;
  }
  int array_length = get_array_size(variable_name, &execute_status);
  if (from < 0 || from >= array_length) {
    print_string(stdout, "ERROR! Index \"%d\" is out of array bounds!\n", from);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  int to = get_integer_from_string(arg, &execute_status);
  if (execute_status != NUMBER) {
    print_string(stdout, "ERROR! Wrong number \"%s\" as start index passed!\n", arg);
    free_heap_string(arg);
    return;
  }
  if (to < 0 || to >= array_length) {
    print_string(stdout, "ERROR! Index \"%d\" is out of array bounds!\n", to);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  char variable_name2 = toupper(_char(arg));
  int* array2;
  get_variable_or_null(variable_name2, &array2, &execute_status);
  if (execute_status == WRONG_VARIABLE_NAME || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name \"%s\" passed in copy command.\n", arg);
    free_heap_string(arg);
    return;
  }
  int flag = 0;
  if (array == array2) {
    flag = 1;
  }
  if (array2 != NULL) {
    free(array2);
  }
  array2 = (int*)malloc(sizeof(int) * (to - from + 1));
  if (array2 == NULL) {
    free_heap_string(arg);
    return;
  }
  for (int i = from; i <= to; ++i) {
    array2[i - from] = array[i];
  }
  if (flag) {
    free(array);
  }
  update_variable(variable_name2, array2, (to - from + 1));
  free_heap_string(arg);
}

int compare_int_max(const void* lhs, const void* rhs) {
  int ilhs = *(const int*) lhs;
  int irhs = *(const int*) rhs;
  return (ilhs > irhs) - (ilhs < irhs);
}

int compare_int_min(const void* lhs, const void* rhs) {
  int ilhs = *(const int*) lhs;
  int irhs = *(const int*) rhs;
  return (ilhs < irhs) - (ilhs > irhs);
}

void sort(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in sort command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  if (sat(arguments, start - 1) == '+') {
    qsort(array, get_array_size(variable_name, &execute_status), sizeof(int), compare_int_max);
  } else {
    qsort(array, get_array_size(variable_name, &execute_status), sizeof(int), compare_int_min);
  }
  free_heap_string(arg);
}

//Shuffle A
void shuffle(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in shuffle command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  int arrays_size = get_array_size(variable_name, &execute_status);
  srand(time(NULL));
  for (int i = 0; i < arrays_size; ++i) {
    int index = rand() % arrays_size;
    int save = array[i];
    array[i] = array[index];
    array[index] = save;
  }
  free_heap_string(arg);
}

void stats(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  char variable_name = toupper(_char(arg));
  int* array;
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in stats command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  int array_length = get_array_size(variable_name, &execute_status);
  free_heap_string(arg);
  int* array_copy = (int*)malloc(sizeof(int) * array_length);
  if (array_copy == NULL) {
    free_heap_string(arg);
    return;
  }
  for (int i = 0; i < array_length; ++i) {
    array_copy[i] = array[i];
  }
  qsort(array_copy, array_length, sizeof(int), compare_int_min);
  int max_element = INT_MIN, max_element_index;
  int min_element = INT_MAX, min_element_index;
  int most_common_element = array_copy[0], most_common_element_count = 1;
  int most_common_element_max, most_common_element_count_max = 0;
  int sum = 0;
  for (int i = 0; i < array_length; ++i) {
    sum += array[i];
    if (i && most_common_element == array_copy[i]) {
      ++most_common_element_count;
    } else if (i) {
      if (most_common_element_count_max < most_common_element_count) {
        most_common_element_max = most_common_element;
        most_common_element_count_max = most_common_element_count;
      }
      most_common_element = array_copy[i];
      most_common_element_count = 1;
    }
    if (max_element < array[i]) {
      max_element = array[i];
      max_element_index = i;
    }
    if (min_element > array[i]) {
      min_element = array[i];
      min_element_index = i;
    }
  }
  if (most_common_element_count_max < most_common_element_count) {
    most_common_element_max = most_common_element;
    most_common_element_count_max = most_common_element_count;
  }
  free(array_copy);
  double middle_value = (double)sum / array_length;
  double max_deviation = fabs(max_element - middle_value);
  if (max_deviation < fabs(min_element - middle_value)) {
    max_deviation = fabs(min_element - middle_value);
  }
  print_string(stdout, "Information about array \"%c\":\nSize: %d\nMaximum element: %d (index=%d)\nMinimum element: %d (index=%d)\nMost common element: %d\nMiddle value: %f\nMaximum deviation from middle value: %f\n", variable_name, array_length, max_element, max_element_index, min_element, min_element_index, most_common_element_max, middle_value, max_deviation);
}

void print(string* arguments, unsigned long int start) {
  string* arg = heap_string(EMPTY);
  if (arg == NULL) {
    return;
  }
  get_next_argument(arg, arguments, &start);
  int execute_status;
  int* array;
  char variable_name = toupper(_char(arg));
  get_variable_or_null(variable_name, &array, &execute_status);
  if (execute_status != SUCCESS_FUNCTION_RETURN || sat(arg, 1) != '\0') {
    print_string(stdout, "ERROR! Wrong variable name passed in print command or it is not initialized.\n", arg);
    free_heap_string(arg);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  int array_length = get_array_size(variable_name, &execute_status);
  if (!scompare(arg, "all")) {
    for (int i = 0; i < array_length; ++i) {
      print_string(stdout, "%d ", array[i]);
    }
    print_string(stdout, "\n");
    free_heap_string(arg);
    return;
  }
  int begin = get_integer_from_string(arg, &execute_status);
  if (execute_status != NUMBER) {
    free_heap_string(arg);
    print_string(stdout, "ERROR! Begin index not a number.\n");
    return;
  }
  if (begin >= array_length || begin < 0) {
    free_heap_string(arg);
    print_string(stdout, "ERROR! Begin index \"%d\" out of bounds.\n", begin);
    return;
  }
  ++start;
  get_next_argument(arg, arguments, &start);
  int end = get_integer_from_string(arg, &execute_status);
  if (execute_status != NUMBER) {
    free_heap_string(arg);
    print_string(stdout, "ERROR! End index not a number.\n");
    return;
  }
  if (end >= array_length || end < 0) {
    free_heap_string(arg);
    print_string(stdout, "ERROR! End index \"%d\" out of bounds.\n", end);
    return;
  }
  for (int i = begin; i <= end; ++i) {
    print_string(stdout, "%d ", array[i]);
  }
  print_string(stdout, "\n");
  free_heap_string(arg);
}
