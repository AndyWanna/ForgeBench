
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ap_fixed.h>
#include <hls_math.h>
#include <stdlib.h>
#include <cstdint>
#include <hls_math.h>
using namespace std;

typedef float data_t;

data_t BRAM_attn_input[8][128];
data_t BRAM_WQ[128][128];
data_t BRAM_WK[128][128];
data_t BRAM_WV[128][128];
data_t BRAM_attn_output[8][128];
data_t BRAM_norm_output[8][128];
data_t BRAM_norm_gamma[128];
data_t BRAM_norm_beta[128];

void load_8_128_float(data_t input[8][128], data_t output[8][128])
{
    for (int idx0 = 0; idx0 < 8; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void load_128_128_float(data_t input[128][128], data_t output[128][128])
{
    for (int idx0 = 0; idx0 < 128; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

/*
 * Auto-generated Grouped Multi-head Attention (with optional inline RoPE)
 *
 * Input     : [8][128]
 * W_q/k/v   : [128][128], DIM_OUT = NUM_HEADS * HEAD_DIM
 * Output    : [8][128]
 *
 * Data type : float
 * Num Heads : 8
 * Head Dim  : 16
 */

 void grouped_multihead_attention_8_128_8_16_rope_float(
    data_t input[8][128],
    data_t W_q[128][128],
    data_t W_k[128][128],
    data_t W_v[128][128],
    data_t output[8][128],
    int groups
)
{
    const int num_heads = 8;   // total number of heads (must equal DIM_OUT / HEAD_DIM)
    const int head_dim = 16;       // dimension per head
    const int heads_per_group = num_heads / groups;
    const data_t scale = (data_t)0.25;   // 1/sqrt(head_dim), computed at generation time (head_dim itself may not fit in data_t)

    data_t Q[8][128];
    data_t K[8][128];
    data_t V[8][128];
    #pragma HLS ARRAY_PARTITION variable=Q cyclic factor=1 dim=2
    #pragma HLS ARRAY_PARTITION variable=K cyclic factor=1 dim=2
    #pragma HLS ARRAY_PARTITION variable=V cyclic factor=1 dim=2

    // Compute Q, K, V
    for (int seq = 0; seq < 8; seq++) {
        for (int dout = 0; dout < 128; dout++) {
            Q[seq][dout] = 0;
            K[seq][dout] = 0;
            V[seq][dout] = 0;
            for (int din = 0; din < 128; din++) {
                Q[seq][dout] += input[seq][din] * W_q[dout][din];
                K[seq][dout] += input[seq][din] * W_k[dout][din];
                V[seq][dout] += input[seq][din] * W_v[dout][din];
            }
        }
    }

    /*==== BEGIN OPTIONAL ROPE LOGIC ====*/
    
    // Inline RoPE logic
    for (int seq = 0; seq < 8; seq++) {
        for (int h = 0; h < 8; h++) {
            for (int d = 0; d < 16; d += 2) {
                int idx = h * 16 + d;
                data_t theta = (data_t)hls::powf(10000.0f, -((float)d) / (float)16);
                data_t angle = seq * theta;
                data_t cos_val = hls::cos(angle);
                data_t sin_val = hls::sin(angle);

                // Apply RoPE to Q
                data_t q0 = Q[seq][idx];
                data_t q1 = Q[seq][idx + 1];
                Q[seq][idx]     = q0 * cos_val - q1 * sin_val;
                Q[seq][idx + 1] = q0 * sin_val + q1 * cos_val;

                // Apply RoPE to K
                data_t k0 = K[seq][idx];
                data_t k1 = K[seq][idx + 1];
                K[seq][idx]     = k0 * cos_val - k1 * sin_val;
                K[seq][idx + 1] = k0 * sin_val + k1 * cos_val;
            }
        }
    }
    /*==== END OPTIONAL ROPE LOGIC ====*/

    // Compute Attention per head in groups.
    for (int g = 0; g < groups; g++) {
        for (int h = 0; h < heads_per_group; h++) {
            int head_index = g * heads_per_group + h;
            data_t scores[8][8];

            // Scaled Dot-product: Q x K^T for head head_index
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    scores[i][j] = 0;
                    for (int d = 0; d < head_dim; d++) {
                        #pragma HLS UNROLL factor=1
                        int idx = head_index * head_dim + d;
                        scores[i][j] += Q[i][idx] * K[j][idx];
                    }
                    scores[i][j] *= scale;
                }
            }

            // Softmax for this head.
            for (int i = 0; i < 8; i++) {
                data_t sum_exp = 0;
                data_t max_score = scores[i][0];
                for (int j = 1; j < 8; j++) {
                    if (scores[i][j] > max_score)
                        max_score = scores[i][j];
                }
                for (int j = 0; j < 8; j++) {
                    scores[i][j] = hls::exp(scores[i][j] - max_score);
                    sum_exp += scores[i][j];
                }
                for (int j = 0; j < 8; j++) {
                    scores[i][j] /= sum_exp;
                }
            }

            // Compute context: scores x V for head head_index.
            for (int i = 0; i < 8; i++) {
                for (int d = 0; d < head_dim; d++) {
                    #pragma HLS UNROLL factor=1
                    data_t context = 0;
                    for (int j = 0; j < 8; j++) {
                        context += scores[i][j] * V[j][head_index * head_dim + d];
                    }
                    output[i][head_index * head_dim + d] = context;
                }
            }
        }
    }
}

void load_128_float(data_t input[128], data_t output[128])
{
    for (int idx0 = 0; idx0 < 128; idx0++) {
        output[idx0] = input[idx0];
    }
}



void layer_norm_8_128_float(
    data_t input[8][128],
    data_t gamma[128],
    data_t beta[128],
    data_t output[8][128]
)
{
    for (int i = 0; i < 8; i++) {
        // Compute mean for the i-th sequence element
        data_t sum = (data_t)0;
        for (int j = 0; j < 128; j++) {
            sum += input[i][j];
        }
        data_t mean = sum / 128;

        // Compute variance for the i-th sequence element
        data_t var_sum = (data_t)0;
        for (int j = 0; j < 128; j++) {
            data_t diff = input[i][j] - mean;
            var_sum += diff * diff;
        }
        data_t variance = var_sum / 128;

        // Normalize and scale: output = gamma * (x - mean) / sqrt(variance + epsilon) + beta
        for (int j = 0; j < 128; j++) {
            output[i][j] = gamma[j] * ((input[i][j] - mean) / hls::sqrt(variance + (data_t)0.01)) + beta[j];
        }
    }
}


void store_8_128_float(data_t input[8][128], data_t output[8][128])
{
    for (int idx0 = 0; idx0 < 8; idx0++) {
        for (int idx1 = 0; idx1 < 128; idx1++) {
            output[idx0][idx1] = input[idx0][idx1];
        }
    }
}

void top(data_t DRAM_attn_input[8][128], data_t DRAM_WQ[128][128], data_t DRAM_WK[128][128], data_t DRAM_WV[128][128], data_t DRAM_norm_output[8][128], data_t DRAM_norm_gamma[128], data_t DRAM_norm_beta[128])
{
    #pragma HLS interface m_axi port=DRAM_attn_input offset=slave bundle=mem1
    #pragma HLS interface m_axi port=DRAM_WQ offset=slave bundle=mem2
    #pragma HLS interface m_axi port=DRAM_WK offset=slave bundle=mem3
    #pragma HLS interface m_axi port=DRAM_WV offset=slave bundle=mem4
    #pragma HLS interface m_axi port=DRAM_norm_output offset=slave bundle=mem6
    #pragma HLS interface m_axi port=DRAM_norm_gamma offset=slave bundle=mem5
    #pragma HLS interface m_axi port=DRAM_norm_beta offset=slave bundle=mem7

    load_8_128_float(DRAM_attn_input, BRAM_attn_input);
    load_128_128_float(DRAM_WQ, BRAM_WQ);
    load_128_128_float(DRAM_WK, BRAM_WK);
    load_128_128_float(DRAM_WV, BRAM_WV);
    grouped_multihead_attention_8_128_8_16_rope_float(BRAM_attn_input, BRAM_WQ, BRAM_WK, BRAM_WV, BRAM_attn_output, 1);
    load_128_float(DRAM_norm_gamma, BRAM_norm_gamma);
    load_128_float(DRAM_norm_beta, BRAM_norm_beta);
    layer_norm_8_128_float(BRAM_attn_output, BRAM_norm_gamma, BRAM_norm_beta, BRAM_norm_output);
    store_8_128_float(BRAM_norm_output, DRAM_norm_output);
}