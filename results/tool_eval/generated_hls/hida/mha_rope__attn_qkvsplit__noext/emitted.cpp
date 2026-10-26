
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
) {	// L6
  for (int v4 = 0; v4 < 8; v4 += 1) {	// L8
    for (int v5 = 0; v5 < 16; v5 += 1) {	// L9
      #pragma HLS pipeline II=1
      v2[v4][(v5 + (v3 * 16))] = (float)0.000000;	// L10
    }
  }
  for (int v6 = 0; v6 < 8; v6 += 1) {	// L13
    for (int v7 = 0; v7 < 8; v7 += 1) {	// L14
      for (int v8 = 0; v8 < 16; v8 += 1) {	// L15
        #pragma HLS pipeline II=1
        float v9 = v0[v7][v6];	// L16
        float v10 = v1[v6][(v8 + (v3 * 16))];	// L17
        float v11 = v9 * v10;	// L18
        float v12 = v2[v7][(v8 + (v3 * 16))];	// L19
        float v13 = v12 + v11;	// L20
        v2[v7][(v8 + (v3 * 16))] = v13;	// L21
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
) {	// L27
  float v20 = v14[v19];	// L28
  for (int v21 = 0; v21 < 8; v21 += 1) {	// L29
    #pragma HLS pipeline II=1
    float v22 = v15[v19][v21];	// L30
    float v23 = v22 - v20;	// L31
    float v24 = exp(v23);	// L32
    v15[v19][v21] = v24;	// L33
    float v25 = v16[v19];	// L34
    float v26 = v25 + v24;	// L35
    v16[v19] = v26;	// L36
  }
  v17.write(v20);	// L38
  float v27 = v16[v19];	// L39
  for (int v28 = 0; v28 < 8; v28 += 1) {	// L40
    #pragma HLS pipeline II=1
    float v29 = v15[v19][v28];	// L41
    float v30 = v29 / v27;	// L42
    v15[v19][v28] = v30;	// L43
  }
  v18.write(v27);	// L45
}

void _Z3topPA128_fS0_S0_S0_S0__node4(
  float v31[8][8],
  float v32[8],
  int v33
) {	// L48
  #pragma HLS inline
  for (int v34 = 0; v34 < 7; v34 += 1) {	// L49
    #pragma HLS pipeline II=1
    float v35 = v31[v33][(v34 + 1)];	// L50
    float v36 = v32[v33];	// L51
    bool v37 = v35 > v36;	// L52
    float v38 = v37 ? v35 : v36;	// L53
    v32[v33] = v38;	// L54
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node2(
  float v39[8][128],
  float v40[8][128],
  float v41[8][8],
  float v42[8],
  float v43[8],
  int v44
) {	// L58
  for (int v45 = 0; v45 < 8; v45 += 1) {	// L61
    for (int v46 = 0; v46 < 8; v46 += 1) {	// L62
      #pragma HLS pipeline II=1
      v41[v45][v46] = (float)0.000000;	// L63
    }
  }
  for (int v47 = 0; v47 < 16; v47 += 1) {	// L66
    for (int v48 = 0; v48 < 8; v48 += 1) {	// L67
      for (int v49 = 0; v49 < 8; v49 += 1) {	// L68
        #pragma HLS pipeline II=1
        float v50 = v39[v48][(v47 + (v44 * 16))];	// L69
        float v51 = v40[v49][(v47 + (v44 * 16))];	// L70
        float v52 = v50 * v51;	// L71
        float v53 = v41[v48][v49];	// L72
        float v54 = v53 + v52;	// L73
        float v55 = v54 * (float)0.250000;	// L74
        float v56 = (((-v47) + 15) == 0) ? v55 : v54;	// L75
        v41[v48][v49] = v56;	// L76
      }
    }
  }
  for (int v57 = 0; v57 < 8; v57 += 1) {	// L80
    #pragma HLS pipeline II=1
    v42[v57] = (float)0.000000;	// L81
  }
  for (int v58 = 0; v58 < 8; v58 += 1) {	// L83
    #pragma HLS pipeline II=1
    float v59 = v41[v58][0];	// L84
    v43[v58] = v59;	// L85
  }
  for (int v60 = 0; v60 < 8; v60 += 1) {	// L87
    _Z3topPA128_fS0_S0_S0_S0__node4(v41, v43, v60);	// L88
    hls::stream<float> v61;	// L89
    hls::stream<float> v62;	// L90
    _Z3topPA128_fS0_S0_S0_S0__node3(v43, v41, v42, v61, v62, v60);	// L91
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node0(
  float v63[8][128],
  float v64[8][128],
  float v65[8][128],
  float v66[8][128]
) {	// L95
  float v67[8];	// L96
  #pragma HLS bind_storage variable=v67 type=ram_t2p impl=bram

  float v68[8];	// L97
  #pragma HLS bind_storage variable=v68 type=ram_t2p impl=bram

  float v69[8][8];	// L98
  #pragma HLS bind_storage variable=v69 type=ram_t2p impl=bram

  for (int v70 = 0; v70 < 8; v70 += 1) {	// L99
    _Z3topPA128_fS0_S0_S0_S0__node2(v65, v64, v69, v68, v67, v70);	// L100
    _Z3topPA128_fS0_S0_S0_S0__node1(v69, v63, v66, v70);	// L101
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node5(
  float v71[8][128],
  float v72[8][128],
  float v73[8][128],
  float v74[8][128]
) {	// L105
  float v75;	// L108
  for (int v76 = 0; v76 < 8; v76 += 1) {	// L109
    for (int v77 = 0; v77 < 128; v77 += 1) {	// L110
      #pragma HLS pipeline II=1
      float v78 = v72[v76][v77];	// L111
      v73[v76][v77] = v78;	// L112
    }
  }
  for (int v79 = 0; v79 < 8; v79 += 1) {	// L115
    for (int v80 = 0; v80 < 128; v80 += 1) {	// L116
      #pragma HLS pipeline II=1
      float v81 = v71[v79][v80];	// L117
      v74[v79][v80] = v81;	// L118
    }
  }
  for (int v82 = 0; v82 < 8; v82 += 1) {	// L121
    for (int v83 = 0; v83 < 8; v83 += 1) {	// L122
      for (int v84 = 0; v84 < 8; v84 += 1) {	// L123
        #pragma HLS pipeline II=1
        ap_int<32> v85 = v82;	// L124
        float v86 = v85;	// L125
        float v87 = v75;	// L126
        float v88 = (v84 == 0 && v83 == 0) ? v86 : v87;	// L127
        v75 = v88;	// L128
        int v89 = (v83 * 2);	// L129
        ap_int<32> v90 = v89;	// L130
        float v91 = v90;	// L131
        float v92 = -(v91);	// L132
        float v93 = v92 / (float)16.000000;	// L133
        float v94 = pow((float)10000.000000, v93);	// L134
        float v95 = v88 * v94;	// L135
        float v96 = cos(v95);	// L136
        float v97 = sin(v95);	// L137
        float v98 = v74[v82][((v84 * 16) + (v83 * 2))];	// L138
        float v99 = v74[v82][(((v84 * 16) + (v83 * 2)) + 1)];	// L139
        float v100 = v98 * v96;	// L140
        float v101 = v99 * v97;	// L141
        float v102 = v100 - v101;	// L142
        v74[v82][((v84 * 16) + (v83 * 2))] = v102;	// L143
        float v103 = v98 * v97;	// L144
        float v104 = v99 * v96;	// L145
        float v105 = v103 + v104;	// L146
        v74[v82][(((v84 * 16) + (v83 * 2)) + 1)] = v105;	// L147
        float v106 = v73[v82][((v84 * 16) + (v83 * 2))];	// L148
        float v107 = v73[v82][(((v84 * 16) + (v83 * 2)) + 1)];	// L149
        float v108 = v106 * v96;	// L150
        float v109 = v107 * v97;	// L151
        float v110 = v108 - v109;	// L152
        v73[v82][((v84 * 16) + (v83 * 2))] = v110;	// L153
        float v111 = v106 * v97;	// L154
        float v112 = v107 * v96;	// L155
        float v113 = v111 + v112;	// L156
        v73[v82][(((v84 * 16) + (v83 * 2)) + 1)] = v113;	// L157
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node6(
  float v114[8][128],
  float v115[128][128],
  float v116[8][128],
  float v117[8][128]
) {	// L163
  #pragma HLS inline
  for (int v118 = 0; v118 < 128; v118 += 1) {	// L164
    for (int v119 = 0; v119 < 8; v119 += 1) {	// L165
      for (int v120 = 0; v120 < 128; v120 += 1) {	// L166
        #pragma HLS pipeline II=1
        float v121 = v114[v119][v118];	// L167
        float v122 = v115[v120][v118];	// L168
        float v123 = v121 * v122;	// L169
        float v124 = v116[v119][v120];	// L170
        float v125 = v117[v119][v120];	// L171
        float v126 = (v118 == 0) ? v124 : v125;	// L172
        float v127 = v126 + v123;	// L173
        v117[v119][v120] = v127;	// L174
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node7(
  float v128[8][128],
  float v129[128][128],
  float v130[8][128],
  float v131[8][128]
) {	// L180
  #pragma HLS inline
  for (int v132 = 0; v132 < 128; v132 += 1) {	// L181
    for (int v133 = 0; v133 < 8; v133 += 1) {	// L182
      for (int v134 = 0; v134 < 128; v134 += 1) {	// L183
        #pragma HLS pipeline II=1
        float v135 = v128[v133][v132];	// L184
        float v136 = v129[v134][v132];	// L185
        float v137 = v135 * v136;	// L186
        float v138 = v130[v133][v134];	// L187
        float v139 = v131[v133][v134];	// L188
        float v140 = (v132 == 0) ? v138 : v139;	// L189
        float v141 = v140 + v137;	// L190
        v131[v133][v134] = v141;	// L191
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node8(
  float v142[8][128],
  float v143[128][128],
  float v144[8][128],
  float v145[8][128]
) {	// L197
  #pragma HLS inline
  for (int v146 = 0; v146 < 128; v146 += 1) {	// L198
    for (int v147 = 0; v147 < 8; v147 += 1) {	// L199
      for (int v148 = 0; v148 < 128; v148 += 1) {	// L200
        #pragma HLS pipeline II=1
        float v149 = v142[v147][v146];	// L201
        float v150 = v143[v148][v146];	// L202
        float v151 = v149 * v150;	// L203
        float v152 = v144[v147][v148];	// L204
        float v153 = v145[v147][v148];	// L205
        float v154 = (v146 == 0) ? v152 : v153;	// L206
        float v155 = v154 + v151;	// L207
        v145[v147][v148] = v155;	// L208
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node9(
  float v156[8][128],
  float v157[8][128]
) {	// L214
  #pragma HLS inline
  for (int v158 = 0; v158 < 8; v158 += 1) {	// L215
    for (int v159 = 0; v159 < 128; v159 += 1) {	// L216
      #pragma HLS pipeline II=1
      float v160 = v156[v158][v159];	// L217
      v157[v158][v159] = v160;	// L218
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node10(
  float v161[8][128],
  float v162[8][128],
  float v163[8][128]
) {	// L223
  #pragma HLS inline
  for (int v164 = 0; v164 < 8; v164 += 1) {	// L225
    for (int v165 = 0; v165 < 128; v165 += 1) {	// L226
      #pragma HLS pipeline II=1
      v161[v164][v165] = (float)0.000000;	// L227
      v162[v164][v165] = (float)0.000000;	// L228
      v163[v164][v165] = (float)0.000000;	// L229
    }
  }
}

/// This is top function.
void _Z3topPA128_fS0_S0_S0_S0_(
  float v166[8][128],
  float v167[8][128],
  float v168[8][128],
  float v169[128][128],
  float v170[128][128],
  float v171[128][128],
  float v172[8][128]
) {	// L234
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v172 bundle=axi_6

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v171 bundle=axi_5

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v170 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v169 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v168 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v167 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v166 bundle=axi_0

  float v180[8][128];	// L249
  #pragma HLS bind_storage variable=v180 type=ram_t2p impl=bram

  float v181[8][128];	// L250
  #pragma HLS bind_storage variable=v181 type=ram_t2p impl=bram

  float v182[8][128];	// L251
  #pragma HLS bind_storage variable=v182 type=ram_t2p impl=bram

  float v183[8][128];	// L252
  #pragma HLS bind_storage variable=v183 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node10(v180, v181, v182);	// L253
  _Z3topPA128_fS0_S0_S0_S0__node9(v182, v183);	// L254
  float v184[8][128];	// L255
  #pragma HLS bind_storage variable=v184 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node8(v168, v169, v180, v184);	// L256
  float v185[8][128];	// L257
  #pragma HLS bind_storage variable=v185 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node7(v167, v170, v181, v185);	// L258
  float v186[8][128];	// L259
  #pragma HLS bind_storage variable=v186 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node6(v166, v171, v183, v186);	// L260
  float v187[8][128];	// L261
  #pragma HLS array_partition variable=v187 cyclic factor=2 dim=2
  #pragma HLS bind_storage variable=v187 type=ram_2p impl=lutram

  float v188[8][128];	// L262
  #pragma HLS array_partition variable=v188 cyclic factor=2 dim=2
  #pragma HLS bind_storage variable=v188 type=ram_2p impl=lutram

  _Z3topPA128_fS0_S0_S0_S0__node5(v184, v185, v188, v187);	// L263
  _Z3topPA128_fS0_S0_S0_S0__node0(v186, v188, v187, v172);	// L264
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v189[8][128],
  float v190[8][128]
) {	// L278
  #pragma HLS inline
  for (int v191 = 0; v191 < 8; v191 += 1) {	// L279
    for (int v192 = 0; v192 < 128; v192 += 1) {	// L280
      #pragma HLS pipeline II=1
      float v193 = v189[v191][v192];	// L281
      v190[v191][v192] = v193;	// L282
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v194[8][128],
  float v195[8][128]
) {	// L287
  _Z16load_8_128_floatPA128_fS0__node0(v194, v195);	// L288
}

void _Z18load_128_128_floatPA128_fS0__node0(
  float v196[128][128],
  float v197[128][128]
) {	// L291
  #pragma HLS inline
  for (int v198 = 0; v198 < 128; v198 += 1) {	// L292
    for (int v199 = 0; v199 < 128; v199 += 1) {	// L293
      #pragma HLS pipeline II=1
      float v200 = v196[v198][v199];	// L294
      v197[v198][v199] = v200;	// L295
    }
  }
}

void _Z18load_128_128_floatPA128_fS0_(
  float v201[128][128],
  float v202[128][128]
) {	// L300
  _Z18load_128_128_floatPA128_fS0__node0(v201, v202);	// L301
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node1(
  float v203[8][8],
  float v204[8][128],
  float v205[8][128],
  int v206
) {	// L304
  for (int v207 = 0; v207 < 8; v207 += 1) {	// L306
    for (int v208 = 0; v208 < 16; v208 += 1) {	// L307
      #pragma HLS pipeline II=1
      v205[v207][(v208 + (v206 * 16))] = (float)0.000000;	// L308
    }
  }
  for (int v209 = 0; v209 < 8; v209 += 1) {	// L311
    for (int v210 = 0; v210 < 8; v210 += 1) {	// L312
      for (int v211 = 0; v211 < 16; v211 += 1) {	// L313
        #pragma HLS pipeline II=1
        float v212 = v203[v210][v209];	// L314
        float v213 = v204[v209][(v211 + (v206 * 16))];	// L315
        float v214 = v212 * v213;	// L316
        float v215 = v205[v210][(v211 + (v206 * 16))];	// L317
        float v216 = v215 + v214;	// L318
        v205[v210][(v211 + (v206 * 16))] = v216;	// L319
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node3(
  float v217[8],
  float v218[8][8],
  float v219[8],
  hls::stream<float> &v220,
  hls::stream<float> &v221,
  int v222
) {	// L325
  float v223 = v217[v222];	// L326
  for (int v224 = 0; v224 < 8; v224 += 1) {	// L327
    #pragma HLS pipeline II=1
    float v225 = v218[v222][v224];	// L328
    float v226 = v225 - v223;	// L329
    float v227 = exp(v226);	// L330
    v218[v222][v224] = v227;	// L331
    float v228 = v219[v222];	// L332
    float v229 = v228 + v227;	// L333
    v219[v222] = v229;	// L334
  }
  v220.write(v223);	// L336
  float v230 = v219[v222];	// L337
  for (int v231 = 0; v231 < 8; v231 += 1) {	// L338
    #pragma HLS pipeline II=1
    float v232 = v218[v222][v231];	// L339
    float v233 = v232 / v230;	// L340
    v218[v222][v231] = v233;	// L341
  }
  v221.write(v230);	// L343
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node4(
  float v234[8][8],
  float v235[8],
  int v236
) {	// L346
  #pragma HLS inline
  for (int v237 = 0; v237 < 7; v237 += 1) {	// L347
    #pragma HLS pipeline II=1
    float v238 = v234[v236][(v237 + 1)];	// L348
    float v239 = v235[v236];	// L349
    bool v240 = v238 > v239;	// L350
    float v241 = v240 ? v238 : v239;	// L351
    v235[v236] = v241;	// L352
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node2(
  float v242[8][128],
  float v243[8][128],
  float v244[8][8],
  float v245[8],
  float v246[8],
  int v247
) {	// L356
  for (int v248 = 0; v248 < 8; v248 += 1) {	// L359
    for (int v249 = 0; v249 < 8; v249 += 1) {	// L360
      #pragma HLS pipeline II=1
      v244[v248][v249] = (float)0.000000;	// L361
    }
  }
  for (int v250 = 0; v250 < 16; v250 += 1) {	// L364
    for (int v251 = 0; v251 < 8; v251 += 1) {	// L365
      for (int v252 = 0; v252 < 8; v252 += 1) {	// L366
        #pragma HLS pipeline II=1
        float v253 = v242[v251][(v250 + (v247 * 16))];	// L367
        float v254 = v243[v252][(v250 + (v247 * 16))];	// L368
        float v255 = v253 * v254;	// L369
        float v256 = v244[v251][v252];	// L370
        float v257 = v256 + v255;	// L371
        float v258 = v257 * (float)0.250000;	// L372
        float v259 = (((-v250) + 15) == 0) ? v258 : v257;	// L373
        v244[v251][v252] = v259;	// L374
      }
    }
  }
  for (int v260 = 0; v260 < 8; v260 += 1) {	// L378
    #pragma HLS pipeline II=1
    v245[v260] = (float)0.000000;	// L379
  }
  for (int v261 = 0; v261 < 8; v261 += 1) {	// L381
    #pragma HLS pipeline II=1
    float v262 = v244[v261][0];	// L382
    v246[v261] = v262;	// L383
  }
  for (int v263 = 0; v263 < 8; v263 += 1) {	// L385
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node4(v244, v246, v263);	// L386
    hls::stream<float> v264;	// L387
    hls::stream<float> v265;	// L388
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node3(v246, v244, v245, v264, v265, v263);	// L389
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node0(
  float v266[8][128],
  float v267[8][128],
  float v268[8][128],
  float v269[8][128]
) {	// L393
  float v270[8];	// L394
  #pragma HLS bind_storage variable=v270 type=ram_t2p impl=bram

  float v271[8];	// L395
  #pragma HLS bind_storage variable=v271 type=ram_t2p impl=bram

  float v272[8][8];	// L396
  #pragma HLS bind_storage variable=v272 type=ram_t2p impl=bram

  for (int v273 = 0; v273 < 8; v273 += 1) {	// L397
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node2(v268, v267, v272, v271, v270, v273);	// L398
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node1(v272, v266, v269, v273);	// L399
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node5(
  float v274[8][128],
  float v275[8][128],
  float v276[8][128],
  float v277[8][128]
) {	// L403
  float v278;	// L406
  for (int v279 = 0; v279 < 8; v279 += 1) {	// L407
    for (int v280 = 0; v280 < 128; v280 += 1) {	// L408
      #pragma HLS pipeline II=1
      float v281 = v275[v279][v280];	// L409
      v276[v279][v280] = v281;	// L410
    }
  }
  for (int v282 = 0; v282 < 8; v282 += 1) {	// L413
    for (int v283 = 0; v283 < 128; v283 += 1) {	// L414
      #pragma HLS pipeline II=1
      float v284 = v274[v282][v283];	// L415
      v277[v282][v283] = v284;	// L416
    }
  }
  for (int v285 = 0; v285 < 8; v285 += 1) {	// L419
    for (int v286 = 0; v286 < 8; v286 += 1) {	// L420
      for (int v287 = 0; v287 < 8; v287 += 1) {	// L421
        #pragma HLS pipeline II=1
        ap_int<32> v288 = v285;	// L422
        float v289 = v288;	// L423
        float v290 = v278;	// L424
        float v291 = (v287 == 0 && v286 == 0) ? v289 : v290;	// L425
        v278 = v291;	// L426
        int v292 = (v286 * 2);	// L427
        ap_int<32> v293 = v292;	// L428
        float v294 = v293;	// L429
        float v295 = -(v294);	// L430
        float v296 = v295 / (float)16.000000;	// L431
        float v297 = pow((float)10000.000000, v296);	// L432
        float v298 = v291 * v297;	// L433
        float v299 = cos(v298);	// L434
        float v300 = sin(v298);	// L435
        float v301 = v277[v285][((v287 * 16) + (v286 * 2))];	// L436
        float v302 = v277[v285][(((v287 * 16) + (v286 * 2)) + 1)];	// L437
        float v303 = v301 * v299;	// L438
        float v304 = v302 * v300;	// L439
        float v305 = v303 - v304;	// L440
        v277[v285][((v287 * 16) + (v286 * 2))] = v305;	// L441
        float v306 = v301 * v300;	// L442
        float v307 = v302 * v299;	// L443
        float v308 = v306 + v307;	// L444
        v277[v285][(((v287 * 16) + (v286 * 2)) + 1)] = v308;	// L445
        float v309 = v276[v285][((v287 * 16) + (v286 * 2))];	// L446
        float v310 = v276[v285][(((v287 * 16) + (v286 * 2)) + 1)];	// L447
        float v311 = v309 * v299;	// L448
        float v312 = v310 * v300;	// L449
        float v313 = v311 - v312;	// L450
        v276[v285][((v287 * 16) + (v286 * 2))] = v313;	// L451
        float v314 = v309 * v300;	// L452
        float v315 = v310 * v299;	// L453
        float v316 = v314 + v315;	// L454
        v276[v285][(((v287 * 16) + (v286 * 2)) + 1)] = v316;	// L455
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node6(
  float v317[8][128],
  float v318[128][128],
  float v319[8][128],
  float v320[8][128]
) {	// L461
  #pragma HLS inline
  for (int v321 = 0; v321 < 128; v321 += 1) {	// L462
    for (int v322 = 0; v322 < 8; v322 += 1) {	// L463
      for (int v323 = 0; v323 < 128; v323 += 1) {	// L464
        #pragma HLS pipeline II=1
        float v324 = v317[v322][v321];	// L465
        float v325 = v318[v323][v321];	// L466
        float v326 = v324 * v325;	// L467
        float v327 = v319[v322][v323];	// L468
        float v328 = v320[v322][v323];	// L469
        float v329 = (v321 == 0) ? v327 : v328;	// L470
        float v330 = v329 + v326;	// L471
        v320[v322][v323] = v330;	// L472
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node7(
  float v331[8][128],
  float v332[128][128],
  float v333[8][128],
  float v334[8][128]
) {	// L478
  #pragma HLS inline
  for (int v335 = 0; v335 < 128; v335 += 1) {	// L479
    for (int v336 = 0; v336 < 8; v336 += 1) {	// L480
      for (int v337 = 0; v337 < 128; v337 += 1) {	// L481
        #pragma HLS pipeline II=1
        float v338 = v331[v336][v335];	// L482
        float v339 = v332[v337][v335];	// L483
        float v340 = v338 * v339;	// L484
        float v341 = v333[v336][v337];	// L485
        float v342 = v334[v336][v337];	// L486
        float v343 = (v335 == 0) ? v341 : v342;	// L487
        float v344 = v343 + v340;	// L488
        v334[v336][v337] = v344;	// L489
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node8(
  float v345[8][128],
  float v346[128][128],
  float v347[8][128],
  float v348[8][128]
) {	// L495
  #pragma HLS inline
  for (int v349 = 0; v349 < 128; v349 += 1) {	// L496
    for (int v350 = 0; v350 < 8; v350 += 1) {	// L497
      for (int v351 = 0; v351 < 128; v351 += 1) {	// L498
        #pragma HLS pipeline II=1
        float v352 = v345[v350][v349];	// L499
        float v353 = v346[v351][v349];	// L500
        float v354 = v352 * v353;	// L501
        float v355 = v347[v350][v351];	// L502
        float v356 = v348[v350][v351];	// L503
        float v357 = (v349 == 0) ? v355 : v356;	// L504
        float v358 = v357 + v354;	// L505
        v348[v350][v351] = v358;	// L506
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node9(
  float v359[8][128],
  float v360[8][128]
) {	// L512
  #pragma HLS inline
  for (int v361 = 0; v361 < 8; v361 += 1) {	// L513
    for (int v362 = 0; v362 < 128; v362 += 1) {	// L514
      #pragma HLS pipeline II=1
      float v363 = v359[v361][v362];	// L515
      v360[v361][v362] = v363;	// L516
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node10(
  float v364[8][128],
  float v365[8][128],
  float v366[8][128]
) {	// L521
  #pragma HLS inline
  for (int v367 = 0; v367 < 8; v367 += 1) {	// L523
    for (int v368 = 0; v368 < 128; v368 += 1) {	// L524
      #pragma HLS pipeline II=1
      v364[v367][v368] = (float)0.000000;	// L525
      v365[v367][v368] = (float)0.000000;	// L526
      v366[v367][v368] = (float)0.000000;	// L527
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i(
  float v369[8][128],
  float v370[128][128],
  float v371[128][128],
  float v372[128][128],
  float v373[8][128],
  ap_int<32> v374
) {	// L532
  float v375[8][128];	// L533
  #pragma HLS bind_storage variable=v375 type=ram_t2p impl=bram

  float v376[8][128];	// L534
  #pragma HLS bind_storage variable=v376 type=ram_t2p impl=bram

  float v377[8][128];	// L535
  #pragma HLS bind_storage variable=v377 type=ram_t2p impl=bram

  float v378[8][128];	// L536
  #pragma HLS bind_storage variable=v378 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node10(v375, v376, v377);	// L537
  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node9(v377, v378);	// L538
  float v379[8][128];	// L539
  #pragma HLS bind_storage variable=v379 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node8(v369, v370, v375, v379);	// L540
  float v380[8][128];	// L541
  #pragma HLS bind_storage variable=v380 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node7(v369, v371, v376, v380);	// L542
  float v381[8][128];	// L543
  #pragma HLS bind_storage variable=v381 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node6(v369, v372, v378, v381);	// L544
  float v382[8][128];	// L545
  #pragma HLS bind_storage variable=v382 type=ram_t2p impl=bram

  float v383[8][128];	// L546
  #pragma HLS bind_storage variable=v383 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node5(v379, v380, v383, v382);	// L547
  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node0(v381, v383, v382, v373);	// L548
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v384[8][128],
  float v385[8][128]
) {	// L551
  #pragma HLS inline
  for (int v386 = 0; v386 < 8; v386 += 1) {	// L552
    for (int v387 = 0; v387 < 128; v387 += 1) {	// L553
      #pragma HLS pipeline II=1
      float v388 = v384[v386][v387];	// L554
      v385[v386][v387] = v388;	// L555
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v389[8][128],
  float v390[8][128]
) {	// L560
  _Z17store_8_128_floatPA128_fS0__node0(v389, v390);	// L561
}

