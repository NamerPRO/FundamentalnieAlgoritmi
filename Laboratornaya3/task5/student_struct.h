#ifndef ___STUDENT_STRUCT___
#define ___STUDENT_STRUCT___

typedef struct student {
  unsigned long int id;
  string* name;
  string* surname;
  short course;
  string* group;
  short* marks;
} student;

typedef struct student_array {
  student* array;
  unsigned long int total_size;
  unsigned long int real_size;
} student_array;

#endif
