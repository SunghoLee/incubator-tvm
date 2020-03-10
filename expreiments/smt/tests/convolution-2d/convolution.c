#include <stdio.h>
#include <stdlib.h>
#include "klee/klee.h"

#define WIDTH 12
#define HEIGHT 12

// A simple 3X3 kernel for 2D convolution
int kernel[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};

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

int** new_and_copy(int src[WIDTH][HEIGHT]){
  int** dst = (int**)malloc(sizeof(int*) * WIDTH);
  int i,j; 

  for(i=0; i<WIDTH; i++){
    dst[i] = (int*)malloc(sizeof(int) * HEIGHT);
  }

  for(i=0; i<WIDTH; i++){
    for(j=0; j<HEIGHT; j++){
      dst[i][j] = src[i][j];
    }
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

int** convolution_2D_1(int matrix[WIDTH][HEIGHT]){
  int** n_matrix = new_and_copy(matrix);
  int i,j;
  int ki,kj;

  for(i=1; i<WIDTH-1; i++){
    for(j=1; j<HEIGHT-1; j++){
      int v = 0;
      for(ki=0; ki<3; ki++){
        for(kj=0; kj<3; kj++){
          v += n_matrix[i+ki-1][j+kj-1] * kernel[ki][kj];
        }
      }
      n_matrix[i][j] = v;
    }
  }
  return n_matrix;
}

int** convolution_2D_2(int matrix[WIDTH][HEIGHT]){
  int** n_matrix = new_and_copy(matrix);
  int i,j;
  int ki,kj;

  for(i=1; i<WIDTH-1; i++){
    for(j=1; j<HEIGHT-1; j++){
      int v = 0;
      for(ki=0; ki<3; ki++){
        for(kj=0; kj<3; kj++){
          v += n_matrix[i+ki-1][j+kj-1] * kernel[ki][kj];
        }
      }
      n_matrix[i][j] = v;
    }
  }
  return n_matrix;
}

int main(){
//  int matrix[WIDTH][HEIGHT] = {{1,2,3,4,5,6,7,8,9,10,11,12},
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

  int matrix[WIDTH][HEIGHT];
  int i, j;

  klee_make_symbolic(matrix, sizeof(matrix), "matrix");

  //print_s(matrix);

  int** x = convolution_2D_1(matrix);
  int** y = convolution_2D_2(matrix);

  //print(x);
  //print(y);

  for(i=0; i<WIDTH; i++){
    for(j=0; j<HEIGHT; j++){
      klee_assert(x[i][j] == y[i][j]);
    }
  }

  //print(x);

  mfree(x);
  mfree(y);
  return 1;
}
