
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
  float v28[8][128]
) {	// L34
  float v29[8];	// L35
  #pragma HLS bind_storage variable=v29 type=ram_t2p impl=bram

  float v30[8];	// L36
  #pragma HLS bind_storage variable=v30 type=ram_t2p impl=bram

  for (int v31 = 0; v31 < 8; v31 += 1) {	// L37
    #pragma HLS pipeline II=1
    float v32 = v27[v31];	// L38
    v29[v31] = v32;	// L39
  }
  for (int v33 = 0; v33 < 8; v33 += 1) {	// L41
    #pragma HLS pipeline II=1
    float v34 = v26[v33];	// L42
    v30[v33] = v34;	// L43
  }
  for (int v35 = 0; v35 < 8; v35 += 1) {	// L45
    _Z3topPA128_fS0__node2(v25, v30, v35);	// L46
    hls::stream<float> v36;	// L47
    hls::stream<float> v37;	// L48
    _Z3topPA128_fS0__node1(v30, v25, v28, v29, v36, v37, v35);	// L49
  }
}

void _Z3topPA128_fS0__node3(
  float v38[8]
) {	// L53
  #pragma HLS inline
  for (int v39 = 0; v39 < 8; v39 += 1) {	// L55
    #pragma HLS pipeline II=1
    v38[v39] = (float)0.000000;	// L56
  }
}

void _Z3topPA128_fS0__node4(
  float v40[8][128],
  float v41[8]
) {	// L60
  #pragma HLS inline
  for (int v42 = 0; v42 < 8; v42 += 1) {	// L61
    #pragma HLS pipeline II=1
    float v43 = v40[v42][0];	// L62
    v41[v42] = v43;	// L63
  }
}

/// This is top function.
void _Z3topPA128_fS0_(
  float v44[8][128],
  float v45[8][128],
  float v46[8][128]
) {	// L67
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v46 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v45 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v44 bundle=axi_0

  float v50[8];	// L74
  #pragma HLS bind_storage variable=v50 type=ram_t2p impl=bram

  float v51[8];	// L75
  #pragma HLS bind_storage variable=v51 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node4(v45, v50);	// L76
  _Z3topPA128_fS0__node3(v51);	// L77
  _Z3topPA128_fS0__node0(v44, v50, v51, v46);	// L78
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v52[8][128],
  float v53[8][128]
) {	// L88
  #pragma HLS inline
  for (int v54 = 0; v54 < 8; v54 += 1) {	// L89
    for (int v55 = 0; v55 < 128; v55 += 1) {	// L90
      #pragma HLS pipeline II=1
      float v56 = v52[v54][v55];	// L91
      v53[v54][v55] = v56;	// L92
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v57[8][128],
  float v58[8][128]
) {	// L97
  _Z16load_8_128_floatPA128_fS0__node0(v57, v58);	// L98
}

void _Z19softmax_8_128_floatPA128_fS0__node1(
  float v59[8],
  float v60[8][128],
  float v61[8][128],
  float v62[8],
  hls::stream<float> &v63,
  hls::stream<float> &v64,
  int v65
) {	// L101
  float v66 = v59[v65];	// L102
  for (int v67 = 0; v67 < 128; v67 += 1) {	// L103
    #pragma HLS pipeline II=1
    float v68 = v60[v65][v67];	// L104
    float v69 = v68 - v66;	// L105
    float v70 = exp(v69);	// L106
    v61[v65][v67] = v70;	// L107
    float v71 = v62[v65];	// L108
    float v72 = v71 + v70;	// L109
    v62[v65] = v72;	// L110
  }
  v63.write(v66);	// L112
  float v73 = v62[v65];	// L113
  for (int v74 = 0; v74 < 128; v74 += 1) {	// L114
    #pragma HLS pipeline II=1
    float v75 = v61[v65][v74];	// L115
    float v76 = v75 / v73;	// L116
    v61[v65][v74] = v76;	// L117
  }
  v64.write(v73);	// L119
}

void _Z19softmax_8_128_floatPA128_fS0__node2(
  float v77[8][128],
  float v78[8],
  int v79
) {	// L122
  #pragma HLS inline
  for (int v80 = 0; v80 < 127; v80 += 1) {	// L123
    #pragma HLS pipeline II=1
    float v81 = v77[v79][(v80 + 1)];	// L124
    float v82 = v78[v79];	// L125
    bool v83 = v81 > v82;	// L126
    if (v83) {	// L127
      v78[v79] = v81;	// L128
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node0(
  float v84[8][128],
  float v85[8],
  float v86[8],
  float v87[8][128]
) {	// L133
  float v88[8];	// L134
  #pragma HLS bind_storage variable=v88 type=ram_t2p impl=bram

  float v89[8];	// L135
  #pragma HLS bind_storage variable=v89 type=ram_t2p impl=bram

  for (int v90 = 0; v90 < 8; v90 += 1) {	// L136
    #pragma HLS pipeline II=1
    float v91 = v86[v90];	// L137
    v88[v90] = v91;	// L138
  }
  for (int v92 = 0; v92 < 8; v92 += 1) {	// L140
    #pragma HLS pipeline II=1
    float v93 = v85[v92];	// L141
    v89[v92] = v93;	// L142
  }
  for (int v94 = 0; v94 < 8; v94 += 1) {	// L144
    _Z19softmax_8_128_floatPA128_fS0__node2(v84, v89, v94);	// L145
    hls::stream<float> v95;	// L146
    hls::stream<float> v96;	// L147
    _Z19softmax_8_128_floatPA128_fS0__node1(v89, v84, v87, v88, v95, v96, v94);	// L148
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node3(
  float v97[8]
) {	// L152
  #pragma HLS inline
  for (int v98 = 0; v98 < 8; v98 += 1) {	// L154
    #pragma HLS pipeline II=1
    v97[v98] = (float)0.000000;	// L155
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node4(
  float v99[8][128],
  float v100[8]
) {	// L159
  #pragma HLS inline
  for (int v101 = 0; v101 < 8; v101 += 1) {	// L160
    #pragma HLS pipeline II=1
    float v102 = v99[v101][0];	// L161
    v100[v101] = v102;	// L162
  }
}

void _Z19softmax_8_128_floatPA128_fS0_(
  float v103[8][128],
  float v104[8][128]
) {	// L166
  float v105[8];	// L167
  #pragma HLS bind_storage variable=v105 type=ram_t2p impl=bram

  float v106[8];	// L168
  #pragma HLS bind_storage variable=v106 type=ram_t2p impl=bram

  _Z19softmax_8_128_floatPA128_fS0__node4(v103, v105);	// L169
  _Z19softmax_8_128_floatPA128_fS0__node3(v106);	// L170
  _Z19softmax_8_128_floatPA128_fS0__node0(v103, v105, v106, v104);	// L171
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v107[8][128],
  float v108[8][128]
) {	// L174
  #pragma HLS inline
  for (int v109 = 0; v109 < 8; v109 += 1) {	// L175
    for (int v110 = 0; v110 < 128; v110 += 1) {	// L176
      #pragma HLS pipeline II=1
      float v111 = v107[v109][v110];	// L177
      v108[v109][v110] = v111;	// L178
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v112[8][128],
  float v113[8][128]
) {	// L183
  _Z17store_8_128_floatPA128_fS0__node0(v112, v113);	// L184
}

