
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ap_fixed.h>
#include <hls_math.h>
#include <stdlib.h>
#include <cstdint>
#include <hls_math.h>
using namespace std;

typedef ap_fixed<16, 5, AP_TRN, AP_WRAP> data_t;

data_t BRAM_1[96][512];
data_t BRAM_2[512][128];
data_t BRAM_3[96][128];
data_t BRAM_4[96][128];

void load_96_512_ap_fixed_16_5_AP_TRN_AP_WRAP_(data_t input[96][512], data_t output[96][512])
{
    for (int idx0 = 0; idx0 < 96; idx0++) {
        for (int idx1 = 0; idx1 < 512; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void load_512_128_ap_fixed_16_5_AP_TRN_AP_WRAP_(data_t input[512][128], data_t output[512][128])
{
    for (int idx0 = 0; idx0 < 512; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void load_96_128_ap_fixed_16_5_AP_TRN_AP_WRAP_(data_t input[96][128], data_t output[96][128])
{
    for (int idx0 = 0; idx0 < 96; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

//////////////////////////////////////////
// Begin: GEMM_IJK FUNCTION
//////////////////////////////////////////
/*==== GEMM_IJK FUNCTION START ====*/
void gemm_ijk(
    ap_fixed<16, 5, AP_TRN, AP_WRAP> input_A[96][512],
    ap_fixed<16, 5, AP_TRN, AP_WRAP> input_B[512][128],
    ap_fixed<16, 5, AP_TRN, AP_WRAP> output[96][128]
)

{
#pragma HLS array_partition variable=input_A cyclic factor=6 dim=1
#pragma HLS array_partition variable=output cyclic factor=6 dim=1
#pragma HLS array_partition variable=input_B cyclic factor=16 dim=1
#pragma HLS array_partition variable=input_A cyclic factor=16 dim=2
#pragma HLS array_partition variable=input_B cyclic factor=4 dim=2
#pragma HLS array_partition variable=output cyclic factor=4 dim=2

for (int i = 0; i < 96; i++) {
#pragma HLS unroll factor=6
for (int k = 0; k < 128; k++) {
#pragma HLS unroll factor=4
    output[i][k] = 0;
}
}


ap_fixed<32, 10, AP_RND, AP_WRAP> acc_out[96][128];
for (int _a0 = 0; _a0 < 96; _a0++) for (int _a1 = 0; _a1 < 128; _a1++) acc_out[_a0][_a1] = output[_a0][_a1];
for (int i = 0; i < 96; i++) {
#pragma HLS unroll factor=6
for (int j = 0; j < 512; j++) {
#pragma HLS unroll factor=16
for (int k = 0; k < 128; k++) {
#pragma HLS unroll factor=4
    acc_out[i][k] += input_A[i][j] * input_B[j][k];
}
}
}
for (int _a0 = 0; _a0 < 96; _a0++) for (int _a1 = 0; _a1 < 128; _a1++) output[_a0][_a1] = (data_t)acc_out[_a0][_a1];
}
/*==== GEMM_IJK FUNCTION END ====*/
//////////////////////////////////////////
// END: GEMM_IJK FUNCTION
//////////////////////////////////////////


void store_96_128_ap_fixed_16_5_AP_TRN_AP_WRAP_(data_t input[96][128], data_t output[96][128])
{
    for (int idx0 = 0; idx0 < 96; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void top(data_t DRAM_1[96][512], data_t DRAM_2[512][128], data_t DRAM_3[96][128], data_t DRAM_4[96][128])
{
    #pragma HLS interface m_axi port=DRAM_1 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_2 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_3 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_4 offset=slave bundle=mem2

    load_96_512_ap_fixed_16_5_AP_TRN_AP_WRAP_(DRAM_1, BRAM_1);
    load_512_128_ap_fixed_16_5_AP_TRN_AP_WRAP_(DRAM_2, BRAM_2);
    load_96_128_ap_fixed_16_5_AP_TRN_AP_WRAP_(DRAM_3, BRAM_3);
    //////////////////////////////////////////
// Begin: Call to GEMM_IJK
//////////////////////////////////////////
gemm_ijk(BRAM_1, BRAM_2, BRAM_4);
//////////////////////////////////////////
// End: Call to GEMM_IJK
//////////////////////////////////////////

    store_96_128_ap_fixed_16_5_AP_TRN_AP_WRAP_(BRAM_4, DRAM_4);
}