/** 
 * This code is written to be used as an input of KLEE. If you want to compile
 * this code as an executable file, please define TEST (i.e. #define TEST) at
 * the top of this code.
 **/
#include <stdio.h>
#include <stdlib.h>

#ifdef TEST
#include <time.h>
#else
#include "klee/klee.h"

// range of elements' values
#define INPUT_UBOUND 255
#define INPUT_LBOUND 0
#endif

// matrix size (i.e. width * height)
#define WIDTH 5
#define HEIGHT 5

// print a static matrix to stdout (for executable)
void print_s(int matrix[WIDTH][HEIGHT]){
  printf("[");
  for(int i=0; i<WIDTH; i++){
    if(i!=0)
      printf(" ");

    for(int j=0; j<HEIGHT; j++){
      if(j!=0)
        printf(", ");
      printf("%d",matrix[i][j]);
    }
    if(i!=WIDTH-1)
      printf("\n");
  }
  printf("]\n");
}

// print a dynamic matrix to stdout (for executable)
void print(int** matrix){
  printf("[");
  for(int i=0; i<WIDTH; i++){
    if(i!=0)
      printf(" ");

    for(int j=0; j<HEIGHT; j++){
      if(j!=0)
        printf(", ");
      printf("%d",matrix[i][j]);
    }
    if(i!=WIDTH-1)
      printf("\n");
  }
  printf("]\n");
}

// create a new dynamic matrix
int** new_matrix(){
  int** dst = (int**)calloc(WIDTH, sizeof(int*));
  int i,j; 

  for(i=0; i<WIDTH; i++){
    dst[i] = (int*)calloc(HEIGHT, sizeof(int));
  }

  return dst;
}

// free a dynamic matrix
void mfree(int** matrix){
  int i;
  for(i=0; i<WIDTH; i++){
    free(matrix[i]);
  }
  free(matrix);
}

// create a new dynamic matrix as the same size of the input matrixes. each
// element of it is calculated as a minimun of two elements of the inputs at the
// same index.
int** min_naive(int matrix1[WIDTH][HEIGHT], int matrix2[WIDTH][HEIGHT]){
  int** n_matrix = new_matrix();
  int i,j;

  for(i=0; i<WIDTH; i++){
    for(j=0; j<HEIGHT; j++){
      int v1 = matrix1[i][j];
      int v2 = matrix2[i][j];
      if(v1 < v2)
        n_matrix[i][j] = v1;
      else
        n_matrix[i][j] = v2;
    }
  }
  return n_matrix;
}

// is the same to the 'min_naive', but has different logic for the minimum
// calculation.
int** min_opt(int matrix1[WIDTH][HEIGHT], int matrix2[WIDTH][HEIGHT]){
  int** n_matrix = new_matrix();
  int i,j;

  for(i=0; i<WIDTH; i++){
    for(j=0; j<HEIGHT; j++){
      int v1 = matrix1[i][j];
      int v2 = matrix2[i][j];

      n_matrix[i][j] = v1 - ((v1 - v2) * (v1 - v2 > 0));
    }
  }

  return n_matrix;
}

// main function
int main(){
  int i, j;
  int matrix1[WIDTH][HEIGHT];
  int matrix2[WIDTH][HEIGHT];

#ifdef TEST
  srand(time(0));

  for(i=0; i<WIDTH; i++){
    for(j=0; j<HEIGHT; j++){
      matrix1[i][j] = rand() % INPUT_UBOUND;
      matrix2[i][j] = rand() % INPUT_UBOUND;
    }
  }

  printf("#initial:\n");
  printf("#matrix1:\n");
  print_s(matrix1);
  printf("#matrix2:\n");
  print_s(matrix2);
#else
  klee_make_symbolic(matrix1, sizeof(matrix1), "matrix1");
  klee_make_symbolic(matrix2, sizeof(matrix2), "matrix2");

  for(i = 0; i < WIDTH; i++){
    for(j = 0; j < HEIGHT; j++){
      klee_assume(INPUT_LBOUND <= matrix1[i][j]);
      klee_assume(matrix1[i][j] <= INPUT_UBOUND);
      klee_assume(INPUT_LBOUND <= matrix2[i][j]);
      klee_assume(matrix2[i][j] <= INPUT_UBOUND);
    }
  }
#endif

  int** x = min_naive(matrix1, matrix2);
  int** y = min_opt(matrix1, matrix2);

#ifdef TEST
  printf("\n#final:\n");
  printf("#matrix1:\n");
  print(x);
  printf("#matrix2:\n");
  print(y);
#else
  for(i=0; i<WIDTH; i++){
    for(j=0; j<HEIGHT; j++){
      klee_assert(x[i][j] == y[i][j]);
    }
  }
#endif
  mfree(x);
  mfree(y);
  return 1;
}
