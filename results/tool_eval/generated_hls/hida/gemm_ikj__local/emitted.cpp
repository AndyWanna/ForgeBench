
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
) {	// L2
  for (int v3 = 0; v3 < 64; v3 += 1) {	// L4
    for (int v4 = 0; v4 < 64; v4 += 1) {	// L5
      #pragma HLS pipeline II=1
      v2[v3][v4] = (float)0.000000;	// L6
    }
  }
  for (int v5 = 0; v5 < 64; v5 += 1) {	// L9
    for (int v6 = 0; v6 < 64; v6 += 1) {	// L10
      for (int v7 = 0; v7 < 64; v7 += 1) {	// L11
        #pragma HLS pipeline II=1
        float v8 = v0[v6][v5];	// L12
        float v9 = v1[v5][v7];	// L13
        float v10 = v8 * v9;	// L14
        float v11 = v2[v6][v7];	// L15
        float v12 = v11 + v10;	// L16
        v2[v6][v7] = v12;	// L17
      }
    }
  }
}

/// This is top function.
void _Z3topPA64_fS0_S0_(
  float v13[64][64],
  float v14[64][64],
  float v15[64][64]
) {	// L23
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v15 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v14 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v13 bundle=axi_0

  _Z3topPA64_fS0_S0__node0(v13, v14, v15);	// L30
}

void _Z16load_64_64_floatPA64_fS0__node0(
  float v19[64][64],
  float v20[64][64]
) {	// L40
  #pragma HLS inline
  for (int v21 = 0; v21 < 64; v21 += 1) {	// L41
    for (int v22 = 0; v22 < 64; v22 += 1) {	// L42
      #pragma HLS pipeline II=1
      float v23 = v19[v21][v22];	// L43
      v20[v21][v22] = v23;	// L44
    }
  }
}

void _Z16load_64_64_floatPA64_fS0_(
  float v24[64][64],
  float v25[64][64]
) {	// L49
  _Z16load_64_64_floatPA64_fS0__node0(v24, v25);	// L50
}

void _Z17store_64_64_floatPA64_fS0__node0(
  float v26[64][64],
  float v27[64][64]
) {	// L53
  #pragma HLS inline
  for (int v28 = 0; v28 < 64; v28 += 1) {	// L54
    for (int v29 = 0; v29 < 64; v29 += 1) {	// L55
      #pragma HLS pipeline II=1
      float v30 = v26[v28][v29];	// L56
      v27[v28][v29] = v30;	// L57
    }
  }
}

void _Z17store_64_64_floatPA64_fS0_(
  float v31[64][64],
  float v32[64][64]
) {	// L62
  _Z17store_64_64_floatPA64_fS0__node0(v31, v32);	// L63
}

