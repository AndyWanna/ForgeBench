#include "stdio.h"
#include "stdlib.h"
#include "math.h"

typedef float data_t;
//#define O 512
#define O 64
#define I 64
//#define R 60
#define R 14
#define C 14
#define K 3

data_t cin[R + K - 1][C + K - 1][I];
data_t w[O][K][K][I];
data_t cout[R][C][O];
data_t bias[O];
data_t cout_golden[R][C][O];

int main(int argc, char **argv){
  // declarations
//  data_t cin[I][R + K - 1][C + K - 1];
//  data_t w[O][I][K][K];
//  data_t cout[O][R][C];
//  data_t cout_golden[O][R][C];

// data initialization
for (int i = 0 ; i < I; i++)
    for (int r = 0; r < R + K - 1; r++)
        for (int c = 0; c < C + K - 1; c++) {
        cin[r][c][i] = 1;
        }

for (int o = 0; o < O; o++)
    for (int i = 0; i < I; i++) 
        for (int p = 0; p < K; p++)
        for (int q = 0; q < K; q++) {
            w[o][p][q][i] = 1;
        }
#pragma scop
for (int o = 0; o < O; o++)
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++){
            cout[r][c][o] = bias[o];
        }
 

for (int o = 0; o < O; o++)
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++) {
        //cout[r][c][o] = 0;
        for (int i = 0; i < I; i++)
            for (int p = 0; p < 3; p++)
            for (int q = 0; q < 3; q++) {
                cout[r][c][o] = cout[r][c][o] + cin[r + p][c + q][i] * w[o][p][q][i];
            }
        }
#pragma endscop  
 
//   for (int o = 0; o < O; o++)
//     for (int r = 0; r < R; r++)
//       for (int c = 0; c < C; c++) {
//         cout_golden[r][c][o] = 0;
//         for (int i = 0; i < I; i++)
//           for (int p = 0; p < 3; p++)
//             for (int q = 0; q < 3; q++) {
//               cout_golden[r][c][o] = cout_golden[r][c][o] + cin[r + p][c + q][i] * w[o][p][q][i];
//             }
//       }

int err = 0;
float thres = 0.001;
for (int o = 0; o < O; o++)
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++) {
        if (fabs((float)cout_golden[r][c][o] - (float)cout[r][c][o]) > thres) {
            err++;
        }
    }

//   if (err) {
//     printf("Test failed with %d errors!\n", err);
//     return -1;
//   } else {
//     printf("Test passed!\n");
//     return 0;
//   }
}