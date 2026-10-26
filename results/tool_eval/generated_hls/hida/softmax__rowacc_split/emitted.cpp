
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
  float v0[8],
  float v1[8][128],
  float v2[8][128],
  float v3[8],
  hls::stream<float> &v4,
  hls::stream<float> &v5,
  int v6
) {	// L2
  float v7 = v0[v6];	// L3
  for (int v8 = 0; v8 < 128; v8 += 1) {	// L4
    #pragma HLS pipeline II=1
    float v9 = v1[v6][v8];	// L5
    float v10 = v9 - v7;	// L6
    float v11 = exp(v10);	// L7
    v2[v6][v8] = v11;	// L8
    float v12 = v3[v6];	// L9
    float v13 = v12 + v11;	// L10
    v3[v6] = v13;	// L11
  }
  v4.write(v7);	// L13
  float v14 = v3[v6];	// L14
  for (int v15 = 0; v15 < 128; v15 += 1) {	// L15
    #pragma HLS pipeline II=1
    float v16 = v2[v6][v15];	// L16
    float v17 = v16 / v14;	// L17
    v2[v6][v15] = v17;	// L18
  }
  v5.write(v14);	// L20
}

void _Z3topPA128_fS0__node2(
  float v18[8][128],
  float v19[8],
  int v20
) {	// L23
  #pragma HLS inline
  for (int v21 = 0; v21 < 127; v21 += 1) {	// L24
    #pragma HLS pipeline II=1
    float v22 = v18[v20][(v21 + 1)];	// L25
    float v23 = v19[v20];	// L26
    bool v24 = v22 > v23;	// L27
    if (v24) {	// L28
      v19[v20] = v22;	// L29
    }
  }
}

void _Z3topPA128_fS0__node0(
  float v25[8][128],
  float v26[8],
  float v27[8],
  float v28[8][128],
  float v29[8][128]
) {	// L34
  float v30[8];	// L35
  #pragma HLS bind_storage variable=v30 type=ram_t2p impl=bram

  float v31[8];	// L36
  #pragma HLS bind_storage variable=v31 type=ram_t2p impl=bram

  for (int v32 = 0; v32 < 8; v32 += 1) {	// L37
    #pragma HLS pipeline II=1
    float v33 = v27[v32];	// L38
    v30[v32] = v33;	// L39
  }
  for (int v34 = 0; v34 < 8; v34 += 1) {	// L41
    #pragma HLS pipeline II=1
    float v35 = v26[v34];	// L42
    v31[v34] = v35;	// L43
  }
  for (int v36 = 0; v36 < 8; v36 += 1) {	// L45
    _Z3topPA128_fS0__node2(v25, v31, v36);	// L46
    hls::stream<float> v37;	// L47
    hls::stream<float> v38;	// L48
    _Z3topPA128_fS0__node1(v31, v28, v29, v30, v37, v38, v36);	// L49
  }
}

void _Z3topPA128_fS0__node3(
  float v39[8]
) {	// L53
  #pragma HLS inline
  for (int v40 = 0; v40 < 8; v40 += 1) {	// L55
    #pragma HLS pipeline II=1
    v39[v40] = (float)0.000000;	// L56
  }
}

void _Z3topPA128_fS0__node4(
  float v41[8][128],
  float v42[8]
) {	// L60
  #pragma HLS inline
  for (int v43 = 0; v43 < 8; v43 += 1) {	// L61
    #pragma HLS pipeline II=1
    float v44 = v41[v43][0];	// L62
    v42[v43] = v44;	// L63
  }
}

/// This is top function.
void _Z3topPA128_fS0_(
  float v45[8][128],
  float v46[8][128],
  float v47[8][128],
  float v48[8][128]
) {	// L67
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v48 bundle=axi_3

  #pragma HLS interface m_axi offset=slave port=v47 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v46 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v45 bundle=axi_0

  float v53[8];	// L76
  #pragma HLS bind_storage variable=v53 type=ram_t2p impl=bram

  float v54[8];	// L77
  #pragma HLS bind_storage variable=v54 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node4(v47, v53);	// L78
  _Z3topPA128_fS0__node3(v54);	// L79
  _Z3topPA128_fS0__node0(v45, v53, v54, v46, v48);	// L80
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v55[8][128],
  float v56[8][128]
) {	// L91
  #pragma HLS inline
  for (int v57 = 0; v57 < 8; v57 += 1) {	// L92
    for (int v58 = 0; v58 < 128; v58 += 1) {	// L93
      #pragma HLS pipeline II=1
      float v59 = v55[v57][v58];	// L94
      v56[v57][v58] = v59;	// L95
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v60[8][128],
  float v61[8][128]
) {	// L100
  _Z16load_8_128_floatPA128_fS0__node0(v60, v61);	// L101
}

void _Z19softmax_8_128_floatPA128_fS0__node1(
  float v62[8],
  float v63[8][128],
  float v64[8][128],
  float v65[8],
  hls::stream<float> &v66,
  hls::stream<float> &v67,
  int v68
) {	// L104
  float v69 = v62[v68];	// L105
  for (int v70 = 0; v70 < 128; v70 += 1) {	// L106
    #pragma HLS pipeline II=1
    float v71 = v63[v68][v70];	// L107
    float v72 = v71 - v69;	// L108
    float v73 = exp(v72);	// L109
    v64[v68][v70] = v73;	// L110
    float v74 = v65[v68];	// L111
    float v75 = v74 + v73;	// L112
    v65[v68] = v75;	// L113
  }
  v66.write(v69);	// L115
  float v76 = v65[v68];	// L116
  for (int v77 = 0; v77 < 128; v77 += 1) {	// L117
    #pragma HLS pipeline II=1
    float v78 = v64[v68][v77];	// L118
    float v79 = v78 / v76;	// L119
    v64[v68][v77] = v79;	// L120
  }
  v67.write(v76);	// L122
}

void _Z19softmax_8_128_floatPA128_fS0__node2(
  float v80[8][128],
  float v81[8],
  int v82
) {	// L125
  #pragma HLS inline
  for (int v83 = 0; v83 < 127; v83 += 1) {	// L126
    #pragma HLS pipeline II=1
    float v84 = v80[v82][(v83 + 1)];	// L127
    float v85 = v81[v82];	// L128
    bool v86 = v84 > v85;	// L129
    if (v86) {	// L130
      v81[v82] = v84;	// L131
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node0(
  float v87[8][128],
  float v88[8],
  float v89[8],
  float v90[8][128],
  float v91[8][128]
) {	// L136
  float v92[8];	// L137
  #pragma HLS bind_storage variable=v92 type=ram_t2p impl=bram

  float v93[8];	// L138
  #pragma HLS bind_storage variable=v93 type=ram_t2p impl=bram

  for (int v94 = 0; v94 < 8; v94 += 1) {	// L139
    #pragma HLS pipeline II=1
    float v95 = v89[v94];	// L140
    v92[v94] = v95;	// L141
  }
  for (int v96 = 0; v96 < 8; v96 += 1) {	// L143
    #pragma HLS pipeline II=1
    float v97 = v88[v96];	// L144
    v93[v96] = v97;	// L145
  }
  for (int v98 = 0; v98 < 8; v98 += 1) {	// L147
    _Z19softmax_8_128_floatPA128_fS0__node2(v87, v93, v98);	// L148
    hls::stream<float> v99;	// L149
    hls::stream<float> v100;	// L150
    _Z19softmax_8_128_floatPA128_fS0__node1(v93, v90, v91, v92, v99, v100, v98);	// L151
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node3(
  float v101[8]
) {	// L155
  #pragma HLS inline
  for (int v102 = 0; v102 < 8; v102 += 1) {	// L157
    #pragma HLS pipeline II=1
    v101[v102] = (float)0.000000;	// L158
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node4(
  float v103[8][128],
  float v104[8]
) {	// L162
  #pragma HLS inline
  for (int v105 = 0; v105 < 8; v105 += 1) {	// L163
    #pragma HLS pipeline II=1
    float v106 = v103[v105][0];	// L164
    v104[v105] = v106;	// L165
  }
}

void _Z19softmax_8_128_floatPA128_fS0_(
  float v107[8][128],
  float v108[8][128]
) {	// L169
  float v109[8];	// L170
  #pragma HLS bind_storage variable=v109 type=ram_t2p impl=bram

  float v110[8];	// L171
  #pragma HLS bind_storage variable=v110 type=ram_t2p impl=bram

  _Z19softmax_8_128_floatPA128_fS0__node4(v107, v109);	// L172
  _Z19softmax_8_128_floatPA128_fS0__node3(v110);	// L173
  _Z19softmax_8_128_floatPA128_fS0__node0(v107, v109, v110, v107, v108);	// L174
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v111[8][128],
  float v112[8][128]
) {	// L177
  #pragma HLS inline
  for (int v113 = 0; v113 < 8; v113 += 1) {	// L178
    for (int v114 = 0; v114 < 128; v114 += 1) {	// L179
      #pragma HLS pipeline II=1
      float v115 = v111[v113][v114];	// L180
      v112[v113][v114] = v115;	// L181
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v116[8][128],
  float v117[8][128]
) {	// L186
  _Z17store_8_128_floatPA128_fS0__node0(v116, v117);	// L187
}

