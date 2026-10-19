
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

void _Z3topPA128_fS0__node0(
  float v0[8],
  float v1[8][128]
) {	// L3
  #pragma HLS inline
  float v2;	// L4
  for (int v3 = 0; v3 < 8; v3 += 1) {	// L5
    for (int v4 = 0; v4 < 128; v4 += 1) {	// L6
      #pragma HLS pipeline II=1
      float v5 = v0[v3];	// L7
      float v6 = v2;	// L8
      float v7 = (v4 == 0) ? v5 : v6;	// L9
      v2 = v7;	// L10
      float v8 = v1[v3][v4];	// L11
      float v9 = v8 / v7;	// L12
      v1[v3][v4] = v9;	// L13
    }
  }
}

void _Z3topPA128_fS0__node1(
  float v10[8],
  float v11[8][128],
  float v12[8],
  float v13[8]
) {	// L18
  #pragma HLS inline
  float v14;	// L19
  for (int v15 = 0; v15 < 8; v15 += 1) {	// L20
    for (int v16 = 0; v16 < 128; v16 += 1) {	// L21
      #pragma HLS pipeline II=1
      float v17 = v10[v15];	// L22
      float v18 = v14;	// L23
      float v19 = (v16 == 0) ? v17 : v18;	// L24
      v14 = v19;	// L25
      float v20 = v11[v15][v16];	// L26
      float v21 = v20 - v19;	// L27
      float v22 = exp(v21);	// L28
      float v23 = v12[v15];	// L29
      float v24 = v13[v15];	// L30
      float v25 = (v16 == 0) ? v23 : v24;	// L31
      float v26 = v25 + v22;	// L32
      v13[v15] = v26;	// L33
    }
  }
}

void _Z3topPA128_fS0__node2(
  float v27[8],
  float v28[8][128],
  float v29[8][128]
) {	// L38
  #pragma HLS inline
  float v30;	// L39
  for (int v31 = 0; v31 < 8; v31 += 1) {	// L40
    for (int v32 = 0; v32 < 128; v32 += 1) {	// L41
      #pragma HLS pipeline II=1
      float v33 = v27[v31];	// L42
      float v34 = v30;	// L43
      float v35 = (v32 == 0) ? v33 : v34;	// L44
      v30 = v35;	// L45
      float v36 = v28[v31][v32];	// L46
      float v37 = v36 - v35;	// L47
      float v38 = exp(v37);	// L48
      v29[v31][v32] = v38;	// L49
    }
  }
}

void _Z3topPA128_fS0__node3(
  float v39[8]
) {	// L54
  #pragma HLS inline
  for (int v40 = 0; v40 < 8; v40 += 1) {	// L56
    #pragma HLS pipeline II=1
    v39[v40] = (float)0.000000;	// L57
  }
}

void _Z3topPA128_fS0__node4(
  float v41[8],
  float v42[8],
  float v43[8]
) {	// L61
  for (int v44 = 0; v44 < 8; v44 += 1) {	// L62
    #pragma HLS pipeline II=1
    float v45 = v41[v44];	// L63
    v42[v44] = v45;	// L64
  }
  for (int v46 = 0; v46 < 8; v46 += 1) {	// L66
    #pragma HLS pipeline II=1
    float v47 = v41[v46];	// L67
    v43[v46] = v47;	// L68
  }
}

void _Z3topPA128_fS0__node5(
  float v48[8][128],
  float v49[8],
  float v50[8]
) {	// L72
  #pragma HLS inline
  for (int v51 = 0; v51 < 127; v51 += 1) {	// L73
    for (int v52 = 0; v52 < 8; v52 += 1) {	// L74
      #pragma HLS pipeline II=1
      float v53 = v48[v52][(v51 + 1)];	// L75
      float v54 = v49[v52];	// L76
      float v55 = v50[v52];	// L77
      float v56 = (v51 == 0) ? v54 : v55;	// L78
      bool v57 = v53 > v56;	// L79
      float v58 = v57 ? v53 : v56;	// L80
      v50[v52] = v58;	// L81
    }
  }
}

void _Z3topPA128_fS0__node6(
  float v59[8][128],
  float v60[8]
) {	// L86
  #pragma HLS inline
  for (int v61 = 0; v61 < 8; v61 += 1) {	// L87
    #pragma HLS pipeline II=1
    float v62 = v59[v61][0];	// L88
    v60[v61] = v62;	// L89
  }
}

/// This is top function.
void _Z3topPA128_fS0_(
  float v63[8][128],
  float v64[8][128],
  float v65[8][128],
  float v66[8][128],
  float v67[8][128],
  float v68[8][128]
) {	// L93
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v68 bundle=axi_5

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v67 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v66 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v65 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v64 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v63 bundle=axi_0

  float v75[8];	// L106
  #pragma HLS bind_storage variable=v75 type=ram_t2p impl=bram

  float v76[8];	// L107
  #pragma HLS bind_storage variable=v76 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node6(v66, v75);	// L108
  float v77[8];	// L109
  #pragma HLS bind_storage variable=v77 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node5(v65, v75, v77);	// L110
  float v78[8];	// L111
  #pragma HLS bind_storage variable=v78 type=ram_t2p impl=bram

  float v79[8];	// L112
  #pragma HLS bind_storage variable=v79 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node4(v77, v78, v79);	// L113
  _Z3topPA128_fS0__node3(v76);	// L114
  _Z3topPA128_fS0__node2(v79, v64, v68);	// L115
  float v80[8];	// L116
  #pragma HLS bind_storage variable=v80 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node1(v78, v63, v76, v80);	// L117
  _Z3topPA128_fS0__node0(v80, v67);	// L118
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v81[8][128],
  float v82[8][128]
) {	// L131
  #pragma HLS inline
  for (int v83 = 0; v83 < 8; v83 += 1) {	// L132
    for (int v84 = 0; v84 < 128; v84 += 1) {	// L133
      #pragma HLS pipeline II=1
      float v85 = v81[v83][v84];	// L134
      v82[v83][v84] = v85;	// L135
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v86[8][128],
  float v87[8][128]
) {	// L140
  _Z16load_8_128_floatPA128_fS0__node0(v86, v87);	// L141
}

void _Z19softmax_8_128_floatPA128_fS0__node0(
  float v88[8],
  float v89[8][128]
) {	// L144
  #pragma HLS inline
  float v90;	// L145
  for (int v91 = 0; v91 < 8; v91 += 1) {	// L146
    for (int v92 = 0; v92 < 128; v92 += 1) {	// L147
      #pragma HLS pipeline II=1
      float v93 = v88[v91];	// L148
      float v94 = v90;	// L149
      float v95 = (v92 == 0) ? v93 : v94;	// L150
      v90 = v95;	// L151
      float v96 = v89[v91][v92];	// L152
      float v97 = v96 / v95;	// L153
      v89[v91][v92] = v97;	// L154
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node1(
  float v98[8][128],
  float v99[8],
  float v100[8]
) {	// L159
  #pragma HLS inline
  for (int v101 = 0; v101 < 128; v101 += 1) {	// L160
    for (int v102 = 0; v102 < 8; v102 += 1) {	// L161
      #pragma HLS pipeline II=1
      float v103 = v98[v102][v101];	// L162
      float v104 = v99[v102];	// L163
      float v105 = v100[v102];	// L164
      float v106 = (v101 == 0) ? v104 : v105;	// L165
      float v107 = v106 + v103;	// L166
      v100[v102] = v107;	// L167
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node2(
  float v108[8],
  float v109[8][128],
  float v110[8][128]
) {	// L172
  #pragma HLS inline
  float v111;	// L173
  for (int v112 = 0; v112 < 8; v112 += 1) {	// L174
    for (int v113 = 0; v113 < 128; v113 += 1) {	// L175
      #pragma HLS pipeline II=1
      float v114 = v108[v112];	// L176
      float v115 = v111;	// L177
      float v116 = (v113 == 0) ? v114 : v115;	// L178
      v111 = v116;	// L179
      float v117 = v109[v112][v113];	// L180
      float v118 = v117 - v116;	// L181
      float v119 = exp(v118);	// L182
      v110[v112][v113] = v119;	// L183
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node3(
  float v120[8]
) {	// L188
  #pragma HLS inline
  for (int v121 = 0; v121 < 8; v121 += 1) {	// L190
    #pragma HLS pipeline II=1
    v120[v121] = (float)0.000000;	// L191
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node4(
  float v122[8][128],
  float v123[8],
  float v124[8]
) {	// L195
  #pragma HLS inline
  for (int v125 = 0; v125 < 127; v125 += 1) {	// L196
    for (int v126 = 0; v126 < 8; v126 += 1) {	// L197
      #pragma HLS pipeline II=1
      float v127 = v122[v126][(v125 + 1)];	// L198
      float v128 = v123[v126];	// L199
      float v129 = v124[v126];	// L200
      float v130 = (v125 == 0) ? v128 : v129;	// L201
      bool v131 = v127 > v130;	// L202
      float v132 = v131 ? v127 : v130;	// L203
      v124[v126] = v132;	// L204
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node5(
  float v133[8][128],
  float v134[8]
) {	// L209
  #pragma HLS inline
  for (int v135 = 0; v135 < 8; v135 += 1) {	// L210
    #pragma HLS pipeline II=1
    float v136 = v133[v135][0];	// L211
    v134[v135] = v136;	// L212
  }
}

void _Z19softmax_8_128_floatPA128_fS0_(
  float v137[8][128],
  float v138[8][128]
) {	// L216
  float v139[8];	// L217
  #pragma HLS bind_storage variable=v139 type=ram_t2p impl=bram

  float v140[8];	// L218
  #pragma HLS bind_storage variable=v140 type=ram_t2p impl=bram

  _Z19softmax_8_128_floatPA128_fS0__node5(v137, v139);	// L219
  float v141[8];	// L220
  #pragma HLS bind_storage variable=v141 type=ram_t2p impl=bram

  _Z19softmax_8_128_floatPA128_fS0__node4(v137, v139, v141);	// L221
  _Z19softmax_8_128_floatPA128_fS0__node3(v140);	// L222
  _Z19softmax_8_128_floatPA128_fS0__node2(v141, v137, v138);	// L223
  float v142[8];	// L224
  #pragma HLS bind_storage variable=v142 type=ram_t2p impl=bram

  _Z19softmax_8_128_floatPA128_fS0__node1(v138, v140, v142);	// L225
  _Z19softmax_8_128_floatPA128_fS0__node0(v142, v138);	// L226
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v143[8][128],
  float v144[8][128]
) {	// L229
  #pragma HLS inline
  for (int v145 = 0; v145 < 8; v145 += 1) {	// L230
    for (int v146 = 0; v146 < 128; v146 += 1) {	// L231
      #pragma HLS pipeline II=1
      float v147 = v143[v145][v146];	// L232
      v144[v145][v146] = v147;	// L233
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v148[8][128],
  float v149[8][128]
) {	// L238
  _Z17store_8_128_floatPA128_fS0__node0(v148, v149);	// L239
}

