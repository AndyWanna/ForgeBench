#include <assert.h>
#include <stdio.h>
#include "top_gemm_new_ijk_bias_kernel.h"


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
  {
    // Allocate memory in host memory
    float *dev_A_unserialized = (float *)malloc((96) * (512) * sizeof(float));
    float *dev_A = (float *)malloc(1572864 * sizeof(float));
    float *dev_B_unserialized = (float *)malloc((512) * (128) * sizeof(float));
    float *dev_B = (float *)malloc(1572864 * sizeof(float));
    float *dev_C_unserialized = (float *)malloc((96) * (128) * sizeof(float));
    float *dev_C = (float *)malloc(12288 * sizeof(float));
    float *dev_bias_unserialized = (float *)malloc((96) * (128) * sizeof(float));
    float *dev_bias = (float *)malloc(12288 * sizeof(float));

    // Initialize host buffers
    memcpy(dev_A_unserialized, A, (96) * (512) * sizeof(float));
    memcpy(dev_B_unserialized, B, (512) * (128) * sizeof(float));
    memcpy(dev_bias_unserialized, bias, (96) * (128) * sizeof(float));
    host_serialize_A(dev_A, dev_A_unserialized);
    host_serialize_B(dev_B, dev_B_unserialized);
    host_serialize_bias(dev_bias, dev_bias_unserialized);

    // Allocate buffers in device memory
    std::vector<A_t16 *> buffer_A;
    std::vector<B_t16 *> buffer_B;
    std::vector<C_t16 *> buffer_C;
    std::vector<bias_t16 *> buffer_bias;
    for (int i = 0; i < 1; i++) {
      A_t16 *buffer_A_tmp = (A_t16 *)malloc((1572864) * sizeof(float));
      buffer_A.push_back(buffer_A_tmp);
    }
    for (int i = 0; i < 1; i++) {
      B_t16 *buffer_B_tmp = (B_t16 *)malloc((1572864) * sizeof(float));
      buffer_B.push_back(buffer_B_tmp);
    }
    for (int i = 0; i < 1; i++) {
      C_t16 *buffer_C_tmp = (C_t16 *)malloc((12288) * sizeof(float));
      buffer_C.push_back(buffer_C_tmp);
    }
    for (int i = 0; i < 1; i++) {
      bias_t16 *buffer_bias_tmp = (bias_t16 *)malloc((12288) * sizeof(float));
      buffer_bias.push_back(buffer_bias_tmp);
    }

    for (int i = 0; i < 1; i++) {
      memcpy(buffer_A[i], dev_A, (1572864) * sizeof(float));
    }

    for (int i = 0; i < 1; i++) {
      memcpy(buffer_B[i], dev_B, (1572864) * sizeof(float));
    }

    for (int i = 0; i < 1; i++) {
      memcpy(buffer_bias[i], dev_bias, (12288) * sizeof(float));
    }

    {
      // Launch the kernel
      kernel0(buffer_A[0], buffer_B[0], buffer_C[0], buffer_bias[0]);
    }
    for (int i = 0; i < 1; i++) {
      memcpy(dev_C, buffer_C[i], (12288) * sizeof(float));
    }

    host_deserialize_C(dev_C_unserialized, dev_C);
    // Restore data from host buffers
    memcpy(C, dev_C_unserialized, (96) * (128) * sizeof(float));

    // Clean up resources
    for (int i = 0; i < 1; i++) {
      free(buffer_A[i]);
    }
    for (int i = 0; i < 1; i++) {
      free(buffer_B[i]);
    }
    for (int i = 0; i < 1; i++) {
      free(buffer_C[i]);
    }
    for (int i = 0; i < 1; i++) {
      free(buffer_bias[i]);
    }
    free(dev_A);
    free(dev_A_unserialized);
    free(dev_B);
    free(dev_B_unserialized);
    free(dev_C);
    free(dev_C_unserialized);
    free(dev_bias);
    free(dev_bias_unserialized);
  }

  for (int i = 0; i < I; i++)
    for (int j = 0; j < J; j++) {
      C_golden[i][j] = bias[i][j];
      for (int k = 0; k < K; k++) {
        C_golden[i][j] = C_golden[i][j] + A[i][k] * B[k][j];
      }
    }

//   int err = 0;
//   for (int i = 0; i < I; i++)
//     for (int j = 0; j < J; j++) {
//       if (fabs((float)C_golden[i][j] - (float)C[i][j]) > 0.001)
//         err++;
//     }

//   if (err)
//     printf("Failed with %d errors!\n", err);
//   else
//     printf("Passed!\n");

  return 0;
}



