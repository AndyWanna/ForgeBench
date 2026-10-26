#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16, 5> data_t;

void top(data_t DRAM_image_input[512][14][14], data_t DRAM_conv_weight_1[512][512][3][3], data_t DRAM_conv_weight_2[512][512][3][3], data_t DRAM_conv_weight_3[512][512][3][3], data_t DRAM_conv_weight_4[512][512][3][3], data_t DRAM_image_output[512][7][7]);

#endif // TOP_H