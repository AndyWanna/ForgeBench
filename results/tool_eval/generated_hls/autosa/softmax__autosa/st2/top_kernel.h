#include <ap_int.h>
#include <hls_stream.h>

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef float output_t1;
typedef float sum_t1;
typedef ap_uint<128> sum_t4;
/* Data Type */

/* Helper Function */
inline void sum_drain_drain_merge(float *sum_to, float *sum_from){
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L3
  // io_L2
  for (int c3 = 0; c3 <= 1; c3 += 1) {
    // io_L1
    // pe
    // latency
    for (int c4 = 0; c4 <= 3; c4 += 1)
      sum_to[4 * c3 + c4] = sum_from[4 * c3 + c4];
  }
}
/* Helper Function */

void kernel0(output_t1 *output, sum_t4 *sum_0, sum_t4 *sum_1, sum_t4 *sum_2);
void output_IO_L1_in_intra_trans(int idx, int idy, int c1, float local_output[4][1], hls::stream<float> &fifo_output_local_out, bool intra_trans_en);
void output_IO_L1_in_inter_trans(int idx, int idy, int c1, float local_output[4][1], hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, bool inter_trans_en);
void output_IO_L1_in_inter_trans_boundary(int idx, int idy, int c1, float local_output[4][1], hls::stream<float> &fifo_output_in, bool inter_trans_en);
void output_IO_L1_in_wrapper(int idx, int idy, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_local_out);
void output_IO_L1_in_boundary_wrapper(int idx, int idy, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_local_out);
void sum_IO_L2_in_intra_trans(int idx, sum_t0 local_sum[4], hls::stream<float> &fifo_sum_local_out, bool intra_trans_en);
void PE_wrapper(int idx, int idy, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_sum_in, hls::stream<float> &fifo_sum_out, hls::stream<float> &fifo_sum_drain_out);
void sum_IO_L2_out_intra_trans(int idx, sum_t4 local_sum[1], hls::stream<float> &fifo_sum_local_in, bool intra_trans_en);
void sum_IO_L2_out_inter_trans(int idx, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_in, hls::stream<sum_t4> &fifo_sum_out, bool inter_trans_en);
void sum_IO_L2_out_inter_trans_boundary(int idx, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_out, bool inter_trans_en);
void sum_drain_IO_L1_out_intra_trans(int idx, int idy, sum_t4 local_sum[1], hls::stream<float> &fifo_sum_drain_local_in);
void sum_drain_IO_L1_out_inter_trans(int idx, int idy, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out);
void sum_drain_IO_L1_out_inter_trans_boundary(int idx, int idy, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_drain_out);
void sum_drain_IO_L1_out_wrapper(int idx, int idy, hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in);
void sum_drain_IO_L1_out_boundary_wrapper(int idx, int idy, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in);
