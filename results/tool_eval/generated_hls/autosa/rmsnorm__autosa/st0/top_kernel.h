#include <ap_int.h>
#include <hls_stream.h>

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef float input_t1;
typedef float sum_sq_t1;
typedef ap_uint<512> input_t16;
typedef ap_uint<128> input_t4;
typedef ap_uint<128> sum_sq_t4;
/* Data Type */

void kernel0(input_t16 *input, sum_sq_t4 *sum_sq);
void input_IO_L1_in_intra_trans(int idx, int c1, input_t4 local_input[4][4], hls::stream<float> &fifo_input_local_out, bool intra_trans_en);
void input_IO_L1_in_inter_trans(int idx, int c1, input_t4 local_input[4][4], hls::stream<input_t4> &fifo_input_in, hls::stream<input_t4> &fifo_input_out, bool inter_trans_en);
void input_IO_L1_in_inter_trans_boundary(int idx, int c1, input_t4 local_input[4][4], hls::stream<input_t4> &fifo_input_in, bool inter_trans_en);
void input_IO_L1_in_wrapper(int idx, hls::stream<input_t4> &fifo_input_in, hls::stream<input_t4> &fifo_input_out, hls::stream<float> &fifo_input_local_out);
void input_IO_L1_in_boundary_wrapper(int idx, hls::stream<input_t4> &fifo_input_in, hls::stream<float> &fifo_input_local_out);
void PE_wrapper(int idx, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_sum_sq_drain_out);
void sum_sq_drain_IO_L1_out_intra_trans(int idx, sum_sq_t4 local_sum_sq[1], hls::stream<float> &fifo_sum_sq_drain_local_in);
void sum_sq_drain_IO_L1_out_inter_trans(int idx, sum_sq_t4 local_sum_sq[1], hls::stream<sum_sq_t4> &fifo_sum_sq_drain_in, hls::stream<sum_sq_t4> &fifo_sum_sq_drain_out);
void sum_sq_drain_IO_L1_out_inter_trans_boundary(int idx, sum_sq_t4 local_sum_sq[1], hls::stream<sum_sq_t4> &fifo_sum_sq_drain_out);
void sum_sq_drain_IO_L1_out_wrapper(int idx, hls::stream<sum_sq_t4> &fifo_sum_sq_drain_in, hls::stream<sum_sq_t4> &fifo_sum_sq_drain_out, hls::stream<float> &fifo_sum_sq_drain_local_in);
void sum_sq_drain_IO_L1_out_boundary_wrapper(int idx, hls::stream<sum_sq_t4> &fifo_sum_sq_drain_out, hls::stream<float> &fifo_sum_sq_drain_local_in);
