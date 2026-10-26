#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16, 5> data_t;

void top(data_t DRAM_image_input[128][56][56], data_t DRAM_conv_weight_1[256][128][3][3], data_t DRAM_conv_weight_2[256][256][3][3], data_t DRAM_conv_weight_3[256][256][3][3], data_t DRAM_conv_weight_4[256][256][3][3], data_t DRAM_image_output[256][28][28]);

#endif // TOP_H