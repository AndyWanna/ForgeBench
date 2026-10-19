
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
  float v7[16][16],
  float v8[16][14][14],
  float v9
) {
  float v10[16][14][14];
  float v11[16][14][14];
  loop3: for (int v12 = 0; v12 < 14; v12++) {
    loop4: for (int v13 = 0; v13 < 14; v13++) {
      loop5: for (int v14 = 0; v14 < 16; v14++) {
        loop6: for (int v15 = 0; v15 < 16; v15++) {
          #pragma HLS pipeline II=1
          #pragma HLS loop_flatten
          if (((-v13) + 13) >= 0 && v13 >= 0 && v12 >= 0 && ((-v12) + 13) >= 0) {
            if (v15 == 0) {
              float v16 = v6[v14][v12][v13];
              v11[v14][v12][v13] = v16;
            }
            float v17 = v11[v14][v12][v13];
            float v18 = v7[v15][v14];
            float v19 = v17 * v18;
            if (v14 == 0) {
              v10[v15][v12][v13] = v9;
            }
            float v20 = v10[v15][v12][v13];
            float v21 = v20 + v19;
            v10[v15][v12][v13] = v21;
            if ((v14 - 15) == 0) {
              float v22 = v10[v15][v12][v13];
              v8[v15][v12][v13] = v22;
            }
          }
        }
      }
    }
  }
  return ;
}

void node2(
  float v23[16][14][14],
  float v24[16][14][14]
) {
  loop7: for (int v25 = 0; v25 < 16; v25++) {
    loop8: for (int v26 = 0; v26 < 14; v26++) {
      loop9: for (int v27 = 0; v27 < 14; v27++) {
        #pragma HLS pipeline II=1
        #pragma HLS loop_flatten
        float v28 = v23[v25][v26][v27];
        v24[v25][v26][v27] = v28;
      }
    }
  }
  return ;
}

void node3(
  float v29[16][16][1][1],
  float v30[16][16]
) {
  loop10: for (int v31 = 0; v31 < 16; v31++) {
    loop11: for (int v32 = 0; v32 < 16; v32++) {
      #pragma HLS pipeline II=1
      #pragma HLS loop_flatten
      float v33 = v29[v31][v32][0][0];
      v30[v31][v32] = v33;
    }
  }
  return ;
}

void _Z3topPA14_A14_fPA16_A1_A1_fS1_(
  float v34[16][14][14],
  float v35[16][16][1][1],
  float v36[16][14][14]
) {
	#pragma HLS DATAFLOW
  float v37[16][14][14];
  float v38[16][14][14];
  float v39[16][16];
  node3(v35, v39);
  node2(v34, v38);
  node1(v38, v39, v37, 0.000000);
  node0(v37, v36);
  return ;
}

