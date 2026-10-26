
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
  float v42[8]
) {	// L61
  #pragma HLS inline
  for (int v43 = 0; v43 < 8; v43 += 1) {	// L62
    #pragma HLS pipeline II=1
    float v44 = v41[v43];	// L63
    v42[v43] = v44;	// L64
  }
}

void _Z3topPA128_fS0__node5(
  float v45[8],
  float v46[8],
  float v47[8]
) {	// L68
  for (int v48 = 0; v48 < 8; v48 += 1) {	// L69
    #pragma HLS pipeline II=1
    float v49 = v45[v48];	// L70
    v46[v48] = v49;	// L71
  }
  for (int v50 = 0; v50 < 8; v50 += 1) {	// L73
    #pragma HLS pipeline II=1
    float v51 = v45[v50];	// L74
    v47[v50] = v51;	// L75
  }
}

void _Z3topPA128_fS0__node6(
  float v52[8][128],
  float v53[8],
  float v54[8]
) {	// L79
  #pragma HLS inline
  for (int v55 = 0; v55 < 127; v55 += 1) {	// L80
    for (int v56 = 0; v56 < 8; v56 += 1) {	// L81
      #pragma HLS pipeline II=1
      float v57 = v52[v56][(v55 + 1)];	// L82
      float v58 = v53[v56];	// L83
      float v59 = v54[v56];	// L84
      float v60 = (v55 == 0) ? v58 : v59;	// L85
      bool v61 = v57 > v60;	// L86
      float v62 = v61 ? v57 : v60;	// L87
      v54[v56] = v62;	// L88
    }
  }
}

void _Z3topPA128_fS0__node7(
  float v63[8][128],
  float v64[8]
) {	// L93
  #pragma HLS inline
  for (int v65 = 0; v65 < 8; v65 += 1) {	// L94
    #pragma HLS pipeline II=1
    float v66 = v63[v65][0];	// L95
    v64[v65] = v66;	// L96
  }
}

/// This is top function.
void _Z3topPA128_fS0_(
  float v67[8][128],
  float v68[8][128],
  float v69[8][128],
  float v70[8][128],
  float v71[8][128],
  float v72[8][128]
) {	// L100
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v72 bundle=axi_5

  #pragma HLS interface m_axi offset=slave port=v71 bundle=axi_4

  #pragma HLS interface m_axi offset=slave port=v70 bundle=axi_3

  #pragma HLS interface m_axi offset=slave port=v69 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v68 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v67 bundle=axi_0

  float v79[8];	// L113
  #pragma HLS bind_storage variable=v79 type=ram_t2p impl=bram

  float v80[8];	// L114
  #pragma HLS bind_storage variable=v80 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node7(v70, v79);	// L115
  float v81[8];	// L116
  #pragma HLS bind_storage variable=v81 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node6(v69, v79, v81);	// L117
  float v82[8];	// L118
  #pragma HLS bind_storage variable=v82 type=ram_t2p impl=bram

  float v83[8];	// L119
  #pragma HLS bind_storage variable=v83 type=ram_t2p impl=bram

  float v84[8];	// L120
  #pragma HLS bind_storage variable=v84 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node5(v81, v82, v83);	// L121
  _Z3topPA128_fS0__node4(v83, v84);	// L122
  _Z3topPA128_fS0__node3(v80);	// L123
  _Z3topPA128_fS0__node2(v84, v68, v72);	// L124
  float v85[8];	// L125
  #pragma HLS bind_storage variable=v85 type=ram_t2p impl=bram

  _Z3topPA128_fS0__node1(v82, v67, v80, v85);	// L126
  _Z3topPA128_fS0__node0(v85, v71);	// L127
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v86[8][128],
  float v87[8][128]
) {	// L140
  #pragma HLS inline
  for (int v88 = 0; v88 < 8; v88 += 1) {	// L141
    for (int v89 = 0; v89 < 128; v89 += 1) {	// L142
      #pragma HLS pipeline II=1
      float v90 = v86[v88][v89];	// L143
      v87[v88][v89] = v90;	// L144
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v91[8][128],
  float v92[8][128]
) {	// L149
  _Z16load_8_128_floatPA128_fS0__node0(v91, v92);	// L150
}

void _Z19softmax_8_128_floatPA128_fS0__node0(
  float v93[8],
  float v94[8][128]
) {	// L153
  #pragma HLS inline
  float v95;	// L154
  for (int v96 = 0; v96 < 8; v96 += 1) {	// L155
    for (int v97 = 0; v97 < 128; v97 += 1) {	// L156
      #pragma HLS pipeline II=1
      float v98 = v93[v96];	// L157
      float v99 = v95;	// L158
      float v100 = (v97 == 0) ? v98 : v99;	// L159
      v95 = v100;	// L160
      float v101 = v94[v96][v97];	// L161
      float v102 = v101 / v100;	// L162
      v94[v96][v97] = v102;	// L163
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node1(
  float v103[8][128],
  float v104[8],
  float v105[8]
) {	// L168
  #pragma HLS inline
  for (int v106 = 0; v106 < 128; v106 += 1) {	// L169
    for (int v107 = 0; v107 < 8; v107 += 1) {	// L170
      #pragma HLS pipeline II=1
      float v108 = v103[v107][v106];	// L171
      float v109 = v104[v107];	// L172
      float v110 = v105[v107];	// L173
      float v111 = (v106 == 0) ? v109 : v110;	// L174
      float v112 = v111 + v108;	// L175
      v105[v107] = v112;	// L176
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node2(
  float v113[8],
  float v114[8][128],
  float v115[8][128]
) {	// L181
  #pragma HLS inline
  float v116;	// L182
  for (int v117 = 0; v117 < 8; v117 += 1) {	// L183
    for (int v118 = 0; v118 < 128; v118 += 1) {	// L184
      #pragma HLS pipeline II=1
      float v119 = v113[v117];	// L185
      float v120 = v116;	// L186
      float v121 = (v118 == 0) ? v119 : v120;	// L187
      v116 = v121;	// L188
      float v122 = v114[v117][v118];	// L189
      float v123 = v122 - v121;	// L190
      float v124 = exp(v123);	// L191
      v115[v117][v118] = v124;	// L192
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node3(
  float v125[8]
) {	// L197
  #pragma HLS inline
  for (int v126 = 0; v126 < 8; v126 += 1) {	// L199
    #pragma HLS pipeline II=1
    v125[v126] = (float)0.000000;	// L200
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node4(
  float v127[8][128],
  float v128[8],
  float v129[8]
) {	// L204
  #pragma HLS inline
  for (int v130 = 0; v130 < 127; v130 += 1) {	// L205
    for (int v131 = 0; v131 < 8; v131 += 1) {	// L206
      #pragma HLS pipeline II=1
      float v132 = v127[v131][(v130 + 1)];	// L207
      float v133 = v128[v131];	// L208
      float v134 = v129[v131];	// L209
      float v135 = (v130 == 0) ? v133 : v134;	// L210
      bool v136 = v132 > v135;	// L211
      float v137 = v136 ? v132 : v135;	// L212
      v129[v131] = v137;	// L213
    }
  }
}

void _Z19softmax_8_128_floatPA128_fS0__node5(
  float v138[8][128],
  float v139[8]
) {	// L218
  #pragma HLS inline
  for (int v140 = 0; v140 < 8; v140 += 1) {	// L219
    #pragma HLS pipeline II=1
    float v141 = v138[v140][0];	// L220
    v139[v140] = v141;	// L221
  }
}

void _Z19softmax_8_128_floatPA128_fS0_(
  float v142[8][128],
  float v143[8][128]
) {	// L225
  float v144[8];	// L226
  #pragma HLS bind_storage variable=v144 type=ram_t2p impl=bram

  float v145[8];	// L227
  #pragma HLS bind_storage variable=v145 type=ram_t2p impl=bram

  _Z19softmax_8_128_floatPA128_fS0__node5(v142, v144);	// L228
  float v146[8];	// L229
  #pragma HLS bind_storage variable=v146 type=ram_t2p impl=bram

  _Z19softmax_8_128_floatPA128_fS0__node4(v142, v144, v146);	// L230
  _Z19softmax_8_128_floatPA128_fS0__node3(v145);	// L231
  _Z19softmax_8_128_floatPA128_fS0__node2(v146, v142, v143);	// L232
  float v147[8];	// L233
  #pragma HLS bind_storage variable=v147 type=ram_t2p impl=bram

  _Z19softmax_8_128_floatPA128_fS0__node1(v143, v145, v147);	// L234
  _Z19softmax_8_128_floatPA128_fS0__node0(v147, v143);	// L235
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v148[8][128],
  float v149[8][128]
) {	// L238
  #pragma HLS inline
  for (int v150 = 0; v150 < 8; v150 += 1) {	// L239
    for (int v151 = 0; v151 < 128; v151 += 1) {	// L240
      #pragma HLS pipeline II=1
      float v152 = v148[v150][v151];	// L241
      v149[v150][v151] = v152;	// L242
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v153[8][128],
  float v154[8][128]
) {	// L247
  _Z17store_8_128_floatPA128_fS0__node0(v153, v154);	// L248
}

