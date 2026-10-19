
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

void _Z3topPA14_A14_fPA16_A1_A1_fS1__node0(
  float v0[16][14][14],
  float v1[16][16],
  float v2[16][14][14]
) {	// L3
  float v3;	// L5
  for (int v4 = 0; v4 < 14; v4 += 1) {	// L6
    for (int v5 = 0; v5 < 14; v5 += 1) {	// L7
      for (int v6 = 0; v6 < 16; v6 += 1) {	// L8
        #pragma HLS pipeline II=1
        v2[v6][v4][v5] = (float)0.000000;	// L9
      }
    }
  }
  for (int v7 = 0; v7 < 14; v7 += 1) {	// L13
    for (int v8 = 0; v8 < 14; v8 += 1) {	// L14
      for (int v9 = 0; v9 < 16; v9 += 1) {	// L15
        for (int v10 = 0; v10 < 16; v10 += 1) {	// L16
          #pragma HLS pipeline II=1
          float v11 = v0[v9][v7][v8];	// L17
          float v12 = v3;	// L18
          float v13 = (v10 == 0) ? v11 : v12;	// L19
          v3 = v13;	// L20
          float v14 = v1[v10][v9];	// L21
          float v15 = v13 * v14;	// L22
          float v16 = v2[v10][v7][v8];	// L23
          float v17 = v16 + v15;	// L24
          v2[v10][v7][v8] = v17;	// L25
        }
      }
    }
  }
}

/// This is top function.
void _Z3topPA14_A14_fPA16_A1_A1_fS1_(
  float v18[16][14][14],
  float v19[16][16],
  float v20[16][14][14]
) {	// L32
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v20 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v19 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v18 bundle=axi_0

  _Z3topPA14_A14_fPA16_A1_A1_fS1__node0(v18, v19, v20);	// L39
}

void _Z19load_16_14_14_floatPA14_A14_fS1__node0(
  float v24[16][14][14],
  float v25[16][14][14]
) {	// L49
  #pragma HLS inline
  for (int v26 = 0; v26 < 16; v26 += 1) {	// L50
    for (int v27 = 0; v27 < 14; v27 += 1) {	// L51
      for (int v28 = 0; v28 < 14; v28 += 1) {	// L52
        #pragma HLS pipeline II=1
        float v29 = v24[v26][v27][v28];	// L53
        v25[v26][v27][v28] = v29;	// L54
      }
    }
  }
}

void _Z19load_16_14_14_floatPA14_A14_fS1_(
  float v30[16][14][14],
  float v31[16][14][14]
) {	// L60
  _Z19load_16_14_14_floatPA14_A14_fS1__node0(v30, v31);	// L61
}

void _Z20load_16_16_1_1_floatPA16_A1_A1_fS2__node0(
  float v32[16][16],
  float v33[16][16]
) {	// L64
  #pragma HLS inline
  for (int v34 = 0; v34 < 16; v34 += 1) {	// L65
    for (int v35 = 0; v35 < 16; v35 += 1) {	// L66
      #pragma HLS pipeline II=1
      float v36 = v32[v34][v35];	// L67
      v33[v34][v35] = v36;	// L68
    }
  }
}

void _Z20load_16_16_1_1_floatPA16_A1_A1_fS2_(
  float v37[16][16],
  float v38[16][16]
) {	// L73
  _Z20load_16_16_1_1_floatPA16_A1_A1_fS2__node0(v37, v38);	// L74
}

void _Z36conv2d_16_16_14_14_14_14_1_0_1_floatPA14_A14_fPA16_A1_A1_fS1__node0(
  float v39[16][14][14],
  float v40[16][16],
  float v41[16][14][14]
) {	// L77
  for (int v42 = 0; v42 < 14; v42 += 1) {	// L79
    for (int v43 = 0; v43 < 14; v43 += 1) {	// L80
      for (int v44 = 0; v44 < 16; v44 += 1) {	// L81
        #pragma HLS pipeline II=1
        v41[v44][v42][v43] = (float)0.000000;	// L82
      }
    }
  }
  for (int v45 = 0; v45 < 16; v45 += 1) {	// L86
    for (int v46 = 0; v46 < 14; v46 += 1) {	// L87
      for (int v47 = 0; v47 < 14; v47 += 1) {	// L88
        for (int v48 = 0; v48 < 16; v48 += 1) {	// L89
          #pragma HLS pipeline II=1
          float v49 = v39[v45][v46][v47];	// L90
          float v50 = v40[v48][v45];	// L91
          float v51 = v49 * v50;	// L92
          float v52 = v41[v48][v46][v47];	// L93
          float v53 = v52 + v51;	// L94
          v41[v48][v46][v47] = v53;	// L95
        }
      }
    }
  }
}

void _Z36conv2d_16_16_14_14_14_14_1_0_1_floatPA14_A14_fPA16_A1_A1_fS1_(
  float v54[16][14][14],
  float v55[16][16],
  float v56[16][14][14]
) {	// L102
  _Z36conv2d_16_16_14_14_14_14_1_0_1_floatPA14_A14_fPA16_A1_A1_fS1__node0(v54, v55, v56);	// L103
}

void _Z20store_16_14_14_floatPA14_A14_fS1__node0(
  float v57[16][14][14],
  float v58[16][14][14]
) {	// L106
  #pragma HLS inline
  for (int v59 = 0; v59 < 16; v59 += 1) {	// L107
    for (int v60 = 0; v60 < 14; v60 += 1) {	// L108
      for (int v61 = 0; v61 < 14; v61 += 1) {	// L109
        #pragma HLS pipeline II=1
        float v62 = v57[v59][v60][v61];	// L110
        v58[v59][v60][v61] = v62;	// L111
      }
    }
  }
}

void _Z20store_16_14_14_floatPA14_A14_fS1_(
  float v63[16][14][14],
  float v64[16][14][14]
) {	// L117
  _Z20store_16_14_14_floatPA14_A14_fS1__node0(v63, v64);	// L118
}

