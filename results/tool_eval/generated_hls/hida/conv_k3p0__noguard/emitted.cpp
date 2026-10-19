
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
  float v0[16][16][3][3],
  float v1[16][14][14],
  float v2[16][12][12]
) {	// L3
  float v3;	// L5
  for (int v4 = 0; v4 < 12; v4 += 1) {	// L6
    for (int v5 = 0; v5 < 12; v5 += 1) {	// L7
      for (int v6 = 0; v6 < 16; v6 += 1) {	// L8
        #pragma HLS pipeline II=1
        v2[v6][v4][v5] = (float)0.000000;	// L9
      }
    }
  }
  for (int v7 = 0; v7 < 12; v7 += 1) {	// L13
    for (int v8 = 0; v8 < 12; v8 += 1) {	// L14
      for (int v9 = 0; v9 < 3; v9 += 1) {	// L15
        for (int v10 = 0; v10 < 3; v10 += 1) {	// L16
          for (int v11 = 0; v11 < 16; v11 += 1) {	// L17
            for (int v12 = 0; v12 < 16; v12 += 1) {	// L18
              #pragma HLS pipeline II=1
              float v13 = v1[v11][(v7 + v9)][(v8 + v10)];	// L19
              float v14 = v3;	// L20
              float v15 = (v12 == 0) ? v13 : v14;	// L21
              v3 = v15;	// L22
              float v16 = v0[v12][v11][v9][v10];	// L23
              float v17 = v15 * v16;	// L24
              float v18 = v2[v12][v7][v8];	// L25
              float v19 = v18 + v17;	// L26
              v2[v12][v7][v8] = v19;	// L27
            }
          }
        }
      }
    }
  }
}

/// This is top function.
void _Z3topPA14_A14_fPA16_A3_A3_fPA12_A12_f(
  float v20[16][14][14],
  float v21[16][16][3][3],
  float v22[16][12][12]
) {	// L36
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v22 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v21 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v20 bundle=axi_0

  _Z3topPA14_A14_fPA16_A3_A3_fPA12_A12_f_node0(v21, v20, v22);	// L43
}

void _Z19load_16_14_14_floatPA14_A14_fS1__node0(
  float v26[16][14][14],
  float v27[16][14][14]
) {	// L53
  #pragma HLS inline
  for (int v28 = 0; v28 < 16; v28 += 1) {	// L54
    for (int v29 = 0; v29 < 14; v29 += 1) {	// L55
      for (int v30 = 0; v30 < 14; v30 += 1) {	// L56
        #pragma HLS pipeline II=1
        float v31 = v26[v28][v29][v30];	// L57
        v27[v28][v29][v30] = v31;	// L58
      }
    }
  }
}

void _Z19load_16_14_14_floatPA14_A14_fS1_(
  float v32[16][14][14],
  float v33[16][14][14]
) {	// L64
  _Z19load_16_14_14_floatPA14_A14_fS1__node0(v32, v33);	// L65
}

void _Z20load_16_16_3_3_floatPA16_A3_A3_fS2__node0(
  float v34[16][16][3][3],
  float v35[16][16][3][3]
) {	// L68
  #pragma HLS inline
  for (int v36 = 0; v36 < 16; v36 += 1) {	// L69
    for (int v37 = 0; v37 < 16; v37 += 1) {	// L70
      for (int v38 = 0; v38 < 3; v38 += 1) {	// L71
        for (int v39 = 0; v39 < 3; v39 += 1) {	// L72
          #pragma HLS pipeline II=1
          float v40 = v34[v36][v37][v38][v39];	// L73
          v35[v36][v37][v38][v39] = v40;	// L74
        }
      }
    }
  }
}

void _Z20load_16_16_3_3_floatPA16_A3_A3_fS2_(
  float v41[16][16][3][3],
  float v42[16][16][3][3]
) {	// L81
  _Z20load_16_16_3_3_floatPA16_A3_A3_fS2__node0(v41, v42);	// L82
}

void _Z36conv2d_16_16_14_14_12_12_3_0_1_floatPA14_A14_fPA16_A3_A3_fPA12_A12_f_node0(
  float v43[16][14][14],
  float v44[16][16][3][3],
  float v45[16][12][12]
) {	// L85
  for (int v46 = 0; v46 < 12; v46 += 1) {	// L87
    for (int v47 = 0; v47 < 12; v47 += 1) {	// L88
      for (int v48 = 0; v48 < 16; v48 += 1) {	// L89
        #pragma HLS pipeline II=1
        v45[v48][v46][v47] = (float)0.000000;	// L90
      }
    }
  }
  for (int v49 = 0; v49 < 3; v49 += 1) {	// L94
    for (int v50 = 0; v50 < 3; v50 += 1) {	// L95
      for (int v51 = 0; v51 < 16; v51 += 1) {	// L96
        for (int v52 = 0; v52 < 12; v52 += 1) {	// L97
          for (int v53 = 0; v53 < 12; v53 += 1) {	// L98
            for (int v54 = 0; v54 < 16; v54 += 1) {	// L99
              #pragma HLS pipeline II=1
              float v55 = v43[v51][(v52 + v49)][(v53 + v50)];	// L100
              float v56 = v44[v54][v51][v49][v50];	// L101
              float v57 = v55 * v56;	// L102
              float v58 = v45[v54][v52][v53];	// L103
              float v59 = v58 + v57;	// L104
              v45[v54][v52][v53] = v59;	// L105
            }
          }
        }
      }
    }
  }
}

void _Z36conv2d_16_16_14_14_12_12_3_0_1_floatPA14_A14_fPA16_A3_A3_fPA12_A12_f(
  float v60[16][14][14],
  float v61[16][16][3][3],
  float v62[16][12][12]
) {	// L114
  _Z36conv2d_16_16_14_14_12_12_3_0_1_floatPA14_A14_fPA16_A3_A3_fPA12_A12_f_node0(v60, v61, v62);	// L115
}

void _Z20store_16_12_12_floatPA12_A12_fS1__node0(
  float v63[16][12][12],
  float v64[16][12][12]
) {	// L118
  #pragma HLS inline
  for (int v65 = 0; v65 < 16; v65 += 1) {	// L119
    for (int v66 = 0; v66 < 12; v66 += 1) {	// L120
      for (int v67 = 0; v67 < 12; v67 += 1) {	// L121
        #pragma HLS pipeline II=1
        float v68 = v63[v65][v66][v67];	// L122
        v64[v65][v66][v67] = v68;	// L123
      }
    }
  }
}

void _Z20store_16_12_12_floatPA12_A12_fS1_(
  float v69[16][12][12],
  float v70[16][12][12]
) {	// L129
  _Z20store_16_12_12_floatPA12_A12_fS1__node0(v69, v70);	// L130
}

