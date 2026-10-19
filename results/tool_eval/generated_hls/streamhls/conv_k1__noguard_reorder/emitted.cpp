
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
  hls::stream<float> &v6,
  float v7[16][14][14],
  float v8[16][14][14],
  float v9
) {
  float v10[16][14][14];
  float v11[16][14][14];
  float v12[16][16];
  loop3: for (int v13 = 0; v13 < 14; v13++) {
    loop4: for (int v14 = 0; v14 < 14; v14++) {
      loop5: for (int v15 = 0; v15 < 16; v15++) {
        loop6: for (int v16 = 0; v16 < 16; v16++) {
          #pragma HLS pipeline II=1
          #pragma HLS loop_flatten
          if (v15 == 0) {
            float v17 = v7[v16][v13][v14];
            v11[v16][v13][v14] = v17;
          }
          float v18 = v11[v16][v13][v14];
          if (v13 == 0 && v14 == 0) {
            float v19 = v6.read();
            v12[v15][v16] = v19;
          }
          float v20 = v12[v15][v16];
          float v21 = v18 * v20;
          if (v16 == 0) {
            v10[v15][v13][v14] = v9;
          }
          float v22 = v10[v15][v13][v14];
          float v23 = v22 + v21;
          v10[v15][v13][v14] = v23;
          if ((v16 - 15) == 0) {
            float v24 = v10[v15][v13][v14];
            v8[v15][v13][v14] = v24;
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
  loop7: for (int v27 = 0; v27 < 16; v27++) {
    loop8: for (int v28 = 0; v28 < 14; v28++) {
      loop9: for (int v29 = 0; v29 < 14; v29++) {
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
  float v31[16][16][1][1],
  hls::stream<float> &v32
) {
  loop10: for (int v33 = 0; v33 < 16; v33++) {
    loop11: for (int v34 = 0; v34 < 16; v34++) {
      #pragma HLS pipeline II=1
      #pragma HLS loop_flatten
      float v35 = v31[v33][v34][0][0];
      v32.write(v35);
    }
  }
  return ;
}

void _Z3topPA14_A14_fPA16_A1_A1_fS1_(
  float v36[16][14][14],
  float v37[16][16][1][1],
  float v38[16][14][14]
) {
	#pragma HLS DATAFLOW
  float v39[16][14][14];
  float v40[16][14][14];
  hls::stream<float> v41("fifo_0");
	#pragma HLS STREAM variable=v41 depth=256
  node3(v37, v41);
  node2(v36, v40);
  node1(v41, v40, v39, 0.000000);
  node0(v39, v38);
  return ;
}

