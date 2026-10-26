
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
  float v1[8],
  float v2[128],
  hls::stream<float> &v3,
  float v4[8][128],
  int v5
) {	// L2
  #pragma HLS inline
  float v6 = v1[v5];	// L5
  float v7 = v6 / (float)128.000000;	// L6
  float v8 = v7 + (float)0.010000;	// L7
  float v9 = sqrt(v8);	// L8
  for (int v10 = 0; v10 < 128; v10 += 1) {	// L9
    #pragma HLS pipeline II=1
    float v11 = v2[v10];	// L10
    float v12 = v0[v5][v10];	// L11
    float v13 = v11 * v12;	// L12
    float v14 = v13 / v9;	// L13
    v4[v5][v10] = v14;	// L14
  }
  v3.write(v9);	// L16
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
  float v23[8][128],
  float v24[128],
  float v25[8],
  float v26[8][128],
  float v27[8][128]
) {	// L29
  float v28[8];	// L30
  #pragma HLS bind_storage variable=v28 type=ram_t2p impl=bram

  for (int v29 = 0; v29 < 8; v29 += 1) {	// L31
    #pragma HLS pipeline II=1
    float v30 = v25[v29];	// L32
    v28[v29] = v30;	// L33
  }
  for (int v31 = 0; v31 < 8; v31 += 1) {	// L35
    _Z3topPA128_fPfS0__node2(v23, v28, v31);	// L36
    hls::stream<float> v32;	// L37
    _Z3topPA128_fPfS0__node1(v26, v28, v24, v32, v27, v31);	// L38
  }
}

void _Z3topPA128_fPfS0__node3(
  float v33[8]
) {	// L42
  #pragma HLS inline
  for (int v34 = 0; v34 < 8; v34 += 1) {	// L44
    #pragma HLS pipeline II=1
    v33[v34] = (float)0.000000;	// L45
  }
}

/// This is top function.
void _Z3topPA128_fPfS0_(
  float v35[8][128],
  float v36[8][128],
  float v37[128],
  float v38[8][128]
) {	// L49
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v38 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v37 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v36 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v35 bundle=axi_0

  float v43[8];	// L58
  #pragma HLS bind_storage variable=v43 type=ram_t2p impl=bram

  _Z3topPA128_fPfS0__node3(v43);	// L59
  _Z3topPA128_fPfS0__node0(v35, v37, v43, v36, v38);	// L60
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v44[8][128],
  float v45[8][128]
) {	// L71
  #pragma HLS inline
  for (int v46 = 0; v46 < 8; v46 += 1) {	// L72
    for (int v47 = 0; v47 < 128; v47 += 1) {	// L73
      #pragma HLS pipeline II=1
      float v48 = v44[v46][v47];	// L74
      v45[v46][v47] = v48;	// L75
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v49[8][128],
  float v50[8][128]
) {	// L80
  _Z16load_8_128_floatPA128_fS0__node0(v49, v50);	// L81
}

void _Z14load_128_floatPfS__node0(
  float v51[128],
  float v52[128]
) {	// L84
  #pragma HLS inline
  for (int v53 = 0; v53 < 128; v53 += 1) {	// L85
    #pragma HLS pipeline II=1
    float v54 = v51[v53];	// L86
    v52[v53] = v54;	// L87
  }
}

void _Z14load_128_floatPfS_(
  float v55[128],
  float v56[128]
) {	// L91
  _Z14load_128_floatPfS__node0(v55, v56);	// L92
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node1(
  float v57[8],
  float v58[8][128],
  float v59[128],
  float v60[8][128],
  hls::stream<float> &v61,
  int v62
) {	// L95
  #pragma HLS inline
  float v63 = v57[v62];	// L98
  float v64 = v63 / (float)128.000000;	// L99
  float v65 = v64 + (float)0.010000;	// L100
  float v66 = sqrt(v65);	// L101
  for (int v67 = 0; v67 < 128; v67 += 1) {	// L102
    #pragma HLS pipeline II=1
    float v68 = v59[v67];	// L103
    float v69 = v58[v62][v67];	// L104
    float v70 = v68 * v69;	// L105
    float v71 = v70 / v66;	// L106
    v60[v62][v67] = v71;	// L107
  }
  v61.write(v66);	// L109
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node2(
  float v72[8][128],
  float v73[8],
  int v74
) {	// L112
  #pragma HLS inline
  for (int v75 = 0; v75 < 128; v75 += 1) {	// L113
    #pragma HLS pipeline II=1
    float v76 = v72[v74][v75];	// L114
    float v77 = v76 * v76;	// L115
    float v78 = v73[v74];	// L116
    float v79 = v78 + v77;	// L117
    v73[v74] = v79;	// L118
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node0(
  float v80[128],
  float v81[8][128],
  float v82[8],
  float v83[8][128],
  float v84[8][128]
) {	// L122
  float v85[8];	// L123
  #pragma HLS bind_storage variable=v85 type=ram_t2p impl=bram

  for (int v86 = 0; v86 < 8; v86 += 1) {	// L124
    #pragma HLS pipeline II=1
    float v87 = v82[v86];	// L125
    v85[v86] = v87;	// L126
  }
  for (int v88 = 0; v88 < 8; v88 += 1) {	// L128
    _Z20rms_norm_8_128_floatPA128_fPfS0__node2(v81, v85, v88);	// L129
    hls::stream<float> v89;	// L130
    _Z20rms_norm_8_128_floatPA128_fPfS0__node1(v85, v83, v80, v84, v89, v88);	// L131
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0__node3(
  float v90[8]
) {	// L135
  #pragma HLS inline
  for (int v91 = 0; v91 < 8; v91 += 1) {	// L137
    #pragma HLS pipeline II=1
    v90[v91] = (float)0.000000;	// L138
  }
}

void _Z20rms_norm_8_128_floatPA128_fPfS0_(
  float v92[8][128],
  float v93[128],
  float v94[8][128]
) {	// L142
  float v95[8];	// L143
  #pragma HLS bind_storage variable=v95 type=ram_t2p impl=bram

  _Z20rms_norm_8_128_floatPA128_fPfS0__node3(v95);	// L144
  _Z20rms_norm_8_128_floatPA128_fPfS0__node0(v93, v92, v95, v92, v94);	// L145
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v96[8][128],
  float v97[8][128]
) {	// L148
  #pragma HLS inline
  for (int v98 = 0; v98 < 8; v98 += 1) {	// L149
    for (int v99 = 0; v99 < 128; v99 += 1) {	// L150
      #pragma HLS pipeline II=1
      float v100 = v96[v98][v99];	// L151
      v97[v98][v99] = v100;	// L152
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v101[8][128],
  float v102[8][128]
) {	// L157
  _Z17store_8_128_floatPA128_fS0__node0(v101, v102);	// L158
}

