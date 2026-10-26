#include <ac_fixed.h>
#include <ac_std_float.h>
#ifndef TOP_H
#define TOP_H

typedef ac_fixed<16,5,true,AC_TRN,AC_WRAP> data_t;

void top(data_t DRAM_1[96][512], data_t DRAM_2[512][128], data_t DRAM_3[96][128], data_t DRAM_4[96][128]);

#endif // TOP_H