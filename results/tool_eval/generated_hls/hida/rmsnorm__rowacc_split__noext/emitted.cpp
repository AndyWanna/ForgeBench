
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
  float v0[8],
  float v1[8][128],
  float v2[128],
  float v3[8][128],
  hls::stream<float> &v4,
  int v5
) {	// L2
  #pragma HLS inline
  float v6 = v0[v5];	// L5
  float v7 = v6 / (float)128.000000;	// L6
  float v8 = v7 + (float)0.010000;	// L7
  float v9 = sqrt(v8);	// L8
  for (int v10 = 0; v10 < 128; v10 += 1) {	// L9
    #pragma HLS pipeline II=1
    float v11 = v2[v10];	// L10
    float v12 = v1[v5][v10];	// L11
    float v13 = v11 * v12;	// L12
    float v14 = v13 / v9;	// L13
    v3[v5][v10] = v14;	// L14
  }
  v4.write(v9);	// L16
}

void _Z3topPA128_fPfS0__node2(
  float v15[8][128],
  float v16[8],
  int v17
) {	// L19
  #pragma HLS inline
  for (int v18 = 0; v18 < 128; v18 += 1) {	// L20
    #pragma HLS pipeline II=1
    float v19 = v15[v17][v18];	// L21
    float v20 = v19 * v19;	// L22
    float v21 = v16[v17];	// L23
    float v22 = v21 + v20;	// L24
    v16[v17] = v22;	// L25
  }
}

void _Z3topPA128_fPfS0__node0(
  float v23[128],
  float v24[8][128],
  float v25[8],
  float v26[8][128]
) {	// L29
  float v27[8];	// L30
  #pragma HLS bind_storage variable=v27 type=ram_t2p impl=bram

  for (int v28 = 0; v28 < 8; v28 += 1) {	// L31
    #pragma HLS pipeline II=1
    float v29 = v25[v28];	// L32
    v27[v28] = v29;	// L33
  }
  for (int v30 = 0; v30 < 8; v30 += 1) {	// L35
    _Z3topPA128_fPfS0__node2(v24, v27, v30);	// L36
    hls::stream<float> v31;	// L37
    _Z3topPA128_fPfS0__node1(v27, v24, v23, v26, v31, v30);	// L38
  }
}

void _Z3topPA128_fPfS0__node3(
  float v32[8]
) {	// L42
  #pragma HLS inline
  for (int v33 = 0; v33 < 8; v33 += 1) {	// L44
    #pragma HLS pipeline II=1
    v32[v33] = (float)0.000000;	// L45
  }
}

/// This is top function.
void _Z3topPA128_fPfS0_(
  float v34[8][128],
  float v35[128],
  float v36[8][128]
) {	// L49
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v36 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v35 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v34 bundle=axi_0

  float v40[8];	// L56
  #pragma HLS bind_storage variable=v40 type=ram_t2p impl=bram

  _Z3topPA128_fPfS0__node3(v40);	// L57
  _Z3topPA128_fPfS0__node0(v35, v34, v40, v36);	// L58
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v41[8][128],
  float v42[8][128]
) {	// L68
  #pragma HLS inline
  for (int v43 = 0; v43 < 8; v43 += 1) {	// L69
    for (int v44 = 0; v44 < 128; v44 += 1) {	// L70
      #pragma HLS pipeline II=1
      float v45 = v41[v43][v44];	// L71
      v42[v43][v44] = v45;	// L72
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v46[8][128],
  float v47[8][128]
) {	// L77
  _Z16load_8_128_floatPA128_fS0__node0(v46, v47);	// L78
}

void _Z14load_128_floatPfS__node0(
  float v48[128],
  float v49[128]
) {	// L81
  #pragma HLS inline
  for (int v50 = 0; v50 < 128; v50 += 1) {	// L82
    #pragma HLS pipeline II=1
    float v51 = v48[v50];	// L83
    v49[v50] = v51;	// L84
  }
}

void _Z14load_128_floatPfS_(
  float v52[128],
  float v53[128]
) {	// L88
  _Z14load_128_floatPfS__node0(v52, v53);	// L89
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node1(
  float v54[8][128],
  float v55[128],
  float v56[8],
  float v57[8][128],
  hls::stream<float> &v58,
  int v59
) {	// L92
  #pragma HLS inline
  float v60 = v56[v59];	// L95
  float v61 = v60 / (float)128.000000;	// L96
  float v62 = v61 + (float)0.010000;	// L97
  float v63 = sqrt(v62);	// L98
  for (int v64 = 0; v64 < 128; v64 += 1) {	// L99
    #pragma HLS pipeline II=1
    float v65 = v55[v64];	// L100
    float v66 = v54[v59][v64];	// L101
    float v67 = v65 * v66;	// L102
    float v68 = v67 / v63;	// L103
    v57[v59][v64] = v68;	// L104
  }
  v58.write(v63);	// L106
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node2(
  float v69[8][128],
  float v70[8],
  int v71
) {	// L109
  #pragma HLS inline
  for (int v72 = 0; v72 < 128; v72 += 1) {	// L110
    #pragma HLS pipeline II=1
    float v73 = v69[v71][v72];	// L111
    float v74 = v73 * v73;	// L112
    float v75 = v70[v71];	// L113
    float v76 = v75 + v74;	// L114
    v70[v71] = v76;	// L115
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node0(
  float v77[128],
  float v78[8][128],
  float v79[8],
  float v80[8][128]
) {	// L119
  float v81[8];	// L120
  #pragma HLS bind_storage variable=v81 type=ram_t2p impl=bram

  for (int v82 = 0; v82 < 8; v82 += 1) {	// L121
    #pragma HLS pipeline II=1
    float v83 = v79[v82];	// L122
    v81[v82] = v83;	// L123
  }
  for (int v84 = 0; v84 < 8; v84 += 1) {	// L125
    _Z20rms_norm_8_128_floatPA128_fPfS0__node2(v78, v81, v84);	// L126
    hls::stream<float> v85;	// L127
    _Z20rms_norm_8_128_floatPA128_fPfS0__node1(v78, v77, v81, v80, v85, v84);	// L128
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node3(
  float v86[8]
) {	// L132
  #pragma HLS inline
  for (int v87 = 0; v87 < 8; v87 += 1) {	// L134
    #pragma HLS pipeline II=1
    v86[v87] = (float)0.000000;	// L135
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0_(
  float v88[8][128],
  float v89[128],
  float v90[8][128]
) {	// L139
  float v91[8];	// L140
  #pragma HLS bind_storage variable=v91 type=ram_t2p impl=bram

  _Z20rms_norm_8_128_floatPA128_fPfS0__node3(v91);	// L141
  _Z20rms_norm_8_128_floatPA128_fPfS0__node0(v89, v88, v91, v90);	// L142
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v92[8][128],
  float v93[8][128]
) {	// L145
  #pragma HLS inline
  for (int v94 = 0; v94 < 8; v94 += 1) {	// L146
    for (int v95 = 0; v95 < 128; v95 += 1) {	// L147
      #pragma HLS pipeline II=1
      float v96 = v92[v94][v95];	// L148
      v93[v94][v95] = v96;	// L149
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v97[8][128],
  float v98[8][128]
) {	// L154
  _Z17store_8_128_floatPA128_fS0__node0(v97, v98);	// L155
}

