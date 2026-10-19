
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

typedef ap_fixed<16,5> data_t;

data_t BRAM_x[64];
data_t BRAM_A[64][64];
data_t BRAM_B[64][64];
data_t BRAM_y[64];
data_t BRAM_bias[1];
data_t BRAM_result[1];
data_t BRAM_xA_bias[64];
data_t BRAM_xA[64];
data_t BRAM_xAB_bias[64];
data_t BRAM_xAB[64];

void load_64_ap_fixed_16_5_(data_t input[64], data_t output[64])
{
    for (int idx0 = 0; idx0 < 64; idx0++) {
        output[idx0] = input[idx0];
    }
}

void load_64_64_ap_fixed_16_5_(data_t input[64][64], data_t output[64][64])
{
    for (int idx0 = 0; idx0 < 64; idx0++) {
        for (int idx1 = 0; idx1 < 64; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void load_1_ap_fixed_16_5_(data_t input[1], data_t output[1])
{
    for (int idx0 = 0; idx0 < 1; idx0++) {
        output[idx0] = input[idx0];
    }
}

//////////////////////////////////////////
// Begin: VMM_JI_64_64 FUNCTION
//////////////////////////////////////////
/*==== VMM_JI_64_64 FUNCTION START ====*/
void vmm_ji_64_64(
    ap_fixed<16,5> input_A[64][64],
    ap_fixed<16,5> input_B[64],
    ap_fixed<16,5> output[64]
)
{

for (int j = 0; j < 64; j++) {
#pragma HLS unroll factor=1
    output[j] = 0;
}


for (int j = 0; j < 64; j++) {
#pragma HLS unroll factor=1
for (int i = 0; i < 64; i++) {
#pragma HLS unroll factor=1
    output[j] += input_A[i][j] * input_B[i];
}
}
}
/*==== VMM_JI_64_64 FUNCTION END ====*/
//////////////////////////////////////////
// END: VMM_JI_64_64 FUNCTION
//////////////////////////////////////////


//////////////////////////////////////////
// Begin: DOT_PRODUCT FUNCTION
//////////////////////////////////////////
/*==== DOT_PRODUCT FUNCTION START ====*/
void dot_product(
    ap_fixed<16,5> input_A[64],
    ap_fixed<16,5> input_B[64],
    ap_fixed<16,5> output[1]
)
{

output[0] = 0;

for (int i = 0; i < 64; i++) {
#pragma HLS unroll factor=1
    output[0] += input_A[i] * input_B[i];
}
}
/*==== DOT_PRODUCT FUNCTION END ====*/
//////////////////////////////////////////
// END: DOT_PRODUCT FUNCTION
//////////////////////////////////////////


void store_1_ap_fixed_16_5_(data_t input[1], data_t output[1])
{
    for (int idx0 = 0; idx0 < 1; idx0++) {
        output[idx0] = input[idx0];
    }
}

void top(data_t DRAM_x[64], data_t DRAM_A[64][64], data_t DRAM_B[64][64], data_t DRAM_y[64], data_t DRAM_bias[1], data_t DRAM_result[1])
{
    #pragma HLS interface m_axi port=DRAM_x offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_A offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_B offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_y offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_bias offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_result offset=slave bundle=mem2

    load_64_ap_fixed_16_5_(DRAM_x, BRAM_x);
    load_64_64_ap_fixed_16_5_(DRAM_A, BRAM_A);
    load_64_64_ap_fixed_16_5_(DRAM_B, BRAM_B);
    load_64_ap_fixed_16_5_(DRAM_y, BRAM_y);
    load_1_ap_fixed_16_5_(DRAM_bias, BRAM_bias);
    //////////////////////////////////////////
// Begin: Inline implementation of VMM_JI
//////////////////////////////////////////

for (int j = 0; j < 64; j++) {
#pragma HLS unroll factor=1
    BRAM_xA[j] = 0;
}


for (int j = 0; j < 64; j++) {
#pragma HLS unroll factor=1
for (int i = 0; i < 64; i++) {
#pragma HLS unroll factor=1
    BRAM_xA[j] += BRAM_A[i][j] * BRAM_x[i];
}
}
//////////////////////////////////////////
// End: Inline implementation of VMM_JI
//////////////////////////////////////////

    //////////////////////////////////////////
// Begin: Inline implementation of VMM_JI
//////////////////////////////////////////

for (int j = 0; j < 64; j++) {
#pragma HLS unroll factor=1
    BRAM_xAB[j] = 0;
}


for (int j = 0; j < 64; j++) {
#pragma HLS unroll factor=1
for (int i = 0; i < 64; i++) {
#pragma HLS unroll factor=1
    BRAM_xAB[j] += BRAM_B[i][j] * BRAM_xA[i];
}
}
//////////////////////////////////////////
// End: Inline implementation of VMM_JI
//////////////////////////////////////////

    //////////////////////////////////////////
// Begin: Inline implementation of DOT_PRODUCT
//////////////////////////////////////////

BRAM_result[0] = 0;

for (int i = 0; i < 64; i++) {
#pragma HLS unroll factor=1
    BRAM_result[0] += BRAM_xAB[i] * BRAM_y[i];
}
//////////////////////////////////////////
// End: Inline implementation of DOT_PRODUCT
//////////////////////////////////////////

    store_1_ap_fixed_16_5_(BRAM_result, DRAM_result);
}