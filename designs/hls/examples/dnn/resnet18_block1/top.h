#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16, 5> data_t;

void top(data_t DRAM_image_input[64][56][56], data_t DRAM_conv_weight_1[64][64][3][3], data_t DRAM_bn_weight_1[4][64], data_t DRAM_conv_weight_2[64][64][3][3], data_t DRAM_bn_weight_2[4][64], data_t DRAM_image_output[64][56][56]);

#endif // TOP_H