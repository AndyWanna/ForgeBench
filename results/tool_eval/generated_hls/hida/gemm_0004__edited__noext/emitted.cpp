
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

void _Z3topPfPA64_fS1_S_S_S__node0(
  float v0,
  float v1
) {	// L3
  float v2 = v0;	// L4
  v1 = v2;	// L5
}

void _Z3topPfPA64_fS1_S_S_S__node1(
  float v3[64],
  float v4[64],
  float v5
) {	// L8
  #pragma HLS inline
  for (int v6 = 0; v6 < 64; v6 += 1) {	// L10
    #pragma HLS pipeline II=1
    float v7 = v5;	// L11
    float v8 = (v6 == 0) ? (float)0.000000 : v7;	// L12
    float v9 = v3[v6];	// L13
    float v10 = v4[v6];	// L14
    float v11 = v9 * v10;	// L15
    float v12 = v8 + v11;	// L16
    v5 = v12;	// L17
  }
}

void _Z3topPfPA64_fS1_S_S_S__node2(
  float v13[64][64],
  float v14[64],
  float v15[64],
  float v16[64]
) {	// L21
  #pragma HLS inline
  for (int v17 = 0; v17 < 64; v17 += 1) {	// L22
    for (int v18 = 0; v18 < 64; v18 += 1) {	// L23
      #pragma HLS pipeline II=1
      float v19 = v13[v17][v18];	// L24
      float v20 = v14[v17];	// L25
      float v21 = v19 * v20;	// L26
      float v22 = v15[v18];	// L27
      float v23 = v16[v18];	// L28
      float v24 = (v17 == 0) ? v22 : v23;	// L29
      float v25 = v24 + v21;	// L30
      v16[v18] = v25;	// L31
    }
  }
}

void _Z3topPfPA64_fS1_S_S_S__node3(
  float v26[64]
) {	// L36
  #pragma HLS inline
  for (int v27 = 0; v27 < 64; v27 += 1) {	// L38
    #pragma HLS pipeline II=1
    v26[v27] = (float)0.000000;	// L39
  }
}

void _Z3topPfPA64_fS1_S_S_S__node4(
  float v28[64][64],
  float v29[64],
  float v30[64],
  float v31[64]
) {	// L43
  #pragma HLS inline
  for (int v32 = 0; v32 < 64; v32 += 1) {	// L44
    for (int v33 = 0; v33 < 64; v33 += 1) {	// L45
      #pragma HLS pipeline II=1
      float v34 = v28[v32][v33];	// L46
      float v35 = v29[v32];	// L47
      float v36 = v34 * v35;	// L48
      float v37 = v30[v33];	// L49
      float v38 = v31[v33];	// L50
      float v39 = (v32 == 0) ? v37 : v38;	// L51
      float v40 = v39 + v36;	// L52
      v31[v33] = v40;	// L53
    }
  }
}

void _Z3topPfPA64_fS1_S_S_S__node5(
  float v41[64]
) {	// L58
  #pragma HLS inline
  for (int v42 = 0; v42 < 64; v42 += 1) {	// L60
    #pragma HLS pipeline II=1
    v41[v42] = (float)0.000000;	// L61
  }
}

/// This is top function.
void _Z3topPfPA64_fS1_S_S_S_(
  float v43[64],
  float v44[64][64],
  float v45[64][64],
  float v46[64],
  float v47
) {	// L65
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v47 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v46 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v45 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v44 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v43 bundle=axi_0

  float v53;	// L76
  float v54[64];	// L77
  #pragma HLS bind_storage variable=v54 type=ram_t2p impl=bram

  float v55[64];	// L78
  #pragma HLS bind_storage variable=v55 type=ram_t2p impl=bram

  _Z3topPfPA64_fS1_S_S_S__node5(v54);	// L79
  float v56[64];	// L80
  #pragma HLS bind_storage variable=v56 type=ram_t2p impl=bram

  _Z3topPfPA64_fS1_S_S_S__node4(v44, v43, v54, v56);	// L81
  _Z3topPfPA64_fS1_S_S_S__node3(v55);	// L82
  float v57[64];	// L83
  #pragma HLS bind_storage variable=v57 type=ram_t2p impl=bram

  _Z3topPfPA64_fS1_S_S_S__node2(v45, v56, v55, v57);	// L84
  _Z3topPfPA64_fS1_S_S_S__node1(v57, v46, v53);	// L85
  _Z3topPfPA64_fS1_S_S_S__node0(v53, v47);	// L86
}

void _Z13load_64_floatPfS__node0(
  float v58[64],
  float v59[64]
) {	// L98
  #pragma HLS inline
  for (int v60 = 0; v60 < 64; v60 += 1) {	// L99
    #pragma HLS pipeline II=1
    float v61 = v58[v60];	// L100
    v59[v60] = v61;	// L101
  }
}

void _Z13load_64_floatPfS_(
  float v62[64],
  float v63[64]
) {	// L105
  _Z13load_64_floatPfS__node0(v62, v63);	// L106
}

void _Z16load_64_64_floatPA64_fS0__node0(
  float v64[64][64],
  float v65[64][64]
) {	// L109
  #pragma HLS inline
  for (int v66 = 0; v66 < 64; v66 += 1) {	// L110
    for (int v67 = 0; v67 < 64; v67 += 1) {	// L111
      #pragma HLS pipeline II=1
      float v68 = v64[v66][v67];	// L112
      v65[v66][v67] = v68;	// L113
    }
  }
}

void _Z16load_64_64_floatPA64_fS0_(
  float v69[64][64],
  float v70[64][64]
) {	// L118
  _Z16load_64_64_floatPA64_fS0__node0(v69, v70);	// L119
}

void _Z12load_1_floatPfS_(
  float v71,
  float v72
) {	// L122
  float v73 = v71;	// L123
  v72 = v73;	// L124
}

void _Z13store_1_floatPfS_(
  float v74,
  float v75
) {	// L127
  float v76 = v74;	// L128
  v75 = v76;	// L129
}

