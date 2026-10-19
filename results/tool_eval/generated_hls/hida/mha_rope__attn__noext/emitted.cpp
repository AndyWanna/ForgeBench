
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
    _Z3topPA128_fS0_S0_S0_S0__node2(v65, v63, v69, v68, v67, v70);	// L100
    _Z3topPA128_fS0_S0_S0_S0__node1(v69, v64, v66, v70);	// L101
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
  float v115[8][128]
) {	// L163
  #pragma HLS inline
  for (int v116 = 0; v116 < 8; v116 += 1) {	// L164
    for (int v117 = 0; v117 < 128; v117 += 1) {	// L165
      #pragma HLS pipeline II=1
      float v118 = v114[v116][v117];	// L166
      v115[v116][v117] = v118;	// L167
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node7(
  float v119[8][128],
  float v120[128][128],
  float v121[128][128],
  float v122[128][128],
  float v123[8][128],
  float v124[8][128],
  float v125[8][128],
  float v126[8][128],
  float v127[8][128],
  float v128[8][128]
) {	// L172
  #pragma HLS inline
  for (int v129 = 0; v129 < 128; v129 += 1) {	// L173
    for (int v130 = 0; v130 < 8; v130 += 1) {	// L174
      for (int v131 = 0; v131 < 128; v131 += 1) {	// L175
        #pragma HLS pipeline II=1
        float v132 = v119[v130][v129];	// L176
        float v133 = v122[v131][v129];	// L177
        float v134 = v132 * v133;	// L178
        float v135 = v123[v130][v131];	// L179
        float v136 = v128[v130][v131];	// L180
        float v137 = (v129 == 0) ? v135 : v136;	// L181
        float v138 = v137 + v134;	// L182
        v128[v130][v131] = v138;	// L183
        float v139 = v120[v131][v129];	// L184
        float v140 = v132 * v139;	// L185
        float v141 = v124[v130][v131];	// L186
        float v142 = v126[v130][v131];	// L187
        float v143 = (v129 == 0) ? v141 : v142;	// L188
        float v144 = v143 + v140;	// L189
        v126[v130][v131] = v144;	// L190
        float v145 = v121[v131][v129];	// L191
        float v146 = v132 * v145;	// L192
        float v147 = v125[v130][v131];	// L193
        float v148 = v127[v130][v131];	// L194
        float v149 = (v129 == 0) ? v147 : v148;	// L195
        float v150 = v149 + v146;	// L196
        v127[v130][v131] = v150;	// L197
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node8(
  float v151[8][128],
  float v152[8][128],
  float v153[8][128]
) {	// L203
  #pragma HLS inline
  for (int v154 = 0; v154 < 8; v154 += 1) {	// L205
    for (int v155 = 0; v155 < 128; v155 += 1) {	// L206
      #pragma HLS pipeline II=1
      v151[v154][v155] = (float)0.000000;	// L207
      v152[v154][v155] = (float)0.000000;	// L208
      v153[v154][v155] = (float)0.000000;	// L209
    }
  }
}

/// This is top function.
void _Z3topPA128_fS0_S0_S0_S0_(
  float v156[8][128],
  float v157[128][128],
  float v158[128][128],
  float v159[128][128],
  float v160[8][128]
) {	// L214
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v160 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v159 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v158 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v157 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v156 bundle=axi_0

  float v166[8][128];	// L225
  #pragma HLS bind_storage variable=v166 type=ram_t2p impl=bram

  float v167[8][128];	// L226
  #pragma HLS bind_storage variable=v167 type=ram_t2p impl=bram

  float v168[8][128];	// L227
  #pragma HLS bind_storage variable=v168 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node8(v166, v167, v168);	// L228
  float v169[8][128];	// L229
  #pragma HLS bind_storage variable=v169 type=ram_t2p impl=bram

  float v170[8][128];	// L230
  #pragma HLS bind_storage variable=v170 type=ram_t2p impl=bram

  float v171[8][128];	// L231
  #pragma HLS bind_storage variable=v171 type=ram_t2p impl=bram

  float v172[8][128];	// L232
  #pragma HLS bind_storage variable=v172 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node7(v156, v158, v159, v157, v166, v167, v168, v170, v171, v169);	// L233
  _Z3topPA128_fS0_S0_S0_S0__node6(v171, v172);	// L234
  float v173[8][128];	// L235
  #pragma HLS array_partition variable=v173 cyclic factor=2 dim=2
  #pragma HLS bind_storage variable=v173 type=ram_2p impl=lutram

  float v174[8][128];	// L236
  #pragma HLS array_partition variable=v174 cyclic factor=2 dim=2
  #pragma HLS bind_storage variable=v174 type=ram_2p impl=lutram

  _Z3topPA128_fS0_S0_S0_S0__node5(v169, v170, v174, v173);	// L237
  _Z3topPA128_fS0_S0_S0_S0__node0(v174, v172, v173, v160);	// L238
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v175[8][128],
  float v176[8][128]
) {	// L250
  #pragma HLS inline
  for (int v177 = 0; v177 < 8; v177 += 1) {	// L251
    for (int v178 = 0; v178 < 128; v178 += 1) {	// L252
      #pragma HLS pipeline II=1
      float v179 = v175[v177][v178];	// L253
      v176[v177][v178] = v179;	// L254
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v180[8][128],
  float v181[8][128]
) {	// L259
  _Z16load_8_128_floatPA128_fS0__node0(v180, v181);	// L260
}

void _Z18load_128_128_floatPA128_fS0__node0(
  float v182[128][128],
  float v183[128][128]
) {	// L263
  #pragma HLS inline
  for (int v184 = 0; v184 < 128; v184 += 1) {	// L264
    for (int v185 = 0; v185 < 128; v185 += 1) {	// L265
      #pragma HLS pipeline II=1
      float v186 = v182[v184][v185];	// L266
      v183[v184][v185] = v186;	// L267
    }
  }
}

void _Z18load_128_128_floatPA128_fS0_(
  float v187[128][128],
  float v188[128][128]
) {	// L272
  _Z18load_128_128_floatPA128_fS0__node0(v187, v188);	// L273
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node1(
  float v189[8][8],
  float v190[8][128],
  float v191[8][128],
  int v192
) {	// L276
  for (int v193 = 0; v193 < 8; v193 += 1) {	// L278
    for (int v194 = 0; v194 < 16; v194 += 1) {	// L279
      #pragma HLS pipeline II=1
      v191[v193][(v194 + (v192 * 16))] = (float)0.000000;	// L280
    }
  }
  for (int v195 = 0; v195 < 8; v195 += 1) {	// L283
    for (int v196 = 0; v196 < 8; v196 += 1) {	// L284
      for (int v197 = 0; v197 < 16; v197 += 1) {	// L285
        #pragma HLS pipeline II=1
        float v198 = v189[v196][v195];	// L286
        float v199 = v190[v195][(v197 + (v192 * 16))];	// L287
        float v200 = v198 * v199;	// L288
        float v201 = v191[v196][(v197 + (v192 * 16))];	// L289
        float v202 = v201 + v200;	// L290
        v191[v196][(v197 + (v192 * 16))] = v202;	// L291
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node3(
  float v203[8],
  float v204[8][8],
  float v205[8],
  hls::stream<float> &v206,
  hls::stream<float> &v207,
  int v208
) {	// L297
  float v209 = v203[v208];	// L298
  for (int v210 = 0; v210 < 8; v210 += 1) {	// L299
    #pragma HLS pipeline II=1
    float v211 = v204[v208][v210];	// L300
    float v212 = v211 - v209;	// L301
    float v213 = exp(v212);	// L302
    v204[v208][v210] = v213;	// L303
    float v214 = v205[v208];	// L304
    float v215 = v214 + v213;	// L305
    v205[v208] = v215;	// L306
  }
  v206.write(v209);	// L308
  float v216 = v205[v208];	// L309
  for (int v217 = 0; v217 < 8; v217 += 1) {	// L310
    #pragma HLS pipeline II=1
    float v218 = v204[v208][v217];	// L311
    float v219 = v218 / v216;	// L312
    v204[v208][v217] = v219;	// L313
  }
  v207.write(v216);	// L315
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node4(
  float v220[8][8],
  float v221[8],
  int v222
) {	// L318
  #pragma HLS inline
  for (int v223 = 0; v223 < 7; v223 += 1) {	// L319
    #pragma HLS pipeline II=1
    float v224 = v220[v222][(v223 + 1)];	// L320
    float v225 = v221[v222];	// L321
    bool v226 = v224 > v225;	// L322
    float v227 = v226 ? v224 : v225;	// L323
    v221[v222] = v227;	// L324
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node2(
  float v228[8][128],
  float v229[8][128],
  float v230[8][8],
  float v231[8],
  float v232[8],
  int v233
) {	// L328
  for (int v234 = 0; v234 < 8; v234 += 1) {	// L331
    for (int v235 = 0; v235 < 8; v235 += 1) {	// L332
      #pragma HLS pipeline II=1
      v230[v234][v235] = (float)0.000000;	// L333
    }
  }
  for (int v236 = 0; v236 < 16; v236 += 1) {	// L336
    for (int v237 = 0; v237 < 8; v237 += 1) {	// L337
      for (int v238 = 0; v238 < 8; v238 += 1) {	// L338
        #pragma HLS pipeline II=1
        float v239 = v228[v237][(v236 + (v233 * 16))];	// L339
        float v240 = v229[v238][(v236 + (v233 * 16))];	// L340
        float v241 = v239 * v240;	// L341
        float v242 = v230[v237][v238];	// L342
        float v243 = v242 + v241;	// L343
        float v244 = v243 * (float)0.250000;	// L344
        float v245 = (((-v236) + 15) == 0) ? v244 : v243;	// L345
        v230[v237][v238] = v245;	// L346
      }
    }
  }
  for (int v246 = 0; v246 < 8; v246 += 1) {	// L350
    #pragma HLS pipeline II=1
    v231[v246] = (float)0.000000;	// L351
  }
  for (int v247 = 0; v247 < 8; v247 += 1) {	// L353
    #pragma HLS pipeline II=1
    float v248 = v230[v247][0];	// L354
    v232[v247] = v248;	// L355
  }
  for (int v249 = 0; v249 < 8; v249 += 1) {	// L357
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node4(v230, v232, v249);	// L358
    hls::stream<float> v250;	// L359
    hls::stream<float> v251;	// L360
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node3(v232, v230, v231, v250, v251, v249);	// L361
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node0(
  float v252[8][128],
  float v253[8][128],
  float v254[8][128],
  float v255[8][128]
) {	// L365
  float v256[8];	// L366
  #pragma HLS bind_storage variable=v256 type=ram_t2p impl=bram

  float v257[8];	// L367
  #pragma HLS bind_storage variable=v257 type=ram_t2p impl=bram

  float v258[8][8];	// L368
  #pragma HLS bind_storage variable=v258 type=ram_t2p impl=bram

  for (int v259 = 0; v259 < 8; v259 += 1) {	// L369
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node2(v252, v254, v258, v257, v256, v259);	// L370
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node1(v258, v253, v255, v259);	// L371
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node5(
  float v260[8][128],
  float v261[8][128],
  float v262[8][128],
  float v263[8][128]
) {	// L375
  float v264;	// L378
  for (int v265 = 0; v265 < 8; v265 += 1) {	// L379
    for (int v266 = 0; v266 < 128; v266 += 1) {	// L380
      #pragma HLS pipeline II=1
      float v267 = v261[v265][v266];	// L381
      v263[v265][v266] = v267;	// L382
    }
  }
  for (int v268 = 0; v268 < 8; v268 += 1) {	// L385
    for (int v269 = 0; v269 < 128; v269 += 1) {	// L386
      #pragma HLS pipeline II=1
      float v270 = v260[v268][v269];	// L387
      v262[v268][v269] = v270;	// L388
    }
  }
  for (int v271 = 0; v271 < 8; v271 += 1) {	// L391
    for (int v272 = 0; v272 < 8; v272 += 1) {	// L392
      for (int v273 = 0; v273 < 8; v273 += 1) {	// L393
        #pragma HLS pipeline II=1
        ap_int<32> v274 = v271;	// L394
        float v275 = v274;	// L395
        float v276 = v264;	// L396
        float v277 = (v273 == 0 && v272 == 0) ? v275 : v276;	// L397
        v264 = v277;	// L398
        int v278 = (v272 * 2);	// L399
        ap_int<32> v279 = v278;	// L400
        float v280 = v279;	// L401
        float v281 = -(v280);	// L402
        float v282 = v281 / (float)16.000000;	// L403
        float v283 = pow((float)10000.000000, v282);	// L404
        float v284 = v277 * v283;	// L405
        float v285 = cos(v284);	// L406
        float v286 = sin(v284);	// L407
        float v287 = v262[v271][((v273 * 16) + (v272 * 2))];	// L408
        float v288 = v262[v271][(((v273 * 16) + (v272 * 2)) + 1)];	// L409
        float v289 = v287 * v285;	// L410
        float v290 = v288 * v286;	// L411
        float v291 = v289 - v290;	// L412
        v262[v271][((v273 * 16) + (v272 * 2))] = v291;	// L413
        float v292 = v287 * v286;	// L414
        float v293 = v288 * v285;	// L415
        float v294 = v292 + v293;	// L416
        v262[v271][(((v273 * 16) + (v272 * 2)) + 1)] = v294;	// L417
        float v295 = v263[v271][((v273 * 16) + (v272 * 2))];	// L418
        float v296 = v263[v271][(((v273 * 16) + (v272 * 2)) + 1)];	// L419
        float v297 = v295 * v285;	// L420
        float v298 = v296 * v286;	// L421
        float v299 = v297 - v298;	// L422
        v263[v271][((v273 * 16) + (v272 * 2))] = v299;	// L423
        float v300 = v295 * v286;	// L424
        float v301 = v296 * v285;	// L425
        float v302 = v300 + v301;	// L426
        v263[v271][(((v273 * 16) + (v272 * 2)) + 1)] = v302;	// L427
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node6(
  float v303[8][128],
  float v304[8][128]
) {	// L433
  #pragma HLS inline
  for (int v305 = 0; v305 < 8; v305 += 1) {	// L434
    for (int v306 = 0; v306 < 128; v306 += 1) {	// L435
      #pragma HLS pipeline II=1
      float v307 = v303[v305][v306];	// L436
      v304[v305][v306] = v307;	// L437
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node7(
  float v308[8][128],
  float v309[128][128],
  float v310[128][128],
  float v311[128][128],
  float v312[8][128],
  float v313[8][128],
  float v314[8][128],
  float v315[8][128],
  float v316[8][128],
  float v317[8][128]
) {	// L442
  #pragma HLS inline
  for (int v318 = 0; v318 < 128; v318 += 1) {	// L443
    for (int v319 = 0; v319 < 8; v319 += 1) {	// L444
      for (int v320 = 0; v320 < 128; v320 += 1) {	// L445
        #pragma HLS pipeline II=1
        float v321 = v308[v319][v318];	// L446
        float v322 = v310[v320][v318];	// L447
        float v323 = v321 * v322;	// L448
        float v324 = v312[v319][v320];	// L449
        float v325 = v315[v319][v320];	// L450
        float v326 = (v318 == 0) ? v324 : v325;	// L451
        float v327 = v326 + v323;	// L452
        v315[v319][v320] = v327;	// L453
        float v328 = v309[v320][v318];	// L454
        float v329 = v321 * v328;	// L455
        float v330 = v313[v319][v320];	// L456
        float v331 = v317[v319][v320];	// L457
        float v332 = (v318 == 0) ? v330 : v331;	// L458
        float v333 = v332 + v329;	// L459
        v317[v319][v320] = v333;	// L460
        float v334 = v311[v320][v318];	// L461
        float v335 = v321 * v334;	// L462
        float v336 = v314[v319][v320];	// L463
        float v337 = v316[v319][v320];	// L464
        float v338 = (v318 == 0) ? v336 : v337;	// L465
        float v339 = v338 + v335;	// L466
        v316[v319][v320] = v339;	// L467
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node8(
  float v340[8][128],
  float v341[8][128],
  float v342[8][128]
) {	// L473
  #pragma HLS inline
  for (int v343 = 0; v343 < 8; v343 += 1) {	// L475
    for (int v344 = 0; v344 < 128; v344 += 1) {	// L476
      #pragma HLS pipeline II=1
      v340[v343][v344] = (float)0.000000;	// L477
      v341[v343][v344] = (float)0.000000;	// L478
      v342[v343][v344] = (float)0.000000;	// L479
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i(
  float v345[8][128],
  float v346[128][128],
  float v347[128][128],
  float v348[128][128],
  float v349[8][128],
  ap_int<32> v350
) {	// L484
  float v351[8][128];	// L485
  #pragma HLS bind_storage variable=v351 type=ram_t2p impl=bram

  float v352[8][128];	// L486
  #pragma HLS bind_storage variable=v352 type=ram_t2p impl=bram

  float v353[8][128];	// L487
  #pragma HLS bind_storage variable=v353 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node8(v351, v352, v353);	// L488
  float v354[8][128];	// L489
  #pragma HLS bind_storage variable=v354 type=ram_t2p impl=bram

  float v355[8][128];	// L490
  #pragma HLS bind_storage variable=v355 type=ram_t2p impl=bram

  float v356[8][128];	// L491
  #pragma HLS bind_storage variable=v356 type=ram_t2p impl=bram

  float v357[8][128];	// L492
  #pragma HLS bind_storage variable=v357 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node7(v345, v347, v346, v348, v351, v352, v353, v354, v356, v355);	// L493
  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node6(v356, v357);	// L494
  float v358[8][128];	// L495
  #pragma HLS bind_storage variable=v358 type=ram_t2p impl=bram

  float v359[8][128];	// L496
  #pragma HLS bind_storage variable=v359 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node5(v354, v355, v358, v359);	// L497
  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node0(v358, v357, v359, v349);	// L498
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v360[8][128],
  float v361[8][128]
) {	// L501
  #pragma HLS inline
  for (int v362 = 0; v362 < 8; v362 += 1) {	// L502
    for (int v363 = 0; v363 < 128; v363 += 1) {	// L503
      #pragma HLS pipeline II=1
      float v364 = v360[v362][v363];	// L504
      v361[v362][v363] = v364;	// L505
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v365[8][128],
  float v366[8][128]
) {	// L510
  _Z17store_8_128_floatPA128_fS0__node0(v365, v366);	// L511
}

