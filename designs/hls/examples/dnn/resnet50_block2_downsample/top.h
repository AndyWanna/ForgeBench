#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16, 5> data_t;

void top(data_t DRAM_image_input[256][56][56], data_t DRAM_conv_weight_1[128][256][1][1], data_t DRAM_bn_weight_1[4][128], data_t DRAM_conv_weight_2[128][128][3][3], data_t DRAM_bn_weight_2[4][128], data_t DRAM_conv_weight_3[512][128][1][1], data_t DRAM_bn_weight_3[4][512], data_t DRAM_conv_weight_downsample[512][256][1][1], data_t DRAM_image_output[512][28][28]);

#endif // TOP_H