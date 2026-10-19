
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

void _Z3topPA128_fPfS0__node1(
  float v0[8][128],
  float v1[128],
  hls::stream<float> &v2,
  hls::stream<float> &v3,
  float v4[8][128],
  int v5
) {	// L2
  #pragma HLS inline
  float v6 = v2.read();	// L5
  float v7 = v6 / (float)128.000000;	// L6
  float v8 = v7 + (float)0.010000;	// L7
  float v9 = sqrt(v8);	// L8
  for (int v10 = 0; v10 < 128; v10 += 1) {	// L9
    #pragma HLS pipeline II=1
    float v11 = v1[v10];	// L10
    float v12 = v0[v5][v10];	// L11
    float v13 = v11 * v12;	// L12
    float v14 = v13 / v9;	// L13
    v4[v5][v10] = v14;	// L14
  }
  v3.write(v9);	// L16
}

void _Z3topPA128_fPfS0__node2(
  float v15[8][128],
  hls::stream<float> &v16,
  int v17
) {	// L19
  #pragma HLS inline
  for (int v18 = 0; v18 < 128; v18 += 1) {	// L21
    #pragma HLS pipeline II=1
    float v19 = v15[v17][v18];	// L22
    float v20 = v19 * v19;	// L23
    float v21 = float v22 + v20;	// L24
  }
  v16.write(float v23);	// L27
}

void _Z3topPA128_fPfS0__node0(
  float v24[8][128],
  float v25[128],
  float v26[8][128],
  float v27[8][128]
) {	// L30
  for (int v28 = 0; v28 < 8; v28 += 1) {	// L31
    hls::stream<float> v29;	// L32
    _Z3topPA128_fPfS0__node2(v24, v29, v28);	// L33
    hls::stream<float> v30;	// L34
    _Z3topPA128_fPfS0__node1(v26, v25, v29, v30, v27, v28);	// L35
  }
}

/// This is top function.
void _Z3topPA128_fPfS0_(
  float v31[8][128],
  float v32[8][128],
  float v33[128],
  float v34[8][128]
) {	// L39
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v34 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v33 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v32 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v31 bundle=axi_0

  _Z3topPA128_fPfS0__node0(v31, v33, v32, v34);	// L48
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

void _Z14load_128_floatPfS__node0(
  float v46[128],
  float v47[128]
) {	// L72
  #pragma HLS inline
  for (int v48 = 0; v48 < 128; v48 += 1) {	// L73
    #pragma HLS pipeline II=1
    float v49 = v46[v48];	// L74
    v47[v48] = v49;	// L75
  }
}

void _Z14load_128_floatPfS_(
  float v50[128],
  float v51[128]
) {	// L79
  _Z14load_128_floatPfS__node0(v50, v51);	// L80
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node1(
  float v52[128],
  float v53[8][128],
  hls::stream<float> &v54,
  float v55[8][128],
  hls::stream<float> &v56,
  int v57
) {	// L83
  #pragma HLS inline
  float v58 = v54.read();	// L86
  float v59 = v58 / (float)128.000000;	// L87
  float v60 = v59 + (float)0.010000;	// L88
  float v61 = sqrt(v60);	// L89
  for (int v62 = 0; v62 < 128; v62 += 1) {	// L90
    #pragma HLS pipeline II=1
    float v63 = v52[v62];	// L91
    float v64 = v53[v57][v62];	// L92
    float v65 = v63 * v64;	// L93
    float v66 = v65 / v61;	// L94
    v55[v57][v62] = v66;	// L95
  }
  v56.write(v61);	// L97
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node2(
  float v67[8][128],
  hls::stream<float> &v68,
  int v69
) {	// L100
  #pragma HLS inline
  for (int v70 = 0; v70 < 128; v70 += 1) {	// L102
    #pragma HLS pipeline II=1
    float v71 = v67[v69][v70];	// L103
    float v72 = v71 * v71;	// L104
    float v73 = float v74 + v72;	// L105
  }
  v68.write(float v75);	// L108
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node0(
  float v76[128],
  float v77[8][128],
  float v78[8][128],
  float v79[8][128]
) {	// L111
  for (int v80 = 0; v80 < 8; v80 += 1) {	// L112
    hls::stream<float> v81;	// L113
    _Z20rms_norm_8_128_floatPA128_fPfS0__node2(v77, v81, v80);	// L114
    hls::stream<float> v82;	// L115
    _Z20rms_norm_8_128_floatPA128_fPfS0__node1(v76, v78, v81, v79, v82, v80);	// L116
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0_(
  float v83[8][128],
  float v84[128],
  float v85[8][128]
) {	// L120
  _Z20rms_norm_8_128_floatPA128_fPfS0__node0(v84, v83, v83, v85);	// L121
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v86[8][128],
  float v87[8][128]
) {	// L124
  #pragma HLS inline
  for (int v88 = 0; v88 < 8; v88 += 1) {	// L125
    for (int v89 = 0; v89 < 128; v89 += 1) {	// L126
      #pragma HLS pipeline II=1
      float v90 = v86[v88][v89];	// L127
      v87[v88][v89] = v90;	// L128
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v91[8][128],
  float v92[8][128]
) {	// L133
  _Z17store_8_128_floatPA128_fS0__node0(v91, v92);	// L134
}

