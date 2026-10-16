#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<16,5> data_t;

void top(data_t DRAM_x[64], data_t DRAM_A[64][64], data_t DRAM_B[64][64], data_t DRAM_y[64], data_t DRAM_bias[1], data_t DRAM_result[1]);

#endif // TOP_H