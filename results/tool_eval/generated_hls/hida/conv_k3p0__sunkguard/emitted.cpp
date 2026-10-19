
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

void _Z3topPA14_A14_fPA16_A3_A3_fPA12_A12_f_node0(
  float v0[16][14][14],
  float v1[16][16][3][3],
  float v2[16][12][12]
) {	// L3
  for (int v3 = 0; v3 < 12; v3 += 1) {	// L5
    for (int v4 = 0; v4 < 12; v4 += 1) {	// L6
      for (int v5 = 0; v5 < 16; v5 += 1) {	// L7
        #pragma HLS pipeline II=1
        v2[v5][v3][v4] = (float)0.000000;	// L8
      }
    }
  }
  for (int v6 = 0; v6 < 3; v6 += 1) {	// L12
    for (int v7 = 0; v7 < 3; v7 += 1) {	// L13
      for (int v8 = 0; v8 < 16; v8 += 1) {	// L14
        for (int v9 = 0; v9 < 12; v9 += 1) {	// L15
          for (int v10 = 0; v10 < 12; v10 += 1) {	// L16
            for (int v11 = 0; v11 < 16; v11 += 1) {	// L17
              #pragma HLS pipeline II=1
              if ((((-v10) - v7) + 13) >= 0 && (v10 + v7) >= 0 && (v9 + v6) >= 0 && (((-v9) - v6) + 13) >= 0) {	// L18
                float v12 = v0[v8][(v9 + v6)][(v10 + v7)];	// L19
                float v13 = v1[v11][v8][v6][v7];	// L20
                float v14 = v12 * v13;	// L21
                float v15 = v2[v11][v9][v10];	// L22
                float v16 = v15 + v14;	// L23
                v2[v11][v9][v10] = v16;	// L24
              }
            }
          }
        }
      }
    }
  }
}

/// This is top function.
void _Z3topPA14_A14_fPA16_A3_A3_fPA12_A12_f(
  float v17[16][14][14],
  float v18[16][16][3][3],
  float v19[16][12][12]
) {	// L34
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v19 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v18 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v17 bundle=axi_0

  _Z3topPA14_A14_fPA16_A3_A3_fPA12_A12_f_node0(v17, v18, v19);	// L41
}

void _Z19load_16_14_14_floatPA14_A14_fS1__node0(
  float v23[16][14][14],
  float v24[16][14][14]
) {	// L51
  #pragma HLS inline
  for (int v25 = 0; v25 < 16; v25 += 1) {	// L52
    for (int v26 = 0; v26 < 14; v26 += 1) {	// L53
      for (int v27 = 0; v27 < 14; v27 += 1) {	// L54
        #pragma HLS pipeline II=1
        float v28 = v23[v25][v26][v27];	// L55
        v24[v25][v26][v27] = v28;	// L56
      }
    }
  }
}

void _Z19load_16_14_14_floatPA14_A14_fS1_(
  float v29[16][14][14],
  float v30[16][14][14]
) {	// L62
  _Z19load_16_14_14_floatPA14_A14_fS1__node0(v29, v30);	// L63
}

void _Z20load_16_16_3_3_floatPA16_A3_A3_fS2__node0(
  float v31[16][16][3][3],
  float v32[16][16][3][3]
) {	// L66
  #pragma HLS inline
  for (int v33 = 0; v33 < 16; v33 += 1) {	// L67
    for (int v34 = 0; v34 < 16; v34 += 1) {	// L68
      for (int v35 = 0; v35 < 3; v35 += 1) {	// L69
        for (int v36 = 0; v36 < 3; v36 += 1) {	// L70
          #pragma HLS pipeline II=1
          float v37 = v31[v33][v34][v35][v36];	// L71
          v32[v33][v34][v35][v36] = v37;	// L72
        }
      }
    }
  }
}

void _Z20load_16_16_3_3_floatPA16_A3_A3_fS2_(
  float v38[16][16][3][3],
  float v39[16][16][3][3]
) {	// L79
  _Z20load_16_16_3_3_floatPA16_A3_A3_fS2__node0(v38, v39);	// L80
}

void _Z36conv2d_16_16_14_14_12_12_3_0_1_floatPA14_A14_fPA16_A3_A3_fPA12_A12_f_node0(
  float v40[16][14][14],
  float v41[16][16][3][3],
  float v42[16][12][12]
) {	// L83
  for (int v43 = 0; v43 < 12; v43 += 1) {	// L85
    for (int v44 = 0; v44 < 12; v44 += 1) {	// L86
      for (int v45 = 0; v45 < 16; v45 += 1) {	// L87
        #pragma HLS pipeline II=1
        v42[v45][v43][v44] = (float)0.000000;	// L88
      }
    }
  }
  for (int v46 = 0; v46 < 3; v46 += 1) {	// L92
    for (int v47 = 0; v47 < 3; v47 += 1) {	// L93
      for (int v48 = 0; v48 < 16; v48 += 1) {	// L94
        for (int v49 = 0; v49 < 12; v49 += 1) {	// L95
          for (int v50 = 0; v50 < 12; v50 += 1) {	// L96
            for (int v51 = 0; v51 < 16; v51 += 1) {	// L97
              #pragma HLS pipeline II=1
              if ((((-v50) - v47) + 13) >= 0 && (v50 + v47) >= 0 && (v49 + v46) >= 0 && (((-v49) - v46) + 13) >= 0) {	// L98
                float v52 = v40[v48][(v49 + v46)][(v50 + v47)];	// L99
                float v53 = v41[v51][v48][v46][v47];	// L100
                float v54 = v52 * v53;	// L101
                float v55 = v42[v51][v49][v50];	// L102
                float v56 = v55 + v54;	// L103
                v42[v51][v49][v50] = v56;	// L104
              }
            }
          }
        }
      }
    }
  }
}

void _Z36conv2d_16_16_14_14_12_12_3_0_1_floatPA14_A14_fPA16_A3_A3_fPA12_A12_f(
  float v57[16][14][14],
  float v58[16][16][3][3],
  float v59[16][12][12]
) {	// L114
  _Z36conv2d_16_16_14_14_12_12_3_0_1_floatPA14_A14_fPA16_A3_A3_fPA12_A12_f_node0(v57, v58, v59);	// L115
}

void _Z20store_16_12_12_floatPA12_A12_fS1__node0(
  float v60[16][12][12],
  float v61[16][12][12]
) {	// L118
  #pragma HLS inline
  for (int v62 = 0; v62 < 16; v62 += 1) {	// L119
    for (int v63 = 0; v63 < 12; v63 += 1) {	// L120
      for (int v64 = 0; v64 < 12; v64 += 1) {	// L121
        #pragma HLS pipeline II=1
        float v65 = v60[v62][v63][v64];	// L122
        v61[v62][v63][v64] = v65;	// L123
      }
    }
  }
}

void _Z20store_16_12_12_floatPA12_A12_fS1_(
  float v66[16][12][12],
  float v67[16][12][12]
) {	// L129
  _Z20store_16_12_12_floatPA12_A12_fS1__node0(v66, v67);	// L130
}

