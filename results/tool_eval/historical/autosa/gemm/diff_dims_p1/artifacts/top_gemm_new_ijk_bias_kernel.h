#include <ap_int.h>
#include <hls_stream.h>

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef float A_t1;
typedef float B_t1;
typedef float C_t1;
typedef float bias_t1;
typedef ap_uint<512> A_t16;
typedef ap_uint<128> A_t4;
typedef ap_uint<512> B_t16;
typedef ap_uint<64> B_t2;
typedef ap_uint<512> C_t16;
typedef ap_uint<64> C_t2;
typedef ap_uint<512> bias_t16;
typedef ap_uint<64> bias_t2;
/* Data Type */

/* Helper Function */
inline void host_serialize_A(float *A_to, float *A_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 23; c0 += 1)
    for (int c1 = 0; c1 <= 31; c1 += 1)
      for (int c2 = 0; c2 <= 127; c2 += 1) {
        // array
        // io_L3
        for (int c3 = 0; c3 <= 1; c3 += 1) {
          // io_L2
          for (int c4 = 0; c4 <= 1; c4 += 1)
            for (int c5 = 0; c5 <= 3; c5 += 1)
              A_to[cnt++] = A_from[(4 * c0 + 2 * c3 + c4) * 512 + (4 * c2 + c5)];
        }
      }
}
/* Helper Function */

/* Helper Function */
inline void host_serialize_B(float *B_to, float *B_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 23; c0 += 1)
    for (int c1 = 0; c1 <= 31; c1 += 1)
      for (int c2 = 0; c2 <= 127; c2 += 1) {
        // array
        // io_L3
        for (int c3 = 0; c3 <= 1; c3 += 1) {
          // io_L2
          for (int c4 = 0; c4 <= 3; c4 += 1)
            for (int c5 = 0; c5 <= 1; c5 += 1)
              B_to[cnt++] = B_from[(4 * c2 + c4) * 128 + (4 * c1 + 2 * c3 + c5)];
        }
      }
}
/* Helper Function */

/* Helper Function */
inline void host_serialize_bias(float *bias_to, float *bias_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 23; c0 += 1)
    for (int c1 = 0; c1 <= 31; c1 += 1) {
      // array
      // io_L3
      for (int c3 = 0; c3 <= 1; c3 += 1) {
        // io_L2
        for (int c4 = 0; c4 <= 1; c4 += 1) {
          // io_L1
          // pe
          for (int c5 = 0; c5 <= 1; c5 += 1)
            for (int c6 = 0; c6 <= 1; c6 += 1)
              bias_to[cnt++] = bias_from[(4 * c0 + 2 * c4 + c5) * 128 + (4 * c1 + 2 * c3 + c6)];
        }
      }
    }
}
/* Helper Function */

/* Helper Function */
inline void host_deserialize_C(float *C_to, float *C_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 23; c0 += 1)
    for (int c1 = 0; c1 <= 31; c1 += 1) {
      // array
      // io_L3
      for (int c3 = 0; c3 <= 1; c3 += 1) {
        // io_L2
        for (int c4 = 0; c4 <= 1; c4 += 1) {
          // io_L1
          // pe
          for (int c5 = 0; c5 <= 1; c5 += 1)
            for (int c6 = 0; c6 <= 1; c6 += 1)
              C_to[(4 * c0 + 2 * c4 + c5) * 128 + (4 * c1 + 2 * c3 + c6)] = C_from[cnt++];
        }
      }
    }
}
/* Helper Function */

void kernel0(A_t16 *A, B_t16 *B, C_t16 *C, bias_t16 *bias);
void A_IO_L2_in_intra_trans(int idx, int c0, int c1, int c2, A_t4 local_A[2][1], hls::stream<float> &fifo_A_local_out, bool intra_trans_en);
void A_IO_L2_in_inter_trans(int idx, int c0, int c1, int c2, A_t4 local_A[2][1], hls::stream<A_t4> &fifo_A_in, hls::stream<A_t4> &fifo_A_out, bool inter_trans_en);
void A_IO_L2_in_inter_trans_boundary(int idx, int c0, int c1, int c2, A_t4 local_A[2][1], hls::stream<A_t4> &fifo_A_in, bool inter_trans_en);
void B_IO_L2_in_intra_trans(int idx, int c0, int c1, int c2, B_t2 local_B[4][1], hls::stream<float> &fifo_B_local_out, bool intra_trans_en);
void B_IO_L2_in_inter_trans(int idx, int c0, int c1, int c2, B_t2 local_B[4][1], hls::stream<B_t2> &fifo_B_in, hls::stream<B_t2> &fifo_B_out, bool inter_trans_en);
void B_IO_L2_in_inter_trans_boundary(int idx, int c0, int c1, int c2, B_t2 local_B[4][1], hls::stream<B_t2> &fifo_B_in, bool inter_trans_en);
void bias_IO_L1_in_intra_trans(int idx, int idy, int c0, int c1, bias_t2 local_bias[2][1], hls::stream<float> &fifo_bias_local_out, bool intra_trans_en);
void bias_IO_L1_in_inter_trans(int idx, int idy, int c0, int c1, bias_t2 local_bias[2][1], hls::stream<bias_t2> &fifo_bias_in, hls::stream<bias_t2> &fifo_bias_out, bool inter_trans_en);
void bias_IO_L1_in_inter_trans_boundary(int idx, int idy, int c0, int c1, bias_t2 local_bias[2][1], hls::stream<bias_t2> &fifo_bias_in, bool inter_trans_en);
void bias_IO_L1_in_wrapper(int idx, int idy, hls::stream<bias_t2> &fifo_bias_in, hls::stream<bias_t2> &fifo_bias_out, hls::stream<float> &fifo_bias_local_out);
void bias_IO_L1_in_boundary_wrapper(int idx, int idy, hls::stream<bias_t2> &fifo_bias_in, hls::stream<float> &fifo_bias_local_out);
void PE_wrapper(int idx, int idy, hls::stream<float> &fifo_A_in, hls::stream<float> &fifo_A_out, hls::stream<float> &fifo_B_in, hls::stream<float> &fifo_B_out, hls::stream<float> &fifo_C_drain_out, hls::stream<float> &fifo_bias_in);
void C_drain_IO_L1_out_intra_trans(int idx, int idy, int c0, int c1, C_t2 local_C[2][1], hls::stream<float> &fifo_C_drain_local_in);
void C_drain_IO_L1_out_inter_trans(int idx, int idy, int c0, int c1, C_t2 local_C[2][1], hls::stream<C_t2> &fifo_C_drain_in, hls::stream<C_t2> &fifo_C_drain_out);
void C_drain_IO_L1_out_inter_trans_boundary(int idx, int idy, int c0, int c1, C_t2 local_C[2][1], hls::stream<C_t2> &fifo_C_drain_out);
void C_drain_IO_L1_out_wrapper(int idx, int idy, hls::stream<C_t2> &fifo_C_drain_in, hls::stream<C_t2> &fifo_C_drain_out, hls::stream<float> &fifo_C_drain_local_in);
void C_drain_IO_L1_out_boundary_wrapper(int idx, int idy, hls::stream<C_t2> &fifo_C_drain_out, hls::stream<float> &fifo_C_drain_local_in);
