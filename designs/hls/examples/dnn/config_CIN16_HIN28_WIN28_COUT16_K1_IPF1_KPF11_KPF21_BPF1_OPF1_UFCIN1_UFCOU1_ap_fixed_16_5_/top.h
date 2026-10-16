#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16,5> data_t;

void top(data_t DRAM_image_input[16][28][28], data_t DRAM_conv_weight[16][16][1][1], data_t DRAM_conv_bias[16], data_t DRAM_batch_norm_weights[4][16], data_t DRAM_image_output[16][30][30]);

#endif // TOP_H