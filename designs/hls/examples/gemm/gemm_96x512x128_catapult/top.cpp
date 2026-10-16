
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ac_fixed.h>
#include <ac_std_float.h>
#include <ac_int.h>
#include <stdlib.h>
#include <cstdint>
#include "forgebench_math.h"
using namespace std;

typedef ac_fixed<16,5,true,AC_TRN,AC_WRAP> data_t;

data_t BRAM_1[96][512];
data_t BRAM_2[512][128];
data_t BRAM_3[96][128];
data_t BRAM_4[96][128];

void load_96_512_fixed_16_5_(data_t input[96][512], data_t output[96][512])
{
    for (int idx0 = 0; idx0 < 96; idx0++) {
        for (int idx1 = 0; idx1 < 512; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void load_512_128_fixed_16_5_(data_t input[512][128], data_t output[512][128])
{
    for (int idx0 = 0; idx0 < 512; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void load_96_128_fixed_16_5_(data_t input[96][128], data_t output[96][128])
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
    ac_fixed<16,5,true,AC_TRN,AC_WRAP> input_A[96][512],
    ac_fixed<16,5,true,AC_TRN,AC_WRAP> input_B[512][128],
    ac_fixed<16,5,true,AC_TRN,AC_WRAP> output[96][128]
)

{

#pragma hls_unroll 6
for (int i = 0; i < 96; i++) {
#pragma hls_unroll 4
for (int k = 0; k < 128; k++) {
    output[i][k] = data_t(0);
}
}


ac_fixed<32,10,true,AC_RND,AC_WRAP> acc_out[96][128];
for (int _a0 = 0; _a0 < 96; _a0++) for (int _a1 = 0; _a1 < 128; _a1++) acc_out[_a0][_a1] = output[_a0][_a1];
#pragma hls_unroll 6
for (int i = 0; i < 96; i++) {
#pragma hls_unroll 16
for (int j = 0; j < 512; j++) {
#pragma hls_unroll 4
for (int k = 0; k < 128; k++) {
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


void store_96_128_fixed_16_5_(data_t input[96][128], data_t output[96][128])
{
    for (int idx0 = 0; idx0 < 96; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void top(data_t DRAM_1[96][512], data_t DRAM_2[512][128], data_t DRAM_3[96][128], data_t DRAM_4[96][128])
{

    load_96_512_fixed_16_5_(DRAM_1, BRAM_1);
    load_512_128_fixed_16_5_(DRAM_2, BRAM_2);
    load_96_128_fixed_16_5_(DRAM_3, BRAM_3);
    //////////////////////////////////////////
// Begin: Call to GEMM_IJK
//////////////////////////////////////////
gemm_ijk(BRAM_1, BRAM_2, BRAM_4);
//////////////////////////////////////////
// End: Call to GEMM_IJK
//////////////////////////////////////////

    store_96_128_fixed_16_5_(BRAM_4, DRAM_4);
}