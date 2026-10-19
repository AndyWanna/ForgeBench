
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

void _Z3topPA128_fS0_S0_S0_S0__node1(
  float v0[8][8],
  float v1[8][128],
  float v2[8][128],
  int v3
) {	// L4
  for (int v4 = 0; v4 < 8; v4 += 1) {	// L6
    for (int v5 = 0; v5 < 16; v5 += 1) {	// L7
      #pragma HLS pipeline II=1
      v2[v4][(v5 + (v3 * 16))] = (float)0.000000;	// L8
    }
  }
  for (int v6 = 0; v6 < 8; v6 += 1) {	// L11
    for (int v7 = 0; v7 < 8; v7 += 1) {	// L12
      for (int v8 = 0; v8 < 16; v8 += 1) {	// L13
        #pragma HLS pipeline II=1
        float v9 = v0[v7][v6];	// L14
        float v10 = v1[v6][(v8 + (v3 * 16))];	// L15
        float v11 = v9 * v10;	// L16
        float v12 = v2[v7][(v8 + (v3 * 16))];	// L17
        float v13 = v12 + v11;	// L18
        v2[v7][(v8 + (v3 * 16))] = v13;	// L19
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node3(
  float v14[8],
  float v15[8][8],
  float v16[8],
  hls::stream<float> &v17,
  hls::stream<float> &v18,
  int v19
) {	// L25
  float v20 = v14[v19];	// L26
  for (int v21 = 0; v21 < 8; v21 += 1) {	// L27
    #pragma HLS pipeline II=1
    float v22 = v15[v19][v21];	// L28
    float v23 = v22 - v20;	// L29
    float v24 = exp(v23);	// L30
    v15[v19][v21] = v24;	// L31
    float v25 = v16[v19];	// L32
    float v26 = v25 + v24;	// L33
    v16[v19] = v26;	// L34
  }
  v17.write(v20);	// L36
  float v27 = v16[v19];	// L37
  for (int v28 = 0; v28 < 8; v28 += 1) {	// L38
    #pragma HLS pipeline II=1
    float v29 = v15[v19][v28];	// L39
    float v30 = v29 / v27;	// L40
    v15[v19][v28] = v30;	// L41
  }
  v18.write(v27);	// L43
}

void _Z3topPA128_fS0_S0_S0_S0__node4(
  float v31[8][8],
  float v32[8],
  int v33
) {	// L46
  #pragma HLS inline
  for (int v34 = 0; v34 < 7; v34 += 1) {	// L47
    #pragma HLS pipeline II=1
    float v35 = v31[v33][(v34 + 1)];	// L48
    float v36 = v32[v33];	// L49
    bool v37 = v35 > v36;	// L50
    float v38 = v37 ? v35 : v36;	// L51
    v32[v33] = v38;	// L52
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node2(
  float v39[8][128],
  float v40[8][128],
  float v41[8][8],
  float v42[8],
  float v43[8],
  int v44
) {	// L56
  for (int v45 = 0; v45 < 8; v45 += 1) {	// L59
    for (int v46 = 0; v46 < 8; v46 += 1) {	// L60
      #pragma HLS pipeline II=1
      v41[v45][v46] = (float)0.000000;	// L61
    }
  }
  for (int v47 = 0; v47 < 16; v47 += 1) {	// L64
    for (int v48 = 0; v48 < 8; v48 += 1) {	// L65
      for (int v49 = 0; v49 < 8; v49 += 1) {	// L66
        #pragma HLS pipeline II=1
        float v50 = v39[v48][(v47 + (v44 * 16))];	// L67
        float v51 = v40[v49][(v47 + (v44 * 16))];	// L68
        float v52 = v50 * v51;	// L69
        float v53 = v41[v48][v49];	// L70
        float v54 = v53 + v52;	// L71
        float v55 = v54 * (float)0.250000;	// L72
        float v56 = (((-v47) + 15) == 0) ? v55 : v54;	// L73
        v41[v48][v49] = v56;	// L74
      }
    }
  }
  for (int v57 = 0; v57 < 8; v57 += 1) {	// L78
    #pragma HLS pipeline II=1
    v42[v57] = (float)0.000000;	// L79
  }
  for (int v58 = 0; v58 < 8; v58 += 1) {	// L81
    #pragma HLS pipeline II=1
    float v59 = v41[v58][0];	// L82
    v43[v58] = v59;	// L83
  }
  for (int v60 = 0; v60 < 8; v60 += 1) {	// L85
    _Z3topPA128_fS0_S0_S0_S0__node4(v41, v43, v60);	// L86
    hls::stream<float> v61;	// L87
    hls::stream<float> v62;	// L88
    _Z3topPA128_fS0_S0_S0_S0__node3(v43, v41, v42, v61, v62, v60);	// L89
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node0(
  float v63[8][128],
  float v64[8][128],
  float v65[8][128],
  float v66[8][128]
) {	// L93
  float v67[8];	// L94
  #pragma HLS bind_storage variable=v67 type=ram_t2p impl=bram

  float v68[8];	// L95
  #pragma HLS bind_storage variable=v68 type=ram_t2p impl=bram

  float v69[8][8];	// L96
  #pragma HLS bind_storage variable=v69 type=ram_t2p impl=bram

  for (int v70 = 0; v70 < 8; v70 += 1) {	// L97
    _Z3topPA128_fS0_S0_S0_S0__node2(v63, v64, v69, v68, v67, v70);	// L98
    _Z3topPA128_fS0_S0_S0_S0__node1(v69, v65, v66, v70);	// L99
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node5(
  float v71[128][128],
  float v72[128][128],
  float v73[128][128],
  float v74[8][128],
  float v75[8][128],
  float v76[8][128],
  float v77[8][128],
  float v78[8][128],
  float v79[8][128],
  float v80[8][128]
) {	// L103
  #pragma HLS inline
  for (int v81 = 0; v81 < 128; v81 += 1) {	// L104
    for (int v82 = 0; v82 < 8; v82 += 1) {	// L105
      for (int v83 = 0; v83 < 128; v83 += 1) {	// L106
        #pragma HLS pipeline II=1
        float v84 = v74[v82][v81];	// L107
        float v85 = v73[v83][v81];	// L108
        float v86 = v84 * v85;	// L109
        float v87 = v75[v82][v83];	// L110
        float v88 = v78[v82][v83];	// L111
        float v89 = (v81 == 0) ? v87 : v88;	// L112
        float v90 = v89 + v86;	// L113
        v78[v82][v83] = v90;	// L114
        float v91 = v72[v83][v81];	// L115
        float v92 = v84 * v91;	// L116
        float v93 = v76[v82][v83];	// L117
        float v94 = v79[v82][v83];	// L118
        float v95 = (v81 == 0) ? v93 : v94;	// L119
        float v96 = v95 + v92;	// L120
        v79[v82][v83] = v96;	// L121
        float v97 = v71[v83][v81];	// L122
        float v98 = v84 * v97;	// L123
        float v99 = v77[v82][v83];	// L124
        float v100 = v80[v82][v83];	// L125
        float v101 = (v81 == 0) ? v99 : v100;	// L126
        float v102 = v101 + v98;	// L127
        v80[v82][v83] = v102;	// L128
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node6(
  float v103[8][128],
  float v104[8][128],
  float v105[8][128]
) {	// L134
  #pragma HLS inline
  for (int v106 = 0; v106 < 8; v106 += 1) {	// L136
    for (int v107 = 0; v107 < 128; v107 += 1) {	// L137
      #pragma HLS pipeline II=1
      v103[v106][v107] = (float)0.000000;	// L138
      v104[v106][v107] = (float)0.000000;	// L139
      v105[v106][v107] = (float)0.000000;	// L140
    }
  }
}

/// This is top function.
void _Z3topPA128_fS0_S0_S0_S0_(
  float v108[8][128],
  float v109[128][128],
  float v110[128][128],
  float v111[128][128],
  float v112[8][128]
) {	// L145
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v112 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v111 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v110 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v109 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v108 bundle=axi_0

  float v118[8][128];	// L156
  #pragma HLS bind_storage variable=v118 type=ram_t2p impl=bram

  float v119[8][128];	// L157
  #pragma HLS bind_storage variable=v119 type=ram_t2p impl=bram

  float v120[8][128];	// L158
  #pragma HLS bind_storage variable=v120 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node6(v118, v119, v120);	// L159
  float v121[8][128];	// L160
  #pragma HLS bind_storage variable=v121 type=ram_t2p impl=bram

  float v122[8][128];	// L161
  #pragma HLS bind_storage variable=v122 type=ram_t2p impl=bram

  float v123[8][128];	// L162
  #pragma HLS bind_storage variable=v123 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node5(v111, v110, v109, v108, v118, v119, v120, v121, v122, v123);	// L163
  _Z3topPA128_fS0_S0_S0_S0__node0(v121, v122, v123, v112);	// L164
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v124[8][128],
  float v125[8][128]
) {	// L176
  #pragma HLS inline
  for (int v126 = 0; v126 < 8; v126 += 1) {	// L177
    for (int v127 = 0; v127 < 128; v127 += 1) {	// L178
      #pragma HLS pipeline II=1
      float v128 = v124[v126][v127];	// L179
      v125[v126][v127] = v128;	// L180
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v129[8][128],
  float v130[8][128]
) {	// L185
  _Z16load_8_128_floatPA128_fS0__node0(v129, v130);	// L186
}

void _Z18load_128_128_floatPA128_fS0__node0(
  float v131[128][128],
  float v132[128][128]
) {	// L189
  #pragma HLS inline
  for (int v133 = 0; v133 < 128; v133 += 1) {	// L190
    for (int v134 = 0; v134 < 128; v134 += 1) {	// L191
      #pragma HLS pipeline II=1
      float v135 = v131[v133][v134];	// L192
      v132[v133][v134] = v135;	// L193
    }
  }
}

void _Z18load_128_128_floatPA128_fS0_(
  float v136[128][128],
  float v137[128][128]
) {	// L198
  _Z18load_128_128_floatPA128_fS0__node0(v136, v137);	// L199
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node1(
  float v138[8][8],
  float v139[8][128],
  float v140[8][128],
  int v141
) {	// L202
  for (int v142 = 0; v142 < 8; v142 += 1) {	// L204
    for (int v143 = 0; v143 < 16; v143 += 1) {	// L205
      #pragma HLS pipeline II=1
      v140[v142][(v143 + (v141 * 16))] = (float)0.000000;	// L206
    }
  }
  for (int v144 = 0; v144 < 8; v144 += 1) {	// L209
    for (int v145 = 0; v145 < 8; v145 += 1) {	// L210
      for (int v146 = 0; v146 < 16; v146 += 1) {	// L211
        #pragma HLS pipeline II=1
        float v147 = v138[v145][v144];	// L212
        float v148 = v139[v144][(v146 + (v141 * 16))];	// L213
        float v149 = v147 * v148;	// L214
        float v150 = v140[v145][(v146 + (v141 * 16))];	// L215
        float v151 = v150 + v149;	// L216
        v140[v145][(v146 + (v141 * 16))] = v151;	// L217
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node3(
  float v152[8],
  float v153[8][8],
  float v154[8],
  hls::stream<float> &v155,
  hls::stream<float> &v156,
  int v157
) {	// L223
  float v158 = v152[v157];	// L224
  for (int v159 = 0; v159 < 8; v159 += 1) {	// L225
    #pragma HLS pipeline II=1
    float v160 = v153[v157][v159];	// L226
    float v161 = v160 - v158;	// L227
    float v162 = exp(v161);	// L228
    v153[v157][v159] = v162;	// L229
    float v163 = v154[v157];	// L230
    float v164 = v163 + v162;	// L231
    v154[v157] = v164;	// L232
  }
  v155.write(v158);	// L234
  float v165 = v154[v157];	// L235
  for (int v166 = 0; v166 < 8; v166 += 1) {	// L236
    #pragma HLS pipeline II=1
    float v167 = v153[v157][v166];	// L237
    float v168 = v167 / v165;	// L238
    v153[v157][v166] = v168;	// L239
  }
  v156.write(v165);	// L241
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node4(
  float v169[8][8],
  float v170[8],
  int v171
) {	// L244
  #pragma HLS inline
  for (int v172 = 0; v172 < 7; v172 += 1) {	// L245
    #pragma HLS pipeline II=1
    float v173 = v169[v171][(v172 + 1)];	// L246
    float v174 = v170[v171];	// L247
    bool v175 = v173 > v174;	// L248
    float v176 = v175 ? v173 : v174;	// L249
    v170[v171] = v176;	// L250
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node2(
  float v177[8][128],
  float v178[8][128],
  float v179[8][8],
  float v180[8],
  float v181[8],
  int v182
) {	// L254
  for (int v183 = 0; v183 < 8; v183 += 1) {	// L257
    for (int v184 = 0; v184 < 8; v184 += 1) {	// L258
      #pragma HLS pipeline II=1
      v179[v183][v184] = (float)0.000000;	// L259
    }
  }
  for (int v185 = 0; v185 < 16; v185 += 1) {	// L262
    for (int v186 = 0; v186 < 8; v186 += 1) {	// L263
      for (int v187 = 0; v187 < 8; v187 += 1) {	// L264
        #pragma HLS pipeline II=1
        float v188 = v177[v186][(v185 + (v182 * 16))];	// L265
        float v189 = v178[v187][(v185 + (v182 * 16))];	// L266
        float v190 = v188 * v189;	// L267
        float v191 = v179[v186][v187];	// L268
        float v192 = v191 + v190;	// L269
        float v193 = v192 * (float)0.250000;	// L270
        float v194 = (((-v185) + 15) == 0) ? v193 : v192;	// L271
        v179[v186][v187] = v194;	// L272
      }
    }
  }
  for (int v195 = 0; v195 < 8; v195 += 1) {	// L276
    #pragma HLS pipeline II=1
    v180[v195] = (float)0.000000;	// L277
  }
  for (int v196 = 0; v196 < 8; v196 += 1) {	// L279
    #pragma HLS pipeline II=1
    float v197 = v179[v196][0];	// L280
    v181[v196] = v197;	// L281
  }
  for (int v198 = 0; v198 < 8; v198 += 1) {	// L283
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node4(v179, v181, v198);	// L284
    hls::stream<float> v199;	// L285
    hls::stream<float> v200;	// L286
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node3(v181, v179, v180, v199, v200, v198);	// L287
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node0(
  float v201[8][128],
  float v202[8][128],
  float v203[8][128],
  float v204[8][128]
) {	// L291
  float v205[8];	// L292
  #pragma HLS bind_storage variable=v205 type=ram_t2p impl=bram

  float v206[8];	// L293
  #pragma HLS bind_storage variable=v206 type=ram_t2p impl=bram

  float v207[8][8];	// L294
  #pragma HLS bind_storage variable=v207 type=ram_t2p impl=bram

  for (int v208 = 0; v208 < 8; v208 += 1) {	// L295
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node2(v201, v203, v207, v206, v205, v208);	// L296
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node1(v207, v202, v204, v208);	// L297
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node5(
  float v209[8][128],
  float v210[128][128],
  float v211[128][128],
  float v212[128][128],
  float v213[8][128],
  float v214[8][128],
  float v215[8][128],
  float v216[8][128],
  float v217[8][128],
  float v218[8][128]
) {	// L301
  #pragma HLS inline
  for (int v219 = 0; v219 < 128; v219 += 1) {	// L302
    for (int v220 = 0; v220 < 8; v220 += 1) {	// L303
      for (int v221 = 0; v221 < 128; v221 += 1) {	// L304
        #pragma HLS pipeline II=1
        float v222 = v209[v220][v219];	// L305
        float v223 = v211[v221][v219];	// L306
        float v224 = v222 * v223;	// L307
        float v225 = v213[v220][v221];	// L308
        float v226 = v216[v220][v221];	// L309
        float v227 = (v219 == 0) ? v225 : v226;	// L310
        float v228 = v227 + v224;	// L311
        v216[v220][v221] = v228;	// L312
        float v229 = v210[v221][v219];	// L313
        float v230 = v222 * v229;	// L314
        float v231 = v214[v220][v221];	// L315
        float v232 = v218[v220][v221];	// L316
        float v233 = (v219 == 0) ? v231 : v232;	// L317
        float v234 = v233 + v230;	// L318
        v218[v220][v221] = v234;	// L319
        float v235 = v212[v221][v219];	// L320
        float v236 = v222 * v235;	// L321
        float v237 = v215[v220][v221];	// L322
        float v238 = v217[v220][v221];	// L323
        float v239 = (v219 == 0) ? v237 : v238;	// L324
        float v240 = v239 + v236;	// L325
        v217[v220][v221] = v240;	// L326
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node6(
  float v241[8][128],
  float v242[8][128],
  float v243[8][128]
) {	// L332
  #pragma HLS inline
  for (int v244 = 0; v244 < 8; v244 += 1) {	// L334
    for (int v245 = 0; v245 < 128; v245 += 1) {	// L335
      #pragma HLS pipeline II=1
      v241[v244][v245] = (float)0.000000;	// L336
      v242[v244][v245] = (float)0.000000;	// L337
      v243[v244][v245] = (float)0.000000;	// L338
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i(
  float v246[8][128],
  float v247[128][128],
  float v248[128][128],
  float v249[128][128],
  float v250[8][128],
  ap_int<32> v251
) {	// L343
  float v252[8][128];	// L344
  #pragma HLS bind_storage variable=v252 type=ram_t2p impl=bram

  float v253[8][128];	// L345
  #pragma HLS bind_storage variable=v253 type=ram_t2p impl=bram

  float v254[8][128];	// L346
  #pragma HLS bind_storage variable=v254 type=ram_t2p impl=bram

  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node6(v252, v253, v254);	// L347
  float v255[8][128];	// L348
  #pragma HLS bind_storage variable=v255 type=ram_t2p impl=bram

  float v256[8][128];	// L349
  #pragma HLS bind_storage variable=v256 type=ram_t2p impl=bram

  float v257[8][128];	// L350
  #pragma HLS bind_storage variable=v257 type=ram_t2p impl=bram

  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node5(v246, v248, v247, v249, v252, v253, v254, v255, v257, v256);	// L351
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node0(v255, v257, v256, v250);	// L352
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v258[8][128],
  float v259[8][128]
) {	// L355
  #pragma HLS inline
  for (int v260 = 0; v260 < 8; v260 += 1) {	// L356
    for (int v261 = 0; v261 < 128; v261 += 1) {	// L357
      #pragma HLS pipeline II=1
      float v262 = v258[v260][v261];	// L358
      v259[v260][v261] = v262;	// L359
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v263[8][128],
  float v264[8][128]
) {	// L364
  _Z17store_8_128_floatPA128_fS0__node0(v263, v264);	// L365
}

