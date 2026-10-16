#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<32, 10, AP_TRN, AP_WRAP> data_t;

void top(data_t DRAM_1[64], data_t DRAM_2[64], data_t DRAM_3[1], data_t DRAM_4[1]);

#endif // TOP_H