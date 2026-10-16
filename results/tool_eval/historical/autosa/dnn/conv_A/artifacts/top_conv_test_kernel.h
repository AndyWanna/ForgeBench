#include <ap_int.h>
#include <hls_stream.h>

//#define min(x,y) ((x < y) ? x : y)
#ifdef min               /* kill any previous macro from ap_int.h or others */
#undef min
#endif
#define min(a,b)  (( (int)(a) < (int)(b) ) ? (int)(a) : (int)(b))
#define max(x,y) ((x > y) ? x : y)

/* Data Type */
typedef float bias_t1;
typedef float cin_t1;
typedef float cout_t1;
typedef float w_t1;
typedef ap_uint<512> bias_t16;
typedef ap_uint<64> bias_t2;
typedef ap_uint<512> cin_t16;
typedef ap_uint<64> cin_t2;
typedef ap_uint<512> cout_t16;
typedef ap_uint<128> cout_t4;
typedef ap_uint<512> w_t16;
/* Data Type */

/* Helper Function */
inline void host_serialize_bias(float *bias_to, float *bias_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 3; c0 += 1)
    for (int c1 = 0; c1 <= 3; c1 += 1)
      for (int c2 = 0; c2 <= 15; c2 += 1) {
        // array
        // io_L3
        for (int c4 = 0; c4 <= 1; c4 += 1) {
          // io_L2
          for (int c5 = 0; c5 <= 1; c5 += 1)
            bias_to[cnt++] = bias_from[4 * c2 + 2 * c4 + c5];
        }
      }
}
/* Helper Function */

/* Helper Function */
inline void host_serialize_cin(float *cin_to, float *cin_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 3; c0 += 1)
    for (int c1 = 0; c1 <= 3; c1 += 1)
      for (int c2 = 0; c2 <= 15; c2 += 1) {
        // array
        // io_L3
        for (int c4 = 0; c4 <= min(1, -2 * c0 + 6); c4 += 1) {
          // io_L2
          for (int c5 = 0; c5 <= 63; c5 += 1)
            for (int c6 = 0; c6 <= 3; c6 += 1)
              for (int c7 = 0; c7 <= min(5, -4 * c1 + 15); c7 += 1)
                cin_to[cnt++] = cin_from[(c5 * 16 + (4 * c0 + 2 * c4 + c6)) * 16 + (4 * c1 + c7)];
        }
      }
}
/* Helper Function */

/* Helper Function */
inline void host_serialize_w(float *w_to, float *w_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 3; c0 += 1)
    for (int c1 = 0; c1 <= 3; c1 += 1)
      for (int c2 = 0; c2 <= 15; c2 += 1) {
        // array
        // io_L3
        for (int c4 = 0; c4 <= 1; c4 += 1) {
          // io_L2
          for (int c5 = 0; c5 <= 1; c5 += 1)
            for (int c6 = 0; c6 <= 63; c6 += 1)
              for (int c7 = 0; c7 <= 2; c7 += 1)
                for (int c8 = 0; c8 <= 2; c8 += 1)
                  w_to[cnt++] = w_from[(((4 * c2 + 2 * c4 + c5) * 64 + c6) * 3 + c7) * 3 + c8];
        }
      }
}
/* Helper Function */

/* Helper Function */
inline void host_deserialize_cout(float *cout_to, float *cout_from){
  /* Variable Declaration */
  unsigned int cnt = 0;
  /* Variable Declaration */

  for (int c0 = 0; c0 <= 3; c0 += 1)
    for (int c1 = 0; c1 <= 3; c1 += 1)
      for (int c2 = 0; c2 <= 15; c2 += 1) {
        // array
        // io_L3
        for (int c4 = 0; c4 <= 1; c4 += 1) {
          // io_L2
          for (int c5 = 0; c5 <= min(1, -2 * c0 + 6); c5 += 1) {
            // io_L1
            // pe
            for (int c6 = 0; c6 <= 1; c6 += 1)
              for (int c7 = 0; c7 <= 1; c7 += 1)
                for (int c8 = 0; c8 <= min(3, -4 * c1 + 13); c8 += 1)
                  cout_to[((4 * c2 + 2 * c4 + c6) * 14 + (4 * c0 + 2 * c5 + c7)) * 14 + (4 * c1 + c8)] = cout_from[cnt++];
          }
        }
      }
}
/* Helper Function */

void kernel0(bias_t16 *bias, cin_t16 *cin, cout_t16 *cout, w_t16 *w);
void bias_IO_L2_in_intra_trans(int idx, int c0, int c1, int c2, bias_t2 local_bias[1], hls::stream<float> &fifo_bias_local_out, bool intra_trans_en);
void bias_IO_L2_in_inter_trans(int idx, int c0, int c1, int c2, bias_t2 local_bias[1], hls::stream<bias_t2> &fifo_bias_in, hls::stream<bias_t2> &fifo_bias_out, bool inter_trans_en);
void bias_IO_L2_in_inter_trans_boundary(int idx, int c0, int c1, int c2, bias_t2 local_bias[1], hls::stream<bias_t2> &fifo_bias_in, bool inter_trans_en);
void cin_IO_L2_in_intra_trans(int idx, int c0, int c1, cin_t2 local_cin[64][4][3], hls::stream<float> &fifo_cin_local_out, bool intra_trans_en);
void cin_IO_L2_in_inter_trans(int idx, int c0, int c1, cin_t2 local_cin[64][4][3], hls::stream<cin_t2> &fifo_cin_in, hls::stream<cin_t2> &fifo_cin_out, bool inter_trans_en);
void cin_IO_L2_in_inter_trans_boundary(int idx, int c0, int c1, cin_t2 local_cin[64][4][3], hls::stream<cin_t2> &fifo_cin_in, bool inter_trans_en);
void w_IO_L2_in_intra_trans(int idx, int c0, int c1, int c2, float local_w[2][64][3][3], hls::stream<float> &fifo_w_local_out, bool intra_trans_en);
void w_IO_L2_in_inter_trans(int idx, int c0, int c1, int c2, float local_w[2][64][3][3], hls::stream<float> &fifo_w_in, hls::stream<float> &fifo_w_out, bool inter_trans_en);
void w_IO_L2_in_inter_trans_boundary(int idx, int c0, int c1, int c2, float local_w[2][64][3][3], hls::stream<float> &fifo_w_in, bool inter_trans_en);
void PE_wrapper(int idx, int idy, hls::stream<float> &fifo_bias_in, hls::stream<float> &fifo_bias_out, hls::stream<float> &fifo_cin_in, hls::stream<float> &fifo_cin_out, hls::stream<float> &fifo_cout_drain_out, hls::stream<float> &fifo_w_in, hls::stream<float> &fifo_w_out);
void cout_drain_IO_L1_out_intra_trans(int idx, int idy, int c0, int c1, int c2, cout_t4 local_cout[2][2][1], hls::stream<float> &fifo_cout_drain_local_in);
void cout_drain_IO_L1_out_inter_trans(int idx, int idy, int c0, int c1, int c2, cout_t4 local_cout[2][2][1], hls::stream<cout_t4> &fifo_cout_drain_in, hls::stream<cout_t4> &fifo_cout_drain_out);
void cout_drain_IO_L1_out_inter_trans_boundary(int idx, int idy, int c0, int c1, int c2, cout_t4 local_cout[2][2][1], hls::stream<cout_t4> &fifo_cout_drain_out);
void cout_drain_IO_L1_out_wrapper(int idx, int idy, hls::stream<cout_t4> &fifo_cout_drain_in, hls::stream<cout_t4> &fifo_cout_drain_out, hls::stream<float> &fifo_cout_drain_local_in);
void cout_drain_IO_L1_out_boundary_wrapper(int idx, int idy, hls::stream<cout_t4> &fifo_cout_drain_out, hls::stream<float> &fifo_cout_drain_local_in);
