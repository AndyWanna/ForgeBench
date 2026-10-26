#include <assert.h>
#include <stdio.h>
#include "top_kernel.h"

#include <math.h>


typedef float data_t;


void load_8_128_float(data_t input[8][128], data_t output[8][128])
{
    for (int idx0 = 0; idx0 < 8; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}



void softmax_8_128_float(
    data_t input[8][128],
    data_t output[8][128]
)
{
    data_t max_val[8];
    data_t sum[8];
    for (int i = 0; i < 8; i++) max_val[i] = input[i][0];
    for (int i = 0; i < 8; i++)
        for (int j = 1; j < 128; j++)
            max_val[i] = (input[i][j] > max_val[i]) ? (data_t)input[i][j] : (data_t)max_val[i];
    for (int i = 0; i < 8; i++) sum[i] = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 128; j++)
            output[i][j] = expf(input[i][j] - max_val[i]);
    {
      // Allocate memory in host memory
      float *dev_output = (float *)malloc((8) * (128) * sizeof(float));
      std::vector<float *> dev_sum;
      for (int i = 0; i < 2; i++) {
        float *dev_sum_tmp = (float *)malloc((8) * sizeof(float));
        dev_sum.push_back(dev_sum_tmp);
      }

      // Initialize host buffers
      memcpy(dev_output, output, (8) * (128) * sizeof(float));
      for (int i = 0; i < 2; i++) {
        memcpy(dev_sum[i], sum, (8) * sizeof(float));
      }

      // Allocate buffers in device memory
      std::vector<float *> buffer_output;
      std::vector<sum_t8 *> buffer_sum;
      for (int i = 0; i < 1; i++) {
        float *buffer_output_tmp = (float *)malloc((8) * (128) * sizeof(float));
        buffer_output.push_back(buffer_output_tmp);
      }
      for (int i = 0; i < 2; i++) {
        sum_t8 *buffer_sum_tmp = (sum_t8 *)malloc((8) * sizeof(float));
        buffer_sum.push_back(buffer_sum_tmp);
      }

      for (int i = 0; i < 1; i++) {
        memcpy(buffer_output[i], dev_output, (8) * (128) * sizeof(float));
      }

      for (int i = 0; i < 2; i++) {
        memcpy(buffer_sum[i], dev_sum[i], (8) * sizeof(float));
      }

      {
        // Launch the kernel
        kernel0(buffer_output[0], buffer_sum[0], buffer_sum[0], buffer_sum[1]);
      }
      for (int i = 0; i < 2; i++) {
        memcpy(dev_sum[i], buffer_sum[i], (8) * sizeof(float));
      }

      // Merge results
      for (int idx = 1; idx < 2; idx++) {
        sum_drain_drain_merge(dev_sum[0], dev_sum[idx]);
      }

      // Restore data from host buffers
      memcpy(sum, dev_sum[0], (8) * sizeof(float));

      // Clean up resources
      for (int i = 0; i < 1; i++) {
        free(buffer_output[i]);
      }
      for (int i = 0; i < 2; i++) {
        free(buffer_sum[i]);
      }
      free(dev_output);
      for (int i = 0; i < 2; i++) {
        free(dev_sum[i]);
      }
    }
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 128; j++)
            output[i][j] /= sum[i];
}


void store_8_128_float(data_t input[8][128], data_t output[8][128])
{
    for (int idx0 = 0; idx0 < 8; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void top(data_t DRAM_input[8][128], data_t DRAM_output[8][128])
{
    #pragma HLS interface m_axi port=DRAM_input offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_output offset=slave bundle=mem2
    data_t BRAM_input[8][128];
    data_t BRAM_output[8][128];


    load_8_128_float(DRAM_input, BRAM_input);
    softmax_8_128_float(BRAM_input, BRAM_output);
    store_8_128_float(BRAM_output, DRAM_output);
}