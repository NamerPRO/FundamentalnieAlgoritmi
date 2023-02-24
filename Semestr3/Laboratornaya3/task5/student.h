#ifndef ___STUDENT___
#define ___STUDENT___

#include <ctype.h>

#include "cconstants.h"
#include "cstring.h"
#include "cio.h"
#include "cinteger.h"
#include "coverflow.h"
#include "clist.h"
#include "student_struct.h"
#include "clist_struct.h"

int create_student_array(student_array* array);
int insert_student_in_array(student_array* array, student* element);
unsigned long int get_student_array_size(student_array* array);
student* get_student_array(student_array* array);
int read_student_arr_from_file(FILE* file, student** student_arr, int* size);
int is_id(int id);
int is_course(int course);
int is_mark(int mark);
int validate_int(string* str, int (*f)(int x), int* execute_status);
int by_name(student* stud, void* data);
int by_id(student* stud, void* data);
int by_surname(student* stud, void* data);
int by_group(student* stud, void* data);
int search_student_by(student* stud_arr, int search_arr_size, int (*cmp)(student* stud, void* data), student** found_stud_arr, int* found_arr_size, void* value);
int q_by_id(const void* lhs, const void* rhs);
int q_by_name(const void* lhs, const void* rhs);
int q_by_surname(const void* lhs, const void* rhs);
int q_by_group(const void* lhs, const void* rhs);
int standart_student_comporator(string* str1, string* str2);
void free_students_array_ptr(student* students_array, unsigned long int student_array_size);
void free_students_array(student_array* students_array);
list* group_students_by_course(student* students_array, unsigned long int student_array_size, list* lst);
void print_students(student* students_array, int student_array_size);
int print_students_list(list* lst_arr);
void clear_student_structure(void* structure);
void free_student(student* stud);
double calculate_averange_score(student* students_array, int student_array_sizes);

#endif
