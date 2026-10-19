#include <assert.h>
#include <stdio.h>
#include "top_kernel.h"



typedef float data_t;

data_t BRAM_A[64][64];
data_t BRAM_B[64][64];
data_t BRAM_bias[64][64];
data_t BRAM_C[64][64];

void load_64_64_float(data_t input[64][64], data_t output[64][64])
{
    for (int idx0 = 0; idx0 < 64; idx0++) {
        for (int idx1 = 0; idx1 < 64; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

//////////////////////////////////////////
// Begin: GEMM_IJK FUNCTION
//////////////////////////////////////////
/*==== GEMM_IJK FUNCTION START ====*/
void gemm_ijk(
    float input_A[64][64],
    float input_B[64][64],
    float output[64][64]
)

{

for (int i = 0; i < 64; i++) {
#pragma HLS unroll factor=1
for (int k = 0; k < 64; k++) {
#pragma HLS unroll factor=1
    output[i][k] = 0;
}
}


for (int i = 0; i < 64; i++) {
#pragma HLS unroll factor=1
for (int j = 0; j < 64; j++) {
#pragma HLS unroll factor=1
for (int k = 0; k < 64; k++) {
#pragma HLS unroll factor=1
    output[i][k] += input_A[i][j] * input_B[j][k];
}
}
}
}
/*==== GEMM_IJK FUNCTION END ====*/
//////////////////////////////////////////
// END: GEMM_IJK FUNCTION
//////////////////////////////////////////


void store_64_64_float(data_t input[64][64], data_t output[64][64])
{
    for (int idx0 = 0; idx0 < 64; idx0++) {
        for (int idx1 = 0; idx1 < 64; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void top(data_t DRAM_A[64][64], data_t DRAM_B[64][64], data_t DRAM_C[64][64])
{
    #pragma HLS interface m_axi port=DRAM_A offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_B offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_C offset=slave bundle=mem2

    load_64_64_float(DRAM_A, BRAM_A);
    load_64_64_float(DRAM_B, BRAM_B);
    //////////////////////////////////////////
// Begin: Inline implementation of GEMM_IJK
//////////////////////////////////////////

for (int i = 0; i < 64; i++) {
#pragma HLS unroll factor=1
for (int k = 0; k < 64; k++) {
#pragma HLS unroll factor=1
    BRAM_C[i][k] = 0;
}
}


{
  // Allocate memory in host memory
  float *dev_BRAM_A = (float *)malloc((64) * (64) * sizeof(float));
  float *dev_BRAM_B = (float *)malloc((64) * (64) * sizeof(float));
  float *dev_BRAM_C = (float *)malloc((64) * (64) * sizeof(float));

  // Initialize host buffers
  memcpy(dev_BRAM_A, BRAM_A, (64) * (64) * sizeof(float));
  memcpy(dev_BRAM_B, BRAM_B, (64) * (64) * sizeof(float));
  memcpy(dev_BRAM_C, BRAM_C, (64) * (64) * sizeof(float));

  // Allocate buffers in device memory
  std::vector<BRAM_A_t16 *> buffer_BRAM_A;
  std::vector<BRAM_B_t4 *> buffer_BRAM_B;
  std::vector<BRAM_C_t4 *> buffer_BRAM_C;
  for (int i = 0; i < 1; i++) {
    BRAM_A_t16 *buffer_BRAM_A_tmp = (BRAM_A_t16 *)malloc((64) * (64) * sizeof(float));
    buffer_BRAM_A.push_back(buffer_BRAM_A_tmp);
  }
  for (int i = 0; i < 1; i++) {
    BRAM_B_t4 *buffer_BRAM_B_tmp = (BRAM_B_t4 *)malloc((64) * (64) * sizeof(float));
    buffer_BRAM_B.push_back(buffer_BRAM_B_tmp);
  }
  for (int i = 0; i < 1; i++) {
    BRAM_C_t4 *buffer_BRAM_C_tmp = (BRAM_C_t4 *)malloc((64) * (64) * sizeof(float));
    buffer_BRAM_C.push_back(buffer_BRAM_C_tmp);
  }

  for (int i = 0; i < 1; i++) {
    memcpy(buffer_BRAM_A[i], dev_BRAM_A, (64) * (64) * sizeof(float));
  }

  for (int i = 0; i < 1; i++) {
    memcpy(buffer_BRAM_B[i], dev_BRAM_B, (64) * (64) * sizeof(float));
  }

  for (int i = 0; i < 1; i++) {
    memcpy(buffer_BRAM_C[i], dev_BRAM_C, (64) * (64) * sizeof(float));
  }

  {
    // Launch the kernel
    kernel0(buffer_BRAM_A[0], buffer_BRAM_B[0], buffer_BRAM_C[0]);
  }
  for (int i = 0; i < 1; i++) {
    memcpy(dev_BRAM_C, buffer_BRAM_C[i], (64) * (64) * sizeof(float));
  }

  // Restore data from host buffers
  memcpy(BRAM_C, dev_BRAM_C, (64) * (64) * sizeof(float));

  // Clean up resources
  for (int i = 0; i < 1; i++) {
    free(buffer_BRAM_A[i]);
  }
  for (int i = 0; i < 1; i++) {
    free(buffer_BRAM_B[i]);
  }
  for (int i = 0; i < 1; i++) {
    free(buffer_BRAM_C[i]);
  }
  free(dev_BRAM_A);
  free(dev_BRAM_B);
  free(dev_BRAM_C);
}
//////////////////////////////////////////
// End: Inline implementation of GEMM_IJK
//////////////////////////////////////////

    store_64_64_float(BRAM_C, DRAM_C);
}