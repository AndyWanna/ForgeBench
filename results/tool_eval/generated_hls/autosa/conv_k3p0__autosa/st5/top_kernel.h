#include <ap_int.h>
#include <hls_stream.h>

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef float input_t1;
typedef float kernel_t1;
typedef float output_t1;
typedef ap_uint<64> input_t2;
typedef ap_uint<128> output_t4;
/* Data Type */

void kernel0(input_t2 *input, kernel_t1 *kernel, output_t4 *output);
void input_IO_L2_in_intra_trans(int idx, input_t2 local_input[16][6][7], hls::stream<float> &fifo_input_local_out, bool intra_trans_en);
void input_IO_L2_in_inter_trans(int idx, input_t2 local_input[16][6][7], hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_out, bool inter_trans_en);
void input_IO_L2_in_inter_trans_boundary(int idx, input_t2 local_input[16][6][7], hls::stream<input_t2> &fifo_input_in, bool inter_trans_en);
void kernel_IO_L2_in_intra_trans(int idx, float local_kernel[4][16][3][3], hls::stream<float> &fifo_kernel_local_out, bool intra_trans_en);
void kernel_IO_L2_in_inter_trans(int idx, float local_kernel[4][16][3][3], hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, bool inter_trans_en);
void kernel_IO_L2_in_inter_trans_boundary(int idx, float local_kernel[4][16][3][3], hls::stream<float> &fifo_kernel_in, bool inter_trans_en);
void PE_wrapper(int idx, int idy, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_input_out, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, hls::stream<float> &fifo_output_drain_out);
void output_drain_IO_L1_out_intra_trans(int idx, int idy, output_t4 local_output[4][4][3], hls::stream<float> &fifo_output_drain_local_in);
void output_drain_IO_L1_out_inter_trans(int idx, int idy, output_t4 local_output[4][4][3], hls::stream<output_t4> &fifo_output_drain_in, hls::stream<output_t4> &fifo_output_drain_out);
void output_drain_IO_L1_out_inter_trans_boundary(int idx, int idy, output_t4 local_output[4][4][3], hls::stream<output_t4> &fifo_output_drain_out);
void output_drain_IO_L1_out_wrapper(int idx, int idy, hls::stream<output_t4> &fifo_output_drain_in, hls::stream<output_t4> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in);
void output_drain_IO_L1_out_boundary_wrapper(int idx, int idy, hls::stream<output_t4> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in);
