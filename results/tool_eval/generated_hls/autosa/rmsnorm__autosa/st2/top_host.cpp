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

void load_128_float(data_t input[128], data_t output[128])
{
    for (int idx0 = 0; idx0 < 128; idx0++) {
        output[idx0] = input[idx0];
    }
}


void rms_norm_8_128_float(
    data_t input[8][128],
    data_t gamma[128],
    data_t output[8][128]
)
{
    data_t sum_sq[8];
    data_t rms[8];
    for (int i = 0; i < 8; i++) sum_sq[i] = (data_t)0;
    {
      // Allocate memory in host memory
      float *dev_input = (float *)malloc((8) * (128) * sizeof(float));
      std::vector<float *> dev_sum_sq;
      for (int i = 0; i < 2; i++) {
        float *dev_sum_sq_tmp = (float *)malloc((8) * sizeof(float));
        dev_sum_sq.push_back(dev_sum_sq_tmp);
      }

      // Initialize host buffers
      memcpy(dev_input, input, (8) * (128) * sizeof(float));
      for (int i = 0; i < 2; i++) {
        memcpy(dev_sum_sq[i], sum_sq, (8) * sizeof(float));
      }

      // Allocate buffers in device memory
      std::vector<float *> buffer_input;
      std::vector<sum_sq_t4 *> buffer_sum_sq;
      for (int i = 0; i < 1; i++) {
        float *buffer_input_tmp = (float *)malloc((8) * (128) * sizeof(float));
        buffer_input.push_back(buffer_input_tmp);
      }
      for (int i = 0; i < 2; i++) {
        sum_sq_t4 *buffer_sum_sq_tmp = (sum_sq_t4 *)malloc((8) * sizeof(float));
        buffer_sum_sq.push_back(buffer_sum_sq_tmp);
      }

      for (int i = 0; i < 1; i++) {
        memcpy(buffer_input[i], dev_input, (8) * (128) * sizeof(float));
      }

      for (int i = 0; i < 2; i++) {
        memcpy(buffer_sum_sq[i], dev_sum_sq[i], (8) * sizeof(float));
      }

      {
        // Launch the kernel
        kernel0(buffer_input[0], buffer_sum_sq[0], buffer_sum_sq[0], buffer_sum_sq[1]);
      }
      for (int i = 0; i < 2; i++) {
        memcpy(dev_sum_sq[i], buffer_sum_sq[i], (8) * sizeof(float));
      }

      // Merge results
      for (int idx = 1; idx < 2; idx++) {
        sum_sq_drain_drain_merge(dev_sum_sq[0], dev_sum_sq[idx]);
      }

      // Restore data from host buffers
      memcpy(sum_sq, dev_sum_sq[0], (8) * sizeof(float));

      // Clean up resources
      for (int i = 0; i < 1; i++) {
        free(buffer_input[i]);
      }
      for (int i = 0; i < 2; i++) {
        free(buffer_sum_sq[i]);
      }
      free(dev_input);
      for (int i = 0; i < 2; i++) {
        free(dev_sum_sq[i]);
      }
    }
    for (int i = 0; i < 8; i++) rms[i] = sqrtf(sum_sq[i] / 128 + (data_t)0.01);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 128; j++)
            output[i][j] = gamma[j] * input[i][j] / rms[i];
}



void store_8_128_float(data_t input[8][128], data_t output[8][128])
{
    for (int idx0 = 0; idx0 < 8; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void top(data_t DRAM_input[8][128], data_t DRAM_gamma[128], data_t DRAM_output[8][128])
{
    #pragma HLS interface m_axi port=DRAM_input offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_gamma offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_output offset=slave bundle=mem2
    data_t BRAM_input[8][128];
    data_t BRAM_gamma[128];
    data_t BRAM_output[8][128];


    load_8_128_float(DRAM_input, BRAM_input);
    load_128_float(DRAM_gamma, BRAM_gamma);
    rms_norm_8_128_float(BRAM_input, BRAM_gamma, BRAM_output);
    store_8_128_float(BRAM_output, DRAM_output);
}