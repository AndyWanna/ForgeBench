
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
  float v0[64][64],
  float v1[64][64]
) {
  loop0: for (int v2 = 0; v2 < 64; v2++) {
    loop1: for (int v3 = 0; v3 < 64; v3++) {
      #pragma HLS pipeline II=1
      #pragma HLS loop_flatten
      float v4 = v0[v2][v3];
      v1[v2][v3] = v4;
    }
  }
  return ;
}

void node1(
  float v5[64][64],
  hls::stream<float> &v6,
  float v7[64][64],
  float v8
) {
  float v9[64][64];
  float v10[64][64];
  float v11[64][64];
  loop2: for (int v12 = 0; v12 < 64; v12++) {
    loop3: for (int v13 = 0; v13 < 64; v13++) {
      loop4: for (int v14 = 0; v14 < 64; v14++) {
        #pragma HLS pipeline II=1
        #pragma HLS loop_flatten
        if (v12 == 0) {
          float v15 = v6.read();
          v10[v13][v14] = v15;
        }
        float v16 = v10[v13][v14];
        if (v13 == 0) {
          float v17 = v5[v14][v12];
          v11[v14][v12] = v17;
        }
        float v18 = v11[v14][v12];
        float v19 = v16 * v18;
        if (v14 == 0) {
          v9[v13][v12] = v8;
        }
        float v20 = v9[v13][v12];
        float v21 = v20 + v19;
        v9[v13][v12] = v21;
        if ((v14 - 63) == 0) {
          float v22 = v9[v13][v12];
          v7[v13][v12] = v22;
        }
      }
    }
  }
  return ;
}

void node2(
  float v23[64][64],
  hls::stream<float> &v24
) {
  loop5: for (int v25 = 0; v25 < 64; v25++) {
    loop6: for (int v26 = 0; v26 < 64; v26++) {
      #pragma HLS pipeline II=1
      #pragma HLS loop_flatten
      float v27 = v23[v25][v26];
      v24.write(v27);
    }
  }
  return ;
}

void node3(
  float v28[64][64],
  float v29[64][64]
) {
  loop7: for (int v30 = 0; v30 < 64; v30++) {
    loop8: for (int v31 = 0; v31 < 64; v31++) {
      #pragma HLS pipeline II=1
      #pragma HLS loop_flatten
      float v32 = v28[v30][v31];
      v29[v30][v31] = v32;
    }
  }
  return ;
}

void _Z3topPA64_fS0_S0_(
  float v33[64][64],
  float v34[64][64],
  float v35[64][64]
) {
	#pragma HLS DATAFLOW
  float v36[64][64];
  hls::stream<float> v37("fifo_0");
	#pragma HLS STREAM variable=v37 depth=4096
  float v38[64][64];
  node3(v34, v38);
  node2(v33, v37);
  node1(v38, v37, v36, 0.000000);
  node0(v36, v35);
  return ;
}

