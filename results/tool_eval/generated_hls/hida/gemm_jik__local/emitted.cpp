
//===------------------------------------------------------------*- C++ -*-===//
//
// Automatically generated file for High-level Synthesis (HLS).
//
//===----------------------------------------------------------------------===//

#include <algorithm>
#include <ap_axi_sdata.h>
#include <ap_fixed.h>
#include <ap_int.h>
#include <hls_math.h>
#include <hls_stream.h>
#include <hls_vector.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

using namespace std;

void _Z3topPA64_fS0_S0__node0(
  float v0[64][64],
  float v1[64][64],
  float v2[64][64]
) {	// L3
  float v3;	// L5
  for (int v4 = 0; v4 < 64; v4 += 1) {	// L6
    for (int v5 = 0; v5 < 64; v5 += 1) {	// L7
      #pragma HLS pipeline II=1
      v2[v4][v5] = (float)0.000000;	// L8
    }
  }
  for (int v6 = 0; v6 < 64; v6 += 1) {	// L11
    for (int v7 = 0; v7 < 64; v7 += 1) {	// L12
      for (int v8 = 0; v8 < 64; v8 += 1) {	// L13
        #pragma HLS pipeline II=1
        float v9 = v0[v7][v6];	// L14
        float v10 = v3;	// L15
        float v11 = (v8 == 0) ? v9 : v10;	// L16
        v3 = v11;	// L17
        float v12 = v1[v6][v8];	// L18
        float v13 = v11 * v12;	// L19
        float v14 = v2[v7][v8];	// L20
        float v15 = v14 + v13;	// L21
        v2[v7][v8] = v15;	// L22
      }
    }
  }
}

/// This is top function.
void _Z3topPA64_fS0_S0_(
  float v16[64][64],
  float v17[64][64],
  float v18[64][64]
) {	// L28
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v18 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v17 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v16 bundle=axi_0

  _Z3topPA64_fS0_S0__node0(v16, v17, v18);	// L35
}

void _Z16load_64_64_floatPA64_fS0__node0(
  float v22[64][64],
  float v23[64][64]
) {	// L45
  #pragma HLS inline
  for (int v24 = 0; v24 < 64; v24 += 1) {	// L46
    for (int v25 = 0; v25 < 64; v25 += 1) {	// L47
      #pragma HLS pipeline II=1
      float v26 = v22[v24][v25];	// L48
      v23[v24][v25] = v26;	// L49
    }
  }
}

void _Z16load_64_64_floatPA64_fS0_(
  float v27[64][64],
  float v28[64][64]
) {	// L54
  _Z16load_64_64_floatPA64_fS0__node0(v27, v28);	// L55
}

void _Z17store_64_64_floatPA64_fS0__node0(
  float v29[64][64],
  float v30[64][64]
) {	// L58
  #pragma HLS inline
  for (int v31 = 0; v31 < 64; v31 += 1) {	// L59
    for (int v32 = 0; v32 < 64; v32 += 1) {	// L60
      #pragma HLS pipeline II=1
      float v33 = v29[v31][v32];	// L61
      v30[v31][v32] = v33;	// L62
    }
  }
}

void _Z17store_64_64_floatPA64_fS0_(
  float v34[64][64],
  float v35[64][64]
) {	// L67
  _Z17store_64_64_floatPA64_fS0__node0(v34, v35);	// L68
}

