
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

data_t BRAM_image_input[64][112][112];
data_t BRAM_conv_weight_1[128][64][3][3];
data_t BRAM_conv_weight_2[128][128][3][3];
data_t BRAM_buffer_1[128][112][112];
data_t BRAM_buffer_2[128][112][112];
data_t BRAM_buffer_3[128][56][56];

void load_64_112_112_ap_fixed_16_5_(data_t input[64][112][112], data_t output[64][112][112])
{
    for (int idx0 = 0; idx0 < 64; idx0++) {
        for (int idx1 = 0; idx1 < 112; idx1++) {
            for (int idx2 = 0; idx2 < 112; idx2++) {
                output[idx0][idx1][idx2] = input[idx0][idx1][idx2];
            }
        }
    }
}

void load_128_64_3_3_ap_fixed_16_5_(data_t input[128][64][3][3], data_t output[128][64][3][3])
{
    for (int idx0 = 0; idx0 < 128; idx0++) {
        for (int idx1 = 0; idx1 < 64; idx1++) {
            for (int idx2 = 0; idx2 < 3; idx2++) {
                for (int idx3 = 0; idx3 < 3; idx3++) {
                    output[idx0][idx1][idx2][idx3] = input[idx0][idx1][idx2][idx3];
                }
            }
        }
    }
}

void load_128_128_3_3_ap_fixed_16_5_(data_t input[128][128][3][3], data_t output[128][128][3][3])
{
    for (int idx0 = 0; idx0 < 128; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
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
 *   Input  : [64][112][112]
 *   Kernel : [128][64][3][3]
 *   Output : [128][112][112]
 *
 * Data type: ap_fixed<16, 5>
 *
 * If bias is included, a 1D bias array of size [128] is expected.
 * For group convolution, we assume:
 *   64 % groups == 0
 *   128 % groups == 0
 */

// Use a typedef for the data type

//////////////////////////////////////////
// Begin: CONV2D FUNCTION (co in the innermost loop)
//////////////////////////////////////////

void conv2d_64_128_112_112_112_112_3_1_1_ap_fixed_16_5_(
    data_t input[64][112][112],
    data_t kernel[128][64][3][3],
    data_t output[128][112][112]
)
{
    #pragma HLS array_partition variable=input  type=cyclic factor=8  dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=64 dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=8 dim=2

    #pragma HLS array_partition variable=output type=cyclic factor=64  dim=1
    // Initialize output to ((data_t)0), with co in the innermost loop
    for (int i = 0; i < 112; i++) {
        for (int j = 0; j < 112; j++) {
            for (int co = 0; co < 128; co++) {
            #pragma HLS unroll factor=64
                output[co][i][j] = ((data_t)0);
            }
        }
    }

    // Perform convolution (co in the innermost loop)
    for (int i = 0; i < 112; i++) {
        for (int j = 0; j < 112; j++) {
            for (int kh = 0; kh < 3; kh++) {
                for (int kw = 0; kw < 3; kw++) {
                    int in_row = i * 1 - 1 + kh;
                    int in_col = j * 1 - 1 + kw;
                    if (in_row >= 0 && in_row < 112 && in_col >= 0 && in_col < 112) {
                        for (int ci = 0; ci < 64; ci++) {
                        #pragma HLS unroll factor=8
                            for (int co = 0; co < 128; co++) {
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
 * Tensor dimensions: [128][112][112]
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

void relu_128_112_112_ap_fixed_16_5_(
    data_t input[128][112][112],
    data_t output[128][112][112]
)
{
    for (int c = 0; c < 128; c++) {
        for (int i = 0; i < 112; i++) {
            for (int j = 0; j < 112; j++) {
                output[c][i][j] = (input[c][i][j] > 0) ? input[c][i][j] : (data_t)0;
            }
        }
    }
}

/*
 * Auto-generated Convolution HLS Code (with optional bias and grouping)
 *
 * Dimensions:
 *   Input  : [128][112][112]
 *   Kernel : [128][128][3][3]
 *   Output : [128][112][112]
 *
 * Data type: ap_fixed<16, 5>
 *
 * If bias is included, a 1D bias array of size [128] is expected.
 * For group convolution, we assume:
 *   128 % groups == 0
 *   128 % groups == 0
 */

// Use a typedef for the data type

//////////////////////////////////////////
// Begin: CONV2D FUNCTION (co in the innermost loop)
//////////////////////////////////////////

void conv2d_128_128_112_112_112_112_3_1_1_ap_fixed_16_5_(
    data_t input[128][112][112],
    data_t kernel[128][128][3][3],
    data_t output[128][112][112]
)
{
    #pragma HLS array_partition variable=input  type=cyclic factor=8  dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=64 dim=1
    #pragma HLS array_partition variable=kernel type=cyclic factor=8 dim=2

    #pragma HLS array_partition variable=output type=cyclic factor=64  dim=1
    // Initialize output to ((data_t)0), with co in the innermost loop
    for (int i = 0; i < 112; i++) {
        for (int j = 0; j < 112; j++) {
            for (int co = 0; co < 128; co++) {
            #pragma HLS unroll factor=64
                output[co][i][j] = ((data_t)0);
            }
        }
    }

    // Perform convolution (co in the innermost loop)
    for (int i = 0; i < 112; i++) {
        for (int j = 0; j < 112; j++) {
            for (int kh = 0; kh < 3; kh++) {
                for (int kw = 0; kw < 3; kw++) {
                    int in_row = i * 1 - 1 + kh;
                    int in_col = j * 1 - 1 + kw;
                    if (in_row >= 0 && in_row < 112 && in_col >= 0 && in_col < 112) {
                        for (int ci = 0; ci < 128; ci++) {
                        #pragma HLS unroll factor=8
                            for (int co = 0; co < 128; co++) {
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
 * Auto-generated Max Pooling HLS Code
 *
 * Dimensions:
 *   Input  : [128][112][112]
 *   Output : [128][56][56]
 *   Pooling Window: [2][2]
 *   Stride: [2][2]
 *
 * Data type: ap_fixed<16, 5>
 */

void maxpool_128_112_112_56_56_2_2_2_2_ap_fixed_16_5_(
    data_t input[128][112][112],
    data_t output[128][56][56]
)
{
    for (int c = 0; c < 128; c++) {
        for (int i = 0; i < 56; i++) {
            for (int j = 0; j < 56; j++) {
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


void store_128_56_56_ap_fixed_16_5_(data_t input[128][56][56], data_t output[128][56][56])
{
    for (int idx0 = 0; idx0 < 128; idx0++) {
        for (int idx1 = 0; idx1 < 56; idx1++) {
            for (int idx2 = 0; idx2 < 56; idx2++) {
                output[idx0][idx1][idx2] = input[idx0][idx1][idx2];
            }
        }
    }
}

void top(data_t DRAM_image_input[64][112][112], data_t DRAM_conv_weight_1[128][64][3][3], data_t DRAM_conv_weight_2[128][128][3][3], data_t DRAM_image_output[128][56][56])
{
    #pragma HLS interface m_axi port=DRAM_image_input offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_conv_weight_1 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_conv_weight_2 offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_image_output offset=slave bundle=mem2

    load_64_112_112_ap_fixed_16_5_(DRAM_image_input, BRAM_image_input);
    load_128_64_3_3_ap_fixed_16_5_(DRAM_conv_weight_1, BRAM_conv_weight_1);
    load_128_128_3_3_ap_fixed_16_5_(DRAM_conv_weight_2, BRAM_conv_weight_2);
    conv2d_64_128_112_112_112_112_3_1_1_ap_fixed_16_5_(BRAM_image_input, BRAM_conv_weight_1, BRAM_buffer_1);
    relu_128_112_112_ap_fixed_16_5_(BRAM_buffer_1, BRAM_buffer_2);
    conv2d_128_128_112_112_112_112_3_1_1_ap_fixed_16_5_(BRAM_buffer_2, BRAM_conv_weight_2, BRAM_buffer_1);
    relu_128_112_112_ap_fixed_16_5_(BRAM_buffer_1, BRAM_buffer_2);
    maxpool_128_112_112_56_56_2_2_2_2_ap_fixed_16_5_(BRAM_buffer_2, BRAM_buffer_3);
    store_128_56_56_ap_fixed_16_5_(BRAM_buffer_3, DRAM_image_output);
}