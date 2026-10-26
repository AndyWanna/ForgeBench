#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16, 5> data_t;

void top(data_t DRAM_image_input[1024][14][14], data_t DRAM_conv_weight_1[256][1024][1][1], data_t DRAM_bn_weight_1[4][256], data_t DRAM_conv_weight_2[256][256][3][3], data_t DRAM_bn_weight_2[4][256], data_t DRAM_conv_weight_3[1024][256][1][1], data_t DRAM_bn_weight_3[4][1024], data_t DRAM_image_output[1024][14][14]);

#endif // TOP_H