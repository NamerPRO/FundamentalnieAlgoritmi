#include <stdio.h>
#include <stdlib.h>

#define MEMORY_EXCEPTION 1
#define EMPTY_MATRIX 2
#define WRONG_MATRIX_EXCEPTION 3

int get_rows(double** matrix) {
  return matrix[0][0];
}

int get_cols(double** matrix) {
  return matrix[0][1];
}

double get(double** matrix, int i, int j) {
  return matrix[i + 1][j];
}

void set(double** matrix, int i, int j, double value) {
  matrix[i + 1][j] = value;
}

void print_matrix(double** matrix) {
  for (int i = 0; i < get_rows(matrix); ++i) {
    for (int j = 0; j < get_cols(matrix); ++j) {
      printf("%lf ", get(matrix, i, j));
    }
    printf("\n");
  }
}

double** generate_matrix(int* status) {
  int rows, cols;
  scanf("%d%d", &rows, &cols);
  ++rows;

  double** matrix = NULL;
  if ((matrix = (double**)malloc(sizeof(double*) * rows)) == NULL) {
    *status = MEMORY_EXCEPTION;
    return NULL;
  }
  if ((matrix[0] = (double*)malloc(sizeof(double) * 2)) == NULL) {
    *status = MEMORY_EXCEPTION;
    //free
    return NULL;
  }
  matrix[0][0] = rows - 1;
  matrix[0][1] = cols;
  for (int i = 1; i < rows; ++i) {
    if ((matrix[i] = (double*)malloc(sizeof(double) * cols)) == NULL) {
      *status = MEMORY_EXCEPTION;
      //free
      return NULL;
    }
    for (int j = 0; j < cols; ++j) {
      matrix[i][j] = rand() % 201 - 100;
    }
  }
  *status = 0;
  return matrix;
}

void free_matrix(double** matrix) {
  int rows = get_rows(matrix) + 1;
  for (int i = 0; i < rows; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

double** mult_matrix(double** matrix1, double** matrix2, int* status) {
  if (get_cols(matrix1) != get_rows(matrix2)) {
    *status = EMPTY_MATRIX;
    return NULL;
  }
  int rows = get_rows(matrix1) + 1;
  int cols = get_cols(matrix2);
  double** matrix = NULL;
  if ((matrix = (double**)malloc(sizeof(double) * rows)) == NULL) {
    *status = MEMORY_EXCEPTION;
    return NULL;
  }
  if ((matrix[0] = (double*)malloc(sizeof(double) * 2)) == NULL) {
    *status = MEMORY_EXCEPTION;
    //free
    return NULL;
  }
  matrix[0][0] = rows - 1;
  matrix[0][1] = cols;
  for (int i = 1; i < rows; ++i) {
    if ((matrix[i] = (double*)malloc(sizeof(double) * cols)) == NULL) {
      *status = MEMORY_EXCEPTION;
      //free
      return NULL;
    }
    for (int j = 0; j < cols; ++j) {
      matrix[i][j] = 0;
      for (int k = 0; k < get_cols(matrix1); ++k) {
        matrix[i][j] += matrix1[i][k] * matrix2[k + 1][j];
      }
    }
  }
  *status = 0;
  return matrix;
}

double get_determinant(double** matrix, int* status) {
  if (matrix == NULL) {
    *status = WRONG_MATRIX_EXCEPTION;
    return 0;
  }
  int n = get_rows(matrix);
  if (n != get_cols(matrix)) {
    *status = WRONG_MATRIX_EXCEPTION;
    return 0;
  }
  int step = 0;
  for (int i = 0; i < n - 1; ++i) {
    double c1 = get(matrix, i, step);
    for (int j = i + 1; j < n; ++j) {
      double c2 = get(matrix, j, step);
      for (int k = step; k < n; ++k) {
        matrix[j + 1][k] = -get(matrix, i, k) / c1 * c2 + get(matrix, j, k);
      }
    }
    ++step;
  }
  int determinant = 1;
  int pos = 0;
  for (int i = 0; i < n; ++i) {
    determinant *= get(matrix, i, pos++);
  }
  *status = 0;
  return determinant;
}

int main() {

  int status;

  double** matrix1 = NULL;
  double** matrix2 = NULL;

  matrix1 = generate_matrix(&status);
  if (!status) {
    printf("First matrix is generated like so:\n");
    print_matrix(matrix1);
  } else {
    printf("Could not generate matrix :(\n");
  }

  matrix2 = generate_matrix(&status);
  if (!status) {
    printf("Second matrix is generated like so:\n");
    print_matrix(matrix2);
  } else {
    printf("Could not generate matrix :(\n");
  }

  double** matrix3 = NULL;
  matrix3 = mult_matrix(matrix1, matrix2, &status);

  if (!status) {
    printf("Result of multiplication of matrix is:\n");
    print_matrix(matrix3);
  } else if (status == EMPTY_MATRIX) {
    printf("The number of columns in the first matrix should be equal to the number of rows in the second.\n");
  } else {
    printf("An internal memory allocation error occured while multiplying two matrixes!\n");
  }

  double det1 = get_determinant(matrix1, &status);
  if (!status) {
    printf("Determinant of a first matrix is %lf\n", det1);
  } else {
    printf("Determinant of a given matrix cannot be computed!\n");
  }
  double det2 = get_determinant(matrix2, &status);
  if (!status) {
    printf("Determinant of a second matrix is %lf\n", det2);
  } else {
    printf("Determinant of a given matrix cannot be computed!\n");
  }
  double det3 = get_determinant(matrix3, &status);
  if (!status) {
    printf("Determinant of a third matrix is %lf\n", det3);
  } else {
    printf("Determinant of a given matrix cannot be computed!\n");
  }

  free_matrix(matrix1);
  free_matrix(matrix2);
  free_matrix(matrix3);

  return 0;
}
