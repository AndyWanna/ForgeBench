
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

data_t BRAM_image_input[256][14][14];
data_t BRAM_conv_weight_1[256][256][3][3];
data_t BRAM_bn_weight_1[4][256];
data_t BRAM_buffer_1[256][14][14];
data_t BRAM_buffer_2[256][14][14];
data_t BRAM_conv_weight_2[256][256][3][3];
data_t BRAM_bn_weight_2[4][256];
data_t BRAM_buffer_3[256][14][14];

void load_256_14_14_ap_fixed_16_5_(data_t input[256][14][14], data_t output[256][14][14])
{
    for (int idx0 = 0; idx0 < 256; idx0++) {
        for (int idx1 = 0; idx1 < 14; idx1++) {
            for (int idx2 = 0; idx2 < 14; idx2++) {
                output[idx0][idx1][idx2] = input[idx0][idx1][idx2];
            }
        }
    }
}

void load_256_256_3_3_ap_fixed_16_5_(data_t input[256][256][3][3], data_t output[256][256][3][3])
{
    for (int idx0 = 0; idx0 < 256; idx0++) {
        for (int idx1 = 0; idx1 < 256; idx1++) {
            for (int idx2 = 0; idx2 < 3; idx2++) {
                for (int idx3 = 0; idx3 < 3; idx3++) {
                    output[idx0][idx1][idx2][idx3] = input[idx0][idx1][idx2][idx3];
                }
            }
        }
    }
}

void load_4_256_ap_fixed_16_5_(data_t input[4][256], data_t output[4][256])
{
    for (int idx0 = 0; idx0 < 4; idx0++) {
        for (int idx1 = 0; idx1 < 256; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

/*
 * Auto-generated Convolution HLS Code (with optional bias and grouping)
 *
 * Dimensions:
 *   Input  : [256][14][14]
 *   Kernel : [256][256][3][3]
 *   Output : [256][14][14]
 *
 * Data type: ap_fixed<16, 5>
 *
 * If bias is included, a 1D bias array of size [256] is expected.
 * For group convolution, we assume:
 *   256 % groups == 0
 *   256 % groups == 0
 */

// Use a typedef for the data type

//////////////////////////////////////////
// Begin: CONV2D FUNCTION (co in the innermost loop)
//////////////////////////////////////////

void conv2d_256_256_14_14_14_14_3_1_1_ap_fixed_16_5_(
    data_t input[256][14][14],
    data_t kernel[256][256][3][3],
    data_t output[256][14][14]
)
{
    #pragma HLS array_partition variable=input  type=cyclic factor=8  dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=64 dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=8 dim=2

    #pragma HLS array_partition variable=output type=cyclic factor=64  dim=1
    // Initialize output to ((data_t)0), with co in the innermost loop
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 14; j++) {
            for (int co = 0; co < 256; co++) {
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
                        for (int ci = 0; ci < 256; ci++) {
                        #pragma HLS unroll factor=8
                            for (int co = 0; co < 256; co++) {
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
 * Auto-generated Batch Normalization HLS Code
 *
 * Dimensions:
 *   Input/Output: [256][14][14]
 * Data type: ap_fixed<16, 5>
 * Epsilon: 1e-05
 *
 * The batch normalization is computed as:
 *   output[c][h][w] = gamma[c]*(input[c][h][w] - mean[c]) / sqrt(variance[c] + epsilon) + beta[c];
 */

// Use a typedef for the data type
// weights[4][C_OUT], 0: gamma, 1: beta, 2: mean, 3: variance
void batch_norm_256_14_14_ap_fixed_16_5_(
    data_t input[256][14][14],
    data_t weights[4][256],
    data_t output[256][14][14]
)
{
    for (int c = 0; c < 256; c++) {
        for (int h = 0; h < 14; h++) {
            for (int w = 0; w < 14; w++) {
                data_t norm = (input[c][h][w] - weights[2][c]) / hls::sqrt(weights[3][c] + (data_t)1e-05);
                output[c][h][w] = weights[0][c] * norm + weights[1][c];
            }
        }
    }
}


/*
 * Auto-generated Activation Functions for CNNs (3D version)
 * 
 * Data type: ap_fixed<16, 5>
 * Tensor dimensions: [256][14][14]
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

void relu_256_14_14_ap_fixed_16_5_(
    data_t input[256][14][14],
    data_t output[256][14][14]
)
{
    for (int c = 0; c < 256; c++) {
        for (int i = 0; i < 14; i++) {
            for (int j = 0; j < 14; j++) {
                output[c][i][j] = (input[c][i][j] > 0) ? input[c][i][j] : (data_t)0;
            }
        }
    }
}


void matrix_add_256_14_14_ap_fixed_16_5_(
    data_t in1[256][14][14],
    data_t in2[256][14][14],
    data_t out[256][14][14]
)
{
    for (int c = 0; c < 256; c++) {
        for (int i = 0; i < 14; i++) {
            for (int j = 0; j < 14; j++) {
                out[c][i][j] = in1[c][i][j] + in2[c][i][j];
            }
        }
    }
}







void store_256_14_14_ap_fixed_16_5_(data_t input[256][14][14], data_t output[256][14][14])
{
    for (int idx0 = 0; idx0 < 256; idx0++) {
        for (int idx1 = 0; idx1 < 14; idx1++) {
            for (int idx2 = 0; idx2 < 14; idx2++) {
                output[idx0][idx1][idx2] = input[idx0][idx1][idx2];
            }
        }
    }
}

void top(data_t DRAM_image_input[256][14][14], data_t DRAM_conv_weight_1[256][256][3][3], data_t DRAM_bn_weight_1[4][256], data_t DRAM_conv_weight_2[256][256][3][3], data_t DRAM_bn_weight_2[4][256], data_t DRAM_image_output[256][14][14])
{
    #pragma HLS interface m_axi port=DRAM_image_input offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_conv_weight_1 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_bn_weight_1 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_conv_weight_2 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_bn_weight_2 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_image_output offset=slave bundle=mem2

    load_256_14_14_ap_fixed_16_5_(DRAM_image_input, BRAM_image_input);
    load_256_256_3_3_ap_fixed_16_5_(DRAM_conv_weight_1, BRAM_conv_weight_1);
    load_4_256_ap_fixed_16_5_(DRAM_bn_weight_1, BRAM_bn_weight_1);
    load_256_256_3_3_ap_fixed_16_5_(DRAM_conv_weight_2, BRAM_conv_weight_2);
    load_4_256_ap_fixed_16_5_(DRAM_bn_weight_2, BRAM_bn_weight_2);
    conv2d_256_256_14_14_14_14_3_1_1_ap_fixed_16_5_(BRAM_image_input, BRAM_conv_weight_1, BRAM_buffer_1);
    batch_norm_256_14_14_ap_fixed_16_5_(BRAM_buffer_1, BRAM_bn_weight_1, BRAM_buffer_2);
    relu_256_14_14_ap_fixed_16_5_(BRAM_buffer_2, BRAM_buffer_1);
    conv2d_256_256_14_14_14_14_3_1_1_ap_fixed_16_5_(BRAM_buffer_1, BRAM_conv_weight_2, BRAM_buffer_2);
    batch_norm_256_14_14_ap_fixed_16_5_(BRAM_buffer_2, BRAM_bn_weight_2, BRAM_buffer_1);
    matrix_add_256_14_14_ap_fixed_16_5_(BRAM_buffer_1, BRAM_image_input, BRAM_buffer_2);
    relu_256_14_14_ap_fixed_16_5_(BRAM_buffer_2, BRAM_buffer_1);
    store_256_14_14_ap_fixed_16_5_(BRAM_buffer_1, DRAM_image_output);
}