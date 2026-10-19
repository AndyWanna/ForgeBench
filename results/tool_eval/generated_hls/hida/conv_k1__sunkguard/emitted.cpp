
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
  for (int v3 = 0; v3 < 14; v3 += 1) {	// L5
    for (int v4 = 0; v4 < 14; v4 += 1) {	// L6
      for (int v5 = 0; v5 < 16; v5 += 1) {	// L7
        #pragma HLS pipeline II=1
        v2[v5][v3][v4] = (float)0.000000;	// L8
      }
    }
  }
  for (int v6 = 0; v6 < 16; v6 += 1) {	// L12
    for (int v7 = 0; v7 < 14; v7 += 1) {	// L13
      for (int v8 = 0; v8 < 14; v8 += 1) {	// L14
        for (int v9 = 0; v9 < 16; v9 += 1) {	// L15
          #pragma HLS pipeline II=1
          if (((-v8) + 13) >= 0 && v8 >= 0 && v7 >= 0 && ((-v7) + 13) >= 0) {	// L16
            float v10 = v0[v6][v7][v8];	// L17
            float v11 = v1[v9][v6];	// L18
            float v12 = v10 * v11;	// L19
            float v13 = v2[v9][v7][v8];	// L20
            float v14 = v13 + v12;	// L21
            v2[v9][v7][v8] = v14;	// L22
          }
        }
      }
    }
  }
}

/// This is top function.
void _Z3topPA14_A14_fPA16_A1_A1_fS1_(
  float v15[16][14][14],
  float v16[16][16],
  float v17[16][14][14]
) {	// L30
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v17 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v16 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v15 bundle=axi_0

  _Z3topPA14_A14_fPA16_A1_A1_fS1__node0(v15, v16, v17);	// L37
}

void _Z19load_16_14_14_floatPA14_A14_fS1__node0(
  float v21[16][14][14],
  float v22[16][14][14]
) {	// L47
  #pragma HLS inline
  for (int v23 = 0; v23 < 16; v23 += 1) {	// L48
    for (int v24 = 0; v24 < 14; v24 += 1) {	// L49
      for (int v25 = 0; v25 < 14; v25 += 1) {	// L50
        #pragma HLS pipeline II=1
        float v26 = v21[v23][v24][v25];	// L51
        v22[v23][v24][v25] = v26;	// L52
      }
    }
  }
}

void _Z19load_16_14_14_floatPA14_A14_fS1_(
  float v27[16][14][14],
  float v28[16][14][14]
) {	// L58
  _Z19load_16_14_14_floatPA14_A14_fS1__node0(v27, v28);	// L59
}

void _Z20load_16_16_1_1_floatPA16_A1_A1_fS2__node0(
  float v29[16][16],
  float v30[16][16]
) {	// L62
  #pragma HLS inline
  for (int v31 = 0; v31 < 16; v31 += 1) {	// L63
    for (int v32 = 0; v32 < 16; v32 += 1) {	// L64
      #pragma HLS pipeline II=1
      float v33 = v29[v31][v32];	// L65
      v30[v31][v32] = v33;	// L66
    }
  }
}

void _Z20load_16_16_1_1_floatPA16_A1_A1_fS2_(
  float v34[16][16],
  float v35[16][16]
) {	// L71
  _Z20load_16_16_1_1_floatPA16_A1_A1_fS2__node0(v34, v35);	// L72
}

void _Z36conv2d_16_16_14_14_14_14_1_0_1_floatPA14_A14_fPA16_A1_A1_fS1__node0(
  float v36[16][14][14],
  float v37[16][16],
  float v38[16][14][14]
) {	// L75
  for (int v39 = 0; v39 < 14; v39 += 1) {	// L77
    for (int v40 = 0; v40 < 14; v40 += 1) {	// L78
      for (int v41 = 0; v41 < 16; v41 += 1) {	// L79
        #pragma HLS pipeline II=1
        v38[v41][v39][v40] = (float)0.000000;	// L80
      }
    }
  }
  for (int v42 = 0; v42 < 16; v42 += 1) {	// L84
    for (int v43 = 0; v43 < 14; v43 += 1) {	// L85
      for (int v44 = 0; v44 < 14; v44 += 1) {	// L86
        for (int v45 = 0; v45 < 16; v45 += 1) {	// L87
          #pragma HLS pipeline II=1
          if (((-v44) + 13) >= 0 && v44 >= 0 && v43 >= 0 && ((-v43) + 13) >= 0) {	// L88
            float v46 = v36[v42][v43][v44];	// L89
            float v47 = v37[v45][v42];	// L90
            float v48 = v46 * v47;	// L91
            float v49 = v38[v45][v43][v44];	// L92
            float v50 = v49 + v48;	// L93
            v38[v45][v43][v44] = v50;	// L94
          }
        }
      }
    }
  }
}

void _Z36conv2d_16_16_14_14_14_14_1_0_1_floatPA14_A14_fPA16_A1_A1_fS1_(
  float v51[16][14][14],
  float v52[16][16],
  float v53[16][14][14]
) {	// L102
  _Z36conv2d_16_16_14_14_14_14_1_0_1_floatPA14_A14_fPA16_A1_A1_fS1__node0(v51, v52, v53);	// L103
}

void _Z20store_16_14_14_floatPA14_A14_fS1__node0(
  float v54[16][14][14],
  float v55[16][14][14]
) {	// L106
  #pragma HLS inline
  for (int v56 = 0; v56 < 16; v56 += 1) {	// L107
    for (int v57 = 0; v57 < 14; v57 += 1) {	// L108
      for (int v58 = 0; v58 < 14; v58 += 1) {	// L109
        #pragma HLS pipeline II=1
        float v59 = v54[v56][v57][v58];	// L110
        v55[v56][v57][v58] = v59;	// L111
      }
    }
  }
}

void _Z20store_16_14_14_floatPA14_A14_fS1_(
  float v60[16][14][14],
  float v61[16][14][14]
) {	// L117
  _Z20store_16_14_14_floatPA14_A14_fS1__node0(v60, v61);	// L118
}

