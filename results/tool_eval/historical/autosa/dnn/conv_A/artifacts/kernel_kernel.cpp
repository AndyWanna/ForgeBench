#include "kernel_kernel.h"
/* Module Definition */
void cin_IO_L3_in(hls::stream<cin_t4> &fifo_cin_in, hls::stream<cin_t4> &fifo_cin_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1)
        for (ap_uint<8> c3 = 0; c3 <= 127; c3 += 1) {
          // array
          // io_L3
          for (ap_uint<2> c4 = 0; c4 <= 1; c4 += 1) {
            // io_L2
            for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
              for (ap_uint<4> c6 = 0; c6 <= 5; c6 += 1) {
              #pragma HLS PIPELINE II=1
                // access_coalesce
                // access_serialize
                {
                  cin_t4 in_data;
                  cin_t4 out_data;
                  in_data = fifo_cin_in.read();
                  out_data = in_data;
                  fifo_cin_local_out.write(out_data);
                }
              }
          }
        }
}
/* Module Definition */

/* Module Definition */
void cin_IO_L3_in_serialize(cin_t16 *cin, hls::stream<cin_t4> &fifo_cin_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  cin_t4 fifo_data;
  cin_t16 mem_data;
  for (ap_uint<27> i = 0; i < 48168960; i++) {
  #pragma HLS PIPELINE II=1
    mem_data = cin[i];
    for (ap_uint<3> p = 0; p < 4; p++) {
      fifo_data = mem_data(127, 0);
      mem_data = mem_data >> 128;
      fifo_cin_local_out.write(fifo_data);
    }
  }
}
/* Module Definition */

/* Module Definition */
void cin_IO_L2_in_intra_trans(int idx, int c0, int c1, int c2, int c3, cin_t4 local_cin[4][6][1], hls::stream<float> &fifo_cin_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L2
  // io_L1
  // pe
  for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
    for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1)
      for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1)
        for (ap_uint<3> c9 = 0; c9 <= 2; c9 += 1) {
          // latency
          for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
            // latency
            for (ap_uint<2> c11 = 0; c11 <= 1; c11 += 1) {
              // latency
              for (ap_uint<2> c12 = 0; c12 <= 1; c12 += 1) {
              #pragma HLS PIPELINE II=1
                {
                  cin_t4 in_data;
                  cin_t1 out_data;
                  in_data = local_cin[c11 + c8][c10 + 2*c6 + c9][c7 / 4];
                  for (ap_uint<3> n = 0; n < 4; n++) {
                  #pragma HLS UNROLL
                    data_split[n] = in_data(31, 0);
                    in_data = in_data >> 32;
                  }
                  int split_idx = (c7) % 4;
                  union {unsigned int ui; float ut;} u;
                  u.ui = (unsigned int)data_split[split_idx];
                  out_data = u.ut;
                  fifo_cin_local_out.write(out_data);
                }
              }
            }
          }
        }
}
/* Module Definition */

/* Module Definition */
void cin_IO_L2_in_inter_trans(int idx, int c0, int c1, int c2, int c3, cin_t4 local_cin[4][6][1], hls::stream<cin_t4> &fifo_cin_in, hls::stream<cin_t4> &fifo_cin_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c4 = p0; c4 <= 1; c4 += 1) {
    // io_L2
    if (c4 == p0) {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<4> c6 = 0; c6 <= 5; c6 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            cin_t4 in_data;
            cin_t4 out_data;
            in_data = fifo_cin_in.read();
            out_data = in_data;
            local_cin[c5][c6][0] = out_data;
          }
        }
    } else {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<4> c6 = 0; c6 <= 5; c6 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            cin_t4 in_data;
            cin_t4 out_data;
            in_data = fifo_cin_in.read();
            out_data = in_data;
            fifo_cin_out.write(out_data);
          }
        }
    }
  }
}
/* Module Definition */

/* Module Definition */
void cin_IO_L2_in_inter_trans_boundary(int idx, int c0, int c1, int c2, int c3, cin_t4 local_cin[4][6][1], hls::stream<cin_t4> &fifo_cin_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c4 = p0; c4 <= 1; c4 += 1)
    if (c4 == p0) {
      // io_L2
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<4> c6 = 0; c6 <= 5; c6 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            cin_t4 in_data;
            cin_t4 out_data;
            in_data = fifo_cin_in.read();
            out_data = in_data;
            local_cin[c5][c6][0] = out_data;
          }
        }
    }
}
/* Module Definition */

/* Module Definition */
void cin_IO_L2_in(int idx, hls::stream<cin_t4> &fifo_cin_in, hls::stream<cin_t4> &fifo_cin_out, hls::stream<float> &fifo_cin_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  cin_t4 local_cin_ping[4][6][1];
  #pragma HLS RESOURCE variable=local_cin_ping core=RAM_2P_BRAM
  cin_t4 local_cin_pong[4][6][1];
  #pragma HLS RESOURCE variable=local_cin_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c0, c0_prev;
  int c1, c1_prev;
  int c2, c2_prev;
  int c3, c3_prev;
  /* Variable Declaration */

  {
    for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
      for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
        for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1)
          for (ap_uint<8> c3 = 0; c3 <= 127; c3 += 1) {
            // array
            // io_L3
            {
              if (arb == 0) {
                cin_IO_L2_in_inter_trans(
                  /* module id */ idx, 
                  /* host iter */ c0, 
                  /* host iter */ c1, 
                  /* host iter */ c2, 
                  /* host iter */ c3, 
                  /* array */ local_cin_pong, 
                  /* fifo */ fifo_cin_in, 
                  /* fifo */ fifo_cin_out, 
                  /* enable */ inter_trans_en
                );
                cin_IO_L2_in_intra_trans(
                  /* module id */ idx, 
                  /* host iter */ c0_prev, 
                  /* host iter */ c1_prev, 
                  /* host iter */ c2_prev, 
                  /* host iter */ c3_prev, 
                  /* array */ local_cin_ping, 
                  /* fifo */ fifo_cin_local_out, 
                  /* enable */ intra_trans_en
                );
              } else {
                cin_IO_L2_in_inter_trans(
                  /* module id */ idx, 
                  /* host iter */ c0, 
                  /* host iter */ c1, 
                  /* host iter */ c2, 
                  /* host iter */ c3, 
                  /* array */ local_cin_ping, 
                  /* fifo */ fifo_cin_in, 
                  /* fifo */ fifo_cin_out, 
                  /* enable */ inter_trans_en
                );
                cin_IO_L2_in_intra_trans(
                  /* module id */ idx, 
                  /* host iter */ c0_prev, 
                  /* host iter */ c1_prev, 
                  /* host iter */ c2_prev, 
                  /* host iter */ c3_prev, 
                  /* array */ local_cin_pong, 
                  /* fifo */ fifo_cin_local_out, 
                  /* enable */ intra_trans_en
                );
              }
              intra_trans_en = 1;
              arb = !arb;
              c0_prev = c0;
              c1_prev = c1;
              c2_prev = c2;
              c3_prev = c3;
            }
          }
    if (arb == 0) {
      cin_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* host iter */ c3_prev, 
        /* array */ local_cin_ping, 
        /* fifo */ fifo_cin_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      cin_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* host iter */ c3_prev, 
        /* array */ local_cin_pong, 
        /* fifo */ fifo_cin_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void cin_IO_L2_in_boundary(int idx, hls::stream<cin_t4> &fifo_cin_in, hls::stream<float> &fifo_cin_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  cin_t4 local_cin_ping[4][6][1];
  #pragma HLS RESOURCE variable=local_cin_ping core=RAM_2P_BRAM
  cin_t4 local_cin_pong[4][6][1];
  #pragma HLS RESOURCE variable=local_cin_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c0, c0_prev;
  int c1, c1_prev;
  int c2, c2_prev;
  int c3, c3_prev;
  /* Variable Declaration */

  {
    for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
      for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
        for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1)
          for (ap_uint<8> c3 = 0; c3 <= 127; c3 += 1) {
            // array
            // io_L3
            {
              if (arb == 0) {
                cin_IO_L2_in_inter_trans_boundary(
                  /* module id */ idx, 
                  /* host iter */ c0, 
                  /* host iter */ c1, 
                  /* host iter */ c2, 
                  /* host iter */ c3, 
                  /* array */ local_cin_pong, 
                  /* fifo */ fifo_cin_in, 
                  /* enable */ inter_trans_en
                );
                cin_IO_L2_in_intra_trans(
                  /* module id */ idx, 
                  /* host iter */ c0_prev, 
                  /* host iter */ c1_prev, 
                  /* host iter */ c2_prev, 
                  /* host iter */ c3_prev, 
                  /* array */ local_cin_ping, 
                  /* fifo */ fifo_cin_local_out, 
                  /* enable */ intra_trans_en
                );
              } else {
                cin_IO_L2_in_inter_trans_boundary(
                  /* module id */ idx, 
                  /* host iter */ c0, 
                  /* host iter */ c1, 
                  /* host iter */ c2, 
                  /* host iter */ c3, 
                  /* array */ local_cin_ping, 
                  /* fifo */ fifo_cin_in, 
                  /* enable */ inter_trans_en
                );
                cin_IO_L2_in_intra_trans(
                  /* module id */ idx, 
                  /* host iter */ c0_prev, 
                  /* host iter */ c1_prev, 
                  /* host iter */ c2_prev, 
                  /* host iter */ c3_prev, 
                  /* array */ local_cin_pong, 
                  /* fifo */ fifo_cin_local_out, 
                  /* enable */ intra_trans_en
                );
              }
              intra_trans_en = 1;
              arb = !arb;
              c0_prev = c0;
              c1_prev = c1;
              c2_prev = c2;
              c3_prev = c3;
            }
          }
    if (arb == 0) {
      cin_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* host iter */ c3_prev, 
        /* array */ local_cin_ping, 
        /* fifo */ fifo_cin_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      cin_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* host iter */ c3_prev, 
        /* array */ local_cin_pong, 
        /* fifo */ fifo_cin_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void w_IO_L3_in(hls::stream<w_t4> &fifo_w_in, hls::stream<w_t4> &fifo_w_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1)
        for (ap_uint<8> c3 = 0; c3 <= 127; c3 += 1) {
          // array
          // io_L3
          for (ap_uint<2> c4 = 0; c4 <= 1; c4 += 1) {
            // io_L2
            for (ap_uint<2> c5 = 0; c5 <= 1; c5 += 1)
              for (ap_uint<3> c6 = 0; c6 <= 2; c6 += 1)
                for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
                #pragma HLS PIPELINE II=1
                  // access_coalesce
                  // access_serialize
                  {
                    w_t4 in_data;
                    w_t4 out_data;
                    in_data = fifo_w_in.read();
                    out_data = in_data;
                    fifo_w_local_out.write(out_data);
                  }
                }
          }
        }
}
/* Module Definition */

/* Module Definition */
void w_IO_L3_in_serialize(w_t16 *w, hls::stream<w_t4> &fifo_w_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  w_t4 fifo_data;
  w_t16 mem_data;
  for (ap_uint<27> i = 0; i < 36126720; i++) {
  #pragma HLS PIPELINE II=1
    mem_data = w[i];
    for (ap_uint<3> p = 0; p < 4; p++) {
      fifo_data = mem_data(127, 0);
      mem_data = mem_data >> 128;
      fifo_w_local_out.write(fifo_data);
    }
  }
}
/* Module Definition */

/* Module Definition */
void w_IO_L2_in_intra_trans(int idx, int c0, int c1, int c2, int c3, w_t4 local_w[2][3][3][1], hls::stream<float> &fifo_w_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L2
  // io_L1
  // pe
  for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
    for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1)
      for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1)
        for (ap_uint<3> c9 = 0; c9 <= 2; c9 += 1) {
          // latency
          for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
            // latency
            for (ap_uint<2> c11 = 0; c11 <= 1; c11 += 1) {
              // latency
              for (ap_uint<2> c12 = 0; c12 <= 1; c12 += 1) {
              #pragma HLS PIPELINE II=1
                {
                  w_t4 in_data;
                  w_t1 out_data;
                  in_data = local_w[c12][c8][c9][c7 / 4];
                  for (ap_uint<3> n = 0; n < 4; n++) {
                  #pragma HLS UNROLL
                    data_split[n] = in_data(31, 0);
                    in_data = in_data >> 32;
                  }
                  int split_idx = (c7) % 4;
                  union {unsigned int ui; float ut;} u;
                  u.ui = (unsigned int)data_split[split_idx];
                  out_data = u.ut;
                  fifo_w_local_out.write(out_data);
                }
              }
            }
          }
        }
}
/* Module Definition */

/* Module Definition */
void w_IO_L2_in_inter_trans(int idx, int c0, int c1, int c2, int c3, w_t4 local_w[2][3][3][1], hls::stream<w_t4> &fifo_w_in, hls::stream<w_t4> &fifo_w_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c4 = p0; c4 <= 1; c4 += 1) {
    // io_L2
    if (c4 == p0) {
      for (ap_uint<2> c5 = 0; c5 <= 1; c5 += 1)
        for (ap_uint<3> c6 = 0; c6 <= 2; c6 += 1)
          for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
          #pragma HLS PIPELINE II=1
            // access_coalesce
            {
              w_t4 in_data;
              w_t4 out_data;
              in_data = fifo_w_in.read();
              out_data = in_data;
              local_w[c5][c6][c7][0] = out_data;
            }
          }
    } else {
      for (ap_uint<2> c5 = 0; c5 <= 1; c5 += 1)
        for (ap_uint<3> c6 = 0; c6 <= 2; c6 += 1)
          for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
          #pragma HLS PIPELINE II=1
            // access_coalesce
            {
              w_t4 in_data;
              w_t4 out_data;
              in_data = fifo_w_in.read();
              out_data = in_data;
              fifo_w_out.write(out_data);
            }
          }
    }
  }
}
/* Module Definition */

/* Module Definition */
void w_IO_L2_in_inter_trans_boundary(int idx, int c0, int c1, int c2, int c3, w_t4 local_w[2][3][3][1], hls::stream<w_t4> &fifo_w_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c4 = p0; c4 <= 1; c4 += 1)
    if (c4 == p0) {
      // io_L2
      for (ap_uint<2> c5 = 0; c5 <= 1; c5 += 1)
        for (ap_uint<3> c6 = 0; c6 <= 2; c6 += 1)
          for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
          #pragma HLS PIPELINE II=1
            // access_coalesce
            {
              w_t4 in_data;
              w_t4 out_data;
              in_data = fifo_w_in.read();
              out_data = in_data;
              local_w[c5][c6][c7][0] = out_data;
            }
          }
    }
}
/* Module Definition */

/* Module Definition */
void w_IO_L2_in(int idx, hls::stream<w_t4> &fifo_w_in, hls::stream<w_t4> &fifo_w_out, hls::stream<float> &fifo_w_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  w_t4 local_w_ping[2][3][3][1];
  #pragma HLS RESOURCE variable=local_w_ping core=RAM_2P_BRAM
  w_t4 local_w_pong[2][3][3][1];
  #pragma HLS RESOURCE variable=local_w_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c0, c0_prev;
  int c1, c1_prev;
  int c2, c2_prev;
  int c3, c3_prev;
  /* Variable Declaration */

  {
    for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
      for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
        for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1)
          for (ap_uint<8> c3 = 0; c3 <= 127; c3 += 1) {
            // array
            // io_L3
            {
              if (arb == 0) {
                w_IO_L2_in_inter_trans(
                  /* module id */ idx, 
                  /* host iter */ c0, 
                  /* host iter */ c1, 
                  /* host iter */ c2, 
                  /* host iter */ c3, 
                  /* array */ local_w_pong, 
                  /* fifo */ fifo_w_in, 
                  /* fifo */ fifo_w_out, 
                  /* enable */ inter_trans_en
                );
                w_IO_L2_in_intra_trans(
                  /* module id */ idx, 
                  /* host iter */ c0_prev, 
                  /* host iter */ c1_prev, 
                  /* host iter */ c2_prev, 
                  /* host iter */ c3_prev, 
                  /* array */ local_w_ping, 
                  /* fifo */ fifo_w_local_out, 
                  /* enable */ intra_trans_en
                );
              } else {
                w_IO_L2_in_inter_trans(
                  /* module id */ idx, 
                  /* host iter */ c0, 
                  /* host iter */ c1, 
                  /* host iter */ c2, 
                  /* host iter */ c3, 
                  /* array */ local_w_ping, 
                  /* fifo */ fifo_w_in, 
                  /* fifo */ fifo_w_out, 
                  /* enable */ inter_trans_en
                );
                w_IO_L2_in_intra_trans(
                  /* module id */ idx, 
                  /* host iter */ c0_prev, 
                  /* host iter */ c1_prev, 
                  /* host iter */ c2_prev, 
                  /* host iter */ c3_prev, 
                  /* array */ local_w_pong, 
                  /* fifo */ fifo_w_local_out, 
                  /* enable */ intra_trans_en
                );
              }
              intra_trans_en = 1;
              arb = !arb;
              c0_prev = c0;
              c1_prev = c1;
              c2_prev = c2;
              c3_prev = c3;
            }
          }
    if (arb == 0) {
      w_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* host iter */ c3_prev, 
        /* array */ local_w_ping, 
        /* fifo */ fifo_w_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      w_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* host iter */ c3_prev, 
        /* array */ local_w_pong, 
        /* fifo */ fifo_w_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void w_IO_L2_in_boundary(int idx, hls::stream<w_t4> &fifo_w_in, hls::stream<float> &fifo_w_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  w_t4 local_w_ping[2][3][3][1];
  #pragma HLS RESOURCE variable=local_w_ping core=RAM_2P_BRAM
  w_t4 local_w_pong[2][3][3][1];
  #pragma HLS RESOURCE variable=local_w_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c0, c0_prev;
  int c1, c1_prev;
  int c2, c2_prev;
  int c3, c3_prev;
  /* Variable Declaration */

  {
    for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
      for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
        for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1)
          for (ap_uint<8> c3 = 0; c3 <= 127; c3 += 1) {
            // array
            // io_L3
            {
              if (arb == 0) {
                w_IO_L2_in_inter_trans_boundary(
                  /* module id */ idx, 
                  /* host iter */ c0, 
                  /* host iter */ c1, 
                  /* host iter */ c2, 
                  /* host iter */ c3, 
                  /* array */ local_w_pong, 
                  /* fifo */ fifo_w_in, 
                  /* enable */ inter_trans_en
                );
                w_IO_L2_in_intra_trans(
                  /* module id */ idx, 
                  /* host iter */ c0_prev, 
                  /* host iter */ c1_prev, 
                  /* host iter */ c2_prev, 
                  /* host iter */ c3_prev, 
                  /* array */ local_w_ping, 
                  /* fifo */ fifo_w_local_out, 
                  /* enable */ intra_trans_en
                );
              } else {
                w_IO_L2_in_inter_trans_boundary(
                  /* module id */ idx, 
                  /* host iter */ c0, 
                  /* host iter */ c1, 
                  /* host iter */ c2, 
                  /* host iter */ c3, 
                  /* array */ local_w_ping, 
                  /* fifo */ fifo_w_in, 
                  /* enable */ inter_trans_en
                );
                w_IO_L2_in_intra_trans(
                  /* module id */ idx, 
                  /* host iter */ c0_prev, 
                  /* host iter */ c1_prev, 
                  /* host iter */ c2_prev, 
                  /* host iter */ c3_prev, 
                  /* array */ local_w_pong, 
                  /* fifo */ fifo_w_local_out, 
                  /* enable */ intra_trans_en
                );
              }
              intra_trans_en = 1;
              arb = !arb;
              c0_prev = c0;
              c1_prev = c1;
              c2_prev = c2;
              c3_prev = c3;
            }
          }
    if (arb == 0) {
      w_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* host iter */ c3_prev, 
        /* array */ local_w_ping, 
        /* fifo */ fifo_w_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      w_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* host iter */ c3_prev, 
        /* array */ local_w_pong, 
        /* fifo */ fifo_w_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void PE(int idx, int idy, hls::stream<float> &fifo_cin_in, hls::stream<float> &fifo_cin_out, hls::stream<float> &fifo_cout_drain_out, hls::stream<float> &fifo_w_in, hls::stream<float> &fifo_w_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  cin_t1 local_cin[1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_cin dim=0 complete
  cout_t1 local_cout[2][4][2];
  #pragma HLS RESOURCE variable=local_cout core=RAM_2P_BRAM
  w_t1 local_w[1][1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_w dim=0 complete
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1)
        for (ap_uint<8> c3 = 0; c3 <= 127; c3 += 1) {
          // array
          // pe
          for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1) {
            if (c3 == 0) {
              // latency
              for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
                // latency
                for (ap_uint<2> c9 = 0; c9 <= 1; c9 += 1) {
                  // latency
                  for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
                  #pragma HLS PIPELINE II=1
                    local_cout[c9][2*c6 + c8][c10] = 0;
                  }
                }
              }
            }
            for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1)
              for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1)
                for (ap_uint<3> c9 = 0; c9 <= 2; c9 += 1) {
                  // latency
                  for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
                    // latency
                    for (ap_uint<2> c11 = 0; c11 <= 1; c11 += 1) {
                      // latency
                      for (ap_uint<2> c12 = 0; c12 <= 1; c12 += 1) {
                      #pragma HLS PIPELINE II=1
                        {
                          local_cin[0][0][0] = fifo_cin_in.read();
                          local_w[0][0][0][0] = fifo_w_in.read();
                          local_cout[c11][c10 + 2*c6][c12] = (local_cout[c11][c10 + 2*c6][c12] + (local_cin[0][0][0] * local_w[0][0][0][0]));
                          fifo_w_out.write(local_w[0][0][0][0]);
                          if (c3 == 127 && c7 == 3 && c8 == 2 && c9 == 2)
                            fifo_cout_drain_out.write(local_cout[c11][c10 + 2*c6][c12]);
                          fifo_cin_out.write(local_cin[0][0][0]);
                        }
                      }
                    }
                  }
                }
          }
        }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, int idy, hls::stream<float> &fifo_cin_in, hls::stream<float> &fifo_cin_out, hls::stream<float> &fifo_cout_drain_out, hls::stream<float> &fifo_w_in, hls::stream<float> &fifo_w_out)
 {
  PE(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_cin_in, 
    /* fifo */ fifo_cin_out, 
    /* fifo */ fifo_cout_drain_out, 
    /* fifo */ fifo_w_in, 
    /* fifo */ fifo_w_out);
}
/* Module Definition */

/* Module Definition */
void cin_PE_dummy_in(int idx, int idy, hls::stream<float> &fifo_cin_in) {
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1) {
        // array
        {
        }
        for (ap_uint<8> c3 = 0; c3 <= 127; c3 += 1) {
          // array
          // pe
          for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
            for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1)
              for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1)
                for (ap_uint<3> c9 = 0; c9 <= 2; c9 += 1) {
                  // latency
                  for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
                    // latency
                    for (ap_uint<2> c11 = 0; c11 <= 1; c11 += 1) {
                      // latency
                      for (ap_uint<2> c12 = 0; c12 <= 1; c12 += 1) {
                      #pragma HLS PIPELINE II=1
                        cin_t1 fifo_data;
                        fifo_data = fifo_cin_in.read();
                      }
                    }
                  }
                }
        }
      }
}
/* Module Definition */

/* Module Definition */
void w_PE_dummy_in(int idx, int idy, hls::stream<float> &fifo_w_in) {
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1) {
        // array
        {
        }
        for (ap_uint<8> c3 = 0; c3 <= 127; c3 += 1) {
          // array
          // pe
          for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
            for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1)
              for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1)
                for (ap_uint<3> c9 = 0; c9 <= 2; c9 += 1) {
                  // latency
                  for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
                    // latency
                    for (ap_uint<2> c11 = 0; c11 <= 1; c11 += 1) {
                      // latency
                      for (ap_uint<2> c12 = 0; c12 <= 1; c12 += 1) {
                      #pragma HLS PIPELINE II=1
                        w_t1 fifo_data;
                        fifo_data = fifo_w_in.read();
                      }
                    }
                  }
                }
        }
      }
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L1_out_intra_trans(int idx, int idy, int c0, int c1, int c2, cout_t2 local_cout[2][4][1], hls::stream<float> &fifo_cout_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  ap_uint<32> data_split[2];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */


  // io_L1
  // pe
  for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1) {
    // latency
    for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
      // latency
      for (ap_uint<2> c11 = 0; c11 <= 1; c11 += 1) {
        // latency
        for (ap_uint<2> c12 = 0; c12 <= 1; c12 += 1) {
        #pragma HLS PIPELINE II=1
          {
            cout_t1 in_data;
            cout_t2 out_data;
            in_data = fifo_cout_drain_local_in.read();
            int split_idx = (c12) % 2;
            out_data = local_cout[c11][c10 + 2*c6][c12 / 2];
            for (ap_uint<2> n = 0; n < 2; n++) {
            #pragma HLS UNROLL
              data_split[n] = out_data(31, 0);
              out_data = out_data >> 32;
            }
            union {unsigned int ui; float ut;} u;
            u.ut = in_data;
            data_split[split_idx] = ap_uint<32>(u.ui);
            out_data = (data_split[1], data_split[0]);            local_cout[c11][c10 + 2*c6][c12 / 2] = out_data;
          }
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L1_out_inter_trans(int idx, int idy, int c0, int c1, int c2, cout_t2 local_cout[2][4][1], hls::stream<cout_t2> &fifo_cout_drain_in, hls::stream<cout_t2> &fifo_cout_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<2> c5 = p1; c5 <= 1; c5 += 1) {
    // io_L1
    if (c5 == p1) {
      for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            cout_t2 in_data;
            cout_t2 out_data;
            in_data = local_cout[c6][c7][0];
            out_data = in_data;
            fifo_cout_drain_out.write(out_data);
          }
        }
    } else {
      for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            cout_t2 in_data;
            cout_t2 out_data;
            in_data = fifo_cout_drain_in.read();
            out_data = in_data;
            fifo_cout_drain_out.write(out_data);
          }
        }
    }
  }
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L1_out_inter_trans_boundary(int idx, int idy, int c0, int c1, int c2, cout_t2 local_cout[2][4][1], hls::stream<cout_t2> &fifo_cout_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<2> c5 = p1; c5 <= 1; c5 += 1)
    if (c5 == p1) {
      // io_L1
      for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            cout_t2 in_data;
            cout_t2 out_data;
            in_data = local_cout[c6][c7][0];
            out_data = in_data;
            fifo_cout_drain_out.write(out_data);
          }
        }
    }
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L1_out(int idx, int idy, hls::stream<cout_t2> &fifo_cout_drain_in, hls::stream<cout_t2> &fifo_cout_drain_out, hls::stream<float> &fifo_cout_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  cout_t2 local_cout[2][4][1];
  #pragma HLS RESOURCE variable=local_cout core=RAM_2P_BRAM
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1) {
        // array
        // io_L3
        // io_L2
        cout_drain_IO_L1_out_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* host iter */ c0, 
          /* host iter */ c1, 
          /* host iter */ c2, 
          /* array */ local_cout, 
          /* fifo */ fifo_cout_drain_local_in
        );
        cout_drain_IO_L1_out_inter_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* host iter */ c0, 
          /* host iter */ c1, 
          /* host iter */ c2, 
          /* array */ local_cout, 
          /* fifo */ fifo_cout_drain_in, 
          /* fifo */ fifo_cout_drain_out
        );
      }
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L1_out_wrapper(int idx, int idy, hls::stream<cout_t2> &fifo_cout_drain_in, hls::stream<cout_t2> &fifo_cout_drain_out, hls::stream<float> &fifo_cout_drain_local_in)
 {
  cout_drain_IO_L1_out(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_cout_drain_in, 
    /* fifo */ fifo_cout_drain_out, 
    /* fifo */ fifo_cout_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L1_out_boundary(int idx, int idy, hls::stream<cout_t2> &fifo_cout_drain_out, hls::stream<float> &fifo_cout_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  cout_t2 local_cout[2][4][1];
  #pragma HLS RESOURCE variable=local_cout core=RAM_2P_BRAM
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1) {
        // array
        // io_L3
        // io_L2
        cout_drain_IO_L1_out_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* host iter */ c0, 
          /* host iter */ c1, 
          /* host iter */ c2, 
          /* array */ local_cout, 
          /* fifo */ fifo_cout_drain_local_in
        );
        cout_drain_IO_L1_out_inter_trans_boundary(
          /* module id */ idx, 
          /* module id */ idy, 
          /* host iter */ c0, 
          /* host iter */ c1, 
          /* host iter */ c2, 
          /* array */ local_cout, 
          /* fifo */ fifo_cout_drain_out
        );
      }
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L1_out_boundary_wrapper(int idx, int idy, hls::stream<cout_t2> &fifo_cout_drain_out, hls::stream<float> &fifo_cout_drain_local_in)
 {
  cout_drain_IO_L1_out_boundary(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_cout_drain_out, 
    /* fifo */ fifo_cout_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L2_out(int idx, hls::stream<cout_t2> &fifo_cout_drain_in, hls::stream<cout_t2> &fifo_cout_drain_out, hls::stream<cout_t2> &fifo_cout_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1) {
        // array
        // io_L3
        for (ap_uint<2> c4 = p0; c4 <= 1; c4 += 1) {
          // io_L2
          if (c4 == p0) {
            for (ap_uint<2> c5 = 0; c5 <= 1; c5 += 1) {
              // io_L1
              for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
                for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
                #pragma HLS PIPELINE II=1
                  // access_coalesce
                  {
                    cout_t2 in_data;
                    cout_t2 out_data;
                    in_data = fifo_cout_drain_local_in.read();
                    out_data = in_data;
                    fifo_cout_drain_out.write(out_data);
                  }
                }
            }
          } else {
            for (ap_uint<2> c5 = 0; c5 <= 1; c5 += 1) {
              // io_L1
              for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
                for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
                #pragma HLS PIPELINE II=1
                  // access_coalesce
                  {
                    cout_t2 in_data;
                    cout_t2 out_data;
                    in_data = fifo_cout_drain_in.read();
                    out_data = in_data;
                    fifo_cout_drain_out.write(out_data);
                  }
                }
            }
          }
        }
      }
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L2_out_boundary(int idx, hls::stream<cout_t2> &fifo_cout_drain_out, hls::stream<cout_t2> &fifo_cout_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1) {
        // array
        // io_L3
        for (ap_uint<2> c4 = p0; c4 <= 1; c4 += 1)
          if (c4 == p0) {
            // io_L2
            for (ap_uint<2> c5 = 0; c5 <= 1; c5 += 1) {
              // io_L1
              for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
                for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
                #pragma HLS PIPELINE II=1
                  // access_coalesce
                  {
                    cout_t2 in_data;
                    cout_t2 out_data;
                    in_data = fifo_cout_drain_local_in.read();
                    out_data = in_data;
                    fifo_cout_drain_out.write(out_data);
                  }
                }
            }
          }
      }
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L3_out(hls::stream<cout_t2> &fifo_cout_drain_out, hls::stream<cout_t2> &fifo_cout_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  for (ap_uint<9> c0 = 0; c0 <= 159; c0 += 1)
    for (ap_uint<5> c1 = 0; c1 <= 13; c1 += 1)
      for (ap_uint<5> c2 = 0; c2 <= 13; c2 += 1) {
        // array
        // io_L3
        for (ap_uint<2> c4 = 0; c4 <= 1; c4 += 1) {
          // io_L2
          for (ap_uint<2> c5 = 0; c5 <= 1; c5 += 1) {
            // io_L1
            for (ap_uint<2> c6 = 0; c6 <= 1; c6 += 1)
              for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
              #pragma HLS PIPELINE II=1
                // access_coalesce
                // access_serialize
                {
                  cout_t2 in_data;
                  cout_t2 out_data;
                  in_data = fifo_cout_drain_local_in.read();
                  out_data = in_data;
                  fifo_cout_drain_out.write(out_data);
                }
              }
          }
        }
      }
}
/* Module Definition */

/* Module Definition */
void cout_drain_IO_L3_out_serialize(cout_t16 *cout, hls::stream<cout_t2> &fifo_cout_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  for (ap_uint<18> i = 0; i < 125440; i++) {
  #pragma HLS PIPELINE II=1
    cout_t2 fifo_data;
    cout_t16 mem_data;
    cout_t2 mem_data_split[8];
    #pragma HLS ARRAY_PARTITION variable=mem_data_split complete
    for (ap_uint<4> p = 0; p < 8; p++) {
      fifo_data = fifo_cout_drain_local_in.read();
      mem_data_split[p] = fifo_data;
    }
    mem_data = (mem_data_split[7], mem_data_split[6], mem_data_split[5], mem_data_split[4], mem_data_split[3], mem_data_split[2], mem_data_split[1], mem_data_split[0]);
    cout[i] = mem_data;
  }
}
/* Module Definition */

void kernel0(cin_t16 *cin, cout_t16 *cout, w_t16 *w)
{
#pragma HLS INTERFACE m_axi port=cin offset=slave bundle=gmem_cin
#pragma HLS INTERFACE m_axi port=cout offset=slave bundle=gmem_cout
#pragma HLS INTERFACE m_axi port=w offset=slave bundle=gmem_w
#pragma HLS INTERFACE s_axilite port=cin bundle=control
#pragma HLS INTERFACE s_axilite port=cout bundle=control
#pragma HLS INTERFACE s_axilite port=w bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATAFLOW

  /* FIFO Declaration */
  /* cin_IO_L3_in_serialize fifo */ hls::stream<cin_t4> fifo_cin_cin_IO_L3_in_serialize;
  #pragma HLS STREAM variable=fifo_cin_cin_IO_L3_in_serialize depth=2
  /* w_IO_L3_in_serialize fifo */ hls::stream<w_t4> fifo_w_w_IO_L3_in_serialize;
  #pragma HLS STREAM variable=fifo_w_w_IO_L3_in_serialize depth=2
  /* cout_drain_IO_L3_out_serialize fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L3_out_serialize;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L3_out_serialize depth=2
  /* cin_IO_L2_in fifo */ hls::stream<cin_t4> fifo_cin_cin_IO_L2_in_0;
  #pragma HLS STREAM variable=fifo_cin_cin_IO_L2_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_cin_cin_IO_L2_in_0 core=FIFO_SRL
  /* cin_IO_L2_in fifo */ hls::stream<cin_t4> fifo_cin_cin_IO_L2_in_1;
  #pragma HLS STREAM variable=fifo_cin_cin_IO_L2_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_cin_cin_IO_L2_in_1 core=FIFO_SRL
  /* cin_IO_L2_in fifo */ hls::stream<cin_t4> fifo_cin_cin_IO_L2_in_2;
  #pragma HLS STREAM variable=fifo_cin_cin_IO_L2_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_cin_cin_IO_L2_in_2 core=FIFO_SRL
  /* w_IO_L2_in fifo */ hls::stream<w_t4> fifo_w_w_IO_L2_in_0;
  #pragma HLS STREAM variable=fifo_w_w_IO_L2_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_w_w_IO_L2_in_0 core=FIFO_SRL
  /* w_IO_L2_in fifo */ hls::stream<w_t4> fifo_w_w_IO_L2_in_1;
  #pragma HLS STREAM variable=fifo_w_w_IO_L2_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_w_w_IO_L2_in_1 core=FIFO_SRL
  /* w_IO_L2_in fifo */ hls::stream<w_t4> fifo_w_w_IO_L2_in_2;
  #pragma HLS STREAM variable=fifo_w_w_IO_L2_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_w_w_IO_L2_in_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cin_PE_0_0;
  #pragma HLS STREAM variable=fifo_cin_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_cin_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cin_PE_1_0;
  #pragma HLS STREAM variable=fifo_cin_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_cin_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cin_PE_2_0;
  #pragma HLS STREAM variable=fifo_cin_PE_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_cin_PE_2_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cin_PE_0_1;
  #pragma HLS STREAM variable=fifo_cin_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_cin_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cin_PE_1_1;
  #pragma HLS STREAM variable=fifo_cin_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_cin_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cin_PE_2_1;
  #pragma HLS STREAM variable=fifo_cin_PE_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_cin_PE_2_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cout_drain_PE_0_0;
  #pragma HLS STREAM variable=fifo_cout_drain_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cout_drain_PE_1_0;
  #pragma HLS STREAM variable=fifo_cout_drain_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cout_drain_PE_0_1;
  #pragma HLS STREAM variable=fifo_cout_drain_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_cout_drain_PE_1_1;
  #pragma HLS STREAM variable=fifo_cout_drain_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_w_PE_0_0;
  #pragma HLS STREAM variable=fifo_w_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_w_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_w_PE_0_1;
  #pragma HLS STREAM variable=fifo_w_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_w_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_w_PE_0_2;
  #pragma HLS STREAM variable=fifo_w_PE_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_w_PE_0_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_w_PE_1_0;
  #pragma HLS STREAM variable=fifo_w_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_w_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_w_PE_1_1;
  #pragma HLS STREAM variable=fifo_w_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_w_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_w_PE_1_2;
  #pragma HLS STREAM variable=fifo_w_PE_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_w_PE_1_2 core=FIFO_SRL
  /* cout_drain_IO_L1_out fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L1_out_0_0;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L1_out_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_cout_drain_IO_L1_out_0_0 core=FIFO_SRL
  /* cout_drain_IO_L1_out fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L1_out_0_1;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L1_out_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_cout_drain_IO_L1_out_0_1 core=FIFO_SRL
  /* cout_drain_IO_L1_out fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L1_out_0_2;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L1_out_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_cout_drain_IO_L1_out_0_2 core=FIFO_SRL
  /* cout_drain_IO_L1_out fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L1_out_1_0;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L1_out_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_cout_drain_IO_L1_out_1_0 core=FIFO_SRL
  /* cout_drain_IO_L1_out fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L1_out_1_1;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L1_out_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_cout_drain_IO_L1_out_1_1 core=FIFO_SRL
  /* cout_drain_IO_L1_out fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L1_out_1_2;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L1_out_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_cout_drain_IO_L1_out_1_2 core=FIFO_SRL
  /* cout_drain_IO_L2_out fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L2_out_0;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L2_out_0 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_cout_drain_IO_L2_out_0 core=FIFO_SRL
  /* cout_drain_IO_L2_out fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L2_out_1;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L2_out_1 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_cout_drain_IO_L2_out_1 core=FIFO_SRL
  /* cout_drain_IO_L2_out fifo */ hls::stream<cout_t2> fifo_cout_drain_cout_drain_IO_L2_out_2;
  #pragma HLS STREAM variable=fifo_cout_drain_cout_drain_IO_L2_out_2 depth=2
  #pragma HLS RESOURCE variable=fifo_cout_drain_cout_drain_IO_L2_out_2 core=FIFO_SRL
  /* FIFO Declaration */

  /* Module Call */
  cin_IO_L3_in_serialize(
    /* array */ cin,
    /* fifo */ fifo_cin_cin_IO_L3_in_serialize
  );
  /* Module Call */

  /* Module Call */
  cin_IO_L3_in(
    /* fifo */ fifo_cin_cin_IO_L3_in_serialize,
    /* fifo */ fifo_cin_cin_IO_L2_in_0
  );
  /* Module Call */

  /* Module Call */
  cin_IO_L2_in(
    /* module id */ 0,
    /* fifo */ fifo_cin_cin_IO_L2_in_0,
    /* fifo */ fifo_cin_cin_IO_L2_in_1,
    /* fifo */ fifo_cin_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  cin_IO_L2_in_boundary(
    /* module id */ 1,
    /* fifo */ fifo_cin_cin_IO_L2_in_1,
    /* fifo */ fifo_cin_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  w_IO_L3_in_serialize(
    /* array */ w,
    /* fifo */ fifo_w_w_IO_L3_in_serialize
  );
  /* Module Call */

  /* Module Call */
  w_IO_L3_in(
    /* fifo */ fifo_w_w_IO_L3_in_serialize,
    /* fifo */ fifo_w_w_IO_L2_in_0
  );
  /* Module Call */

  /* Module Call */
  w_IO_L2_in(
    /* module id */ 0,
    /* fifo */ fifo_w_w_IO_L2_in_0,
    /* fifo */ fifo_w_w_IO_L2_in_1,
    /* fifo */ fifo_w_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  w_IO_L2_in_boundary(
    /* module id */ 1,
    /* fifo */ fifo_w_w_IO_L2_in_1,
    /* fifo */ fifo_w_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_cin_PE_0_0,
    /* fifo */ fifo_cin_PE_1_0,
    /* fifo */ fifo_cout_drain_PE_0_0,
    /* fifo */ fifo_w_PE_0_0,
    /* fifo */ fifo_w_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_cin_PE_0_1,
    /* fifo */ fifo_cin_PE_1_1,
    /* fifo */ fifo_cout_drain_PE_0_1,
    /* fifo */ fifo_w_PE_0_1,
    /* fifo */ fifo_w_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_cin_PE_1_0,
    /* fifo */ fifo_cin_PE_2_0,
    /* fifo */ fifo_cout_drain_PE_1_0,
    /* fifo */ fifo_w_PE_1_0,
    /* fifo */ fifo_w_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_cin_PE_1_1,
    /* fifo */ fifo_cin_PE_2_1,
    /* fifo */ fifo_cout_drain_PE_1_1,
    /* fifo */ fifo_w_PE_1_1,
    /* fifo */ fifo_w_PE_1_2
  );
  /* Module Call */

  /* Module Call */
  cin_PE_dummy_in(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_cin_PE_2_0
  );
  /* Module Call */

  /* Module Call */
  cin_PE_dummy_in(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_cin_PE_2_1
  );
  /* Module Call */

  /* Module Call */
  w_PE_dummy_in(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_w_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  w_PE_dummy_in(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_w_PE_1_2
  );
  /* Module Call */

  /* Module Call */
  cout_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L1_out_0_1,
    /* fifo */ fifo_cout_drain_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  cout_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L1_out_0_1,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L1_out_0_0,
    /* fifo */ fifo_cout_drain_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  cout_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L1_out_1_1,
    /* fifo */ fifo_cout_drain_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  cout_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L1_out_1_1,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L1_out_1_0,
    /* fifo */ fifo_cout_drain_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  cout_drain_IO_L2_out_boundary(
    /* module id */ 1,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L2_out_1,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L1_out_1_0
  );
  /* Module Call */

  /* Module Call */
  cout_drain_IO_L2_out(
    /* module id */ 0,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L2_out_1,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L2_out_0,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L1_out_0_0
  );
  /* Module Call */

  /* Module Call */
  cout_drain_IO_L3_out(
    /* fifo */ fifo_cout_drain_cout_drain_IO_L3_out_serialize,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L2_out_0
  );
  /* Module Call */

  /* Module Call */
  cout_drain_IO_L3_out_serialize(
    /* array */ cout,
    /* fifo */ fifo_cout_drain_cout_drain_IO_L3_out_serialize
  );
  /* Module Call */

}
