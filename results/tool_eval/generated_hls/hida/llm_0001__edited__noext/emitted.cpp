
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

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node1(
  float v0[128],
  float v1[8],
  float v2[8][128],
  float v3[128],
  hls::stream<float> &v4,
  float v5[8][128],
  hls::stream<float> &v6,
  int v7
) {	// L6
  #pragma HLS inline
  float v8 = v4.read();	// L9
  float v9 = v1[v7];	// L10
  float v10 = v9 / (float)128.000000;	// L11
  float v11 = v10 + (float)0.010000;	// L12
  float v12 = sqrt(v11);	// L13
  for (int v13 = 0; v13 < 128; v13 += 1) {	// L14
    #pragma HLS pipeline II=1
    float v14 = v0[v13];	// L15
    float v15 = v2[v7][v13];	// L16
    float v16 = v15 - v8;	// L17
    float v17 = v16 / v12;	// L18
    float v18 = v14 * v17;	// L19
    float v19 = v3[v13];	// L20
    float v20 = v18 + v19;	// L21
    v5[v7][v13] = v20;	// L22
  }
  v6.write(v12);	// L24
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node2(
  float v21[8],
  float v22[8][128],
  float v23[8],
  hls::stream<float> &v24,
  int v25
) {	// L27
  #pragma HLS inline
  float v26 = v21[v25];	// L29
  float v27 = v26 / (float)128.000000;	// L30
  for (int v28 = 0; v28 < 128; v28 += 1) {	// L31
    #pragma HLS pipeline II=1
    float v29 = v22[v25][v28];	// L32
    float v30 = v29 - v27;	// L33
    float v31 = v30 * v30;	// L34
    float v32 = v23[v25];	// L35
    float v33 = v32 + v31;	// L36
    v23[v25] = v33;	// L37
  }
  v24.write(v27);	// L39
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node3(
  float v34[8][128],
  float v35[8],
  int v36
) {	// L42
  #pragma HLS inline
  for (int v37 = 0; v37 < 128; v37 += 1) {	// L43
    #pragma HLS pipeline II=1
    float v38 = v34[v36][v37];	// L44
    float v39 = v35[v36];	// L45
    float v40 = v39 + v38;	// L46
    v35[v36] = v40;	// L47
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node0(
  float v41[128],
  float v42[8][128],
  float v43[128],
  float v44[8],
  float v45[8],
  float v46[8][128]
) {	// L51
  float v47[8];	// L52
  #pragma HLS bind_storage variable=v47 type=ram_t2p impl=bram

  float v48[8];	// L53
  #pragma HLS bind_storage variable=v48 type=ram_t2p impl=bram

  for (int v49 = 0; v49 < 8; v49 += 1) {	// L54
    #pragma HLS pipeline II=1
    float v50 = v45[v49];	// L55
    v47[v49] = v50;	// L56
  }
  for (int v51 = 0; v51 < 8; v51 += 1) {	// L58
    #pragma HLS pipeline II=1
    float v52 = v44[v51];	// L59
    v48[v51] = v52;	// L60
  }
  for (int v53 = 0; v53 < 8; v53 += 1) {	// L62
    _Z3topPA128_fS0_S0_S0_S0_PfS1__node3(v42, v48, v53);	// L63
    hls::stream<float> v54;	// L64
    _Z3topPA128_fS0_S0_S0_S0_PfS1__node2(v48, v42, v47, v54, v53);	// L65
    hls::stream<float> v55;	// L66
    _Z3topPA128_fS0_S0_S0_S0_PfS1__node1(v41, v47, v42, v43, v54, v46, v55, v53);	// L67
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node4(
  float v56[8]
) {	// L71
  #pragma HLS inline
  for (int v57 = 0; v57 < 8; v57 += 1) {	// L73
    #pragma HLS pipeline II=1
    v56[v57] = (float)0.000000;	// L74
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node5(
  float v58[8]
) {	// L78
  #pragma HLS inline
  for (int v59 = 0; v59 < 8; v59 += 1) {	// L80
    #pragma HLS pipeline II=1
    v58[v59] = (float)0.000000;	// L81
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node7(
  float v60[8][8],
  float v61[8][128],
  float v62[8][128],
  int v63
) {	// L85
  for (int v64 = 0; v64 < 8; v64 += 1) {	// L87
    for (int v65 = 0; v65 < 16; v65 += 1) {	// L88
      #pragma HLS pipeline II=1
      v62[v64][(v65 + (v63 * 16))] = (float)0.000000;	// L89
    }
  }
  for (int v66 = 0; v66 < 8; v66 += 1) {	// L92
    for (int v67 = 0; v67 < 8; v67 += 1) {	// L93
      for (int v68 = 0; v68 < 16; v68 += 1) {	// L94
        #pragma HLS pipeline II=1
        float v69 = v60[v67][v66];	// L95
        float v70 = v61[v66][(v68 + (v63 * 16))];	// L96
        float v71 = v69 * v70;	// L97
        float v72 = v62[v67][(v68 + (v63 * 16))];	// L98
        float v73 = v72 + v71;	// L99
        v62[v67][(v68 + (v63 * 16))] = v73;	// L100
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node9(
  float v74[8],
  float v75[8][8],
  float v76[8],
  hls::stream<float> &v77,
  hls::stream<float> &v78,
  int v79
) {	// L106
  float v80 = v74[v79];	// L107
  for (int v81 = 0; v81 < 8; v81 += 1) {	// L108
    #pragma HLS pipeline II=1
    float v82 = v75[v79][v81];	// L109
    float v83 = v82 - v80;	// L110
    float v84 = exp(v83);	// L111
    v75[v79][v81] = v84;	// L112
    float v85 = v76[v79];	// L113
    float v86 = v85 + v84;	// L114
    v76[v79] = v86;	// L115
  }
  v77.write(v80);	// L117
  float v87 = v76[v79];	// L118
  for (int v88 = 0; v88 < 8; v88 += 1) {	// L119
    #pragma HLS pipeline II=1
    float v89 = v75[v79][v88];	// L120
    float v90 = v89 / v87;	// L121
    v75[v79][v88] = v90;	// L122
  }
  v78.write(v87);	// L124
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node10(
  float v91[8][8],
  float v92[8],
  int v93
) {	// L127
  #pragma HLS inline
  for (int v94 = 0; v94 < 7; v94 += 1) {	// L128
    #pragma HLS pipeline II=1
    float v95 = v91[v93][(v94 + 1)];	// L129
    float v96 = v92[v93];	// L130
    bool v97 = v95 > v96;	// L131
    float v98 = v97 ? v95 : v96;	// L132
    v92[v93] = v98;	// L133
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node8(
  float v99[8][128],
  float v100[8][128],
  float v101[8][8],
  float v102[8],
  float v103[8],
  int v104
) {	// L137
  for (int v105 = 0; v105 < 8; v105 += 1) {	// L140
    for (int v106 = 0; v106 < 8; v106 += 1) {	// L141
      #pragma HLS pipeline II=1
      v101[v105][v106] = (float)0.000000;	// L142
    }
  }
  for (int v107 = 0; v107 < 16; v107 += 1) {	// L145
    for (int v108 = 0; v108 < 8; v108 += 1) {	// L146
      for (int v109 = 0; v109 < 8; v109 += 1) {	// L147
        #pragma HLS pipeline II=1
        float v110 = v99[v108][(v107 + (v104 * 16))];	// L148
        float v111 = v100[v109][(v107 + (v104 * 16))];	// L149
        float v112 = v110 * v111;	// L150
        float v113 = v101[v108][v109];	// L151
        float v114 = v113 + v112;	// L152
        float v115 = v114 * (float)0.250000;	// L153
        float v116 = (((-v107) + 15) == 0) ? v115 : v114;	// L154
        v101[v108][v109] = v116;	// L155
      }
    }
  }
  for (int v117 = 0; v117 < 8; v117 += 1) {	// L159
    #pragma HLS pipeline II=1
    v102[v117] = (float)0.000000;	// L160
  }
  for (int v118 = 0; v118 < 8; v118 += 1) {	// L162
    #pragma HLS pipeline II=1
    float v119 = v101[v118][0];	// L163
    v103[v118] = v119;	// L164
  }
  for (int v120 = 0; v120 < 8; v120 += 1) {	// L166
    _Z3topPA128_fS0_S0_S0_S0_PfS1__node10(v101, v103, v120);	// L167
    hls::stream<float> v121;	// L168
    hls::stream<float> v122;	// L169
    _Z3topPA128_fS0_S0_S0_S0_PfS1__node9(v103, v101, v102, v121, v122, v120);	// L170
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node6(
  float v123[8][128],
  float v124[8][128],
  float v125[8][128],
  float v126[8][128]
) {	// L174
  float v127[8];	// L175
  #pragma HLS bind_storage variable=v127 type=ram_t2p impl=bram

  float v128[8];	// L176
  #pragma HLS bind_storage variable=v128 type=ram_t2p impl=bram

  float v129[8][8];	// L177
  #pragma HLS bind_storage variable=v129 type=ram_t2p impl=bram

  for (int v130 = 0; v130 < 8; v130 += 1) {	// L178
    _Z3topPA128_fS0_S0_S0_S0_PfS1__node8(v125, v124, v129, v128, v127, v130);	// L179
    _Z3topPA128_fS0_S0_S0_S0_PfS1__node7(v129, v123, v126, v130);	// L180
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node11(
  float v131[8][128],
  float v132[8][128],
  float v133[8][128],
  float v134[8][128]
) {	// L184
  float v135;	// L187
  for (int v136 = 0; v136 < 8; v136 += 1) {	// L188
    for (int v137 = 0; v137 < 128; v137 += 1) {	// L189
      #pragma HLS pipeline II=1
      float v138 = v132[v136][v137];	// L190
      v133[v136][v137] = v138;	// L191
    }
  }
  for (int v139 = 0; v139 < 8; v139 += 1) {	// L194
    for (int v140 = 0; v140 < 128; v140 += 1) {	// L195
      #pragma HLS pipeline II=1
      float v141 = v131[v139][v140];	// L196
      v134[v139][v140] = v141;	// L197
    }
  }
  for (int v142 = 0; v142 < 8; v142 += 1) {	// L200
    for (int v143 = 0; v143 < 8; v143 += 1) {	// L201
      for (int v144 = 0; v144 < 8; v144 += 1) {	// L202
        #pragma HLS pipeline II=1
        ap_int<32> v145 = v142;	// L203
        float v146 = v145;	// L204
        float v147 = v135;	// L205
        float v148 = (v144 == 0 && v143 == 0) ? v146 : v147;	// L206
        v135 = v148;	// L207
        int v149 = (v143 * 2);	// L208
        ap_int<32> v150 = v149;	// L209
        float v151 = v150;	// L210
        float v152 = -(v151);	// L211
        float v153 = v152 / (float)16.000000;	// L212
        float v154 = pow((float)10000.000000, v153);	// L213
        float v155 = v148 * v154;	// L214
        float v156 = cos(v155);	// L215
        float v157 = sin(v155);	// L216
        float v158 = v134[v142][((v144 * 16) + (v143 * 2))];	// L217
        float v159 = v134[v142][(((v144 * 16) + (v143 * 2)) + 1)];	// L218
        float v160 = v158 * v156;	// L219
        float v161 = v159 * v157;	// L220
        float v162 = v160 - v161;	// L221
        v134[v142][((v144 * 16) + (v143 * 2))] = v162;	// L222
        float v163 = v158 * v157;	// L223
        float v164 = v159 * v156;	// L224
        float v165 = v163 + v164;	// L225
        v134[v142][(((v144 * 16) + (v143 * 2)) + 1)] = v165;	// L226
        float v166 = v133[v142][((v144 * 16) + (v143 * 2))];	// L227
        float v167 = v133[v142][(((v144 * 16) + (v143 * 2)) + 1)];	// L228
        float v168 = v166 * v156;	// L229
        float v169 = v167 * v157;	// L230
        float v170 = v168 - v169;	// L231
        v133[v142][((v144 * 16) + (v143 * 2))] = v170;	// L232
        float v171 = v166 * v157;	// L233
        float v172 = v167 * v156;	// L234
        float v173 = v171 + v172;	// L235
        v133[v142][(((v144 * 16) + (v143 * 2)) + 1)] = v173;	// L236
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node12(
  float v174[8][128],
  float v175[8][128]
) {	// L242
  #pragma HLS inline
  for (int v176 = 0; v176 < 8; v176 += 1) {	// L243
    for (int v177 = 0; v177 < 128; v177 += 1) {	// L244
      #pragma HLS pipeline II=1
      float v178 = v174[v176][v177];	// L245
      v175[v176][v177] = v178;	// L246
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node13(
  float v179[128][128],
  float v180[128][128],
  float v181[8][128],
  float v182[128][128],
  float v183[8][128],
  float v184[8][128],
  float v185[8][128],
  float v186[8][128],
  float v187[8][128],
  float v188[8][128]
) {	// L251
  #pragma HLS inline
  for (int v189 = 0; v189 < 128; v189 += 1) {	// L252
    for (int v190 = 0; v190 < 8; v190 += 1) {	// L253
      for (int v191 = 0; v191 < 128; v191 += 1) {	// L254
        #pragma HLS pipeline II=1
        float v192 = v181[v190][v189];	// L255
        float v193 = v182[v191][v189];	// L256
        float v194 = v192 * v193;	// L257
        float v195 = v183[v190][v191];	// L258
        float v196 = v188[v190][v191];	// L259
        float v197 = (v189 == 0) ? v195 : v196;	// L260
        float v198 = v197 + v194;	// L261
        v188[v190][v191] = v198;	// L262
        float v199 = v179[v191][v189];	// L263
        float v200 = v192 * v199;	// L264
        float v201 = v184[v190][v191];	// L265
        float v202 = v187[v190][v191];	// L266
        float v203 = (v189 == 0) ? v201 : v202;	// L267
        float v204 = v203 + v200;	// L268
        v187[v190][v191] = v204;	// L269
        float v205 = v180[v191][v189];	// L270
        float v206 = v192 * v205;	// L271
        float v207 = v185[v190][v191];	// L272
        float v208 = v186[v190][v191];	// L273
        float v209 = (v189 == 0) ? v207 : v208;	// L274
        float v210 = v209 + v206;	// L275
        v186[v190][v191] = v210;	// L276
      }
    }
  }
}

void _Z3topPA128_fS0_S0_S0_S0_PfS1__node14(
  float v211[8][128],
  float v212[8][128],
  float v213[8][128]
) {	// L282
  #pragma HLS inline
  for (int v214 = 0; v214 < 8; v214 += 1) {	// L284
    for (int v215 = 0; v215 < 128; v215 += 1) {	// L285
      #pragma HLS pipeline II=1
      v211[v214][v215] = (float)0.000000;	// L286
      v212[v214][v215] = (float)0.000000;	// L287
      v213[v214][v215] = (float)0.000000;	// L288
    }
  }
}

/// This is top function.
void _Z3topPA128_fS0_S0_S0_S0_PfS1_(
  float v216[8][128],
  float v217[128][128],
  float v218[128][128],
  float v219[128][128],
  float v220[8][128],
  float v221[128],
  float v222[128]
) {	// L293
  #pragma HLS interface s_axilite port=return bundle=ctrl
  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v222 bundle=axi_6

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v221 bundle=axi_5

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v220 bundle=axi_4

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v219 bundle=axi_3

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v218 bundle=axi_2

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v217 bundle=axi_1

  #pragma HLS interface bram storage_type=ram_t2p storage_impl=bram port=v216 bundle=axi_0

  float v230[8];	// L308
  #pragma HLS bind_storage variable=v230 type=ram_t2p impl=bram

  float v231[8];	// L309
  #pragma HLS bind_storage variable=v231 type=ram_t2p impl=bram

  float v232[8][128];	// L310
  #pragma HLS bind_storage variable=v232 type=ram_t2p impl=bram

  float v233[8][128];	// L311
  #pragma HLS bind_storage variable=v233 type=ram_t2p impl=bram

  float v234[8][128];	// L312
  #pragma HLS bind_storage variable=v234 type=ram_t2p impl=bram

  float v235[8][128];	// L313
  #pragma HLS bind_storage variable=v235 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0_PfS1__node14(v232, v233, v234);	// L314
  float v236[8][128];	// L315
  #pragma HLS bind_storage variable=v236 type=ram_t2p impl=bram

  float v237[8][128];	// L316
  #pragma HLS bind_storage variable=v237 type=ram_t2p impl=bram

  float v238[8][128];	// L317
  #pragma HLS bind_storage variable=v238 type=ram_t2p impl=bram

  float v239[8][128];	// L318
  #pragma HLS bind_storage variable=v239 type=ram_t2p impl=bram

  _Z3topPA128_fS0_S0_S0_S0_PfS1__node13(v218, v219, v216, v217, v232, v233, v234, v238, v237, v236);	// L319
  _Z3topPA128_fS0_S0_S0_S0_PfS1__node12(v238, v239);	// L320
  float v240[8][128];	// L321
  #pragma HLS array_partition variable=v240 cyclic factor=2 dim=2
  #pragma HLS bind_storage variable=v240 type=ram_2p impl=lutram

  float v241[8][128];	// L322
  #pragma HLS array_partition variable=v241 cyclic factor=2 dim=2
  #pragma HLS bind_storage variable=v241 type=ram_2p impl=lutram

  _Z3topPA128_fS0_S0_S0_S0_PfS1__node11(v236, v237, v241, v240);	// L323
  _Z3topPA128_fS0_S0_S0_S0_PfS1__node6(v239, v241, v240, v235);	// L324
  _Z3topPA128_fS0_S0_S0_S0_PfS1__node5(v230);	// L325
  _Z3topPA128_fS0_S0_S0_S0_PfS1__node4(v231);	// L326
  _Z3topPA128_fS0_S0_S0_S0_PfS1__node0(v221, v235, v222, v230, v231, v220);	// L327
}

void _Z16load_8_128_floatPA128_fS0__node0(
  float v242[8][128],
  float v243[8][128]
) {	// L341
  #pragma HLS inline
  for (int v244 = 0; v244 < 8; v244 += 1) {	// L342
    for (int v245 = 0; v245 < 128; v245 += 1) {	// L343
      #pragma HLS pipeline II=1
      float v246 = v242[v244][v245];	// L344
      v243[v244][v245] = v246;	// L345
    }
  }
}

void _Z16load_8_128_floatPA128_fS0_(
  float v247[8][128],
  float v248[8][128]
) {	// L350
  _Z16load_8_128_floatPA128_fS0__node0(v247, v248);	// L351
}

void _Z18load_128_128_floatPA128_fS0__node0(
  float v249[128][128],
  float v250[128][128]
) {	// L354
  #pragma HLS inline
  for (int v251 = 0; v251 < 128; v251 += 1) {	// L355
    for (int v252 = 0; v252 < 128; v252 += 1) {	// L356
      #pragma HLS pipeline II=1
      float v253 = v249[v251][v252];	// L357
      v250[v251][v252] = v253;	// L358
    }
  }
}

void _Z18load_128_128_floatPA128_fS0_(
  float v254[128][128],
  float v255[128][128]
) {	// L363
  _Z18load_128_128_floatPA128_fS0__node0(v254, v255);	// L364
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node1(
  float v256[8][8],
  float v257[8][128],
  float v258[8][128],
  int v259
) {	// L367
  for (int v260 = 0; v260 < 8; v260 += 1) {	// L369
    for (int v261 = 0; v261 < 16; v261 += 1) {	// L370
      #pragma HLS pipeline II=1
      v258[v260][(v261 + (v259 * 16))] = (float)0.000000;	// L371
    }
  }
  for (int v262 = 0; v262 < 8; v262 += 1) {	// L374
    for (int v263 = 0; v263 < 8; v263 += 1) {	// L375
      for (int v264 = 0; v264 < 16; v264 += 1) {	// L376
        #pragma HLS pipeline II=1
        float v265 = v256[v263][v262];	// L377
        float v266 = v257[v262][(v264 + (v259 * 16))];	// L378
        float v267 = v265 * v266;	// L379
        float v268 = v258[v263][(v264 + (v259 * 16))];	// L380
        float v269 = v268 + v267;	// L381
        v258[v263][(v264 + (v259 * 16))] = v269;	// L382
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node3(
  float v270[8],
  float v271[8][8],
  float v272[8],
  hls::stream<float> &v273,
  hls::stream<float> &v274,
  int v275
) {	// L388
  float v276 = v270[v275];	// L389
  for (int v277 = 0; v277 < 8; v277 += 1) {	// L390
    #pragma HLS pipeline II=1
    float v278 = v271[v275][v277];	// L391
    float v279 = v278 - v276;	// L392
    float v280 = exp(v279);	// L393
    v271[v275][v277] = v280;	// L394
    float v281 = v272[v275];	// L395
    float v282 = v281 + v280;	// L396
    v272[v275] = v282;	// L397
  }
  v273.write(v276);	// L399
  float v283 = v272[v275];	// L400
  for (int v284 = 0; v284 < 8; v284 += 1) {	// L401
    #pragma HLS pipeline II=1
    float v285 = v271[v275][v284];	// L402
    float v286 = v285 / v283;	// L403
    v271[v275][v284] = v286;	// L404
  }
  v274.write(v283);	// L406
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node4(
  float v287[8][8],
  float v288[8],
  int v289
) {	// L409
  #pragma HLS inline
  for (int v290 = 0; v290 < 7; v290 += 1) {	// L410
    #pragma HLS pipeline II=1
    float v291 = v287[v289][(v290 + 1)];	// L411
    float v292 = v288[v289];	// L412
    bool v293 = v291 > v292;	// L413
    float v294 = v293 ? v291 : v292;	// L414
    v288[v289] = v294;	// L415
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node2(
  float v295[8][128],
  float v296[8][128],
  float v297[8][8],
  float v298[8],
  float v299[8],
  int v300
) {	// L419
  for (int v301 = 0; v301 < 8; v301 += 1) {	// L422
    for (int v302 = 0; v302 < 8; v302 += 1) {	// L423
      #pragma HLS pipeline II=1
      v297[v301][v302] = (float)0.000000;	// L424
    }
  }
  for (int v303 = 0; v303 < 16; v303 += 1) {	// L427
    for (int v304 = 0; v304 < 8; v304 += 1) {	// L428
      for (int v305 = 0; v305 < 8; v305 += 1) {	// L429
        #pragma HLS pipeline II=1
        float v306 = v295[v304][(v303 + (v300 * 16))];	// L430
        float v307 = v296[v305][(v303 + (v300 * 16))];	// L431
        float v308 = v306 * v307;	// L432
        float v309 = v297[v304][v305];	// L433
        float v310 = v309 + v308;	// L434
        float v311 = v310 * (float)0.250000;	// L435
        float v312 = (((-v303) + 15) == 0) ? v311 : v310;	// L436
        v297[v304][v305] = v312;	// L437
      }
    }
  }
  for (int v313 = 0; v313 < 8; v313 += 1) {	// L441
    #pragma HLS pipeline II=1
    v298[v313] = (float)0.000000;	// L442
  }
  for (int v314 = 0; v314 < 8; v314 += 1) {	// L444
    #pragma HLS pipeline II=1
    float v315 = v297[v314][0];	// L445
    v299[v314] = v315;	// L446
  }
  for (int v316 = 0; v316 < 8; v316 += 1) {	// L448
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node4(v297, v299, v316);	// L449
    hls::stream<float> v317;	// L450
    hls::stream<float> v318;	// L451
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node3(v299, v297, v298, v317, v318, v316);	// L452
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node0(
  float v319[8][128],
  float v320[8][128],
  float v321[8][128],
  float v322[8][128]
) {	// L456
  float v323[8];	// L457
  #pragma HLS bind_storage variable=v323 type=ram_t2p impl=bram

  float v324[8];	// L458
  #pragma HLS bind_storage variable=v324 type=ram_t2p impl=bram

  float v325[8][8];	// L459
  #pragma HLS bind_storage variable=v325 type=ram_t2p impl=bram

  for (int v326 = 0; v326 < 8; v326 += 1) {	// L460
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node2(v319, v320, v325, v324, v323, v326);	// L461
    _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node1(v325, v321, v322, v326);	// L462
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node5(
  float v327[8][128],
  float v328[8][128],
  float v329[8][128],
  float v330[8][128]
) {	// L466
  float v331;	// L469
  for (int v332 = 0; v332 < 8; v332 += 1) {	// L470
    for (int v333 = 0; v333 < 128; v333 += 1) {	// L471
      #pragma HLS pipeline II=1
      float v334 = v328[v332][v333];	// L472
      v330[v332][v333] = v334;	// L473
    }
  }
  for (int v335 = 0; v335 < 8; v335 += 1) {	// L476
    for (int v336 = 0; v336 < 128; v336 += 1) {	// L477
      #pragma HLS pipeline II=1
      float v337 = v327[v335][v336];	// L478
      v329[v335][v336] = v337;	// L479
    }
  }
  for (int v338 = 0; v338 < 8; v338 += 1) {	// L482
    for (int v339 = 0; v339 < 8; v339 += 1) {	// L483
      for (int v340 = 0; v340 < 8; v340 += 1) {	// L484
        #pragma HLS pipeline II=1
        ap_int<32> v341 = v338;	// L485
        float v342 = v341;	// L486
        float v343 = v331;	// L487
        float v344 = (v340 == 0 && v339 == 0) ? v342 : v343;	// L488
        v331 = v344;	// L489
        int v345 = (v339 * 2);	// L490
        ap_int<32> v346 = v345;	// L491
        float v347 = v346;	// L492
        float v348 = -(v347);	// L493
        float v349 = v348 / (float)16.000000;	// L494
        float v350 = pow((float)10000.000000, v349);	// L495
        float v351 = v344 * v350;	// L496
        float v352 = cos(v351);	// L497
        float v353 = sin(v351);	// L498
        float v354 = v329[v338][((v340 * 16) + (v339 * 2))];	// L499
        float v355 = v329[v338][(((v340 * 16) + (v339 * 2)) + 1)];	// L500
        float v356 = v354 * v352;	// L501
        float v357 = v355 * v353;	// L502
        float v358 = v356 - v357;	// L503
        v329[v338][((v340 * 16) + (v339 * 2))] = v358;	// L504
        float v359 = v354 * v353;	// L505
        float v360 = v355 * v352;	// L506
        float v361 = v359 + v360;	// L507
        v329[v338][(((v340 * 16) + (v339 * 2)) + 1)] = v361;	// L508
        float v362 = v330[v338][((v340 * 16) + (v339 * 2))];	// L509
        float v363 = v330[v338][(((v340 * 16) + (v339 * 2)) + 1)];	// L510
        float v364 = v362 * v352;	// L511
        float v365 = v363 * v353;	// L512
        float v366 = v364 - v365;	// L513
        v330[v338][((v340 * 16) + (v339 * 2))] = v366;	// L514
        float v367 = v362 * v353;	// L515
        float v368 = v363 * v352;	// L516
        float v369 = v367 + v368;	// L517
        v330[v338][(((v340 * 16) + (v339 * 2)) + 1)] = v369;	// L518
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node6(
  float v370[8][128],
  float v371[8][128]
) {	// L524
  #pragma HLS inline
  for (int v372 = 0; v372 < 8; v372 += 1) {	// L525
    for (int v373 = 0; v373 < 128; v373 += 1) {	// L526
      #pragma HLS pipeline II=1
      float v374 = v370[v372][v373];	// L527
      v371[v372][v373] = v374;	// L528
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node7(
  float v375[128][128],
  float v376[128][128],
  float v377[8][128],
  float v378[128][128],
  float v379[8][128],
  float v380[8][128],
  float v381[8][128],
  float v382[8][128],
  float v383[8][128],
  float v384[8][128]
) {	// L533
  #pragma HLS inline
  for (int v385 = 0; v385 < 128; v385 += 1) {	// L534
    for (int v386 = 0; v386 < 8; v386 += 1) {	// L535
      for (int v387 = 0; v387 < 128; v387 += 1) {	// L536
        #pragma HLS pipeline II=1
        float v388 = v377[v386][v385];	// L537
        float v389 = v378[v387][v385];	// L538
        float v390 = v388 * v389;	// L539
        float v391 = v379[v386][v387];	// L540
        float v392 = v382[v386][v387];	// L541
        float v393 = (v385 == 0) ? v391 : v392;	// L542
        float v394 = v393 + v390;	// L543
        v382[v386][v387] = v394;	// L544
        float v395 = v376[v387][v385];	// L545
        float v396 = v388 * v395;	// L546
        float v397 = v380[v386][v387];	// L547
        float v398 = v384[v386][v387];	// L548
        float v399 = (v385 == 0) ? v397 : v398;	// L549
        float v400 = v399 + v396;	// L550
        v384[v386][v387] = v400;	// L551
        float v401 = v375[v387][v385];	// L552
        float v402 = v388 * v401;	// L553
        float v403 = v381[v386][v387];	// L554
        float v404 = v383[v386][v387];	// L555
        float v405 = (v385 == 0) ? v403 : v404;	// L556
        float v406 = v405 + v402;	// L557
        v383[v386][v387] = v406;	// L558
      }
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node8(
  float v407[8][128],
  float v408[8][128],
  float v409[8][128]
) {	// L564
  #pragma HLS inline
  for (int v410 = 0; v410 < 8; v410 += 1) {	// L566
    for (int v411 = 0; v411 < 128; v411 += 1) {	// L567
      #pragma HLS pipeline II=1
      v407[v410][v411] = (float)0.000000;	// L568
      v408[v410][v411] = (float)0.000000;	// L569
      v409[v410][v411] = (float)0.000000;	// L570
    }
  }
}

void _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i(
  float v412[8][128],
  float v413[128][128],
  float v414[128][128],
  float v415[128][128],
  float v416[8][128],
  ap_int<32> v417
) {	// L575
  float v418[8][128];	// L576
  #pragma HLS bind_storage variable=v418 type=ram_t2p impl=bram

  float v419[8][128];	// L577
  #pragma HLS bind_storage variable=v419 type=ram_t2p impl=bram

  float v420[8][128];	// L578
  #pragma HLS bind_storage variable=v420 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node8(v418, v419, v420);	// L579
  float v421[8][128];	// L580
  #pragma HLS bind_storage variable=v421 type=ram_t2p impl=bram

  float v422[8][128];	// L581
  #pragma HLS bind_storage variable=v422 type=ram_t2p impl=bram

  float v423[8][128];	// L582
  #pragma HLS bind_storage variable=v423 type=ram_t2p impl=bram

  float v424[8][128];	// L583
  #pragma HLS bind_storage variable=v424 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node7(v415, v414, v412, v413, v418, v419, v420, v421, v423, v422);	// L584
  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node6(v423, v424);	// L585
  float v425[8][128];	// L586
  #pragma HLS bind_storage variable=v425 type=ram_t2p impl=bram

  float v426[8][128];	// L587
  #pragma HLS bind_storage variable=v426 type=ram_t2p impl=bram

  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node5(v421, v422, v425, v426);	// L588
  _Z49grouped_multihead_attention_8_128_8_16_rope_floatPA128_fS0_S0_S0_S0_i_node0(v425, v426, v424, v416);	// L589
}

void _Z14load_128_floatPfS__node0(
  float v427[128],
  float v428[128]
) {	// L592
  #pragma HLS inline
  for (int v429 = 0; v429 < 128; v429 += 1) {	// L593
    #pragma HLS pipeline II=1
    float v430 = v427[v429];	// L594
    v428[v429] = v430;	// L595
  }
}

void _Z14load_128_floatPfS_(
  float v431[128],
  float v432[128]
) {	// L599
  _Z14load_128_floatPfS__node0(v431, v432);	// L600
}

void _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node1(
  float v433[128],
  float v434[128],
  float v435[8],
  hls::stream<float> &v436,
  float v437[8][128],
  hls::stream<float> &v438,
  float v439[8][128],
  int v440
) {	// L603
  #pragma HLS inline
  float v441 = v436.read();	// L606
  float v442 = v435[v440];	// L607
  float v443 = v442 / (float)128.000000;	// L608
  float v444 = v443 + (float)0.010000;	// L609
  float v445 = sqrt(v444);	// L610
  for (int v446 = 0; v446 < 128; v446 += 1) {	// L611
    #pragma HLS pipeline II=1
    float v447 = v433[v446];	// L612
    float v448 = v437[v440][v446];	// L613
    float v449 = v448 - v441;	// L614
    float v450 = v449 / v445;	// L615
    float v451 = v447 * v450;	// L616
    float v452 = v434[v446];	// L617
    float v453 = v451 + v452;	// L618
    v439[v440][v446] = v453;	// L619
  }
  v438.write(v445);	// L621
}

void _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node2(
  float v454[8],
  float v455[8][128],
  float v456[8],
  hls::stream<float> &v457,
  int v458
) {	// L624
  #pragma HLS inline
  float v459 = v454[v458];	// L626
  float v460 = v459 / (float)128.000000;	// L627
  for (int v461 = 0; v461 < 128; v461 += 1) {	// L628
    #pragma HLS pipeline II=1
    float v462 = v455[v458][v461];	// L629
    float v463 = v462 - v460;	// L630
    float v464 = v463 * v463;	// L631
    float v465 = v456[v458];	// L632
    float v466 = v465 + v464;	// L633
    v456[v458] = v466;	// L634
  }
  v457.write(v460);	// L636
}

void _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node3(
  float v467[8][128],
  float v468[8],
  int v469
) {	// L639
  #pragma HLS inline
  for (int v470 = 0; v470 < 128; v470 += 1) {	// L640
    #pragma HLS pipeline II=1
    float v471 = v467[v469][v470];	// L641
    float v472 = v468[v469];	// L642
    float v473 = v472 + v471;	// L643
    v468[v469] = v473;	// L644
  }
}

void _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node0(
  float v474[8][128],
  float v475[128],
  float v476[128],
  float v477[8],
  float v478[8],
  float v479[8][128]
) {	// L648
  float v480[8];	// L649
  #pragma HLS bind_storage variable=v480 type=ram_t2p impl=bram

  float v481[8];	// L650
  #pragma HLS bind_storage variable=v481 type=ram_t2p impl=bram

  for (int v482 = 0; v482 < 8; v482 += 1) {	// L651
    #pragma HLS pipeline II=1
    float v483 = v478[v482];	// L652
    v480[v482] = v483;	// L653
  }
  for (int v484 = 0; v484 < 8; v484 += 1) {	// L655
    #pragma HLS pipeline II=1
    float v485 = v477[v484];	// L656
    v481[v484] = v485;	// L657
  }
  for (int v486 = 0; v486 < 8; v486 += 1) {	// L659
    _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node3(v474, v481, v486);	// L660
    hls::stream<float> v487;	// L661
    _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node2(v481, v474, v480, v487, v486);	// L662
    hls::stream<float> v488;	// L663
    _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node1(v476, v475, v480, v487, v474, v488, v479, v486);	// L664
  }
}

void _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node4(
  float v489[8]
) {	// L668
  #pragma HLS inline
  for (int v490 = 0; v490 < 8; v490 += 1) {	// L670
    #pragma HLS pipeline II=1
    v489[v490] = (float)0.000000;	// L671
  }
}

void _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node5(
  float v491[8]
) {	// L675
  #pragma HLS inline
  for (int v492 = 0; v492 < 8; v492 += 1) {	// L677
    #pragma HLS pipeline II=1
    v491[v492] = (float)0.000000;	// L678
  }
}

void _Z22layer_norm_8_128_floatPA128_fPfS1_S0_(
  float v493[8][128],
  float v494[128],
  float v495[128],
  float v496[8][128]
) {	// L682
  float v497[8];	// L683
  #pragma HLS bind_storage variable=v497 type=ram_t2p impl=bram

  float v498[8];	// L684
  #pragma HLS bind_storage variable=v498 type=ram_t2p impl=bram

  _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node5(v497);	// L685
  _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node4(v498);	// L686
  _Z22layer_norm_8_128_floatPA128_fPfS1_S0__node0(v493, v495, v494, v497, v498, v496);	// L687
}

void _Z17store_8_128_floatPA128_fS0__node0(
  float v499[8][128],
  float v500[8][128]
) {	// L690
  #pragma HLS inline
  for (int v501 = 0; v501 < 8; v501 += 1) {	// L691
    for (int v502 = 0; v502 < 128; v502 += 1) {	// L692
      #pragma HLS pipeline II=1
      float v503 = v499[v501][v502];	// L693
      v500[v501][v502] = v503;	// L694
    }
  }
}

void _Z17store_8_128_floatPA128_fS0_(
  float v504[8][128],
  float v505[8][128]
) {	// L699
  _Z17store_8_128_floatPA128_fS0__node0(v504, v505);	// L700
}

