#include <ap_fixed.h>
#ifndef TOP_H
#define TOP_H

typedef ap_fixed<32, 10, AP_TRN, AP_WRAP> data_t;

void top(data_t DRAM_input_tanh[64][28][28], data_t DRAM_output_tanh[64][28][28]);

#endif // TOP_H