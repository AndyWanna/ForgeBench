
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
  float v124[8][128]
) {	// L165
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v124 bundle=axi_6

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v123 bundle=axi_5

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v122 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v121 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v120 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v119 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v118 bundle=axi_0

  float v132[8][128];	// L180
  #pragma HLS bind_storage variable=v132 type=ram_t2p impl=bram

  float v133[8][128];	// L181
  #pragma HLS bind_storage variable=v133 type=ram_t2p impl=bram

  float v134[8][128];	// L182
  #pragma HLS bind_storage variable=v134 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node8(v132, v133, v134);	// L183
  float v135[8][128];	// L184
  #pragma HLS bind_storage variable=v135 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node7(v120, v121, v132, v135);	// L185
  float v136[8][128];	// L186
  #pragma HLS bind_storage variable=v136 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node6(v119, v122, v133, v136);	// L187
  float v137[8][128];	// L188
  #pragma HLS bind_storage variable=v137 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0__node5(v118, v123, v134, v137);	// L189
  _Z3topPA128_fS0_S0_S0_S0__node0(v135, v136, v137, v124);	// L190
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v138[8][128],
  float v139[8][128]
) {	// L204
  #pragma HLS inline
  for (int v140 = 0; v140 < 8; v140 += 1) {	// L205
    for (int v141 = 0; v141 < 128; v141 += 1) {	// L206
      #pragma HLS pipeline II=1
      float v142 = v138[v140][v141];	// L207
      v139[v140][v141] = v142;	// L208
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v143[8][128],
  float v144[8][128]
) {	// L213
  _Z16load_8_128_floatPA128_fS0__node0(v143, v144);	// L214
}

void _Z18load_128_128_floatPA128_fS0__node0(
  float v145[128][128],
  float v146[128][128]
) {	// L217
  #pragma HLS inline
  for (int v147 = 0; v147 < 128; v147 += 1) {	// L218
    for (int v148 = 0; v148 < 128; v148 += 1) {	// L219
      #pragma HLS pipeline II=1
      float v149 = v145[v147][v148];	// L220
      v146[v147][v148] = v149;	// L221
    }
  }
}

void _Z18load_128_128_floatPA128_fS0_(
  float v150[128][128],
  float v151[128][128]
) {	// L226
  _Z18load_128_128_floatPA128_fS0__node0(v150, v151);	// L227
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node1(
  float v152[8][8],
  float v153[8][128],
  float v154[8][128],
  int v155
) {	// L230
  for (int v156 = 0; v156 < 8; v156 += 1) {	// L232
    for (int v157 = 0; v157 < 16; v157 += 1) {	// L233
      #pragma HLS pipeline II=1
      v154[v156][(v157 + (v155 * 16))] = (float)0.000000;	// L234
    }
  }
  for (int v158 = 0; v158 < 8; v158 += 1) {	// L237
    for (int v159 = 0; v159 < 8; v159 += 1) {	// L238
      for (int v160 = 0; v160 < 16; v160 += 1) {	// L239
        #pragma HLS pipeline II=1
        float v161 = v152[v159][v158];	// L240
        float v162 = v153[v158][(v160 + (v155 * 16))];	// L241
        float v163 = v161 * v162;	// L242
        float v164 = v154[v159][(v160 + (v155 * 16))];	// L243
        float v165 = v164 + v163;	// L244
        v154[v159][(v160 + (v155 * 16))] = v165;	// L245
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node3(
  float v166[8],
  float v167[8][8],
  float v168[8],
  hls::stream<float> &v169,
  hls::stream<float> &v170,
  int v171
) {	// L251
  float v172 = v166[v171];	// L252
  for (int v173 = 0; v173 < 8; v173 += 1) {	// L253
    #pragma HLS pipeline II=1
    float v174 = v167[v171][v173];	// L254
    float v175 = v174 - v172;	// L255
    float v176 = exp(v175);	// L256
    v167[v171][v173] = v176;	// L257
    float v177 = v168[v171];	// L258
    float v178 = v177 + v176;	// L259
    v168[v171] = v178;	// L260
  }
  v169.write(v172);	// L262
  float v179 = v168[v171];	// L263
  for (int v180 = 0; v180 < 8; v180 += 1) {	// L264
    #pragma HLS pipeline II=1
    float v181 = v167[v171][v180];	// L265
    float v182 = v181 / v179;	// L266
    v167[v171][v180] = v182;	// L267
  }
  v170.write(v179);	// L269
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node4(
  float v183[8][8],
  float v184[8],
  int v185
) {	// L272
  #pragma HLS inline
  for (int v186 = 0; v186 < 7; v186 += 1) {	// L273
    #pragma HLS pipeline II=1
    float v187 = v183[v185][(v186 + 1)];	// L274
    float v188 = v184[v185];	// L275
    bool v189 = v187 > v188;	// L276
    float v190 = v189 ? v187 : v188;	// L277
    v184[v185] = v190;	// L278
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node2(
  float v191[8][128],
  float v192[8][128],
  float v193[8][8],
  float v194[8],
  float v195[8],
  int v196
) {	// L282
  for (int v197 = 0; v197 < 8; v197 += 1) {	// L285
    for (int v198 = 0; v198 < 8; v198 += 1) {	// L286
      #pragma HLS pipeline II=1
      v193[v197][v198] = (float)0.000000;	// L287
    }
  }
  for (int v199 = 0; v199 < 16; v199 += 1) {	// L290
    for (int v200 = 0; v200 < 8; v200 += 1) {	// L291
      for (int v201 = 0; v201 < 8; v201 += 1) {	// L292
        #pragma HLS pipeline II=1
        float v202 = v191[v200][(v199 + (v196 * 16))];	// L293
        float v203 = v192[v201][(v199 + (v196 * 16))];	// L294
        float v204 = v202 * v203;	// L295
        float v205 = v193[v200][v201];	// L296
        float v206 = v205 + v204;	// L297
        float v207 = v206 * (float)0.250000;	// L298
        float v208 = (((-v199) + 15) == 0) ? v207 : v206;	// L299
        v193[v200][v201] = v208;	// L300
      }
    }
  }
  for (int v209 = 0; v209 < 8; v209 += 1) {	// L304
    #pragma HLS pipeline II=1
    v194[v209] = (float)0.000000;	// L305
  }
  for (int v210 = 0; v210 < 8; v210 += 1) {	// L307
    #pragma HLS pipeline II=1
    float v211 = v193[v210][0];	// L308
    v195[v210] = v211;	// L309
  }
  for (int v212 = 0; v212 < 8; v212 += 1) {	// L311
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node4(v193, v195, v212);	// L312
    hls::stream<float> v213;	// L313
    hls::stream<float> v214;	// L314
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node3(v195, v193, v194, v213, v214, v212);	// L315
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node0(
  float v215[8][128],
  float v216[8][128],
  float v217[8][128],
  float v218[8][128]
) {	// L319
  float v219[8];	// L320
  #pragma HLS bind_storage variable=v219 type=ram_t2p impl=bram

  float v220[8];	// L321
  #pragma HLS bind_storage variable=v220 type=ram_t2p impl=bram

  float v221[8][8];	// L322
  #pragma HLS bind_storage variable=v221 type=ram_t2p impl=bram

  for (int v222 = 0; v222 < 8; v222 += 1) {	// L323
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node2(v217, v216, v221, v220, v219, v222);	// L324
    _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node1(v221, v215, v218, v222);	// L325
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node5(
  float v223[8][128],
  float v224[128][128],
  float v225[8][128],
  float v226[8][128]
) {	// L329
  #pragma HLS inline
  for (int v227 = 0; v227 < 128; v227 += 1) {	// L330
    for (int v228 = 0; v228 < 8; v228 += 1) {	// L331
      for (int v229 = 0; v229 < 128; v229 += 1) {	// L332
        #pragma HLS pipeline II=1
        float v230 = v223[v228][v227];	// L333
        float v231 = v224[v229][v227];	// L334
        float v232 = v230 * v231;	// L335
        float v233 = v225[v228][v229];	// L336
        float v234 = v226[v228][v229];	// L337
        float v235 = (v227 == 0) ? v233 : v234;	// L338
        float v236 = v235 + v232;	// L339
        v226[v228][v229] = v236;	// L340
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node6(
  float v237[8][128],
  float v238[128][128],
  float v239[8][128],
  float v240[8][128]
) {	// L346
  #pragma HLS inline
  for (int v241 = 0; v241 < 128; v241 += 1) {	// L347
    for (int v242 = 0; v242 < 8; v242 += 1) {	// L348
      for (int v243 = 0; v243 < 128; v243 += 1) {	// L349
        #pragma HLS pipeline II=1
        float v244 = v237[v242][v241];	// L350
        float v245 = v238[v243][v241];	// L351
        float v246 = v244 * v245;	// L352
        float v247 = v239[v242][v243];	// L353
        float v248 = v240[v242][v243];	// L354
        float v249 = (v241 == 0) ? v247 : v248;	// L355
        float v250 = v249 + v246;	// L356
        v240[v242][v243] = v250;	// L357
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node7(
  float v251[8][128],
  float v252[128][128],
  float v253[8][128],
  float v254[8][128]
) {	// L363
  #pragma HLS inline
  for (int v255 = 0; v255 < 128; v255 += 1) {	// L364
    for (int v256 = 0; v256 < 8; v256 += 1) {	// L365
      for (int v257 = 0; v257 < 128; v257 += 1) {	// L366
        #pragma HLS pipeline II=1
        float v258 = v251[v256][v255];	// L367
        float v259 = v252[v257][v255];	// L368
        float v260 = v258 * v259;	// L369
        float v261 = v253[v256][v257];	// L370
        float v262 = v254[v256][v257];	// L371
        float v263 = (v255 == 0) ? v261 : v262;	// L372
        float v264 = v263 + v260;	// L373
        v254[v256][v257] = v264;	// L374
      }
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node8(
  float v265[8][128],
  float v266[8][128],
  float v267[8][128]
) {	// L380
  #pragma HLS inline
  for (int v268 = 0; v268 < 8; v268 += 1) {	// L382
    for (int v269 = 0; v269 < 128; v269 += 1) {	// L383
      #pragma HLS pipeline II=1
      v265[v268][v269] = (float)0.000000;	// L384
      v266[v268][v269] = (float)0.000000;	// L385
      v267[v268][v269] = (float)0.000000;	// L386
    }
  }
}

void _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i(
  float v270[8][128],
  float v271[128][128],
  float v272[128][128],
  float v273[128][128],
  float v274[8][128],
  ap_int<32> v275
) {	// L391
  float v276[8][128];	// L392
  #pragma HLS bind_storage variable=v276 type=ram_t2p impl=bram

  float v277[8][128];	// L393
  #pragma HLS bind_storage variable=v277 type=ram_t2p impl=bram

  float v278[8][128];	// L394
  #pragma HLS bind_storage variable=v278 type=ram_t2p impl=bram

  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node8(v276, v277, v278);	// L395
  float v279[8][128];	// L396
  #pragma HLS bind_storage variable=v279 type=ram_t2p impl=bram

  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node7(v270, v271, v276, v279);	// L397
  float v280[8][128];	// L398
  #pragma HLS bind_storage variable=v280 type=ram_t2p impl=bram

  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node6(v270, v272, v277, v280);	// L399
  float v281[8][128];	// L400
  #pragma HLS bind_storage variable=v281 type=ram_t2p impl=bram

  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node5(v270, v273, v278, v281);	// L401
  _Z44grouped_multihead_attention_8_128_8_16_floatPA128_fS0_S0_S0_S0_i_node0(v281, v280, v279, v274);	// L402
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v282[8][128],
  float v283[8][128]
) {	// L405
  #pragma HLS inline
  for (int v284 = 0; v284 < 8; v284 += 1) {	// L406
    for (int v285 = 0; v285 < 128; v285 += 1) {	// L407
      #pragma HLS pipeline II=1
      float v286 = v282[v284][v285];	// L408
      v283[v284][v285] = v286;	// L409
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v287[8][128],
  float v288[8][128]
) {	// L414
  _Z17store_8_128_floatPA128_fS0__node0(v287, v288);	// L415
}

