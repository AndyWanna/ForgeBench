
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef float data_t;

#define I 96
#define J 128
#define K 512

data_t A[I][K]; 
data_t B[K][J]; 
data_t C[I][J];
data_t bias[I][J];
data_t C_golden[I][J]; 

int main(int argc, char **argv) {
   

  for (int i = 0; i < I; i++) 
    for (int k = 0; k < K; k++) {
      A[i][k] = (data_t)rand() / RAND_MAX;
    }

  // for (int j = 0; j < J; j++)
  //   for (int k = 0; k < K; k++) {
  //     B[j][k] = (data_t)rand() / RAND_MAX;      
  //   }
  for (int k = 0; k < K; k++)
    for (int j = 0; j < J; j++) {
      B[k][j] = (data_t)rand() / RAND_MAX;      
    }

  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      bias[i][j] = (data_t)rand() / RAND_MAX;      
    }
#pragma scop
  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      C[i][j] = bias[i][j];     
    }

  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      //C[i][j] = 0;
      for (int k = 0; k < K; k++) {        
        C[i][j] = C[i][j] + A[i][k] * B[k][j];
      }
    }
#pragma endscop

  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      C_golden[i][j] = bias[i][j];
      for (int k = 0; k < K; k++) {
        C_golden[i][j] = C_golden[i][j] + A[i][k] * B[k][j];
      }
    }

  int err = 0;
  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      if (fabs((float)C_golden[i][j] - (float)C[i][j]) > 0.001)
        err++;
    }

  if (err)
    printf("Failed with %d errors!\n", err);
  else
    printf("Passed!\n");

  return 0;
}



