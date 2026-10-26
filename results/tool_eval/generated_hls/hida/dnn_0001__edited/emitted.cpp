
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

void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node0(
  float v0[16][58][58],
  float v1[16][58][58]
) {	// L4
  #pragma HLS inline
  for (int v2 = 0; v2 < 16; v2 += 1) {	// L6
    for (int v3 = 0; v3 < 58; v3 += 1) {	// L7
      for (int v4 = 0; v4 < 58; v4 += 1) {	// L8
        #pragma HLS pipeline II=1
        float v5 = v0[v2][v3][v4];	// L9
        bool v6 = v5 > (float)0.000000;	// L10
        float v7 = v6 ? v5 : (float)0.000000;	// L11
        v1[v2][v3][v4] = v7;	// L12
      }
    }
  }
}

void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node1(
  float v8[16][58][58],
  float v9[16][58][58]
) {	// L18
  #pragma HLS inline
  for (int v10 = 0; v10 < 16; v10 += 1) {	// L20
    for (int v11 = 0; v11 < 58; v11 += 1) {	// L21
      for (int v12 = 0; v12 < 58; v12 += 1) {	// L22
        #pragma HLS pipeline II=1
        float v13 = v8[v10][v11][v12];	// L23
        bool v14 = v13 > (float)0.000000;	// L24
        float v15 = v14 ? v13 : (float)0.000000;	// L25
        v9[v10][v11][v12] = v15;	// L26
      }
    }
  }
}

void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node2(
  float v16[4][16],
  float v17[16][58][58],
  float v18[16][58][58]
) {	// L32
  #pragma HLS inline
  float v19;	// L34
  float v20;	// L35
  float v21;	// L36
  float v22;	// L37
  for (int v23 = 0; v23 < 16; v23 += 1) {	// L38
    for (int v24 = 0; v24 < 58; v24 += 1) {	// L39
      for (int v25 = 0; v25 < 58; v25 += 1) {	// L40
        #pragma HLS pipeline II=1
        float v26 = v16[2][v23];	// L41
        float v27 = v19;	// L42
        float v28 = (v25 == 0) ? v26 : v27;	// L43
        v19 = v28;	// L44
        float v29 = v16[3][v23];	// L45
        float v30 = v29 + (float)0.000010;	// L46
        float v31 = sqrt(v30);	// L47
        float v32 = v20;	// L48
        float v33 = (v25 == 0) ? v31 : v32;	// L49
        v20 = v33;	// L50
        float v34 = v16[0][v23];	// L51
        float v35 = v21;	// L52
        float v36 = (v25 == 0) ? v34 : v35;	// L53
        v21 = v36;	// L54
        float v37 = v16[1][v23];	// L55
        float v38 = v22;	// L56
        float v39 = (v25 == 0) ? v37 : v38;	// L57
        v22 = v39;	// L58
        float v40 = v17[v23][v24][v25];	// L59
        float v41 = v40 - v28;	// L60
        float v42 = v41 / v33;	// L61
        float v43 = v36 * v42;	// L62
        float v44 = v43 + v39;	// L63
        v18[v23][v24][v25] = v44;	// L64
      }
    }
  }
}

void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node3(
  float v45[16][56][56],
  float v46[16][16],
  float v47[16][58][58],
  float v48[16][58][58]
) {	// L70
  #pragma HLS inline
  for (int v49 = 0; v49 < 16; v49 += 1) {	// L71
    for (int v50 = 0; v50 < 58; v50 += 1) {	// L72
      for (int v51 = 0; v51 < 58; v51 += 1) {	// L73
        for (int v52 = 0; v52 < 16; v52 += 1) {	// L74
          #pragma HLS pipeline II=1
          if (((-v51) + 56) >= 0 && (v51 - 1) >= 0 && (v50 - 1) >= 0 && ((-v50) + 56) >= 0) {	// L75
            float v53 = v45[v49][(v50 - 1)][(v51 - 1)];	// L76
            float v54 = v46[v52][v49];	// L77
            float v55 = v53 * v54;	// L78
            float v56 = v47[v52][v50][v51];	// L79
            float v57 = v48[v52][v50][v51];	// L80
            float v58 = (v49 == 0) ? v56 : v57;	// L81
            float v59 = v58 + v55;	// L82
            v48[v52][v50][v51] = v59;	// L83
          }
        }
      }
    }
  }
}

void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node4(
  float v60[16],
  float v61[16][58][58]
) {	// L91
  #pragma HLS inline
  for (int v62 = 0; v62 < 58; v62 += 1) {	// L92
    for (int v63 = 0; v63 < 58; v63 += 1) {	// L93
      for (int v64 = 0; v64 < 16; v64 += 1) {	// L94
        #pragma HLS pipeline II=1
        float v65 = v60[v64];	// L95
        v61[v64][v62][v63] = v65;	// L96
      }
    }
  }
}

/// This is top function.
void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f(
  float v66[16][56][56],
  float v67[16][16],
  float v68[16],
  float v69[4][16],
  float v70[16][58][58],
  float v71[16][58][58],
  float v72[16][58][58],
  float v73[16][58][58],
  float v74[16][58][58],
  float v75[16][58][58],
  float v76[16][58][58],
  float v77[16][58][58],
  float v78[16][58][58]
) {	// L102
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v78 bundle=axi_12

  #pragma HLS interface m_axi offset=slave port=v77 bundle=axi_11

  #pragma HLS interface m_axi offset=slave port=v76 bundle=axi_10

  #pragma HLS interface m_axi offset=slave port=v75 bundle=axi_9

  #pragma HLS interface m_axi offset=slave port=v74 bundle=axi_8

  #pragma HLS interface m_axi offset=slave port=v73 bundle=axi_7

  #pragma HLS interface m_axi offset=slave port=v72 bundle=axi_6

  #pragma HLS interface m_axi offset=slave port=v71 bundle=axi_5

  #pragma HLS interface m_axi offset=slave port=v70 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_2p storage_impl=lutram port=v69 bundle=axi_3
  #pragma HLS array_partition variable=v69 cyclic factor=4 dim=1


  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v68 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v67 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v66 bundle=axi_0

  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node4(v68, v71);	// L129
  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node3(v66, v67, v72, v76);	// L130
  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node2(v69, v77, v73);	// L131
  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node1(v74, v78);	// L132
  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node0(v75, v70);	// L133
}

void _Z19load_16_56_56_floatPA56_A56_fS1__node0(
  float v92[16][56][56],
  float v93[16][56][56]
) {	// L157
  #pragma HLS inline
  for (int v94 = 0; v94 < 16; v94 += 1) {	// L158
    for (int v95 = 0; v95 < 56; v95 += 1) {	// L159
      for (int v96 = 0; v96 < 56; v96 += 1) {	// L160
        #pragma HLS pipeline II=1
        float v97 = v92[v94][v95][v96];	// L161
        v93[v94][v95][v96] = v97;	// L162
      }
    }
  }
}

void _Z19load_16_56_56_floatPA56_A56_fS1_(
  float v98[16][56][56],
  float v99[16][56][56]
) {	// L168
  _Z19load_16_56_56_floatPA56_A56_fS1__node0(v98, v99);	// L169
}

void _Z20load_16_16_1_1_floatPA16_A1_A1_fS2__node0(
  float v100[16][16],
  float v101[16][16]
) {	// L172
  #pragma HLS inline
  for (int v102 = 0; v102 < 16; v102 += 1) {	// L173
    for (int v103 = 0; v103 < 16; v103 += 1) {	// L174
      #pragma HLS pipeline II=1
      float v104 = v100[v102][v103];	// L175
      v101[v102][v103] = v104;	// L176
    }
  }
}

void _Z20load_16_16_1_1_floatPA16_A1_A1_fS2_(
  float v105[16][16],
  float v106[16][16]
) {	// L181
  _Z20load_16_16_1_1_floatPA16_A1_A1_fS2__node0(v105, v106);	// L182
}

void _Z13load_16_floatPfS__node0(
  float v107[16],
  float v108[16]
) {	// L185
  #pragma HLS inline
  for (int v109 = 0; v109 < 16; v109 += 1) {	// L186
    #pragma HLS pipeline II=1
    float v110 = v107[v109];	// L187
    v108[v109] = v110;	// L188
  }
}

void _Z13load_16_floatPfS_(
  float v111[16],
  float v112[16]
) {	// L192
  _Z13load_16_floatPfS__node0(v111, v112);	// L193
}

void _Z15load_4_16_floatPA16_fS0__node0(
  float v113[4][16],
  float v114[4][16]
) {	// L196
  #pragma HLS inline
  for (int v115 = 0; v115 < 4; v115 += 1) {	// L197
    for (int v116 = 0; v116 < 16; v116 += 1) {	// L198
      #pragma HLS pipeline II=1
      float v117 = v113[v115][v116];	// L199
      v114[v115][v116] = v117;	// L200
    }
  }
}

void _Z15load_4_16_floatPA16_fS0_(
  float v118[4][16],
  float v119[4][16]
) {	// L205
  _Z15load_4_16_floatPA16_fS0__node0(v118, v119);	// L206
}

void _Z41conv2d_16_16_56_56_58_58_1_1_1_float_biasPA56_A56_fPA16_A1_A1_fPfPA58_A58_f_node0(
  float v120[16],
  float v121[16][56][56],
  float v122[16][16],
  float v123[16][58][58]
) {	// L209
  for (int v124 = 0; v124 < 58; v124 += 1) {	// L210
    for (int v125 = 0; v125 < 58; v125 += 1) {	// L211
      for (int v126 = 0; v126 < 16; v126 += 1) {	// L212
        #pragma HLS pipeline II=1
        float v127 = v120[v126];	// L213
        v123[v126][v124][v125] = v127;	// L214
      }
    }
  }
  for (int v128 = 0; v128 < 16; v128 += 1) {	// L218
    for (int v129 = 0; v129 < 58; v129 += 1) {	// L219
      for (int v130 = 0; v130 < 58; v130 += 1) {	// L220
        for (int v131 = 0; v131 < 16; v131 += 1) {	// L221
          #pragma HLS pipeline II=1
          if (((-v130) + 56) >= 0 && (v130 - 1) >= 0 && (v129 - 1) >= 0 && ((-v129) + 56) >= 0) {	// L222
            float v132 = v121[v128][(v129 - 1)][(v130 - 1)];	// L223
            float v133 = v122[v131][v128];	// L224
            float v134 = v132 * v133;	// L225
            float v135 = v123[v131][v129][v130];	// L226
            float v136 = v135 + v134;	// L227
            v123[v131][v129][v130] = v136;	// L228
          }
        }
      }
    }
  }
}

void _Z41conv2d_16_16_56_56_58_58_1_1_1_float_biasPA56_A56_fPA16_A1_A1_fPfPA58_A58_f(
  float v137[16][56][56],
  float v138[16][16],
  float v139[16],
  float v140[16][58][58]
) {	// L236
  _Z41conv2d_16_16_56_56_58_58_1_1_1_float_biasPA56_A56_fPA16_A1_A1_fPfPA58_A58_f_node0(v139, v137, v138, v140);	// L237
}

void _Z25batch_norm_16_58_58_floatPA58_A58_fPA16_fS1__node0(
  float v141[4][16],
  float v142[16][58][58],
  float v143[16][58][58]
) {	// L240
  #pragma HLS inline
  for (int v144 = 0; v144 < 16; v144 += 1) {	// L242
    for (int v145 = 0; v145 < 58; v145 += 1) {	// L243
      for (int v146 = 0; v146 < 58; v146 += 1) {	// L244
        #pragma HLS pipeline II=1
        float v147 = v142[v144][v145][v146];	// L245
        float v148 = v141[2][v144];	// L246
        float v149 = v147 - v148;	// L247
        float v150 = v141[3][v144];	// L248
        float v151 = v150 + (float)0.000010;	// L249
        float v152 = sqrt(v151);	// L250
        float v153 = v149 / v152;	// L251
        float v154 = v141[0][v144];	// L252
        float v155 = v154 * v153;	// L253
        float v156 = v141[1][v144];	// L254
        float v157 = v155 + v156;	// L255
        v143[v144][v145][v146] = v157;	// L256
      }
    }
  }
}

void _Z25batch_norm_16_58_58_floatPA58_A58_fPA16_fS1_(
  float v158[16][58][58],
  float v159[4][16],
  float v160[16][58][58]
) {	// L262
  _Z25batch_norm_16_58_58_floatPA58_A58_fPA16_fS1__node0(v159, v158, v160);	// L263
}

void _Z19relu_16_58_58_floatPA58_A58_fS1__node0(
  float v161[16][58][58],
  float v162[16][58][58]
) {	// L266
  #pragma HLS inline
  for (int v163 = 0; v163 < 16; v163 += 1) {	// L268
    for (int v164 = 0; v164 < 58; v164 += 1) {	// L269
      for (int v165 = 0; v165 < 58; v165 += 1) {	// L270
        #pragma HLS pipeline II=1
        float v166 = v161[v163][v164][v165];	// L271
        bool v167 = v166 > (float)0.000000;	// L272
        float v168 = v167 ? v166 : (float)0.000000;	// L273
        v162[v163][v164][v165] = v168;	// L274
      }
    }
  }
}

void _Z19relu_16_58_58_floatPA58_A58_fS1_(
  float v169[16][58][58],
  float v170[16][58][58]
) {	// L280
  _Z19relu_16_58_58_floatPA58_A58_fS1__node0(v169, v170);	// L281
}

void _Z20store_16_58_58_floatPA58_A58_fS1__node0(
  float v171[16][58][58],
  float v172[16][58][58]
) {	// L284
  #pragma HLS inline
  for (int v173 = 0; v173 < 16; v173 += 1) {	// L285
    for (int v174 = 0; v174 < 58; v174 += 1) {	// L286
      for (int v175 = 0; v175 < 58; v175 += 1) {	// L287
        #pragma HLS pipeline II=1
        float v176 = v171[v173][v174][v175];	// L288
        v172[v173][v174][v175] = v176;	// L289
      }
    }
  }
}

void _Z20store_16_58_58_floatPA58_A58_fS1_(
  float v177[16][58][58],
  float v178[16][58][58]
) {	// L295
  _Z20store_16_58_58_floatPA58_A58_fS1__node0(v177, v178);	// L296
}

