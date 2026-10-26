#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ap_fixed.h>
#include "top.h"  // Include the top function declaration

#ifndef VERIF_RTOL
#define VERIF_RTOL 1e-3
#endif
#ifndef VERIF_ATOL
#define VERIF_ATOL 1e-5
#endif
#ifndef VERIF_ATOL_SCALE
#define VERIF_ATOL_SCALE 5e-5
#endif

typedef ap_fixed<32, 10, AP_TRN, AP_WRAP> data_t;

data_t DRAM_input_sigmoid[64][28][28];
data_t DRAM_output_sigmoid[64][28][28];

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

int main() {
    load_txt_to_array("DRAM_input_sigmoid.txt", (data_t*)DRAM_input_sigmoid, 50176);
    load_txt_to_array("DRAM_output_sigmoid.txt", (data_t*)DRAM_output_sigmoid, 50176);

    top(DRAM_input_sigmoid, DRAM_output_sigmoid);

    double verif_max_abs = 0.0, verif_max_rel = 0.0;
    long verif_n_mismatch = 0, verif_n_total = 0, verif_n_checked = 0;

    // Write contents of DRAM_output_sigmoid to DRAM_output_sigmoid_output.txt
    {
        FILE *fp = fopen("DRAM_output_sigmoid_output.txt", "w");
        if (fp != NULL) {
            for (int i = 0; i < 50176; i++) {
                fprintf(fp, "%.17g ", (double)((data_t*)DRAM_output_sigmoid)[i]);
            }
            fclose(fp);
        }
    }
    // Compare DRAM_output_sigmoid against DRAM_output_sigmoid.golden.txt if present
    {
        FILE *gf = fopen("DRAM_output_sigmoid.golden.txt", "r");
        if (gf != NULL) {
            static float verif_golden[50176];
            long gn = 0;
            for (int i = 0; i < 50176; i++) {
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
                double a = (double)(float)((data_t*)DRAM_output_sigmoid)[i];
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
        return 0;
    }
    if (verif_n_mismatch == 0) {
        printf("VERIFICATION: PASS (max_abs=%g, max_rel=%g, n=%ld)\n", verif_max_abs, verif_max_rel, verif_n_total);
        return 0;
    }
    printf("VERIFICATION: FAIL (max_abs=%g, max_rel=%g, n_mismatch=%ld/%ld)\n", verif_max_abs, verif_max_rel, verif_n_mismatch, verif_n_total);
    return 1;
}