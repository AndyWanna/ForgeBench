
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
        float v91 = v71[v83][v81];	// L115
        float v92 = v84 * v91;	// L116
        float v93 = v76[v82][v83];	// L117
        float v94 = v79[v82][v83];	// L118
        float v95 = (v81 == 0) ? v93 : v94;	// L119
        float v96 = v95 + v92;	// L120
        v79[v82][v83] = v96;	// L121
        float v97 = v72[v83][v81];	// L122
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
  float v112[8][128],
  float v113[8][128],
  float v114[8][128],
  float v115[8][128],
  float v116[8][128],
  float v117[8][128],
  float v118[8][128],
  float v119[8][128],
  float v120[8][128],
  float v121[8][128],
  float v122[8][128],
  float v123[8][128],
  float v124[8][128]
) {	// L145
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v124 bundle=axi_16

  #pragma HLS interface m_axi offset=slave port=v123 bundle=axi_15

  #pragma HLS interface m_axi offset=slave port=v122 bundle=axi_14

  #pragma HLS interface m_axi offset=slave port=v121 bundle=axi_13

  #pragma HLS interface m_axi offset=slave port=v120 bundle=axi_12

  #pragma HLS interface m_axi offset=slave port=v119 bundle=axi_11

  #pragma HLS interface m_axi offset=slave port=v118 bundle=axi_10

  #pragma HLS interface m_axi offset=slave port=v117 bundle=axi_9

  #pragma HLS interface m_axi offset=slave port=v116 bundle=axi_8

  #pragma HLS interface m_axi offset=slave port=v115 bundle=axi_7

  #pragma HLS interface m_axi offset=slave port=v114 bundle=axi_6

  #pragma HLS interface m_axi offset=slave port=v113 bundle=axi_5

  #pragma HLS interface m_axi offset=slave port=v112 bundle=axi_4

  #pragma HLS interface m_axi offset=slave port=v111 bundle=axi_3

  #pragma HLS interface m_axi offset=slave port=v110 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v109 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v108 bundle=axi_0

  _Z3topPA128_fS0_S0_S0_S0__node6(v113, v115, v117);	// L180
  _Z3topPA128_fS0_S0_S0_S0__node5(v110, v111, v109, v108, v114, v116, v118, v119, v121, v123);	// L181
  _Z3topPA128_fS0_S0_S0_S0__node0(v120, v122, v124, v112);	// L182
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v142[8][128],
  float v143[8][128]
) {	// L212
  #pragma HLS inline
  for (int v144 = 0; v144 < 8; v144 += 1) {	// L213
    for (int v145 = 0; v145 < 128; v145 += 1) {	// L214
      #pragma HLS pipeline II=1
      float v146 = v142[v144][v145];	// L215
      v143[v144][v145] = v146;	// L216
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v147[8][128],
  float v148[8][128]
) {	// L221
  _Z16load_8_128_floatPA128_fS0__node0(v147, v148);	// L222
}

void _Z18load_128_128_floatPA128_fS0__node0(
  float v149[128][128],
  float v150[128][128]
) {	// L225
  #pragma HLS inline
  for (int v151 = 0; v151 < 128; v151 += 1) {	// L226
    for (int v152 = 0; v152 < 128; v152 += 1) {	// L227
      #pragma HLS pipeline II=1
      float v153 = v149[v151][v152];	// L228
      v150[v151][v152] = v153;	// L229
    }
  }
}

void _Z18load_128_128_floatPA128_fS0_(
  float v154[128][128],
  float v155[128][128]
) {	// L234
  _Z18load_128_128_floatPA128_fS0__node0(v154, v155);	// L235
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node1(
  float v156[8][8],
  float v157[8][128],
  float v158[8][128],
  int v159
) {	// L238
  for (int v160 = 0; v160 < 8; v160 += 1) {	// L240
    for (int v161 = 0; v161 < 16; v161 += 1) {	// L241
      #pragma HLS pipeline II=1
      v158[v160][(v161 + (v159 * 16))] = (float)0.000000;	// L242
    }
  }
  for (int v162 = 0; v162 < 8; v162 += 1) {	// L245
    for (int v163 = 0; v163 < 8; v163 += 1) {	// L246
      for (int v164 = 0; v164 < 16; v164 += 1) {	// L247
        #pragma HLS pipeline II=1
        float v165 = v156[v163][v162];	// L248
        float v166 = v157[v162][(v164 + (v159 * 16))];	// L249
        float v167 = v165 * v166;	// L250
        float v168 = v158[v163][(v164 + (v159 * 16))];	// L251
        float v169 = v168 + v167;	// L252
        v158[v163][(v164 + (v159 * 16))] = v169;	// L253
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node3(
  float v170[8],
  float v171[8][8],
  float v172[8],
  hls::stream<float> &v173,
  hls::stream<float> &v174,
  int v175
) {	// L259
  float v176 = v170[v175];	// L260
  for (int v177 = 0; v177 < 8; v177 += 1) {	// L261
    #pragma HLS pipeline II=1
    float v178 = v171[v175][v177];	// L262
    float v179 = v178 - v176;	// L263
    float v180 = exp(v179);	// L264
    v171[v175][v177] = v180;	// L265
    float v181 = v172[v175];	// L266
    float v182 = v181 + v180;	// L267
    v172[v175] = v182;	// L268
  }
  v173.write(v176);	// L270
  float v183 = v172[v175];	// L271
  for (int v184 = 0; v184 < 8; v184 += 1) {	// L272
    #pragma HLS pipeline II=1
    float v185 = v171[v175][v184];	// L273
    float v186 = v185 / v183;	// L274
    v171[v175][v184] = v186;	// L275
  }
  v174.write(v183);	// L277
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node4(
  float v187[8][8],
  float v188[8],
  int v189
) {	// L280
  #pragma HLS inline
  for (int v190 = 0; v190 < 7; v190 += 1) {	// L281
    #pragma HLS pipeline II=1
    float v191 = v187[v189][(v190 + 1)];	// L282
    float v192 = v188[v189];	// L283
    bool v193 = v191 > v192;	// L284
    float v194 = v193 ? v191 : v192;	// L285
    v188[v189] = v194;	// L286
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node2(
  float v195[8][128],
  float v196[8][128],
  float v197[8][8],
  float v198[8],
  float v199[8],
  int v200
) {	// L290
  for (int v201 = 0; v201 < 8; v201 += 1) {	// L293
    for (int v202 = 0; v202 < 8; v202 += 1) {	// L294
      #pragma HLS pipeline II=1
      v197[v201][v202] = (float)0.000000;	// L295
    }
  }
  for (int v203 = 0; v203 < 16; v203 += 1) {	// L298
    for (int v204 = 0; v204 < 8; v204 += 1) {	// L299
      for (int v205 = 0; v205 < 8; v205 += 1) {	// L300
        #pragma HLS pipeline II=1
        float v206 = v195[v204][(v203 + (v200 * 16))];	// L301
        float v207 = v196[v205][(v203 + (v200 * 16))];	// L302
        float v208 = v206 * v207;	// L303
        float v209 = v197[v204][v205];	// L304
        float v210 = v209 + v208;	// L305
        float v211 = v210 * (float)0.250000;	// L306
        float v212 = (((-v203) + 15) == 0) ? v211 : v210;	// L307
        v197[v204][v205] = v212;	// L308
      }
    }
  }
  for (int v213 = 0; v213 < 8; v213 += 1) {	// L312
    #pragma HLS pipeline II=1
    v198[v213] = (float)0.000000;	// L313
  }
  for (int v214 = 0; v214 < 8; v214 += 1) {	// L315
    #pragma HLS pipeline II=1
    float v215 = v197[v214][0];	// L316
    v199[v214] = v215;	// L317
  }
  for (int v216 = 0; v216 < 8; v216 += 1) {	// L319
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node4(v197, v199, v216);	// L320
    hls::stream<float> v217;	// L321
    hls::stream<float> v218;	// L322
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node3(v199, v197, v198, v217, v218, v216);	// L323
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node0(
  float v219[8][128],
  float v220[8][128],
  float v221[8][128],
  float v222[8][128]
) {	// L327
  float v223[8];	// L328
  #pragma HLS bind_storage variable=v223 type=ram_t2p impl=bram

  float v224[8];	// L329
  #pragma HLS bind_storage variable=v224 type=ram_t2p impl=bram

  float v225[8][8];	// L330
  #pragma HLS bind_storage variable=v225 type=ram_t2p impl=bram

  for (int v226 = 0; v226 < 8; v226 += 1) {	// L331
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node2(v219, v221, v225, v224, v223, v226);	// L332
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node1(v225, v220, v222, v226);	// L333
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node5(
  float v227[128][128],
  float v228[128][128],
  float v229[8][128],
  float v230[128][128],
  float v231[8][128],
  float v232[8][128],
  float v233[8][128],
  float v234[8][128],
  float v235[8][128],
  float v236[8][128]
) {	// L337
  #pragma HLS inline
  for (int v237 = 0; v237 < 128; v237 += 1) {	// L338
    for (int v238 = 0; v238 < 8; v238 += 1) {	// L339
      for (int v239 = 0; v239 < 128; v239 += 1) {	// L340
        #pragma HLS pipeline II=1
        float v240 = v229[v238][v237];	// L341
        float v241 = v227[v239][v237];	// L342
        float v242 = v240 * v241;	// L343
        float v243 = v231[v238][v239];	// L344
        float v244 = v234[v238][v239];	// L345
        float v245 = (v237 == 0) ? v243 : v244;	// L346
        float v246 = v245 + v242;	// L347
        v234[v238][v239] = v246;	// L348
        float v247 = v228[v239][v237];	// L349
        float v248 = v240 * v247;	// L350
        float v249 = v232[v238][v239];	// L351
        float v250 = v236[v238][v239];	// L352
        float v251 = (v237 == 0) ? v249 : v250;	// L353
        float v252 = v251 + v248;	// L354
        v236[v238][v239] = v252;	// L355
        float v253 = v230[v239][v237];	// L356
        float v254 = v240 * v253;	// L357
        float v255 = v233[v238][v239];	// L358
        float v256 = v235[v238][v239];	// L359
        float v257 = (v237 == 0) ? v255 : v256;	// L360
        float v258 = v257 + v254;	// L361
        v235[v238][v239] = v258;	// L362
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node6(
  float v259[8][128],
  float v260[8][128],
  float v261[8][128]
) {	// L368
  #pragma HLS inline
  for (int v262 = 0; v262 < 8; v262 += 1) {	// L370
    for (int v263 = 0; v263 < 128; v263 += 1) {	// L371
      #pragma HLS pipeline II=1
      v259[v262][v263] = (float)0.000000;	// L372
      v260[v262][v263] = (float)0.000000;	// L373
      v261[v262][v263] = (float)0.000000;	// L374
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i(
  float v264[8][128],
  float v265[128][128],
  float v266[128][128],
  float v267[128][128],
  float v268[8][128],
  ap_int<32> v269
) {	// L379
  float v270[8][128];	// L380
  float v271[8][128];	// L381
  float v272[8][128];	// L382
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node6(v270, v271, v272);	// L383
  float v273[8][128];	// L384
  float v274[8][128];	// L385
  float v275[8][128];	// L386
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node5(v265, v266, v264, v267, v270, v271, v272, v273, v275, v274);	// L387
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node0(v273, v275, v274, v268);	// L388
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v276[8][128],
  float v277[8][128]
) {	// L391
  #pragma HLS inline
  for (int v278 = 0; v278 < 8; v278 += 1) {	// L392
    for (int v279 = 0; v279 < 128; v279 += 1) {	// L393
      #pragma HLS pipeline II=1
      float v280 = v276[v278][v279];	// L394
      v277[v278][v279] = v280;	// L395
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v281[8][128],
  float v282[8][128]
) {	// L400
  _Z17store_8_128_floatPA128_fS0__node0(v281, v282);	// L401
}

