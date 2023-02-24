#include "student.h"

int create_student_array(student_array* array) {
  array->array = (student*)malloc(sizeof(student) * STANDART_ARRAY_SIZE);
  if (array->array == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  array->total_size = STANDART_ARRAY_SIZE;
  array->real_size = 0;
  return SUCCESS_FUNCTION_RETURN;
}

int insert_student_in_array(student_array* array, student* element) {
  if (array->real_size == array->total_size) {
    student* possible_array = (student*)realloc(array->array, 2 * sizeof(student) * array->total_size);
    if (possible_array == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    array->total_size *= 2;
    array->array = possible_array;
  }
  array->array[array->real_size++] = *element;
  free(element);
  return SUCCESS_FUNCTION_RETURN;
}

student* get_student_array(student_array* array) {
  return array->array;
}

unsigned long int get_student_array_size(student_array* array) {
  return array->real_size;
}

list* group_students_by_course(student* students_array, unsigned long int student_array_size, list* lst) {
  for (unsigned long int i = 0; i < student_array_size; ++i) {
    list_push_back(lst + students_array[i].course - 1, students_array + i);
  }
  return lst;
}

void free_students_array(student_array* students_array) {
  free_students_array_ptr(students_array->array, students_array->real_size);
}

void free_student(student* stud) {
  free(stud->marks);
  free_string(stud->name);
  free(stud->name);
  free_string(stud->surname);
  free(stud->surname);
  free_string(stud->group);
  free(stud->group);
  //free(stud);
}

void free_students_array_ptr(student* students_array, unsigned long int student_array_size) {
  for (unsigned long int i = 0; i < student_array_size; ++i) {
    free_student(students_array + i);
  }
  free(students_array);
}

int standart_student_comporator(string* str1_old, string* str2_old) {
  string str1;
  create_string_duplicate(str1_old, &str1);
  to_upper_case(&str1);
  string str2;
  create_string_duplicate(str2_old, &str2);
  to_upper_case(&str2);
  unsigned long int len1 = get_string_size(&str1);
  unsigned long int len2 = get_string_size(&str2);
  unsigned long int len3 = (len1 > len2) ? len2 : len1;
  for(unsigned long int i = 0; i < len3; ++i) {
    if (str1.str[i] > str2.str[i]) {
      free_string(&str1);
      free_string(&str2);
      return 1;
    }
    if (str2.str[i] > str1.str[i]) {
      free_string(&str1);
      free_string(&str2);
      return -1;
    }
  }
  if (len1 > len3) {
    free_string(&str1);
    free_string(&str2);
    return -1;
  }
  if (len2 > len3) {
    free_string(&str1);
    free_string(&str2);
    return 1;
  }
  free_string(&str1);
  free_string(&str2);
  return 0;
}

int q_by_id(const void* lhs, const void* rhs) {
  return ((student*)lhs)->id - ((student*)rhs)->id;
}

int q_by_name(const void* lhs, const void* rhs) {
  return string_compare("%s", standart_student_comporator, ((student*)lhs)->name, ((student*)rhs)->name);
}

int q_by_surname(const void* lhs, const void* rhs) {
  return string_compare("%s", standart_student_comporator, ((student*)lhs)->surname, ((student*)rhs)->surname);
}

int q_by_group(const void* lhs, const void* rhs) {
  return string_compare("%s", standart_student_comporator, ((student*)lhs)->group, ((student*)rhs)->group);
}

int by_id(student* stud, void* data) {
  return (stud->id == *((unsigned long int*)data)) ? 1 : 0;
}

int by_name(student* stud, void* data) {
  return !string_compare("%s", standart_string_comporator, stud->name, (string*)data);
}

int by_surname(student* stud, void* data) {
  return !string_compare("%s", standart_string_comporator, stud->surname, (string*)data);
}

int by_group(student* stud, void* data) {
  return !string_compare("%s", standart_string_comporator, stud->group, (string*)data);
}

int search_student_by(student* stud_arr, int search_arr_size, int (*cmp)(student* stud, void* data), student** found_stud_arr, int* found_arr_size, void* value) {
  student_array res_stud_arr;
  if (create_student_array(&res_stud_arr) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  for (int i = 0; i < search_arr_size; ++i) {
    if (cmp(stud_arr + i, value)) {
      if (insert_student_in_array(&res_stud_arr, stud_arr + i) == MEMORY_ALLOCATE_EXCEPTION) {
        return MEMORY_ALLOCATE_EXCEPTION;
      }
    }
  }
  *found_stud_arr = get_student_array(&res_stud_arr);
  *found_arr_size = get_student_array_size(&res_stud_arr);
  return SUCCESS_FUNCTION_RETURN;
}

double calculate_averange_score(student* students_array, int student_array_size) {
  double student_averange, total_averange = 0;
  for (int i = 0; i < student_array_size; ++i) {
    student_averange = 0;
    for (int j = 0; j < 5; ++j) {
      student_averange += students_array[i].marks[j];
    }
    student_averange /= 5.0;
    total_averange += student_averange;
  }
  return total_averange / student_array_size;
}

void print_students(student* students_array, int student_array_size) {
  int averange;
  for (int i = 0; i < student_array_size; ++i) {
    printf("%s %s %hd %s ", get_raw_string(students_array[i].surname), get_raw_string(students_array[i].name), students_array[i].course, get_raw_string(students_array[i].group));
    averange = 0;
    for (int j = 0; j < 5; ++j) {
      averange += students_array[i].marks[j];
    }
    printf("%lf\n", averange / 5.0);
  }
}

void clear_student_structure(void* structure) {
  free_student((student*)structure);
}

int print_students_list(list* lst_arr) {
  int execute_status;
  for (int i = 0; i < 4; ++i) {
    list* lst = lst_arr + i;
    for (unsigned long int j = 0; j < lst->size; ++j) {
      student* std = list_element_at(lst, j, &execute_status);
      if (execute_status != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
      printf("%ld %s %s %hd %s ", std->id, get_raw_string(std->name), get_raw_string(std->surname), std->course, get_raw_string(std->group));
      for (int k = 0; k < 5; ++k) {
        printf("%hd ", std->marks[k]);
      }
      printf("\n");
    }
  }
  return SUCCESS_FUNCTION_RETURN;
}

int is_id(int id) {
  return (id > 0) ? 1 : 0;
}

int is_course(int course) {
  return (course > 0 && course < 5) ? 1 : 0;
}

int is_mark(int mark) {
  return (mark > 1 && mark < 6) ? 1 : 0;
}

int validate_int(string* str, int (*f)(int x), int* execute_status) {
  int x = get_digit_from_string(str, execute_status);
  free_string(str);
  free(str);
  if (*execute_status != NUMBER) {
    return 0;
  }
  if (!f(x)) {
    *execute_status = WRONG_INPUT_EXCEPTION;
    return 0;
  }
  *execute_status = SUCCESS_FUNCTION_RETURN;
  return x;
}

int read_student_arr_from_file(FILE* file, student** student_arr, int* size) {
  int state = 0;
  int execute_status;
  student* stud;
  student_array stud_arr;
  if (create_student_array(&stud_arr) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  while (1) {
    string* str = (string*)malloc(sizeof(string));
    if (str == NULL) {
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    str->str = (char*)malloc(sizeof(char*) * STANDART_ARRAY_SIZE);
    if (str->str == NULL) {
      free(str);
      return MEMORY_ALLOCATE_EXCEPTION;
    }
    str->size = STANDART_ARRAY_SIZE;
    if (read_string(file, str, isspace) == EOF) {
      free(str->str);
      free(str);
      break;
    }
    int id, course, mark;
    switch (state) {
      case 0:
        stud = (student*)malloc(sizeof(student));
        if (stud == NULL) {
          free(str->str);
          free(str);
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        stud->marks = (short*)malloc(sizeof(short) * 5);
        if (stud->marks == NULL) {
          free(str->str);
          free(str);
          free(stud);
          return MEMORY_ALLOCATE_EXCEPTION;
        }
        stud->id = validate_int(str, is_id, &execute_status);
        if (execute_status != SUCCESS_FUNCTION_RETURN) {
          free(stud);
          return execute_status;
        }
        state = 1;
        break;
      case 1:
        if (!get_string_size(str)
            || !contains_only(str, standart_string_range)) {
          free(str->str);
          free(str);
          free(stud);
          return WRONG_INPUT_EXCEPTION;
        }
        stud->name = str;
        state = 2;
        break;
      case 2:
        if (!get_string_size(str)
            || !contains_only(str, standart_string_range)) {
          free(str->str);
          free(str);
          free(stud);
          return WRONG_INPUT_EXCEPTION;
        }
        stud->surname = str;
        state = 3;
        break;
      case 3:
        stud->course = validate_int(str, is_course, &execute_status);
        if (execute_status != SUCCESS_FUNCTION_RETURN) {
          free(stud);
          return execute_status;
        }
        state = 4;
        break;
      case 4:
        if (!get_string_size(str)) {
          free(str->str);
          free(str);
          free(stud);
          return WRONG_INPUT_EXCEPTION;
        }
        stud->group = str;
        state = 5;
        break;
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
        stud->marks[state - 5] = validate_int(str, is_mark, &execute_status);
        if (execute_status != SUCCESS_FUNCTION_RETURN) {
          free(stud);
          return execute_status;
        }
        if (state == 9) {
          if (insert_student_in_array(&stud_arr, stud) == MEMORY_ALLOCATE_EXCEPTION) {
            free(stud);
            return MEMORY_ALLOCATE_EXCEPTION;
          }
        }
        state = (state + 1) % 10;
        break;
    }
  }
  *student_arr = get_student_array(&stud_arr);
  *size = get_student_array_size(&stud_arr);
  return SUCCESS_FUNCTION_RETURN;
}
