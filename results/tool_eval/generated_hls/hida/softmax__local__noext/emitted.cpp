
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
  float v28[8][128]
) {	// L33
  for (int v29 = 0; v29 < 8; v29 += 1) {	// L34
    hls::stream<float> v30;	// L35
    _Z3topPA128_fS0__node2(v27, v30, v29);	// L36
    hls::stream<float> v31;	// L37
    _Z3topPA128_fS0__node1(v30, v27, v28, v31, v29);	// L38
  }
}

/// This is top function.
void _Z3topPA128_fS0_(
  float v32[8][128],
  float v33[8][128]
) {	// L42
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v33 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v32 bundle=axi_0

  _Z3topPA128_fS0__node0(v32, v33);	// L47
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v36[8][128],
  float v37[8][128]
) {	// L56
  #pragma HLS inline
  for (int v38 = 0; v38 < 8; v38 += 1) {	// L57
    for (int v39 = 0; v39 < 128; v39 += 1) {	// L58
      #pragma HLS pipeline II=1
      float v40 = v36[v38][v39];	// L59
      v37[v38][v39] = v40;	// L60
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v41[8][128],
  float v42[8][128]
) {	// L65
  _Z16load_8_128_floatPA128_fS0__node0(v41, v42);	// L66
}

void _Z19softmax_8_128_floatPA128_fS0__node1(
  hls::stream<float> &v43,
  float v44[8][128],
  float v45[8][128],
  hls::stream<float> &v46,
  int v47
) {	// L69
  float v48 = v43.read();	// L71
  for (int v49 = 0; v49 < 128; v49 += 1) {	// L72
    #pragma HLS pipeline II=1
    float v50 = v44[v47][v49];	// L73
    float v51 = v50 - v48;	// L74
    float v52 = exp(v51);	// L75
    v45[v47][v49] = v52;	// L76
    float v53 = float v54 + v52;	// L77
  }
  v46.write(float v55);	// L80
  float v56 = v46.read();	// L81
  for (int v57 = 0; v57 < 128; v57 += 1) {	// L82
    #pragma HLS pipeline II=1
    float v58 = v45[v47][v57];	// L83
    float v59 = v58 / v56;	// L84
    v45[v47][v57] = v59;	// L85
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node2(
  float v60[8][128],
  hls::stream<float> &v61,
  int v62
) {	// L89
  #pragma HLS inline
  float v63 = v60[v62][0];	// L90
  for (int v64 = 0; v64 < 127; v64 += 1) {	// L91
    #pragma HLS pipeline II=1
    float v65 = v60[v62][(v64 + 1)];	// L92
    bool v66 = v65 > float v67;	// L93
    float v68 = v66 ? v65 : v67;	// L94
  }
  v61.write(float v69);	// L97
}

void _Z19softmax_8_128_floatPA128_fS0__node0(
  float v70[8][128],
  float v71[8][128]
) {	// L100
  for (int v72 = 0; v72 < 8; v72 += 1) {	// L101
    hls::stream<float> v73;	// L102
    _Z19softmax_8_128_floatPA128_fS0__node2(v70, v73, v72);	// L103
    hls::stream<float> v74;	// L104
    _Z19softmax_8_128_floatPA128_fS0__node1(v73, v70, v71, v74, v72);	// L105
  }
}

void _Z19softmax_8_128_floatPA128_fS0_(
  float v75[8][128],
  float v76[8][128]
) {	// L109
  _Z19softmax_8_128_floatPA128_fS0__node0(v75, v76);	// L110
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v77[8][128],
  float v78[8][128]
) {	// L113
  #pragma HLS inline
  for (int v79 = 0; v79 < 8; v79 += 1) {	// L114
    for (int v80 = 0; v80 < 128; v80 += 1) {	// L115
      #pragma HLS pipeline II=1
      float v81 = v77[v79][v80];	// L116
      v78[v79][v80] = v81;	// L117
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v82[8][128],
  float v83[8][128]
) {	// L122
  _Z17store_8_128_floatPA128_fS0__node0(v82, v83);	// L123
}

