
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
  hls::stream<float> &v0,
  float v1[128],
  float v2[8][128],
  hls::stream<float> &v3,
  float v4[8][128],
  int v5
) {	// L2
  #pragma HLS inline
  float v6 = v0.read();	// L5
  float v7 = v6 / (float)128.000000;	// L6
  float v8 = v7 + (float)0.010000;	// L7
  float v9 = sqrt(v8);	// L8
  for (int v10 = 0; v10 < 128; v10 += 1) {	// L9
    #pragma HLS pipeline II=1
    float v11 = v1[v10];	// L10
    float v12 = v2[v5][v10];	// L11
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
  float v24[128],
  float v25[8][128],
  float v26[8][128]
) {	// L30
  for (int v27 = 0; v27 < 8; v27 += 1) {	// L31
    hls::stream<float> v28;	// L32
    _Z3topPA128_fPfS0__node2(v25, v28, v27);	// L33
    hls::stream<float> v29;	// L34
    _Z3topPA128_fPfS0__node1(v28, v24, v25, v29, v26, v27);	// L35
  }
}

/// This is top function.
void _Z3topPA128_fPfS0_(
  float v30[8][128],
  float v31[128],
  float v32[8][128]
) {	// L39
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v32 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v31 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v30 bundle=axi_0

  _Z3topPA128_fPfS0__node0(v31, v30, v32);	// L46
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

void _Z14load_128_floatPfS__node0(
  float v43[128],
  float v44[128]
) {	// L69
  #pragma HLS inline
  for (int v45 = 0; v45 < 128; v45 += 1) {	// L70
    #pragma HLS pipeline II=1
    float v46 = v43[v45];	// L71
    v44[v45] = v46;	// L72
  }
}

void _Z14load_128_floatPfS_(
  float v47[128],
  float v48[128]
) {	// L76
  _Z14load_128_floatPfS__node0(v47, v48);	// L77
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node1(
  float v49[128],
  float v50[8][128],
  hls::stream<float> &v51,
  float v52[8][128],
  hls::stream<float> &v53,
  int v54
) {	// L80
  #pragma HLS inline
  float v55 = v51.read();	// L83
  float v56 = v55 / (float)128.000000;	// L84
  float v57 = v56 + (float)0.010000;	// L85
  float v58 = sqrt(v57);	// L86
  for (int v59 = 0; v59 < 128; v59 += 1) {	// L87
    #pragma HLS pipeline II=1
    float v60 = v49[v59];	// L88
    float v61 = v50[v54][v59];	// L89
    float v62 = v60 * v61;	// L90
    float v63 = v62 / v58;	// L91
    v52[v54][v59] = v63;	// L92
  }
  v53.write(v58);	// L94
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node2(
  float v64[8][128],
  hls::stream<float> &v65,
  int v66
) {	// L97
  #pragma HLS inline
  for (int v67 = 0; v67 < 128; v67 += 1) {	// L99
    #pragma HLS pipeline II=1
    float v68 = v64[v66][v67];	// L100
    float v69 = v68 * v68;	// L101
    float v70 = float v71 + v69;	// L102
  }
  v65.write(float v72);	// L105
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node0(
  float v73[8][128],
  float v74[128],
  float v75[8][128]
) {	// L108
  for (int v76 = 0; v76 < 8; v76 += 1) {	// L109
    hls::stream<float> v77;	// L110
    _Z20rms_norm_8_128_floatPA128_fPfS0__node2(v73, v77, v76);	// L111
    hls::stream<float> v78;	// L112
    _Z20rms_norm_8_128_floatPA128_fPfS0__node1(v74, v73, v77, v75, v78, v76);	// L113
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0_(
  float v79[8][128],
  float v80[128],
  float v81[8][128]
) {	// L117
  _Z20rms_norm_8_128_floatPA128_fPfS0__node0(v79, v80, v81);	// L118
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v82[8][128],
  float v83[8][128]
) {	// L121
  #pragma HLS inline
  for (int v84 = 0; v84 < 8; v84 += 1) {	// L122
    for (int v85 = 0; v85 < 128; v85 += 1) {	// L123
      #pragma HLS pipeline II=1
      float v86 = v82[v84][v85];	// L124
      v83[v84][v85] = v86;	// L125
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v87[8][128],
  float v88[8][128]
) {	// L130
  _Z17store_8_128_floatPA128_fS0__node0(v87, v88);	// L131
}

