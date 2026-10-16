#include <assert.h>
#include <stdio.h>
#include "top_conv_test_kernel.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

typedef float data_t;
//#define O 512
#define O 64
#define I 64
//#define R 60
#define R 14
#define C 14
#define K 3

// data_t cin[R + K - 1][C + K - 1][I];
// data_t w[O][K][K][I];
// data_t cout[R][C][O];
// data_t bias[O];
// data_t cout_golden[R][C][O];

data_t cin[I][R + K - 1][C + K - 1];
data_t w[O][I][K][K];
data_t cout[O][R][C];
data_t bias[O];
data_t cout_golden[O][R][C];

int main(int argc, char **argv){
 
// data initialization
for (int i = 0 ; i < I; i++)
    for (int r = 0; r < R + K - 1; r++)
        for (int c = 0; c < C + K - 1; c++) {
        cin[i][r][c] = 1;
        }

for (int o = 0; o < O; o++)
    for (int i = 0; i < I; i++) 
        for (int p = 0; p < K; p++)
        for (int q = 0; q < K; q++) {
            w[o][i][p][q] = 1;
        }

for (int o = 0; o < O; o++){
    bias[o] = 1;
}
{
  // Allocate memory in host memory
  float *dev_bias_unserialized = (float *)malloc((64) * sizeof(float));
  float *dev_bias = (float *)malloc(1024 * sizeof(float));
  float *dev_cin_unserialized = (float *)malloc((64) * (16) * (16) * sizeof(float));
  float *dev_cin = (float *)malloc(630784 * sizeof(float));
  float *dev_cout_unserialized = (float *)malloc((64) * (14) * (14) * sizeof(float));
  float *dev_cout = (float *)malloc(12544 * sizeof(float));
  float *dev_w_unserialized = (float *)malloc((64) * (64) * (3) * (3) * sizeof(float));
  float *dev_w = (float *)malloc(589824 * sizeof(float));

  // Initialize host buffers
  memcpy(dev_bias_unserialized, bias, (64) * sizeof(float));
  memcpy(dev_cin_unserialized, cin, (64) * (16) * (16) * sizeof(float));
  memcpy(dev_w_unserialized, w, (64) * (64) * (3) * (3) * sizeof(float));
  host_serialize_bias(dev_bias, dev_bias_unserialized);
  host_serialize_cin(dev_cin, dev_cin_unserialized);
  host_serialize_w(dev_w, dev_w_unserialized);

  // Allocate buffers in device memory
  std::vector<bias_t16 *> buffer_bias;
  std::vector<cin_t16 *> buffer_cin;
  std::vector<cout_t16 *> buffer_cout;
  std::vector<w_t16 *> buffer_w;
  for (int i = 0; i < 1; i++) {
    bias_t16 *buffer_bias_tmp = (bias_t16 *)malloc((1024) * sizeof(float));
    buffer_bias.push_back(buffer_bias_tmp);
  }
  for (int i = 0; i < 1; i++) {
    cin_t16 *buffer_cin_tmp = (cin_t16 *)malloc((630784) * sizeof(float));
    buffer_cin.push_back(buffer_cin_tmp);
  }
  for (int i = 0; i < 1; i++) {
    cout_t16 *buffer_cout_tmp = (cout_t16 *)malloc((12544) * sizeof(float));
    buffer_cout.push_back(buffer_cout_tmp);
  }
  for (int i = 0; i < 1; i++) {
    w_t16 *buffer_w_tmp = (w_t16 *)malloc((589824) * sizeof(float));
    buffer_w.push_back(buffer_w_tmp);
  }

  for (int i = 0; i < 1; i++) {
    memcpy(buffer_bias[i], dev_bias, (1024) * sizeof(float));
  }

  for (int i = 0; i < 1; i++) {
    memcpy(buffer_cin[i], dev_cin, (630784) * sizeof(float));
  }

  for (int i = 0; i < 1; i++) {
    memcpy(buffer_w[i], dev_w, (589824) * sizeof(float));
  }

  {
    // Launch the kernel
    kernel0(buffer_bias[0], buffer_cin[0], buffer_cout[0], buffer_w[0]);
  }
  for (int i = 0; i < 1; i++) {
    memcpy(dev_cout, buffer_cout[i], (12544) * sizeof(float));
  }

  host_deserialize_cout(dev_cout_unserialized, dev_cout);
  // Restore data from host buffers
  memcpy(cout, dev_cout_unserialized, (64) * (14) * (14) * sizeof(float));

  // Clean up resources
  for (int i = 0; i < 1; i++) {
    free(buffer_bias[i]);
  }
  for (int i = 0; i < 1; i++) {
    free(buffer_cin[i]);
  }
  for (int i = 0; i < 1; i++) {
    free(buffer_cout[i]);
  }
  for (int i = 0; i < 1; i++) {
    free(buffer_w[i]);
  }
  free(dev_bias);
  free(dev_bias_unserialized);
  free(dev_cin);
  free(dev_cin_unserialized);
  free(dev_cout);
  free(dev_cout_unserialized);
  free(dev_w);
  free(dev_w_unserialized);
}
 

int err = 0;
float thres = 0.001;
for (int o = 0; o < O; o++)
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++) {
        if (fabs((float)cout_golden[r][c][o] - (float)cout[r][c][o]) > thres) {
            err++;
        }
    }

}