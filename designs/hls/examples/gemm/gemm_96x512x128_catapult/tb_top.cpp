#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ac_fixed.h>
#include <ac_std_float.h>
#include "top.h"  // Include the top function declaration
#include <mc_scverify.h>

// ac_fixed has no conversion operator to double/float (only to_double()); the testbench converts through these.
inline double fb_to_double(double x) { return x; }
template <int W, int I, bool S, ac_q_mode Q, ac_o_mode O>
inline double fb_to_double(const ac_fixed<W, I, S, Q, O> &x) { return x.to_double(); }
template <ac_ieee_float_format F>
inline double fb_to_double(const ac_ieee_float<F> &x) { return x.to_float(); }

#ifndef VERIF_RTOL
#define VERIF_RTOL 1e-3
#endif
#ifndef VERIF_ATOL
#define VERIF_ATOL 1e-5
#endif
#ifndef VERIF_ATOL_SCALE
#define VERIF_ATOL_SCALE 5e-5
#endif

typedef ac_fixed<16,5,true,AC_TRN,AC_WRAP> data_t;

data_t DRAM_1[96][512];
data_t DRAM_2[512][128];
data_t DRAM_3[96][128];
data_t DRAM_4[96][128];

void load_txt_to_array(const char *filename, data_t *array, int total_size) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open %s\n", filename);
        exit(1);
    }
    for (int i = 0; i < total_size; i++) {
        double temp;   // double: a float32 read cannot resolve wide fixed-point types
        fscanf(fp, "%lf", &temp);
        array[i] = (data_t)temp;
    }
    fclose(fp);
}

CCS_MAIN(int argc, char *argv[]) {
    load_txt_to_array("DRAM_1.txt", (data_t*)DRAM_1, 49152);
    load_txt_to_array("DRAM_2.txt", (data_t*)DRAM_2, 65536);
    load_txt_to_array("DRAM_3.txt", (data_t*)DRAM_3, 12288);
    load_txt_to_array("DRAM_4.txt", (data_t*)DRAM_4, 12288);

    CCS_DESIGN(top)(DRAM_1, DRAM_2, DRAM_3, DRAM_4);

    double verif_max_abs = 0.0, verif_max_rel = 0.0;
    long verif_n_mismatch = 0, verif_n_total = 0, verif_n_checked = 0;

    // Write contents of DRAM_4 to DRAM_4_output.txt
    {
        FILE *fp = fopen("DRAM_4_output.txt", "w");
        if (fp != NULL) {
            for (int i = 0; i < 12288; i++) {
                fprintf(fp, "%.17g ", fb_to_double(((data_t*)DRAM_4)[i]));
            }
            fclose(fp);
        }
    }
    // Compare DRAM_4 against DRAM_4.golden.txt if present
    {
        FILE *gf = fopen("DRAM_4.golden.txt", "r");
        if (gf != NULL) {
            static float verif_golden[12288];
            long gn = 0;
            for (int i = 0; i < 12288; i++) {
                float gv;
                if (fscanf(gf, "%f", &gv) != 1) break;
                verif_golden[i] = gv; gn++;
            }
            double verif_scale = 0.0;
            for (long i = 0; i < gn; i++) {
                double m = fabs((double)verif_golden[i]);
                if (m > verif_scale) verif_scale = m;
            }
            double verif_atol = VERIF_ATOL + VERIF_ATOL_SCALE * verif_scale;
            for (long i = 0; i < gn; i++) {
                double a = (double)(float)fb_to_double(((data_t*)DRAM_4)[i]);
                double b = (double)verif_golden[i];
                double abs_err = fabs(a - b);
                double rel_err = abs_err / (fabs(b) + 1e-12);
                double tol = verif_atol + VERIF_RTOL * fabs(b);
                int bad = (!(abs_err <= tol)) || (!isfinite(a));
                if (abs_err > verif_max_abs) verif_max_abs = abs_err;
                if (rel_err > verif_max_rel) verif_max_rel = rel_err;
                if (bad) verif_n_mismatch++;
                verif_n_total++;
            }
            fclose(gf);
            verif_n_checked++;
        }
    }

    if (verif_n_checked == 0) {
        printf("VERIFICATION: SKIP (no golden files found)\n");
        CCS_RETURN(0);
    }
    if (verif_n_mismatch == 0) {
        printf("VERIFICATION: PASS (max_abs=%g, max_rel=%g, n=%ld)\n", verif_max_abs, verif_max_rel, verif_n_total);
        CCS_RETURN(0);
    }
    printf("VERIFICATION: FAIL (max_abs=%g, max_rel=%g, n_mismatch=%ld/%ld)\n", verif_max_abs, verif_max_rel, verif_n_mismatch, verif_n_total);
    CCS_RETURN(1);
}