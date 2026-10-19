#include <ap_int.h>
#include <hls_stream.h>

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef float input_t1;
typedef float sum_sq_t1;
typedef ap_uint<256> sum_sq_t8;
typedef ap_uint<128> sum_sq_t4;
/* Data Type */

/* Helper Function */
inline void sum_sq_drain_drain_merge(float *sum_sq_to, float *sum_sq_from){
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L2
  // io_L1
  // pe
  for (int c3 = 0; c3 <= 1; c3 += 1) {
    // latency
    for (int c4 = 0; c4 <= 3; c4 += 1)
      sum_sq_to[4 * c3 + c4] = sum_sq_from[4 * c3 + c4];
  }
}
/* Helper Function */

void kernel0(input_t1 *input, sum_sq_t8 *sum_sq_0, sum_sq_t8 *sum_sq_1, sum_sq_t8 *sum_sq_2);
void input_IO_L1_in_intra_trans(int idx, int c1, float local_input[8][1], hls::stream<float> &fifo_input_local_out, bool intra_trans_en);
void input_IO_L1_in_inter_trans(int idx, int c1, float local_input[8][1], hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_input_out, bool inter_trans_en);
void input_IO_L1_in_inter_trans_boundary(int idx, int c1, float local_input[8][1], hls::stream<float> &fifo_input_in, bool inter_trans_en);
void input_IO_L1_in_wrapper(int idx, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_input_out, hls::stream<float> &fifo_input_local_out);
void input_IO_L1_in_boundary_wrapper(int idx, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_input_local_out);
void sum_sq_IO_L2_in_intra_trans(int c1, sum_sq_t8 *sum_sq, sum_sq_t8 local_sum_sq[1], hls::stream<float> &fifo_sum_sq_local_out, bool intra_trans_en);
void PE_wrapper(int idx, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_sum_sq_in, hls::stream<float> &fifo_sum_sq_out, hls::stream<float> &fifo_sum_sq_drain_out);
void sum_sq_IO_L2_out_intra_trans(int c1, sum_sq_t8 *sum_sq, sum_sq_t8 local_sum_sq[1], hls::stream<float> &fifo_sum_sq_local_in, bool intra_trans_en);
void sum_sq_IO_L2_out_inter_trans_boundary(sum_sq_t8 *sum_sq, sum_sq_t8 local_sum_sq[1], hls::stream<sum_sq_t8> &fifo_sum_sq_out, bool inter_trans_en);
void sum_sq_drain_IO_L1_out_intra_trans(int idx, sum_sq_t4 local_sum_sq[2], hls::stream<float> &fifo_sum_sq_drain_local_in);
void sum_sq_drain_IO_L1_out_inter_trans(int idx, sum_sq_t4 local_sum_sq[2], hls::stream<sum_sq_t4> &fifo_sum_sq_drain_in, hls::stream<sum_sq_t4> &fifo_sum_sq_drain_out);
void sum_sq_drain_IO_L1_out_inter_trans_boundary(int idx, sum_sq_t4 local_sum_sq[2], hls::stream<sum_sq_t4> &fifo_sum_sq_drain_out);
void sum_sq_drain_IO_L1_out_wrapper(int idx, hls::stream<sum_sq_t4> &fifo_sum_sq_drain_in, hls::stream<sum_sq_t4> &fifo_sum_sq_drain_out, hls::stream<float> &fifo_sum_sq_drain_local_in);
void sum_sq_drain_IO_L1_out_boundary_wrapper(int idx, hls::stream<sum_sq_t4> &fifo_sum_sq_drain_out, hls::stream<float> &fifo_sum_sq_drain_local_in);
