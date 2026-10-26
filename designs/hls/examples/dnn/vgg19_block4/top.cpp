
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

typedef ap_fixed<16, 5> data_t;

data_t BRAM_image_input[512][14][14];
data_t BRAM_conv_weight_1[512][512][3][3];
data_t BRAM_conv_weight_2[512][512][3][3];
data_t BRAM_conv_weight_3[512][512][3][3];
data_t BRAM_conv_weight_4[512][512][3][3];
data_t BRAM_buffer_1[512][14][14];
data_t BRAM_buffer_2[512][14][14];
data_t BRAM_buffer_3[512][7][7];

void load_512_14_14_ap_fixed_16_5_(data_t input[512][14][14], data_t output[512][14][14])
{
    for (int idx0 = 0; idx0 < 512; idx0++) {
        for (int idx1 = 0; idx1 < 14; idx1++) {
            for (int idx2 = 0; idx2 < 14; idx2++) {
                output[idx0][idx1][idx2] = input[idx0][idx1][idx2];
            }
        }
    }
}

void load_512_512_3_3_ap_fixed_16_5_(data_t input[512][512][3][3], data_t output[512][512][3][3])
{
    for (int idx0 = 0; idx0 < 512; idx0++) {
        for (int idx1 = 0; idx1 < 512; idx1++) {
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
 *   Input  : [512][14][14]
 *   Kernel : [512][512][3][3]
 *   Output : [512][14][14]
 *
 * Data type: ap_fixed<16, 5>
 *
 * If bias is included, a 1D bias array of size [512] is expected.
 * For group convolution, we assume:
 *   512 % groups == 0
 *   512 % groups == 0
 */

// Use a typedef for the data type

//////////////////////////////////////////
// Begin: CONV2D FUNCTION (co in the innermost loop)
//////////////////////////////////////////

void conv2d_512_512_14_14_14_14_3_1_1_ap_fixed_16_5_(
    data_t input[512][14][14],
    data_t kernel[512][512][3][3],
    data_t output[512][14][14]
)
{
    #pragma HLS array_partition variable=input  type=cyclic factor=8  dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=64 dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=8 dim=2

    #pragma HLS array_partition variable=output type=cyclic factor=64  dim=1
    // Initialize output to ((data_t)0), with co in the innermost loop
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 14; j++) {
            for (int co = 0; co < 512; co++) {
            #pragma HLS unroll factor=64
                output[co][i][j] = ((data_t)0);
            }
        }
    }

    // Perform convolution (co in the innermost loop)
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 14; j++) {
            for (int kh = 0; kh < 3; kh++) {
                for (int kw = 0; kw < 3; kw++) {
                    int in_row = i * 1 - 1 + kh;
                    int in_col = j * 1 - 1 + kw;
                    if (in_row >= 0 && in_row < 14 && in_col >= 0 && in_col < 14) {
                        for (int ci = 0; ci < 512; ci++) {
                        #pragma HLS unroll factor=8
                            for (int co = 0; co < 512; co++) {
                                #pragma HLS unroll factor=64
                                output[co][i][j] += input[ci][in_row][in_col] * kernel[co][ci][kh][kw];
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
 * Auto-generated Activation Functions for CNNs (3D version)
 * 
 * Data type: ap_fixed<16, 5>
 * Tensor dimensions: [512][14][14]
 *
 * Available functions:
 *   - relu
 *   - leaky_relu
 *   - prelu
 *   - rrelu
 *   - thresholded_relu
 *   - relu6
 *   - sigmoid
 *   - tanh_act
 *   - elu
 *   - selu
 *   - gelu
 *   - swish
 *   - softmax
 */

void relu_512_14_14_ap_fixed_16_5_(
    data_t input[512][14][14],
    data_t output[512][14][14]
)
{
    for (int c = 0; c < 512; c++) {
        for (int i = 0; i < 14; i++) {
            for (int j = 0; j < 14; j++) {
                output[c][i][j] = (input[c][i][j] > 0) ? input[c][i][j] : (data_t)0;
            }
        }
    }
}


/*
 * Auto-generated Max Pooling HLS Code
 *
 * Dimensions:
 *   Input  : [512][14][14]
 *   Output : [512][7][7]
 *   Pooling Window: [2][2]
 *   Stride: [2][2]
 *
 * Data type: ap_fixed<16, 5>
 */

void maxpool_512_14_14_7_7_2_2_2_2_ap_fixed_16_5_(
    data_t input[512][14][14],
    data_t output[512][7][7]
)
{
    for (int c = 0; c < 512; c++) {
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                // Initialize max value from the top-left corner of the window
                data_t max_val = input[c][i * 2][j * 2];
                for (int kh = 0; kh < 2; kh++) {
                    for (int kw = 0; kw < 2; kw++) {
                        int row = i * 2 + kh;
                        int col = j * 2 + kw;
                        if (input[c][row][col] > max_val) {
                            max_val = input[c][row][col];
                        }
                    }
                }
                output[c][i][j] = max_val;
            }
        }
    }
}


void store_512_7_7_ap_fixed_16_5_(data_t input[512][7][7], data_t output[512][7][7])
{
    for (int idx0 = 0; idx0 < 512; idx0++) {
        for (int idx1 = 0; idx1 < 7; idx1++) {
            for (int idx2 = 0; idx2 < 7; idx2++) {
                output[idx0][idx1][idx2] = input[idx0][idx1][idx2];
            }
        }
    }
}

void top(data_t DRAM_image_input[512][14][14], data_t DRAM_conv_weight_1[512][512][3][3], data_t DRAM_conv_weight_2[512][512][3][3], data_t DRAM_conv_weight_3[512][512][3][3], data_t DRAM_conv_weight_4[512][512][3][3], data_t DRAM_image_output[512][7][7])
{
    #pragma HLS interface m_axi port=DRAM_image_input offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_conv_weight_1 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_conv_weight_2 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_conv_weight_3 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_conv_weight_4 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_image_output offset=slave bundle=mem2

    load_512_14_14_ap_fixed_16_5_(DRAM_image_input, BRAM_image_input);
    load_512_512_3_3_ap_fixed_16_5_(DRAM_conv_weight_1, BRAM_conv_weight_1);
    load_512_512_3_3_ap_fixed_16_5_(DRAM_conv_weight_2, BRAM_conv_weight_2);
    load_512_512_3_3_ap_fixed_16_5_(DRAM_conv_weight_3, BRAM_conv_weight_3);
    load_512_512_3_3_ap_fixed_16_5_(DRAM_conv_weight_4, BRAM_conv_weight_4);
    conv2d_512_512_14_14_14_14_3_1_1_ap_fixed_16_5_(BRAM_image_input, BRAM_conv_weight_1, BRAM_buffer_1);
    relu_512_14_14_ap_fixed_16_5_(BRAM_buffer_1, BRAM_buffer_2);
    conv2d_512_512_14_14_14_14_3_1_1_ap_fixed_16_5_(BRAM_buffer_2, BRAM_conv_weight_2, BRAM_buffer_1);
    relu_512_14_14_ap_fixed_16_5_(BRAM_buffer_1, BRAM_buffer_2);
    conv2d_512_512_14_14_14_14_3_1_1_ap_fixed_16_5_(BRAM_buffer_2, BRAM_conv_weight_3, BRAM_buffer_1);
    relu_512_14_14_ap_fixed_16_5_(BRAM_buffer_1, BRAM_buffer_2);
    conv2d_512_512_14_14_14_14_3_1_1_ap_fixed_16_5_(BRAM_buffer_2, BRAM_conv_weight_4, BRAM_buffer_1);
    relu_512_14_14_ap_fixed_16_5_(BRAM_buffer_1, BRAM_buffer_2);
    maxpool_512_14_14_7_7_2_2_2_2_ap_fixed_16_5_(BRAM_buffer_2, BRAM_buffer_3);
    store_512_7_7_ap_fixed_16_5_(BRAM_buffer_3, DRAM_image_output);
}