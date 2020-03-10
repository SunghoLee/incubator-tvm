#include <stdio.h>
#include <stdlib.h>
#include "klee/klee.h"

#define WIDTH 12
#define HEIGHT 12

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
//void print(int matrix[WIDTH][HEIGHT]){
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

int** new_matrix(){
  int** dst = (int**)calloc(WIDTH, sizeof(int*));
  int i,j; 

  for(i=0; i<WIDTH; i++){
    dst[i] = (int*)calloc(HEIGHT, sizeof(int));
  }

  return dst;
}

void mfree(int** matrix){
  int i;
  for(i=0; i<WIDTH; i++){
    free(matrix[i]);
  }
  free(matrix);
}

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

int** min_opt(int matrix1[WIDTH][HEIGHT], int matrix2[WIDTH][HEIGHT]){
  int** n_matrix = new_matrix();
  int i,j;

  for(i=0; i<WIDTH; i++){
    for(j=0; j<HEIGHT; j++){
      int v1 = matrix1[i][j];
      int v2 = matrix2[i][j];
      int v3 = v1 - v2;

      if(v3 < 0)
        v3 = 0;

      n_matrix[i][j] = v1 - v3;
    }
  }

  return n_matrix;
}

int main(){
//  int matrix1[WIDTH][HEIGHT] = {{1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12},
//  {1,2,3,4,5,6,7,8,9,10,11,12}
//  };
//
//  int matrix2[WIDTH][HEIGHT] = {{12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1},
//  {12,11,10,9,8,7,6,5,4,3,2,1}
//  };

  int matrix1[WIDTH][HEIGHT];
  int matrix2[WIDTH][HEIGHT];
  int i, j;

  klee_make_symbolic(matrix1, sizeof(matrix1), "matrix1");
  klee_make_symbolic(matrix2, sizeof(matrix2), "matrix2");

  //print_s(matrix);

  int** x = min_naive(matrix1, matrix2);
  int** y = min_opt(matrix1, matrix2);

  //print(x);
  //print(y);

  for(i=0; i<WIDTH; i++){
    for(j=0; j<HEIGHT; j++){
      klee_assert(x[i][j] == matrix1[i][j]);
    }
  }


  mfree(x);
  mfree(y);
  return 1;
}
