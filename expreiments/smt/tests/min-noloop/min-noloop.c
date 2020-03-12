#include <stdio.h>

#ifdef TEST
#include <time.h>
#include <stdlib.h>
#else
#include "klee/klee.h"
#endif
 
int min_naive(int x, int y){
  return (x > y)? y : x;
}

int min_opt(int x, int y){
//  int z = x - y;
//
//  if(z <= 0)
//    z = 0;
//
//  return x - z;
  return x - ((x - y) * (x - y > 0));
}

int main(){
  int x, y;
#ifdef TEST
  srand(time(0));
  x = rand();
  y = rand();
  //counter example breaking the eqaulity because of integer overflow
  //x = -1612185601;
  //y = 1540882431;
#else
  klee_make_symbolic(&x, sizeof(x), "x");
  klee_assume(x <= 255 && x >= 0);
  klee_make_symbolic(&y, sizeof(y), "y");
  klee_assume(y <= 255 && y >= 0);
#endif

  int min_n = min_naive(x, y);
  int min_o = min_opt(x, y);

#ifdef TEST
  printf("X: %d, Y:%d, Min-NAIVE: %d, MIN-OPT: %d => %s\n", x, y, min_n, min_o, (min_n == min_o)? "true" : "false");
#else
  klee_assert(min_n == min_o);
#endif 
  return 1;
}
