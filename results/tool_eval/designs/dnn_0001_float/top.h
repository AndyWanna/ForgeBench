#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef float data_t;

void top(data_t DRAM_image_input[16][56][56], data_t DRAM_conv_weight[16][16][1][1], data_t DRAM_conv_bias[16], data_t DRAM_batch_norm_weights[4][16], data_t DRAM_image_output[16][58][58]);

#endif // TOP_H