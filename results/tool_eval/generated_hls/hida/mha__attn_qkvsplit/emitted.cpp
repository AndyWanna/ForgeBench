
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
    _Z3topPA128_fS0_S0_S0_S0__node2(v63, v65, v69, v68, v67, v70);	// L98
    _Z3topPA128_fS0_S0_S0_S0__node1(v69, v64, v66, v70);	// L99
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node5(
  float v71[8][128],
  float v72[128][128],
  float v73[8][128],
  float v74[8][128]
) {	// L103
  #pragma HLS inline
  for (int v75 = 0; v75 < 128; v75 += 1) {	// L104
    for (int v76 = 0; v76 < 8; v76 += 1) {	// L105
      for (int v77 = 0; v77 < 128; v77 += 1) {	// L106
        #pragma HLS pipeline II=1
        float v78 = v71[v76][v75];	// L107
        float v79 = v72[v77][v75];	// L108
        float v80 = v78 * v79;	// L109
        float v81 = v73[v76][v77];	// L110
        float v82 = v74[v76][v77];	// L111
        float v83 = (v75 == 0) ? v81 : v82;	// L112
        float v84 = v83 + v80;	// L113
        v74[v76][v77] = v84;	// L114
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node6(
  float v85[8][128],
  float v86[128][128],
  float v87[8][128],
  float v88[8][128]
) {	// L120
  #pragma HLS inline
  for (int v89 = 0; v89 < 128; v89 += 1) {	// L121
    for (int v90 = 0; v90 < 8; v90 += 1) {	// L122
      for (int v91 = 0; v91 < 128; v91 += 1) {	// L123
        #pragma HLS pipeline II=1
        float v92 = v85[v90][v89];	// L124
        float v93 = v86[v91][v89];	// L125
        float v94 = v92 * v93;	// L126
        float v95 = v87[v90][v91];	// L127
        float v96 = v88[v90][v91];	// L128
        float v97 = (v89 == 0) ? v95 : v96;	// L129
        float v98 = v97 + v94;	// L130
        v88[v90][v91] = v98;	// L131
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node7(
  float v99[8][128],
  float v100[128][128],
  float v101[8][128],
  float v102[8][128]
) {	// L137
  #pragma HLS inline
  for (int v103 = 0; v103 < 128; v103 += 1) {	// L138
    for (int v104 = 0; v104 < 8; v104 += 1) {	// L139
      for (int v105 = 0; v105 < 128; v105 += 1) {	// L140
        #pragma HLS pipeline II=1
        float v106 = v99[v104][v103];	// L141
        float v107 = v100[v105][v103];	// L142
        float v108 = v106 * v107;	// L143
        float v109 = v101[v104][v105];	// L144
        float v110 = v102[v104][v105];	// L145
        float v111 = (v103 == 0) ? v109 : v110;	// L146
        float v112 = v111 + v108;	// L147
        v102[v104][v105] = v112;	// L148
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0__node8(
  float v113[8][128],
  float v114[8][128],
  float v115[8][128]
) {	// L154
  #pragma HLS inline
  for (int v116 = 0; v116 < 8; v116 += 1) {	// L156
    for (int v117 = 0; v117 < 128; v117 += 1) {	// L157
      #pragma HLS pipeline II=1
      v113[v116][v117] = (float)0.000000;	// L158
      v114[v116][v117] = (float)0.000000;	// L159
      v115[v116][v117] = (float)0.000000;	// L160
    }
  }
}

/// This is top function.
void _Z3topPA128_fS0_S0_S0_S0_(
  float v118[8][128],
  float v119[8][128],
  float v120[8][128],
  float v121[128][128],
  float v122[128][128],
  float v123[128][128],
  float v124[8][128],
  float v125[8][128],
  float v126[8][128],
  float v127[8][128],
  float v128[8][128],
  float v129[8][128],
  float v130[8][128],
  float v131[8][128],
  float v132[8][128],
  float v133[8][128],
  float v134[8][128],
  float v135[8][128],
  float v136[8][128]
) {	// L165
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface m_axi offset=slave port=v136 bundle=axi_18

  #pragma HLS interface m_axi offset=slave port=v135 bundle=axi_17

  #pragma HLS interface m_axi offset=slave port=v134 bundle=axi_16

  #pragma HLS interface m_axi offset=slave port=v133 bundle=axi_15

  #pragma HLS interface m_axi offset=slave port=v132 bundle=axi_14

  #pragma HLS interface m_axi offset=slave port=v131 bundle=axi_13

  #pragma HLS interface m_axi offset=slave port=v130 bundle=axi_12

  #pragma HLS interface m_axi offset=slave port=v129 bundle=axi_11

  #pragma HLS interface m_axi offset=slave port=v128 bundle=axi_10

  #pragma HLS interface m_axi offset=slave port=v127 bundle=axi_9

  #pragma HLS interface m_axi offset=slave port=v126 bundle=axi_8

  #pragma HLS interface m_axi offset=slave port=v125 bundle=axi_7

  #pragma HLS interface m_axi offset=slave port=v124 bundle=axi_6

  #pragma HLS interface m_axi offset=slave port=v123 bundle=axi_5

  #pragma HLS interface m_axi offset=slave port=v122 bundle=axi_4

  #pragma HLS interface m_axi offset=slave port=v121 bundle=axi_3

  #pragma HLS interface m_axi offset=slave port=v120 bundle=axi_2

  #pragma HLS interface m_axi offset=slave port=v119 bundle=axi_1

  #pragma HLS interface m_axi offset=slave port=v118 bundle=axi_0

  _Z3topPA128_fS0_S0_S0_S0__node8(v125, v127, v129);	// L204
  _Z3topPA128_fS0_S0_S0_S0__node7(v120, v121, v126, v131);	// L205
  _Z3topPA128_fS0_S0_S0_S0__node6(v119, v122, v128, v133);	// L206
  _Z3topPA128_fS0_S0_S0_S0__node5(v118, v123, v130, v135);	// L207
  _Z3topPA128_fS0_S0_S0_S0__node0(v132, v136, v134, v124);	// L208
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v156[8][128],
  float v157[8][128]
) {	// L240
  #pragma HLS inline
  for (int v158 = 0; v158 < 8; v158 += 1) {	// L241
    for (int v159 = 0; v159 < 128; v159 += 1) {	// L242
      #pragma HLS pipeline II=1
      float v160 = v156[v158][v159];	// L243
      v157[v158][v159] = v160;	// L244
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v161[8][128],
  float v162[8][128]
) {	// L249
  _Z16load_8_128_floatPA128_fS0__node0(v161, v162);	// L250
}

void _Z18load_128_128_floatPA128_fS0__node0(
  float v163[128][128],
  float v164[128][128]
) {	// L253
  #pragma HLS inline
  for (int v165 = 0; v165 < 128; v165 += 1) {	// L254
    for (int v166 = 0; v166 < 128; v166 += 1) {	// L255
      #pragma HLS pipeline II=1
      float v167 = v163[v165][v166];	// L256
      v164[v165][v166] = v167;	// L257
    }
  }
}

void _Z18load_128_128_floatPA128_fS0_(
  float v168[128][128],
  float v169[128][128]
) {	// L262
  _Z18load_128_128_floatPA128_fS0__node0(v168, v169);	// L263
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node1(
  float v170[8][8],
  float v171[8][128],
  float v172[8][128],
  int v173
) {	// L266
  for (int v174 = 0; v174 < 8; v174 += 1) {	// L268
    for (int v175 = 0; v175 < 16; v175 += 1) {	// L269
      #pragma HLS pipeline II=1
      v172[v174][(v175 + (v173 * 16))] = (float)0.000000;	// L270
    }
  }
  for (int v176 = 0; v176 < 8; v176 += 1) {	// L273
    for (int v177 = 0; v177 < 8; v177 += 1) {	// L274
      for (int v178 = 0; v178 < 16; v178 += 1) {	// L275
        #pragma HLS pipeline II=1
        float v179 = v170[v177][v176];	// L276
        float v180 = v171[v176][(v178 + (v173 * 16))];	// L277
        float v181 = v179 * v180;	// L278
        float v182 = v172[v177][(v178 + (v173 * 16))];	// L279
        float v183 = v182 + v181;	// L280
        v172[v177][(v178 + (v173 * 16))] = v183;	// L281
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node3(
  float v184[8],
  float v185[8][8],
  float v186[8],
  hls::stream<float> &v187,
  hls::stream<float> &v188,
  int v189
) {	// L287
  float v190 = v184[v189];	// L288
  for (int v191 = 0; v191 < 8; v191 += 1) {	// L289
    #pragma HLS pipeline II=1
    float v192 = v185[v189][v191];	// L290
    float v193 = v192 - v190;	// L291
    float v194 = exp(v193);	// L292
    v185[v189][v191] = v194;	// L293
    float v195 = v186[v189];	// L294
    float v196 = v195 + v194;	// L295
    v186[v189] = v196;	// L296
  }
  v187.write(v190);	// L298
  float v197 = v186[v189];	// L299
  for (int v198 = 0; v198 < 8; v198 += 1) {	// L300
    #pragma HLS pipeline II=1
    float v199 = v185[v189][v198];	// L301
    float v200 = v199 / v197;	// L302
    v185[v189][v198] = v200;	// L303
  }
  v188.write(v197);	// L305
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node4(
  float v201[8][8],
  float v202[8],
  int v203
) {	// L308
  #pragma HLS inline
  for (int v204 = 0; v204 < 7; v204 += 1) {	// L309
    #pragma HLS pipeline II=1
    float v205 = v201[v203][(v204 + 1)];	// L310
    float v206 = v202[v203];	// L311
    bool v207 = v205 > v206;	// L312
    float v208 = v207 ? v205 : v206;	// L313
    v202[v203] = v208;	// L314
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node2(
  float v209[8][128],
  float v210[8][128],
  float v211[8][8],
  float v212[8],
  float v213[8],
  int v214
) {	// L318
  for (int v215 = 0; v215 < 8; v215 += 1) {	// L321
    for (int v216 = 0; v216 < 8; v216 += 1) {	// L322
      #pragma HLS pipeline II=1
      v211[v215][v216] = (float)0.000000;	// L323
    }
  }
  for (int v217 = 0; v217 < 16; v217 += 1) {	// L326
    for (int v218 = 0; v218 < 8; v218 += 1) {	// L327
      for (int v219 = 0; v219 < 8; v219 += 1) {	// L328
        #pragma HLS pipeline II=1
        float v220 = v209[v218][(v217 + (v214 * 16))];	// L329
        float v221 = v210[v219][(v217 + (v214 * 16))];	// L330
        float v222 = v220 * v221;	// L331
        float v223 = v211[v218][v219];	// L332
        float v224 = v223 + v222;	// L333
        float v225 = v224 * (float)0.250000;	// L334
        float v226 = (((-v217) + 15) == 0) ? v225 : v224;	// L335
        v211[v218][v219] = v226;	// L336
      }
    }
  }
  for (int v227 = 0; v227 < 8; v227 += 1) {	// L340
    #pragma HLS pipeline II=1
    v212[v227] = (float)0.000000;	// L341
  }
  for (int v228 = 0; v228 < 8; v228 += 1) {	// L343
    #pragma HLS pipeline II=1
    float v229 = v211[v228][0];	// L344
    v213[v228] = v229;	// L345
  }
  for (int v230 = 0; v230 < 8; v230 += 1) {	// L347
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node4(v211, v213, v230);	// L348
    hls::stream<float> v231;	// L349
    hls::stream<float> v232;	// L350
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node3(v213, v211, v212, v231, v232, v230);	// L351
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node0(
  float v233[8][128],
  float v234[8][128],
  float v235[8][128],
  float v236[8][128]
) {	// L355
  float v237[8];	// L356
  #pragma HLS bind_storage variable=v237 type=ram_t2p impl=bram

  float v238[8];	// L357
  #pragma HLS bind_storage variable=v238 type=ram_t2p impl=bram

  float v239[8][8];	// L358
  #pragma HLS bind_storage variable=v239 type=ram_t2p impl=bram

  for (int v240 = 0; v240 < 8; v240 += 1) {	// L359
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node2(v235, v234, v239, v238, v237, v240);	// L360
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node1(v239, v233, v236, v240);	// L361
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node5(
  float v241[8][128],
  float v242[128][128],
  float v243[8][128],
  float v244[8][128]
) {	// L365
  #pragma HLS inline
  for (int v245 = 0; v245 < 128; v245 += 1) {	// L366
    for (int v246 = 0; v246 < 8; v246 += 1) {	// L367
      for (int v247 = 0; v247 < 128; v247 += 1) {	// L368
        #pragma HLS pipeline II=1
        float v248 = v241[v246][v245];	// L369
        float v249 = v242[v247][v245];	// L370
        float v250 = v248 * v249;	// L371
        float v251 = v243[v246][v247];	// L372
        float v252 = v244[v246][v247];	// L373
        float v253 = (v245 == 0) ? v251 : v252;	// L374
        float v254 = v253 + v250;	// L375
        v244[v246][v247] = v254;	// L376
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node6(
  float v255[8][128],
  float v256[128][128],
  float v257[8][128],
  float v258[8][128]
) {	// L382
  #pragma HLS inline
  for (int v259 = 0; v259 < 128; v259 += 1) {	// L383
    for (int v260 = 0; v260 < 8; v260 += 1) {	// L384
      for (int v261 = 0; v261 < 128; v261 += 1) {	// L385
        #pragma HLS pipeline II=1
        float v262 = v255[v260][v259];	// L386
        float v263 = v256[v261][v259];	// L387
        float v264 = v262 * v263;	// L388
        float v265 = v257[v260][v261];	// L389
        float v266 = v258[v260][v261];	// L390
        float v267 = (v259 == 0) ? v265 : v266;	// L391
        float v268 = v267 + v264;	// L392
        v258[v260][v261] = v268;	// L393
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node7(
  float v269[8][128],
  float v270[128][128],
  float v271[8][128],
  float v272[8][128]
) {	// L399
  #pragma HLS inline
  for (int v273 = 0; v273 < 128; v273 += 1) {	// L400
    for (int v274 = 0; v274 < 8; v274 += 1) {	// L401
      for (int v275 = 0; v275 < 128; v275 += 1) {	// L402
        #pragma HLS pipeline II=1
        float v276 = v269[v274][v273];	// L403
        float v277 = v270[v275][v273];	// L404
        float v278 = v276 * v277;	// L405
        float v279 = v271[v274][v275];	// L406
        float v280 = v272[v274][v275];	// L407
        float v281 = (v273 == 0) ? v279 : v280;	// L408
        float v282 = v281 + v278;	// L409
        v272[v274][v275] = v282;	// L410
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node8(
  float v283[8][128],
  float v284[8][128],
  float v285[8][128]
) {	// L416
  #pragma HLS inline
  for (int v286 = 0; v286 < 8; v286 += 1) {	// L418
    for (int v287 = 0; v287 < 128; v287 += 1) {	// L419
      #pragma HLS pipeline II=1
      v283[v286][v287] = (float)0.000000;	// L420
      v284[v286][v287] = (float)0.000000;	// L421
      v285[v286][v287] = (float)0.000000;	// L422
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i(
  float v288[8][128],
  float v289[128][128],
  float v290[128][128],
  float v291[128][128],
  float v292[8][128],
  ap_int<32> v293
) {	// L427
  float v294[8][128];	// L428
  float v295[8][128];	// L429
  float v296[8][128];	// L430
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node8(v294, v295, v296);	// L431
  float v297[8][128];	// L432
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node7(v288, v289, v294, v297);	// L433
  float v298[8][128];	// L434
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node6(v288, v290, v295, v298);	// L435
  float v299[8][128];	// L436
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node5(v288, v291, v296, v299);	// L437
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node0(v299, v298, v297, v292);	// L438
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v300[8][128],
  float v301[8][128]
) {	// L441
  #pragma HLS inline
  for (int v302 = 0; v302 < 8; v302 += 1) {	// L442
    for (int v303 = 0; v303 < 128; v303 += 1) {	// L443
      #pragma HLS pipeline II=1
      float v304 = v300[v302][v303];	// L444
      v301[v302][v303] = v304;	// L445
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v305[8][128],
  float v306[8][128]
) {	// L450
  _Z17store_8_128_floatPA128_fS0__node0(v305, v306);	// L451
}

