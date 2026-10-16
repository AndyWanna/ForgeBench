#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16, 5> data_t;

void top(data_t DRAM_image_input[256][14][14], data_t DRAM_conv_weight_1[512][256][3][3], data_t DRAM_bn_weight_1[4][512], data_t DRAM_conv_weight_2[512][512][3][3], data_t DRAM_conv_weight_downsample[512][256][1][1], data_t DRAM_bn_weight_2[4][512], data_t DRAM_image_output[512][7][7]);

#endif // TOP_H