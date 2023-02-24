#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "cconstants.h"
#include "cinteger.h"
#include "cio.h"
#include "coverflow.h"
#include "cstring.h"
#include "student.h"
#include "clist.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return ARGUMENTS_EXCEPTION;
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    return FILE_INTERRACT_EXCEPTION;
  }
  int execute_status;
  string str_student;
  student* students_array = NULL;
  int student_array_size;
  if ((execute_status = read_student_arr_from_file(file, &students_array, &student_array_size)) != SUCCESS_FUNCTION_RETURN) {
    return execute_status;
  }
  fclose(file);
  student* found_stud_arr = NULL;
  int found_arr_size = 0;

  //qsort(students_array, student_array_size, sizeof(student), q_by_group);

  //string s; int e;
  //create_string(&s, str("jaja", &e)); //искать по строке
  // unsigned long int s = 1; //по числу
  //search_student_by(students_array, student_array_size, by_group, &found_stud_arr, &found_arr_size, &s);

  //===
  list* lst_arr = (list*)malloc(sizeof(list) * 4);
  if (lst_arr == NULL) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  create_list(lst_arr); create_list(lst_arr + 1);
  create_list(lst_arr + 2); create_list(lst_arr + 3);
  group_students_by_course(students_array, student_array_size, lst_arr);
  print_students_list(lst_arr);
  //===

  string path;
  if (create_string(&path, argv[1]) == MEMORY_ALLOCATE_EXCEPTION) {
    return MEMORY_ALLOCATE_EXCEPTION;
  }
  string_concat("%p", &path, "_x");
  int path_length = get_string_size(&path) - 1;
  double total_averange_score = calculate_averange_score(students_array, student_array_size);
  double current_averange_score;
  for (int i = 0; i < 4; ++i) {
    string_gnaw(&path, path_length, path_length);
    string_concat("%d", &path, i + 1);
    file = fopen(get_raw_string(&path), "a");
    if (file == NULL) {
      return FILE_INTERRACT_EXCEPTION;
    }
    list* lst = lst_arr + i;
    for (unsigned long int j = 0; j < lst->size; ++j) {
      student* std = list_element_at(lst, j, &execute_status);
      if (execute_status != SUCCESS_FUNCTION_RETURN) {
        return execute_status;
      }
      current_averange_score = 0;
      for (int k = 0; k < 5; ++k) {
        current_averange_score += std->marks[k];
      }
      if (current_averange_score / 5.0 > total_averange_score) {
        print_string(file, "%s %s %d %s %f\n", std->name, std->surname, std->course, std->group, current_averange_score / 5.0);
      }
    }
    fclose(file);
  }

  // print_students(students_array, student_array_size);
  //free_students_array_ptr(students_array, student_array_size);
  free_string(&path);
  for (int i = 0; i < 4; ++i) {
    list_clear(lst_arr + i, clear_student_structure);
  }
  free(lst_arr);
  free(students_array);
  return 0;
}
