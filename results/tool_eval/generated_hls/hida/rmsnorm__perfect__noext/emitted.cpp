
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

void _Z3topPA128_fPfS0__node0(
  float v0[128],
  float v1[8],
  float v2[8][128],
  float v3[8][128]
) {	// L3
  #pragma HLS inline
  float v4;	// L6
  for (int v5 = 0; v5 < 8; v5 += 1) {	// L7
    for (int v6 = 0; v6 < 128; v6 += 1) {	// L8
      #pragma HLS pipeline II=1
      float v7 = v1[v5];	// L9
      float v8 = v7 / (float)128.000000;	// L10
      float v9 = v8 + (float)0.010000;	// L11
      float v10 = sqrt(v9);	// L12
      float v11 = v4;	// L13
      float v12 = (v6 == 0) ? v10 : v11;	// L14
      v4 = v12;	// L15
      float v13 = v0[v6];	// L16
      float v14 = v2[v5][v6];	// L17
      float v15 = v13 * v14;	// L18
      float v16 = v15 / v12;	// L19
      v3[v5][v6] = v16;	// L20
    }
  }
}

void _Z3topPA128_fPfS0__node1(
  float v17[8][128],
  float v18[8],
  float v19[8]
) {	// L25
  #pragma HLS inline
  for (int v20 = 0; v20 < 128; v20 += 1) {	// L26
    for (int v21 = 0; v21 < 8; v21 += 1) {	// L27
      #pragma HLS pipeline II=1
      float v22 = v17[v21][v20];	// L28
      float v23 = v22 * v22;	// L29
      float v24 = v18[v21];	// L30
      float v25 = v19[v21];	// L31
      float v26 = (v20 == 0) ? v24 : v25;	// L32
      float v27 = v26 + v23;	// L33
      v19[v21] = v27;	// L34
    }
  }
}

void _Z3topPA128_fPfS0__node2(
  float v28[8]
) {	// L39
  #pragma HLS inline
  for (int v29 = 0; v29 < 8; v29 += 1) {	// L41
    #pragma HLS pipeline II=1
    v28[v29] = (float)0.000000;	// L42
  }
}

/// This is top function.
void _Z3topPA128_fPfS0_(
  float v30[8][128],
  float v31[8][128],
  float v32[128],
  float v33[8][128]
) {	// L46
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v33 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v32 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v31 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v30 bundle=axi_0

  float v38[8];	// L55
  #pragma HLS bind_storage variable=v38 type=ram_t2p impl=bram

  _Z3topPA128_fPfS0__node2(v38);	// L56
  float v39[8];	// L57
  #pragma HLS bind_storage variable=v39 type=ram_t2p impl=bram

  _Z3topPA128_fPfS0__node1(v31, v38, v39);	// L58
  _Z3topPA128_fPfS0__node0(v32, v39, v30, v33);	// L59
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v40[8][128],
  float v41[8][128]
) {	// L70
  #pragma HLS inline
  for (int v42 = 0; v42 < 8; v42 += 1) {	// L71
    for (int v43 = 0; v43 < 128; v43 += 1) {	// L72
      #pragma HLS pipeline II=1
      float v44 = v40[v42][v43];	// L73
      v41[v42][v43] = v44;	// L74
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v45[8][128],
  float v46[8][128]
) {	// L79
  _Z16load_8_128_floatPA128_fS0__node0(v45, v46);	// L80
}

void _Z14load_128_floatPfS__node0(
  float v47[128],
  float v48[128]
) {	// L83
  #pragma HLS inline
  for (int v49 = 0; v49 < 128; v49 += 1) {	// L84
    #pragma HLS pipeline II=1
    float v50 = v47[v49];	// L85
    v48[v49] = v50;	// L86
  }
}

void _Z14load_128_floatPfS_(
  float v51[128],
  float v52[128]
) {	// L90
  _Z14load_128_floatPfS__node0(v51, v52);	// L91
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node0(
  float v53[128],
  float v54[8][128],
  float v55[8],
  float v56[8][128]
) {	// L94
  #pragma HLS inline
  float v57;	// L97
  for (int v58 = 0; v58 < 8; v58 += 1) {	// L98
    for (int v59 = 0; v59 < 128; v59 += 1) {	// L99
      #pragma HLS pipeline II=1
      float v60 = v55[v58];	// L100
      float v61 = v60 / (float)128.000000;	// L101
      float v62 = v61 + (float)0.010000;	// L102
      float v63 = sqrt(v62);	// L103
      float v64 = v57;	// L104
      float v65 = (v59 == 0) ? v63 : v64;	// L105
      v57 = v65;	// L106
      float v66 = v53[v59];	// L107
      float v67 = v54[v58][v59];	// L108
      float v68 = v66 * v67;	// L109
      float v69 = v68 / v65;	// L110
      v56[v58][v59] = v69;	// L111
    }
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node1(
  float v70[8][128],
  float v71[8],
  float v72[8]
) {	// L116
  #pragma HLS inline
  for (int v73 = 0; v73 < 128; v73 += 1) {	// L117
    for (int v74 = 0; v74 < 8; v74 += 1) {	// L118
      #pragma HLS pipeline II=1
      float v75 = v70[v74][v73];	// L119
      float v76 = v75 * v75;	// L120
      float v77 = v71[v74];	// L121
      float v78 = v72[v74];	// L122
      float v79 = (v73 == 0) ? v77 : v78;	// L123
      float v80 = v79 + v76;	// L124
      v72[v74] = v80;	// L125
    }
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node2(
  float v81[8]
) {	// L130
  #pragma HLS inline
  for (int v82 = 0; v82 < 8; v82 += 1) {	// L132
    #pragma HLS pipeline II=1
    v81[v82] = (float)0.000000;	// L133
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0_(
  float v83[8][128],
  float v84[128],
  float v85[8][128]
) {	// L137
  float v86[8];	// L138
  #pragma HLS bind_storage variable=v86 type=ram_t2p impl=bram

  _Z20rms_norm_8_128_floatPA128_fPfS0__node2(v86);	// L139
  float v87[8];	// L140
  #pragma HLS bind_storage variable=v87 type=ram_t2p impl=bram

  _Z20rms_norm_8_128_floatPA128_fPfS0__node1(v83, v86, v87);	// L141
  _Z20rms_norm_8_128_floatPA128_fPfS0__node0(v84, v83, v87, v85);	// L142
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v88[8][128],
  float v89[8][128]
) {	// L145
  #pragma HLS inline
  for (int v90 = 0; v90 < 8; v90 += 1) {	// L146
    for (int v91 = 0; v91 < 128; v91 += 1) {	// L147
      #pragma HLS pipeline II=1
      float v92 = v88[v90][v91];	// L148
      v89[v90][v91] = v92;	// L149
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v93[8][128],
  float v94[8][128]
) {	// L154
  _Z17store_8_128_floatPA128_fS0__node0(v93, v94);	// L155
}

