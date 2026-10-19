
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

void _Z3topPA128_fS0__node1(
  hls::stream<float> &v0,
  float v1[8][128],
  float v2[8][128],
  hls::stream<float> &v3,
  int v4
) {	// L2
  float v5 = v0.read();	// L4
  for (int v6 = 0; v6 < 128; v6 += 1) {	// L5
    #pragma HLS pipeline II=1
    float v7 = v1[v4][v6];	// L6
    float v8 = v7 - v5;	// L7
    float v9 = exp(v8);	// L8
    v2[v4][v6] = v9;	// L9
    float v10 = float v11 + v9;	// L10
  }
  v3.write(float v12);	// L13
  float v13 = v3.read();	// L14
  for (int v14 = 0; v14 < 128; v14 += 1) {	// L15
    #pragma HLS pipeline II=1
    float v15 = v2[v4][v14];	// L16
    float v16 = v15 / v13;	// L17
    v2[v4][v14] = v16;	// L18
  }
}

void _Z3topPA128_fS0__node2(
  float v17[8][128],
  hls::stream<float> &v18,
  int v19
) {	// L22
  #pragma HLS inline
  float v20 = v17[v19][0];	// L23
  for (int v21 = 0; v21 < 127; v21 += 1) {	// L24
    #pragma HLS pipeline II=1
    float v22 = v17[v19][(v21 + 1)];	// L25
    bool v23 = v22 > float v24;	// L26
    float v25 = v23 ? v22 : v24;	// L27
  }
  v18.write(float v26);	// L30
}

void _Z3topPA128_fS0__node0(
  float v27[8][128],
  float v28[8][128],
  float v29[8][128]
) {	// L33
  for (int v30 = 0; v30 < 8; v30 += 1) {	// L34
    hls::stream<float> v31;	// L35
    _Z3topPA128_fS0__node2(v27, v31, v30);	// L36
    hls::stream<float> v32;	// L37
    _Z3topPA128_fS0__node1(v31, v28, v29, v32, v30);	// L38
  }
}

/// This is top function.
void _Z3topPA128_fS0_(
  float v33[8][128],
  float v34[8][128],
  float v35[8][128]
) {	// L42
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v35 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v34 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v33 bundle=axi_0

  _Z3topPA128_fS0__node0(v33, v34, v35);	// L49
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v39[8][128],
  float v40[8][128]
) {	// L59
  #pragma HLS inline
  for (int v41 = 0; v41 < 8; v41 += 1) {	// L60
    for (int v42 = 0; v42 < 128; v42 += 1) {	// L61
      #pragma HLS pipeline II=1
      float v43 = v39[v41][v42];	// L62
      v40[v41][v42] = v43;	// L63
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v44[8][128],
  float v45[8][128]
) {	// L68
  _Z16load_8_128_floatPA128_fS0__node0(v44, v45);	// L69
}

void _Z19softmax_8_128_floatPA128_fS0__node1(
  hls::stream<float> &v46,
  float v47[8][128],
  float v48[8][128],
  hls::stream<float> &v49,
  int v50
) {	// L72
  float v51 = v46.read();	// L74
  for (int v52 = 0; v52 < 128; v52 += 1) {	// L75
    #pragma HLS pipeline II=1
    float v53 = v47[v50][v52];	// L76
    float v54 = v53 - v51;	// L77
    float v55 = exp(v54);	// L78
    v48[v50][v52] = v55;	// L79
    float v56 = float v57 + v55;	// L80
  }
  v49.write(float v58);	// L83
  float v59 = v49.read();	// L84
  for (int v60 = 0; v60 < 128; v60 += 1) {	// L85
    #pragma HLS pipeline II=1
    float v61 = v48[v50][v60];	// L86
    float v62 = v61 / v59;	// L87
    v48[v50][v60] = v62;	// L88
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node2(
  float v63[8][128],
  hls::stream<float> &v64,
  int v65
) {	// L92
  #pragma HLS inline
  float v66 = v63[v65][0];	// L93
  for (int v67 = 0; v67 < 127; v67 += 1) {	// L94
    #pragma HLS pipeline II=1
    float v68 = v63[v65][(v67 + 1)];	// L95
    bool v69 = v68 > float v70;	// L96
    float v71 = v69 ? v68 : v70;	// L97
  }
  v64.write(float v72);	// L100
}

void _Z19softmax_8_128_floatPA128_fS0__node0(
  float v73[8][128],
  float v74[8][128],
  float v75[8][128]
) {	// L103
  for (int v76 = 0; v76 < 8; v76 += 1) {	// L104
    hls::stream<float> v77;	// L105
    _Z19softmax_8_128_floatPA128_fS0__node2(v73, v77, v76);	// L106
    hls::stream<float> v78;	// L107
    _Z19softmax_8_128_floatPA128_fS0__node1(v77, v74, v75, v78, v76);	// L108
  }
}

void _Z19softmax_8_128_floatPA128_fS0_(
  float v79[8][128],
  float v80[8][128]
) {	// L112
  _Z19softmax_8_128_floatPA128_fS0__node0(v79, v79, v80);	// L113
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v81[8][128],
  float v82[8][128]
) {	// L116
  #pragma HLS inline
  for (int v83 = 0; v83 < 8; v83 += 1) {	// L117
    for (int v84 = 0; v84 < 128; v84 += 1) {	// L118
      #pragma HLS pipeline II=1
      float v85 = v81[v83][v84];	// L119
      v82[v83][v84] = v85;	// L120
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v86[8][128],
  float v87[8][128]
) {	// L125
  _Z17store_8_128_floatPA128_fS0__node0(v86, v87);	// L126
}

