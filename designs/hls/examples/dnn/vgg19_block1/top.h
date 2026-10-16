#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16, 5> data_t;

void top(data_t DRAM_image_input[64][112][112], data_t DRAM_conv_weight_1[128][64][3][3], data_t DRAM_conv_weight_2[128][128][3][3], data_t DRAM_image_output[128][56][56]);

#endif // TOP_H