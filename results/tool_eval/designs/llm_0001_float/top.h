#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef float data_t;

void top(data_t DRAM_attn_input[8][128], data_t DRAM_WQ[128][128], data_t DRAM_WK[128][128], data_t DRAM_WV[128][128], data_t DRAM_norm_output[8][128], data_t DRAM_norm_gamma[128], data_t DRAM_norm_beta[128]);

#endif // TOP_H