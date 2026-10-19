#include <ap_int.h>
#include <hls_stream.h>

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef float output_t1;
typedef float sum_t1;
typedef ap_uint<512> output_t16;
typedef ap_uint<128> output_t4;
typedef ap_uint<128> sum_t4;
/* Data Type */

void kernel0(output_t16 *output, sum_t4 *sum);
void output_IO_L1_in_intra_trans(int idx, int c1, output_t4 local_output[4][4], hls::stream<float> &fifo_output_local_out, bool intra_trans_en);
void output_IO_L1_in_inter_trans(int idx, int c1, output_t4 local_output[4][4], hls::stream<output_t4> &fifo_output_in, hls::stream<output_t4> &fifo_output_out, bool inter_trans_en);
void output_IO_L1_in_inter_trans_boundary(int idx, int c1, output_t4 local_output[4][4], hls::stream<output_t4> &fifo_output_in, bool inter_trans_en);
void output_IO_L1_in_wrapper(int idx, hls::stream<output_t4> &fifo_output_in, hls::stream<output_t4> &fifo_output_out, hls::stream<float> &fifo_output_local_out);
void output_IO_L1_in_boundary_wrapper(int idx, hls::stream<output_t4> &fifo_output_in, hls::stream<float> &fifo_output_local_out);
void PE_wrapper(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_sum_drain_out);
void sum_drain_IO_L1_out_intra_trans(int idx, sum_t4 local_sum[1], hls::stream<float> &fifo_sum_drain_local_in);
void sum_drain_IO_L1_out_inter_trans(int idx, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out);
void sum_drain_IO_L1_out_inter_trans_boundary(int idx, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_drain_out);
void sum_drain_IO_L1_out_wrapper(int idx, hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in);
void sum_drain_IO_L1_out_boundary_wrapper(int idx, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in);
