
//===------------------------------------------------------------*- C++ -*-===//
//
// Automatically generated file for High-level Synthesis (HLS).
//
//===----------------------------------------------------------------------===//
#include <hls_stream.h>
#include <hls_half.h>
#include <cassert>
#include <hls_math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

using namespace std;

void node0(
  float v0[16][12][12],
  float v1[16][12][12]
) {
  loop0: for (int v2 = 0; v2 < 16; v2++) {
    loop1: for (int v3 = 0; v3 < 12; v3++) {
      loop2: for (int v4 = 0; v4 < 12; v4++) {
        #pragma HLS pipeline II=1
        #pragma HLS loop_flatten
        float v5 = v0[v2][v3][v4];
        v1[v2][v3][v4] = v5;
      }
    }
  }
  return ;
}

void node1(
  float v6[16][14][14],
  float v7[16][16][3][3],
  float v8[16][12][12],
  float v9
) {
  float v10[16][12][12];
  float v11[16][14][14];
  float v12[16][16][3][3];
  loop3: for (int v13 = 0; v13 < 12; v13++) {
    loop4: for (int v14 = 0; v14 < 12; v14++) {
      loop5: for (int v15 = 0; v15 < 16; v15++) {
        loop6: for (int v16 = 0; v16 < 3; v16++) {
          loop7: for (int v17 = 0; v17 < 3; v17++) {
            loop8: for (int v18 = 0; v18 < 16; v18++) {
              #pragma HLS pipeline II=1
              #pragma HLS loop_flatten
              if (v15 == 0) {
                float v19 = v6[v18][(v13 + v16)][(v14 + v17)];
                v11[v18][(v13 + v16)][(v14 + v17)] = v19;
              }
              float v20 = v11[v18][(v13 + v16)][(v14 + v17)];
              if (v13 == 0 && v14 == 0) {
                float v21 = v7[v15][v18][v16][v17];
                v12[v15][v18][v16][v17] = v21;
              }
              float v22 = v12[v15][v18][v16][v17];
              float v23 = v20 * v22;
              if (v16 == 0 && v17 == 0 && v18 == 0) {
                v10[v15][v13][v14] = v9;
              }
              float v24 = v10[v15][v13][v14];
              float v25 = v24 + v23;
              v10[v15][v13][v14] = v25;
              if ((v16 - 2) == 0 && (v17 - 2) == 0 && (v18 - 15) == 0) {
                float v26 = v10[v15][v13][v14];
                v8[v15][v13][v14] = v26;
              }
            }
          }
        }
      }
    }
  }
  return ;
}

void node2(
  float v27[16][14][14],
  float v28[16][14][14]
) {
  loop9: for (int v29 = 0; v29 < 16; v29++) {
    loop10: for (int v30 = 0; v30 < 14; v30++) {
      loop11: for (int v31 = 0; v31 < 14; v31++) {
        #pragma HLS pipeline II=1
        #pragma HLS loop_flatten
        float v32 = v27[v29][v30][v31];
        v28[v29][v30][v31] = v32;
      }
    }
  }
  return ;
}

void node3(
  float v33[16][16][3][3],
  float v34[16][16][3][3]
) {
  loop12: for (int v35 = 0; v35 < 16; v35++) {
    loop13: for (int v36 = 0; v36 < 16; v36++) {
      loop14: for (int v37 = 0; v37 < 3; v37++) {
        loop15: for (int v38 = 0; v38 < 3; v38++) {
          #pragma HLS pipeline II=1
          #pragma HLS loop_flatten
          float v39 = v33[v35][v36][v37][v38];
          v34[v35][v36][v37][v38] = v39;
        }
      }
    }
  }
  return ;
}

void _Z3topPA14_A14_fPA16_A3_A3_fPA12_A12_f(
  float v40[16][14][14],
  float v41[16][16][3][3],
  float v42[16][12][12]
) {
	#pragma HLS DATAFLOW
  float v43[16][12][12];
  float v44[16][14][14];
  float v45[16][16][3][3];
  node3(v41, v45);
  node2(v40, v44);
  node1(v44, v45, v43, 0.000000);
  node0(v43, v42);
  return ;
}

