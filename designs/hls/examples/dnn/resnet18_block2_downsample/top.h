#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16, 5> data_t;

void top(data_t DRAM_image_input[64][56][56], data_t DRAM_conv_weight_1[128][64][3][3], data_t DRAM_bn_weight_1[4][128], data_t DRAM_conv_weight_2[128][128][3][3], data_t DRAM_conv_weight_downsample[128][64][1][1], data_t DRAM_bn_weight_2[4][128], data_t DRAM_image_output[128][28][28]);

#endif // TOP_H