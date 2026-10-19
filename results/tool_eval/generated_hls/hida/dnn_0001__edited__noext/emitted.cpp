
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
  for (int v10 = 0; v10 < 16; v10 += 1) {	// L19
    for (int v11 = 0; v11 < 58; v11 += 1) {	// L20
      for (int v12 = 0; v12 < 58; v12 += 1) {	// L21
        #pragma HLS pipeline II=1
        float v13 = v8[v10][v11][v12];	// L22
        v9[v10][v11][v12] = v13;	// L23
      }
    }
  }
}

void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node2(
  float v14[16][58][58],
  float v15[4][16],
  float v16[16][58][58]
) {	// L29
  #pragma HLS inline
  float v17;	// L31
  float v18;	// L32
  float v19;	// L33
  float v20;	// L34
  for (int v21 = 0; v21 < 16; v21 += 1) {	// L35
    for (int v22 = 0; v22 < 58; v22 += 1) {	// L36
      for (int v23 = 0; v23 < 58; v23 += 1) {	// L37
        #pragma HLS pipeline II=1
        float v24 = v15[2][v21];	// L38
        float v25 = v17;	// L39
        float v26 = (v23 == 0) ? v24 : v25;	// L40
        v17 = v26;	// L41
        float v27 = v15[3][v21];	// L42
        float v28 = v27 + (float)0.000010;	// L43
        float v29 = sqrt(v28);	// L44
        float v30 = v18;	// L45
        float v31 = (v23 == 0) ? v29 : v30;	// L46
        v18 = v31;	// L47
        float v32 = v15[0][v21];	// L48
        float v33 = v19;	// L49
        float v34 = (v23 == 0) ? v32 : v33;	// L50
        v19 = v34;	// L51
        float v35 = v15[1][v21];	// L52
        float v36 = v20;	// L53
        float v37 = (v23 == 0) ? v35 : v36;	// L54
        v20 = v37;	// L55
        float v38 = v14[v21][v22][v23];	// L56
        float v39 = v38 - v26;	// L57
        float v40 = v39 / v31;	// L58
        float v41 = v34 * v40;	// L59
        float v42 = v41 + v37;	// L60
        v16[v21][v22][v23] = v42;	// L61
      }
    }
  }
}

void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node3(
  float v43[16][58][58],
  float v44[16][58][58]
) {	// L67
  #pragma HLS inline
  for (int v45 = 0; v45 < 16; v45 += 1) {	// L68
    for (int v46 = 0; v46 < 58; v46 += 1) {	// L69
      for (int v47 = 0; v47 < 58; v47 += 1) {	// L70
        #pragma HLS pipeline II=1
        float v48 = v43[v45][v46][v47];	// L71
        v44[v45][v46][v47] = v48;	// L72
      }
    }
  }
}

void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node4(
  float v49[16][56][56],
  float v50[16][16],
  float v51[16][58][58],
  float v52[16][58][58]
) {	// L78
  #pragma HLS inline
  for (int v53 = 0; v53 < 16; v53 += 1) {	// L79
    for (int v54 = 0; v54 < 58; v54 += 1) {	// L80
      for (int v55 = 0; v55 < 58; v55 += 1) {	// L81
        for (int v56 = 0; v56 < 16; v56 += 1) {	// L82
          #pragma HLS pipeline II=1
          if (((-v55) + 56) >= 0 && (v55 - 1) >= 0 && (v54 - 1) >= 0 && ((-v54) + 56) >= 0) {	// L83
            float v57 = v49[v53][(v54 - 1)][(v55 - 1)];	// L84
            float v58 = v50[v56][v53];	// L85
            float v59 = v57 * v58;	// L86
            float v60 = v51[v56][v54][v55];	// L87
            float v61 = v52[v56][v54][v55];	// L88
            float v62 = (v53 == 0) ? v60 : v61;	// L89
            float v63 = v62 + v59;	// L90
            v52[v56][v54][v55] = v63;	// L91
          }
        }
      }
    }
  }
}

void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node5(
  float v64[16],
  float v65[16][58][58]
) {	// L99
  #pragma HLS inline
  for (int v66 = 0; v66 < 58; v66 += 1) {	// L100
    for (int v67 = 0; v67 < 58; v67 += 1) {	// L101
      for (int v68 = 0; v68 < 16; v68 += 1) {	// L102
        #pragma HLS pipeline II=1
        float v69 = v64[v68];	// L103
        v65[v68][v66][v67] = v69;	// L104
      }
    }
  }
}

/// This is top function.
void _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f(
  float v70[16][56][56],
  float v71[16][16],
  float v72[16],
  float v73[4][16],
  float v74[16][58][58]
) {	// L110
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v74 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_2p storage_impl=lutram port=v73 bundle=axi_3
  #pragma HLS array_partition variable=v73 cyclic factor=4 dim=1


  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v72 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v71 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v70 bundle=axi_0

  float v80[16][58][58];	// L121
  #pragma HLS bind_storage variable=v80 type=ram_t2p impl=bram

  float v81[16][58][58];	// L122
  #pragma HLS bind_storage variable=v81 type=ram_t2p impl=bram

  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node5(v72, v80);	// L123
  float v82[16][58][58];	// L124
  #pragma HLS bind_storage variable=v82 type=ram_t2p impl=bram

  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node4(v70, v71, v80, v82);	// L125
  float v83[16][58][58];	// L126
  #pragma HLS bind_storage variable=v83 type=ram_t2p impl=bram

  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node3(v82, v83);	// L127
  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node2(v83, v73, v81);	// L128
  float v84[16][58][58];	// L129
  #pragma HLS bind_storage variable=v84 type=ram_t2p impl=bram

  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node1(v81, v84);	// L130
  _Z3topPA56_A56_fPA16_A1_A1_fPfPA16_fPA58_A58_f_node0(v84, v74);	// L131
}

void _Z19load_16_56_56_floatPA56_A56_fS1__node0(
  float v85[16][56][56],
  float v86[16][56][56]
) {	// L143
  #pragma HLS inline
  for (int v87 = 0; v87 < 16; v87 += 1) {	// L144
    for (int v88 = 0; v88 < 56; v88 += 1) {	// L145
      for (int v89 = 0; v89 < 56; v89 += 1) {	// L146
        #pragma HLS pipeline II=1
        float v90 = v85[v87][v88][v89];	// L147
        v86[v87][v88][v89] = v90;	// L148
      }
    }
  }
}

void _Z19load_16_56_56_floatPA56_A56_fS1_(
  float v91[16][56][56],
  float v92[16][56][56]
) {	// L154
  _Z19load_16_56_56_floatPA56_A56_fS1__node0(v91, v92);	// L155
}

void _Z20load_16_16_1_1_floatPA16_A1_A1_fS2__node0(
  float v93[16][16],
  float v94[16][16]
) {	// L158
  #pragma HLS inline
  for (int v95 = 0; v95 < 16; v95 += 1) {	// L159
    for (int v96 = 0; v96 < 16; v96 += 1) {	// L160
      #pragma HLS pipeline II=1
      float v97 = v93[v95][v96];	// L161
      v94[v95][v96] = v97;	// L162
    }
  }
}

void _Z20load_16_16_1_1_floatPA16_A1_A1_fS2_(
  float v98[16][16],
  float v99[16][16]
) {	// L167
  _Z20load_16_16_1_1_floatPA16_A1_A1_fS2__node0(v98, v99);	// L168
}

void _Z13load_16_floatPfS__node0(
  float v100[16],
  float v101[16]
) {	// L171
  #pragma HLS inline
  for (int v102 = 0; v102 < 16; v102 += 1) {	// L172
    #pragma HLS pipeline II=1
    float v103 = v100[v102];	// L173
    v101[v102] = v103;	// L174
  }
}

void _Z13load_16_floatPfS_(
  float v104[16],
  float v105[16]
) {	// L178
  _Z13load_16_floatPfS__node0(v104, v105);	// L179
}

void _Z15load_4_16_floatPA16_fS0__node0(
  float v106[4][16],
  float v107[4][16]
) {	// L182
  #pragma HLS inline
  for (int v108 = 0; v108 < 4; v108 += 1) {	// L183
    for (int v109 = 0; v109 < 16; v109 += 1) {	// L184
      #pragma HLS pipeline II=1
      float v110 = v106[v108][v109];	// L185
      v107[v108][v109] = v110;	// L186
    }
  }
}

void _Z15load_4_16_floatPA16_fS0_(
  float v111[4][16],
  float v112[4][16]
) {	// L191
  _Z15load_4_16_floatPA16_fS0__node0(v111, v112);	// L192
}

void _Z41conv2d_16_16_56_56_58_58_1_1_1_float_biasPA56_A56_fPA16_A1_A1_fPfPA58_A58_f_node0(
  float v113[16],
  float v114[16][56][56],
  float v115[16][16],
  float v116[16][58][58]
) {	// L195
  for (int v117 = 0; v117 < 58; v117 += 1) {	// L196
    for (int v118 = 0; v118 < 58; v118 += 1) {	// L197
      for (int v119 = 0; v119 < 16; v119 += 1) {	// L198
        #pragma HLS pipeline II=1
        float v120 = v113[v119];	// L199
        v116[v119][v117][v118] = v120;	// L200
      }
    }
  }
  for (int v121 = 0; v121 < 16; v121 += 1) {	// L204
    for (int v122 = 0; v122 < 58; v122 += 1) {	// L205
      for (int v123 = 0; v123 < 58; v123 += 1) {	// L206
        for (int v124 = 0; v124 < 16; v124 += 1) {	// L207
          #pragma HLS pipeline II=1
          if (((-v123) + 56) >= 0 && (v123 - 1) >= 0 && (v122 - 1) >= 0 && ((-v122) + 56) >= 0) {	// L208
            float v125 = v114[v121][(v122 - 1)][(v123 - 1)];	// L209
            float v126 = v115[v124][v121];	// L210
            float v127 = v125 * v126;	// L211
            float v128 = v116[v124][v122][v123];	// L212
            float v129 = v128 + v127;	// L213
            v116[v124][v122][v123] = v129;	// L214
          }
        }
      }
    }
  }
}

void _Z41conv2d_16_16_56_56_58_58_1_1_1_float_biasPA56_A56_fPA16_A1_A1_fPfPA58_A58_f(
  float v130[16][56][56],
  float v131[16][16],
  float v132[16],
  float v133[16][58][58]
) {	// L222
  _Z41conv2d_16_16_56_56_58_58_1_1_1_float_biasPA56_A56_fPA16_A1_A1_fPfPA58_A58_f_node0(v132, v130, v131, v133);	// L223
}

void _Z25batch_norm_16_58_58_floatPA58_A58_fPA16_fS1__node0(
  float v134[16][58][58],
  float v135[4][16],
  float v136[16][58][58]
) {	// L226
  #pragma HLS inline
  for (int v137 = 0; v137 < 16; v137 += 1) {	// L228
    for (int v138 = 0; v138 < 58; v138 += 1) {	// L229
      for (int v139 = 0; v139 < 58; v139 += 1) {	// L230
        #pragma HLS pipeline II=1
        float v140 = v134[v137][v138][v139];	// L231
        float v141 = v135[2][v137];	// L232
        float v142 = v140 - v141;	// L233
        float v143 = v135[3][v137];	// L234
        float v144 = v143 + (float)0.000010;	// L235
        float v145 = sqrt(v144);	// L236
        float v146 = v142 / v145;	// L237
        float v147 = v135[0][v137];	// L238
        float v148 = v147 * v146;	// L239
        float v149 = v135[1][v137];	// L240
        float v150 = v148 + v149;	// L241
        v136[v137][v138][v139] = v150;	// L242
      }
    }
  }
}

void _Z25batch_norm_16_58_58_floatPA58_A58_fPA16_fS1_(
  float v151[16][58][58],
  float v152[4][16],
  float v153[16][58][58]
) {	// L248
  _Z25batch_norm_16_58_58_floatPA58_A58_fPA16_fS1__node0(v151, v152, v153);	// L249
}

void _Z19relu_16_58_58_floatPA58_A58_fS1__node0(
  float v154[16][58][58],
  float v155[16][58][58]
) {	// L252
  #pragma HLS inline
  for (int v156 = 0; v156 < 16; v156 += 1) {	// L254
    for (int v157 = 0; v157 < 58; v157 += 1) {	// L255
      for (int v158 = 0; v158 < 58; v158 += 1) {	// L256
        #pragma HLS pipeline II=1
        float v159 = v154[v156][v157][v158];	// L257
        bool v160 = v159 > (float)0.000000;	// L258
        float v161 = v160 ? v159 : (float)0.000000;	// L259
        v155[v156][v157][v158] = v161;	// L260
      }
    }
  }
}

void _Z19relu_16_58_58_floatPA58_A58_fS1_(
  float v162[16][58][58],
  float v163[16][58][58]
) {	// L266
  _Z19relu_16_58_58_floatPA58_A58_fS1__node0(v162, v163);	// L267
}

void _Z20store_16_58_58_floatPA58_A58_fS1__node0(
  float v164[16][58][58],
  float v165[16][58][58]
) {	// L270
  #pragma HLS inline
  for (int v166 = 0; v166 < 16; v166 += 1) {	// L271
    for (int v167 = 0; v167 < 58; v167 += 1) {	// L272
      for (int v168 = 0; v168 < 58; v168 += 1) {	// L273
        #pragma HLS pipeline II=1
        float v169 = v164[v166][v167][v168];	// L274
        v165[v166][v167][v168] = v169;	// L275
      }
    }
  }
}

void _Z20store_16_58_58_floatPA58_A58_fS1_(
  float v170[16][58][58],
  float v171[16][58][58]
) {	// L281
  _Z20store_16_58_58_floatPA58_A58_fS1__node0(v170, v171);	// L282
}

