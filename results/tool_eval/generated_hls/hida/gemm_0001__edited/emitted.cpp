
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
  float v13[64],
  float v14[64][64],
  float v15[64],
  float v16[64]
) {	// L21
  #pragma HLS inline
  float v17;	// L22
  for (int v18 = 0; v18 < 64; v18 += 1) {	// L23
    for (int v19 = 0; v19 < 64; v19 += 1) {	// L24
      #pragma HLS pipeline II=1
      float v20 = v13[v18];	// L25
      float v21 = v17;	// L26
      float v22 = (v19 == 0) ? v20 : v21;	// L27
      v17 = v22;	// L28
      float v23 = v14[v18][v19];	// L29
      float v24 = v23 * v22;	// L30
      float v25 = v15[v19];	// L31
      float v26 = v16[v19];	// L32
      float v27 = (v18 == 0) ? v25 : v26;	// L33
      float v28 = v27 + v24;	// L34
      v16[v19] = v28;	// L35
    }
  }
}

void _Z3topPfPA64_fS1_S_S_S__node3(
  float v29[64]
) {	// L40
  #pragma HLS inline
  for (int v30 = 0; v30 < 64; v30 += 1) {	// L42
    #pragma HLS pipeline II=1
    v29[v30] = (float)0.000000;	// L43
  }
}

void _Z3topPfPA64_fS1_S_S_S__node4(
  float v31[64],
  float v32[64][64],
  float v33[64],
  float v34[64]
) {	// L47
  #pragma HLS inline
  float v35;	// L48
  for (int v36 = 0; v36 < 64; v36 += 1) {	// L49
    for (int v37 = 0; v37 < 64; v37 += 1) {	// L50
      #pragma HLS pipeline II=1
      float v38 = v31[v36];	// L51
      float v39 = v35;	// L52
      float v40 = (v37 == 0) ? v38 : v39;	// L53
      v35 = v40;	// L54
      float v41 = v32[v36][v37];	// L55
      float v42 = v41 * v40;	// L56
      float v43 = v33[v37];	// L57
      float v44 = v34[v37];	// L58
      float v45 = (v36 == 0) ? v43 : v44;	// L59
      float v46 = v45 + v42;	// L60
      v34[v37] = v46;	// L61
    }
  }
}

void _Z3topPfPA64_fS1_S_S_S__node5(
  float v47[64]
) {	// L66
  #pragma HLS inline
  for (int v48 = 0; v48 < 64; v48 += 1) {	// L68
    #pragma HLS pipeline II=1
    v47[v48] = (float)0.000000;	// L69
  }
}

/// This is top function.
void _Z3topPfPA64_fS1_S_S_S_(
  float v49[64],
  float v50[64][64],
  float v51[64][64],
  float v52[64],
  float v53
) {	// L73
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v53 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v52 bundle=axi_3

  #pragma HLS interface m_axi offset=slave port=v51 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v50 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v49 bundle=axi_0

  float v59;	// L84
  float v60[64];	// L85
  #pragma HLS bind_storage variable=v60 type=ram_t2p impl=bram

  float v61[64];	// L86
  #pragma HLS bind_storage variable=v61 type=ram_t2p impl=bram

  _Z3topPfPA64_fS1_S_S_S__node5(v60);	// L87
  float v62[64];	// L88
  #pragma HLS bind_storage variable=v62 type=ram_t2p impl=bram

  _Z3topPfPA64_fS1_S_S_S__node4(v49, v50, v60, v62);	// L89
  _Z3topPfPA64_fS1_S_S_S__node3(v61);	// L90
  float v63[64];	// L91
  #pragma HLS bind_storage variable=v63 type=ram_t2p impl=bram

  _Z3topPfPA64_fS1_S_S_S__node2(v62, v51, v61, v63);	// L92
  _Z3topPfPA64_fS1_S_S_S__node1(v63, v52, v59);	// L93
  _Z3topPfPA64_fS1_S_S_S__node0(v59, v53);	// L94
}

void _Z13load_64_floatPfS__node0(
  float v64[64],
  float v65[64]
) {	// L106
  #pragma HLS inline
  for (int v66 = 0; v66 < 64; v66 += 1) {	// L107
    #pragma HLS pipeline II=1
    float v67 = v64[v66];	// L108
    v65[v66] = v67;	// L109
  }
}

void _Z13load_64_floatPfS_(
  float v68[64],
  float v69[64]
) {	// L113
  _Z13load_64_floatPfS__node0(v68, v69);	// L114
}

void _Z16load_64_64_floatPA64_fS0__node0(
  float v70[64][64],
  float v71[64][64]
) {	// L117
  #pragma HLS inline
  for (int v72 = 0; v72 < 64; v72 += 1) {	// L118
    for (int v73 = 0; v73 < 64; v73 += 1) {	// L119
      #pragma HLS pipeline II=1
      float v74 = v70[v72][v73];	// L120
      v71[v72][v73] = v74;	// L121
    }
  }
}

void _Z16load_64_64_floatPA64_fS0_(
  float v75[64][64],
  float v76[64][64]
) {	// L126
  _Z16load_64_64_floatPA64_fS0__node0(v75, v76);	// L127
}

void _Z12load_1_floatPfS_(
  float v77,
  float v78
) {	// L130
  float v79 = v77;	// L131
  v78 = v79;	// L132
}

void _Z13store_1_floatPfS_(
  float v80,
  float v81
) {	// L135
  float v82 = v80;	// L136
  v81 = v82;	// L137
}

