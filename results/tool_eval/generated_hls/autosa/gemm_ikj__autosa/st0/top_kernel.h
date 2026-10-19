#include <ap_int.h>
#include <hls_stream.h>

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef float BRAM_A_t1;
typedef float BRAM_B_t1;
typedef float BRAM_C_t1;
typedef ap_uint<512> BRAM_A_t16;
typedef ap_uint<128> BRAM_A_t4;
typedef ap_uint<512> BRAM_B_t16;
typedef ap_uint<512> BRAM_C_t16;
typedef ap_uint<128> BRAM_C_t4;
/* Data Type */

void kernel0(BRAM_A_t16 *BRAM_A, BRAM_B_t16 *BRAM_B, BRAM_C_t16 *BRAM_C);
void BRAM_A_IO_L1_in_intra_trans(int idx, int c0, int c1, int c2, BRAM_A_t4 local_BRAM_A[4][4], hls::stream<float> &fifo_BRAM_A_local_out, bool intra_trans_en);
void BRAM_A_IO_L1_in_inter_trans(int idx, int c0, int c1, int c2, BRAM_A_t4 local_BRAM_A[4][4], hls::stream<BRAM_A_t4> &fifo_BRAM_A_in, hls::stream<BRAM_A_t4> &fifo_BRAM_A_out, bool inter_trans_en);
void BRAM_A_IO_L1_in_inter_trans_boundary(int idx, int c0, int c1, int c2, BRAM_A_t4 local_BRAM_A[4][4], hls::stream<BRAM_A_t4> &fifo_BRAM_A_in, bool inter_trans_en);
void BRAM_A_IO_L1_in_wrapper(int idx, hls::stream<BRAM_A_t4> &fifo_BRAM_A_in, hls::stream<BRAM_A_t4> &fifo_BRAM_A_out, hls::stream<float> &fifo_BRAM_A_local_out);
void BRAM_A_IO_L1_in_boundary_wrapper(int idx, hls::stream<BRAM_A_t4> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_local_out);
void BRAM_B_IO_L2_in_intra_trans(int c0, int c1, int c2, BRAM_B_t16 *BRAM_B, BRAM_B_t16 local_BRAM_B[16][1], hls::stream<float> &fifo_BRAM_B_local_out, bool intra_trans_en);
void BRAM_B_IO_L2_in_inter_trans_boundary(int c0, int c1, int c2, BRAM_B_t16 *BRAM_B, BRAM_B_t16 local_BRAM_B[16][1], hls::stream<BRAM_B_t16> &fifo_BRAM_B_in, bool inter_trans_en);
void PE_wrapper(int idx, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_B_in, hls::stream<float> &fifo_BRAM_B_out, hls::stream<float> &fifo_BRAM_C_drain_out);
void BRAM_C_drain_IO_L1_out_intra_trans(int idx, int c0, int c1, BRAM_C_t4 local_BRAM_C[4][4], hls::stream<float> &fifo_BRAM_C_drain_local_in);
void BRAM_C_drain_IO_L1_out_inter_trans(int idx, int c0, int c1, BRAM_C_t4 local_BRAM_C[4][4], hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_in, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out);
void BRAM_C_drain_IO_L1_out_inter_trans_boundary(int idx, int c0, int c1, BRAM_C_t4 local_BRAM_C[4][4], hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out);
void BRAM_C_drain_IO_L1_out_wrapper(int idx, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_in, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out, hls::stream<float> &fifo_BRAM_C_drain_local_in);
void BRAM_C_drain_IO_L1_out_boundary_wrapper(int idx, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out, hls::stream<float> &fifo_BRAM_C_drain_local_in);
