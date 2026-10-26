#include <assert.h>
#include <stdio.h>
#include "top_kernel.h"



typedef float data_t;

data_t BRAM_input[16][14][14];
data_t BRAM_weight[16][16][3][3];
data_t BRAM_output[16][12][12];

void load_16_14_14_float(data_t input[16][14][14], data_t output[16][14][14])
{
    for (int idx0 = 0; idx0 < 16; idx0++) {
        for (int idx1 = 0; idx1 < 14; idx1++) {
            for (int idx2 = 0; idx2 < 14; idx2++) {
                output[idx0][idx1][idx2] = input[idx0][idx1][idx2];
            }
        }
    }
}

void load_16_16_3_3_float(data_t input[16][16][3][3], data_t output[16][16][3][3])
{
    for (int idx0 = 0; idx0 < 16; idx0++) {
        for (int idx1 = 0; idx1 < 16; idx1++) {
            for (int idx2 = 0; idx2 < 3; idx2++) {
                for (int idx3 = 0; idx3 < 3; idx3++) {
                    output[idx0][idx1][idx2][idx3] = input[idx0][idx1][idx2][idx3];
                }
            }
        }
    }
}

/*
 * Auto-generated Convolution HLS Code (with optional bias and grouping)
 *
 * Dimensions:
 *   Input  : [16][14][14]
 *   Kernel : [16][16][3][3]
 *   Output : [16][12][12]
 *
 * Data type: float
 *
 * If bias is included, a 1D bias array of size [16] is expected.
 * For group convolution, we assume:
 *   16 % groups == 0
 *   16 % groups == 0
 */

// Use a typedef for the data type

//////////////////////////////////////////
// Begin: CONV2D FUNCTION (co in the innermost loop)
//////////////////////////////////////////

void conv2d_16_16_14_14_12_12_3_0_1_float(
    data_t input[16][14][14],
    data_t kernel[16][16][3][3],
    data_t output[16][12][12]
)
{
    #pragma HLS array_partition variable=input  type=cyclic factor=1  dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=1 dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=1 dim=2

    #pragma HLS array_partition variable=output type=cyclic factor=1  dim=1
    // Initialize output to ((data_t)0), with co in the innermost loop
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            for (int co = 0; co < 16; co++) {
            #pragma HLS unroll factor=1
                output[co][i][j] = ((data_t)0);
            }
        }
    }

    // Perform convolution (co in the innermost loop)
    {
      // Allocate memory in host memory
      float *dev_input = (float *)malloc((16) * (14) * (14) * sizeof(float));
      float *dev_kernel = (float *)malloc((16) * (16) * (3) * (3) * sizeof(float));
      float *dev_output = (float *)malloc((16) * (12) * (12) * sizeof(float));

      // Initialize host buffers
      memcpy(dev_input, input, (16) * (14) * (14) * sizeof(float));
      memcpy(dev_kernel, kernel, (16) * (16) * (3) * (3) * sizeof(float));
      memcpy(dev_output, output, (16) * (12) * (12) * sizeof(float));

      // Allocate buffers in device memory
      std::vector<input_t2 *> buffer_input;
      std::vector<float *> buffer_kernel;
      std::vector<output_t4 *> buffer_output;
      for (int i = 0; i < 1; i++) {
        input_t2 *buffer_input_tmp = (input_t2 *)malloc((16) * (14) * (14) * sizeof(float));
        buffer_input.push_back(buffer_input_tmp);
      }
      for (int i = 0; i < 1; i++) {
        float *buffer_kernel_tmp = (float *)malloc((16) * (16) * (3) * (3) * sizeof(float));
        buffer_kernel.push_back(buffer_kernel_tmp);
      }
      for (int i = 0; i < 1; i++) {
        output_t4 *buffer_output_tmp = (output_t4 *)malloc((16) * (12) * (12) * sizeof(float));
        buffer_output.push_back(buffer_output_tmp);
      }

      for (int i = 0; i < 1; i++) {
        memcpy(buffer_input[i], dev_input, (16) * (14) * (14) * sizeof(float));
      }

      for (int i = 0; i < 1; i++) {
        memcpy(buffer_kernel[i], dev_kernel, (16) * (16) * (3) * (3) * sizeof(float));
      }

      for (int i = 0; i < 1; i++) {
        memcpy(buffer_output[i], dev_output, (16) * (12) * (12) * sizeof(float));
      }

      {
        // Launch the kernel
        kernel0(buffer_input[0], buffer_kernel[0], buffer_output[0]);
      }
      for (int i = 0; i < 1; i++) {
        memcpy(dev_output, buffer_output[i], (16) * (12) * (12) * sizeof(float));
      }

      // Restore data from host buffers
      memcpy(output, dev_output, (16) * (12) * (12) * sizeof(float));

      // Clean up resources
      for (int i = 0; i < 1; i++) {
        free(buffer_input[i]);
      }
      for (int i = 0; i < 1; i++) {
        free(buffer_kernel[i]);
      }
      for (int i = 0; i < 1; i++) {
        free(buffer_output[i]);
      }
      free(dev_input);
      free(dev_kernel);
      free(dev_output);
    }
}

void store_16_12_12_float(data_t input[16][12][12], data_t output[16][12][12])
{
    for (int idx0 = 0; idx0 < 16; idx0++) {
        for (int idx1 = 0; idx1 < 12; idx1++) {
            for (int idx2 = 0; idx2 < 12; idx2++) {
                output[idx0][idx1][idx2] = input[idx0][idx1][idx2];
            }
        }
    }
}

void top(data_t DRAM_input[16][14][14], data_t DRAM_weight[16][16][3][3], data_t DRAM_output[16][12][12])
{
    #pragma HLS interface m_axi port=DRAM_input offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_weight offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_output offset=slave bundle=mem2

    load_16_14_14_float(DRAM_input, BRAM_input);
    load_16_16_3_3_float(DRAM_weight, BRAM_weight);
    conv2d_16_16_14_14_12_12_3_0_1_float(BRAM_input, BRAM_weight, BRAM_output);
    store_16_12_12_float(BRAM_output, DRAM_output);
}