
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
  float v0[16][14][14],
  float v1[16][14][14]
) {
  loop0: for (int v2 = 0; v2 < 16; v2++) {
    loop1: for (int v3 = 0; v3 < 14; v3++) {
      loop2: for (int v4 = 0; v4 < 14; v4++) {
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
  float v8[16][14][14],
  float v9
) {
  float v10[16][14][14];
  float v11[16][14][14];
  loop3: for (int v12 = 0; v12 < 14; v12++) {
    loop4: for (int v13 = 0; v13 < 14; v13++) {
      loop5: for (int v14 = 0; v14 < 16; v14++) {
        loop6: for (int v15 = 0; v15 < 3; v15++) {
          loop7: for (int v16 = 0; v16 < 3; v16++) {
            loop8: for (int v17 = 0; v17 < 16; v17++) {
              #pragma HLS pipeline II=1
              #pragma HLS loop_flatten
              if ((((-v13) - v16) + 14) >= 0 && ((v13 + v16) - 1) >= 0 && ((v15 + v12) - 1) >= 0 && (((-v15) - v12) + 14) >= 0) {
                if (v14 == 0) {
                  float v18 = v6[v17][((v15 + v12) - 1)][((v16 + v13) - 1)];
                  v11[v17][((v15 + v12) - 1)][((v16 + v13) - 1)] = v18;
                }
                float v19 = v11[v17][((v15 + v12) - 1)][((v16 + v13) - 1)];
                float v20 = v7[v14][v17][v15][v16];
                float v21 = v19 * v20;
                if (v17 == 0) {
                  v10[v14][v12][v13] = v9;
                }
                float v22 = v10[v14][v12][v13];
                float v23 = v22 + v21;
                v10[v14][v12][v13] = v23;
                if ((v17 - 15) == 0) {
                  float v24 = v10[v14][v12][v13];
                  v8[v14][v12][v13] = v24;
                }
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
  float v25[16][14][14],
  float v26[16][14][14]
) {
  loop9: for (int v27 = 0; v27 < 16; v27++) {
    loop10: for (int v28 = 0; v28 < 14; v28++) {
      loop11: for (int v29 = 0; v29 < 14; v29++) {
        #pragma HLS pipeline II=1
        #pragma HLS loop_flatten
        float v30 = v25[v27][v28][v29];
        v26[v27][v28][v29] = v30;
      }
    }
  }
  return ;
}

void node3(
  float v31[16][16][3][3],
  float v32[16][16][3][3]
) {
  loop12: for (int v33 = 0; v33 < 16; v33++) {
    loop13: for (int v34 = 0; v34 < 16; v34++) {
      loop14: for (int v35 = 0; v35 < 3; v35++) {
        loop15: for (int v36 = 0; v36 < 3; v36++) {
          #pragma HLS pipeline II=1
          #pragma HLS loop_flatten
          float v37 = v31[v33][v34][v35][v36];
          v32[v33][v34][v35][v36] = v37;
        }
      }
    }
  }
  return ;
}

void _Z3topPA14_A14_fPA16_A3_A3_fS1_(
  float v38[16][14][14],
  float v39[16][16][3][3],
  float v40[16][14][14]
) {
	#pragma HLS DATAFLOW
  float v41[16][14][14];
  float v42[16][14][14];
  float v43[16][16][3][3];
  node3(v39, v43);
  node2(v38, v42);
  node1(v42, v43, v41, 0.000000);
  node0(v41, v40);
  return ;
}

