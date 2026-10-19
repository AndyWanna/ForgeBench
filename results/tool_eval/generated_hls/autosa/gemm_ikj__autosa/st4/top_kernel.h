#include <ap_int.h>
#include <hls_stream.h>

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef float BRAM_A_t1;
typedef float BRAM_B_t1;
typedef float BRAM_C_t1;
typedef ap_uint<512> BRAM_B_t16;
typedef ap_uint<512> BRAM_C_t16;
typedef ap_uint<128> BRAM_C_t4;
/* Data Type */

/* Helper Function */
inline void BRAM_C_drain_drain_merge(float *BRAM_C_to, float *BRAM_C_from){
  /* Variable Declaration */
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 3; c0 += 1)
    for (int c1 = 0; c1 <= 3; c1 += 1) {
      // array
      // io_L3
      // io_L2
      for (int c4 = 0; c4 <= 3; c4 += 1) {
        // io_L1
        // pe
        for (int c5 = 0; c5 <= 3; c5 += 1) {
          // latency
          for (int c6 = 0; c6 <= 3; c6 += 1) {
            // latency
            for (int c7 = 0; c7 <= 3; c7 += 1)
              BRAM_C_to[(16 * c0 + 4 * c4 + c7) * 64 + (16 * c1 + 4 * c5 + c6)] = BRAM_C_from[(16 * c0 + 4 * c4 + c7) * 64 + (16 * c1 + 4 * c5 + c6)];
          }
        }
      }
    }
}
/* Helper Function */

void kernel0(BRAM_A_t1 *BRAM_A, BRAM_B_t16 *BRAM_B, BRAM_C_t16 *BRAM_C_0, BRAM_C_t16 *BRAM_C_1, BRAM_C_t16 *BRAM_C_2);
void BRAM_A_IO_L1_in_intra_trans(int idx, int idy, int c0, int c1, int c2, float local_BRAM_A[4][1], hls::stream<float> &fifo_BRAM_A_local_out, bool intra_trans_en);
void BRAM_A_IO_L1_in_inter_trans(int idx, int idy, int c0, int c1, int c2, float local_BRAM_A[4][1], hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_out, bool inter_trans_en);
void BRAM_A_IO_L1_in_inter_trans_boundary(int idx, int idy, int c0, int c1, int c2, float local_BRAM_A[4][1], hls::stream<float> &fifo_BRAM_A_in, bool inter_trans_en);
void BRAM_A_IO_L1_in_wrapper(int idx, int idy, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_out, hls::stream<float> &fifo_BRAM_A_local_out);
void BRAM_A_IO_L1_in_boundary_wrapper(int idx, int idy, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_local_out);
void BRAM_B_IO_L2_in_intra_trans(int idx, int c0, int c1, int c2, BRAM_B_t16 local_BRAM_B[1][1], hls::stream<float> &fifo_BRAM_B_local_out, bool intra_trans_en);
void BRAM_B_IO_L2_in_inter_trans(int idx, int c0, int c1, int c2, BRAM_B_t16 local_BRAM_B[1][1], hls::stream<BRAM_B_t16> &fifo_BRAM_B_in, hls::stream<BRAM_B_t16> &fifo_BRAM_B_out, bool inter_trans_en);
void BRAM_B_IO_L2_in_inter_trans_boundary(int idx, int c0, int c1, int c2, BRAM_B_t16 local_BRAM_B[1][1], hls::stream<BRAM_B_t16> &fifo_BRAM_B_in, bool inter_trans_en);
void BRAM_C_IO_L2_in_intra_trans(int idx, int c0, int c1, BRAM_C_t0 local_BRAM_C[4][16], hls::stream<float> &fifo_BRAM_C_local_out, bool intra_trans_en);
void PE_wrapper(int idx, int idy, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_B_in, hls::stream<float> &fifo_BRAM_B_out, hls::stream<float> &fifo_BRAM_C_in, hls::stream<float> &fifo_BRAM_C_out, hls::stream<float> &fifo_BRAM_C_drain_out);
void BRAM_C_IO_L2_out_intra_trans(int idx, int c0, int c1, BRAM_C_t16 local_BRAM_C[4][1], hls::stream<float> &fifo_BRAM_C_local_in, bool intra_trans_en);
void BRAM_C_IO_L2_out_inter_trans(int idx, int c0, int c1, BRAM_C_t16 local_BRAM_C[4][1], hls::stream<BRAM_C_t16> &fifo_BRAM_C_in, hls::stream<BRAM_C_t16> &fifo_BRAM_C_out, bool inter_trans_en);
void BRAM_C_IO_L2_out_inter_trans_boundary(int idx, int c0, int c1, BRAM_C_t16 local_BRAM_C[4][1], hls::stream<BRAM_C_t16> &fifo_BRAM_C_out, bool inter_trans_en);
void BRAM_C_drain_IO_L1_out_intra_trans(int idx, int idy, int c0, int c1, BRAM_C_t4 local_BRAM_C[4][4], hls::stream<float> &fifo_BRAM_C_drain_local_in);
void BRAM_C_drain_IO_L1_out_inter_trans(int idx, int idy, int c0, int c1, BRAM_C_t4 local_BRAM_C[4][4], hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_in, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out);
void BRAM_C_drain_IO_L1_out_inter_trans_boundary(int idx, int idy, int c0, int c1, BRAM_C_t4 local_BRAM_C[4][4], hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out);
void BRAM_C_drain_IO_L1_out_wrapper(int idx, int idy, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_in, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out, hls::stream<float> &fifo_BRAM_C_drain_local_in);
void BRAM_C_drain_IO_L1_out_boundary_wrapper(int idx, int idy, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out, hls::stream<float> &fifo_BRAM_C_drain_local_in);
