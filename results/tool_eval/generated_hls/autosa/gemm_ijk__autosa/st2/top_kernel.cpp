#include "top_kernel.h"
/* Module Definition */
void BRAM_A_IO_L2_in(BRAM_A_t1 *BRAM_A, hls::stream<float> &fifo_BRAM_A_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
      for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
        // array
        // io_L2
        for (ap_uint<5> c3 = 0; c3 <= 15; c3 += 1) {
          // io_L1
          for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
          #pragma HLS PIPELINE II=1
            // access_coalesce
            {
              BRAM_A_t1 in_data;
              BRAM_A_t1 out_data;
              in_data = BRAM_A[1024*c0 + 16*c2 + c3 + 64*c4];
              out_data = in_data;
              fifo_BRAM_A_local_out.write(out_data);
            }
          }
        }
      }
}
/* Module Definition */

/* Module Definition */
void BRAM_A_IO_L1_in_intra_trans(int idx, int c0, int c1, int c2, float local_BRAM_A[16][1], hls::stream<float> &fifo_BRAM_A_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L1
  // pe
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1)
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // latency
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
        // latency
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            BRAM_A_t1 in_data;
            BRAM_A_t1 out_data;
            in_data = local_BRAM_A[4*c4 + c7][0];
            out_data = in_data;
            fifo_BRAM_A_local_out.write(out_data);
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_A_IO_L1_in_inter_trans(int idx, int c0, int c1, int c2, float local_BRAM_A[16][1], hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c3 = p0; c3 <= 15; c3 += 1) {
    // io_L1
    if (c3 == p0) {
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          BRAM_A_t1 in_data;
          BRAM_A_t1 out_data;
          in_data = fifo_BRAM_A_in.read();
          out_data = in_data;
          local_BRAM_A[c4][0] = out_data;
        }
      }
    } else {
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          BRAM_A_t1 in_data;
          BRAM_A_t1 out_data;
          in_data = fifo_BRAM_A_in.read();
          out_data = in_data;
          fifo_BRAM_A_out.write(out_data);
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_A_IO_L1_in_inter_trans_boundary(int idx, int c0, int c1, int c2, float local_BRAM_A[16][1], hls::stream<float> &fifo_BRAM_A_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c3 = p0; c3 <= 15; c3 += 1)
    if (c3 == p0) {
      // io_L1
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          BRAM_A_t1 in_data;
          BRAM_A_t1 out_data;
          in_data = fifo_BRAM_A_in.read();
          out_data = in_data;
          local_BRAM_A[c4][0] = out_data;
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_A_IO_L1_in(int idx, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_out, hls::stream<float> &fifo_BRAM_A_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  BRAM_A_t1 local_BRAM_A_ping[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_A_ping core=RAM_1P_BRAM
  float local_BRAM_A_pong[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_A_pong core=RAM_1P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c0, c0_prev;
  int c1, c1_prev;
  int c2, c2_prev;
  /* Variable Declaration */

  {
    for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
      for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
        for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
          // array
          // io_L2
          {
            if (arb == 0) {
              BRAM_A_IO_L1_in_inter_trans(
                /* module id */ idx, 
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ local_BRAM_A_pong, 
                /* fifo */ fifo_BRAM_A_in, 
                /* fifo */ fifo_BRAM_A_out, 
                /* enable */ inter_trans_en
              );
              BRAM_A_IO_L1_in_intra_trans(
                /* module id */ idx, 
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ local_BRAM_A_ping, 
                /* fifo */ fifo_BRAM_A_local_out, 
                /* enable */ intra_trans_en
              );
            } else {
              BRAM_A_IO_L1_in_inter_trans(
                /* module id */ idx, 
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ local_BRAM_A_ping, 
                /* fifo */ fifo_BRAM_A_in, 
                /* fifo */ fifo_BRAM_A_out, 
                /* enable */ inter_trans_en
              );
              BRAM_A_IO_L1_in_intra_trans(
                /* module id */ idx, 
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ local_BRAM_A_pong, 
                /* fifo */ fifo_BRAM_A_local_out, 
                /* enable */ intra_trans_en
              );
            }
            intra_trans_en = 1;
            arb = !arb;
            c0_prev = c0;
            c1_prev = c1;
            c2_prev = c2;
          }
        }
    if (arb == 0) {
      BRAM_A_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ local_BRAM_A_ping, 
        /* fifo */ fifo_BRAM_A_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      BRAM_A_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ local_BRAM_A_pong, 
        /* fifo */ fifo_BRAM_A_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_A_IO_L1_in_wrapper(int idx, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_out, hls::stream<float> &fifo_BRAM_A_local_out)
 {
  BRAM_A_IO_L1_in(
    /* module id */ idx, 
    /* fifo */ fifo_BRAM_A_in, 
    /* fifo */ fifo_BRAM_A_out, 
    /* fifo */ fifo_BRAM_A_local_out);
}
/* Module Definition */

/* Module Definition */
void BRAM_A_IO_L1_in_boundary(int idx, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_local_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  BRAM_A_t1 local_BRAM_A_ping[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_A_ping core=RAM_1P_BRAM
  float local_BRAM_A_pong[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_A_pong core=RAM_1P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c0, c0_prev;
  int c1, c1_prev;
  int c2, c2_prev;
  /* Variable Declaration */

  {
    for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
      for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
        for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
          // array
          // io_L2
          {
            if (arb == 0) {
              BRAM_A_IO_L1_in_inter_trans_boundary(
                /* module id */ idx, 
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ local_BRAM_A_pong, 
                /* fifo */ fifo_BRAM_A_in, 
                /* enable */ inter_trans_en
              );
              BRAM_A_IO_L1_in_intra_trans(
                /* module id */ idx, 
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ local_BRAM_A_ping, 
                /* fifo */ fifo_BRAM_A_local_out, 
                /* enable */ intra_trans_en
              );
            } else {
              BRAM_A_IO_L1_in_inter_trans_boundary(
                /* module id */ idx, 
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ local_BRAM_A_ping, 
                /* fifo */ fifo_BRAM_A_in, 
                /* enable */ inter_trans_en
              );
              BRAM_A_IO_L1_in_intra_trans(
                /* module id */ idx, 
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ local_BRAM_A_pong, 
                /* fifo */ fifo_BRAM_A_local_out, 
                /* enable */ intra_trans_en
              );
            }
            intra_trans_en = 1;
            arb = !arb;
            c0_prev = c0;
            c1_prev = c1;
            c2_prev = c2;
          }
        }
    if (arb == 0) {
      BRAM_A_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ local_BRAM_A_ping, 
        /* fifo */ fifo_BRAM_A_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      BRAM_A_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ local_BRAM_A_pong, 
        /* fifo */ fifo_BRAM_A_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_A_IO_L1_in_boundary_wrapper(int idx, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_local_out)
 {
  BRAM_A_IO_L1_in_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_BRAM_A_in, 
    /* fifo */ fifo_BRAM_A_local_out);
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L2_in(BRAM_B_t16 *BRAM_B, hls::stream<BRAM_B_t4> &fifo_BRAM_B_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  BRAM_B_t4 data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
      for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
        // array
        // io_L2
        for (ap_uint<5> c3 = 0; c3 <= 15; c3 += 1) {
          // io_L1
          // access_coalesce
          for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
          #pragma HLS PIPELINE II=1
            {
              BRAM_B_t16 in_data;
              BRAM_B_t4 out_data;
              if (c5 % 4 == 0) {
                in_data = BRAM_B[((16 * c2 + c3) * 64 + (16 * c1 + 4 * c5)) / 16];
                for (ap_uint<3> n = 0; n < 4; n++) {
                #pragma HLS UNROLL
                  data_split[n] = in_data(127, 0);
                  in_data = in_data >> 128;
                }
              }
              int split_idx = (4*c1 + 256*c2 + 16*c3 + c5) % 4;
              out_data = data_split[split_idx];
              fifo_BRAM_B_local_out.write(out_data);
            }
          }
        }
      }
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L1_in_intra_trans(int idx, int c0, int c1, int c2, BRAM_B_t4 local_BRAM_B[1][4], hls::stream<float> &fifo_BRAM_B_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L1
  // pe
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1)
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // latency
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
        // latency
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            BRAM_B_t4 in_data;
            BRAM_B_t1 out_data;
            in_data = local_BRAM_B[0][(4 * c5 + c6) / 4];
            for (ap_uint<3> n = 0; n < 4; n++) {
            #pragma HLS UNROLL
              data_split[n] = in_data(31, 0);
              in_data = in_data >> 32;
            }
            int split_idx = (4*c5 + c6) % 4;
            union {unsigned int ui; float ut;} u;
            u.ui = (unsigned int)data_split[split_idx];
            out_data = u.ut;
            fifo_BRAM_B_local_out.write(out_data);
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L1_in_inter_trans(int idx, int c0, int c1, int c2, BRAM_B_t4 local_BRAM_B[1][4], hls::stream<BRAM_B_t4> &fifo_BRAM_B_in, hls::stream<BRAM_B_t4> &fifo_BRAM_B_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c3 = p0; c3 <= 15; c3 += 1) {
    // io_L1
    if (c3 == p0) {
      // access_coalesce
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      #pragma HLS PIPELINE II=1
        {
          BRAM_B_t4 in_data;
          BRAM_B_t4 out_data;
          in_data = fifo_BRAM_B_in.read();
          out_data = in_data;
          local_BRAM_B[0][c5] = out_data;
        }
      }
    } else {
      // access_coalesce
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      #pragma HLS PIPELINE II=1
        {
          BRAM_B_t4 in_data;
          BRAM_B_t4 out_data;
          in_data = fifo_BRAM_B_in.read();
          out_data = in_data;
          fifo_BRAM_B_out.write(out_data);
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L1_in_inter_trans_boundary(int idx, int c0, int c1, int c2, BRAM_B_t4 local_BRAM_B[1][4], hls::stream<BRAM_B_t4> &fifo_BRAM_B_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c3 = p0; c3 <= 15; c3 += 1)
    if (c3 == p0) {
      // io_L1
      // access_coalesce
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      #pragma HLS PIPELINE II=1
        {
          BRAM_B_t4 in_data;
          BRAM_B_t4 out_data;
          in_data = fifo_BRAM_B_in.read();
          out_data = in_data;
          local_BRAM_B[0][c5] = out_data;
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L1_in(int idx, hls::stream<BRAM_B_t4> &fifo_BRAM_B_in, hls::stream<BRAM_B_t4> &fifo_BRAM_B_out, hls::stream<float> &fifo_BRAM_B_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  BRAM_B_t4 local_BRAM_B_ping[1][4];
  #pragma HLS RESOURCE variable=local_BRAM_B_ping core=RAM_2P_BRAM
  BRAM_B_t4 local_BRAM_B_pong[1][4];
  #pragma HLS RESOURCE variable=local_BRAM_B_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c0, c0_prev;
  int c1, c1_prev;
  int c2, c2_prev;
  /* Variable Declaration */

  {
    for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
      for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
        for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
          // array
          // io_L2
          {
            if (arb == 0) {
              BRAM_B_IO_L1_in_inter_trans(
                /* module id */ idx, 
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ local_BRAM_B_pong, 
                /* fifo */ fifo_BRAM_B_in, 
                /* fifo */ fifo_BRAM_B_out, 
                /* enable */ inter_trans_en
              );
              BRAM_B_IO_L1_in_intra_trans(
                /* module id */ idx, 
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ local_BRAM_B_ping, 
                /* fifo */ fifo_BRAM_B_local_out, 
                /* enable */ intra_trans_en
              );
            } else {
              BRAM_B_IO_L1_in_inter_trans(
                /* module id */ idx, 
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ local_BRAM_B_ping, 
                /* fifo */ fifo_BRAM_B_in, 
                /* fifo */ fifo_BRAM_B_out, 
                /* enable */ inter_trans_en
              );
              BRAM_B_IO_L1_in_intra_trans(
                /* module id */ idx, 
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ local_BRAM_B_pong, 
                /* fifo */ fifo_BRAM_B_local_out, 
                /* enable */ intra_trans_en
              );
            }
            intra_trans_en = 1;
            arb = !arb;
            c0_prev = c0;
            c1_prev = c1;
            c2_prev = c2;
          }
        }
    if (arb == 0) {
      BRAM_B_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ local_BRAM_B_ping, 
        /* fifo */ fifo_BRAM_B_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      BRAM_B_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ local_BRAM_B_pong, 
        /* fifo */ fifo_BRAM_B_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L1_in_wrapper(int idx, hls::stream<BRAM_B_t4> &fifo_BRAM_B_in, hls::stream<BRAM_B_t4> &fifo_BRAM_B_out, hls::stream<float> &fifo_BRAM_B_local_out)
 {
  BRAM_B_IO_L1_in(
    /* module id */ idx, 
    /* fifo */ fifo_BRAM_B_in, 
    /* fifo */ fifo_BRAM_B_out, 
    /* fifo */ fifo_BRAM_B_local_out);
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L1_in_boundary(int idx, hls::stream<BRAM_B_t4> &fifo_BRAM_B_in, hls::stream<float> &fifo_BRAM_B_local_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  BRAM_B_t4 local_BRAM_B_ping[1][4];
  #pragma HLS RESOURCE variable=local_BRAM_B_ping core=RAM_2P_BRAM
  BRAM_B_t4 local_BRAM_B_pong[1][4];
  #pragma HLS RESOURCE variable=local_BRAM_B_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c0, c0_prev;
  int c1, c1_prev;
  int c2, c2_prev;
  /* Variable Declaration */

  {
    for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
      for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
        for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
          // array
          // io_L2
          {
            if (arb == 0) {
              BRAM_B_IO_L1_in_inter_trans_boundary(
                /* module id */ idx, 
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ local_BRAM_B_pong, 
                /* fifo */ fifo_BRAM_B_in, 
                /* enable */ inter_trans_en
              );
              BRAM_B_IO_L1_in_intra_trans(
                /* module id */ idx, 
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ local_BRAM_B_ping, 
                /* fifo */ fifo_BRAM_B_local_out, 
                /* enable */ intra_trans_en
              );
            } else {
              BRAM_B_IO_L1_in_inter_trans_boundary(
                /* module id */ idx, 
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ local_BRAM_B_ping, 
                /* fifo */ fifo_BRAM_B_in, 
                /* enable */ inter_trans_en
              );
              BRAM_B_IO_L1_in_intra_trans(
                /* module id */ idx, 
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ local_BRAM_B_pong, 
                /* fifo */ fifo_BRAM_B_local_out, 
                /* enable */ intra_trans_en
              );
            }
            intra_trans_en = 1;
            arb = !arb;
            c0_prev = c0;
            c1_prev = c1;
            c2_prev = c2;
          }
        }
    if (arb == 0) {
      BRAM_B_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ local_BRAM_B_ping, 
        /* fifo */ fifo_BRAM_B_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      BRAM_B_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ local_BRAM_B_pong, 
        /* fifo */ fifo_BRAM_B_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L1_in_boundary_wrapper(int idx, hls::stream<BRAM_B_t4> &fifo_BRAM_B_in, hls::stream<float> &fifo_BRAM_B_local_out)
 {
  BRAM_B_IO_L1_in_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_BRAM_B_in, 
    /* fifo */ fifo_BRAM_B_local_out);
}
/* Module Definition */

/* Module Definition */
void BRAM_C_IO_L2_in_intra_trans(int c0, int c1, int c2, BRAM_C_t16 *BRAM_C, BRAM_C_t16 local_BRAM_C[16][1], hls::stream<float> &fifo_BRAM_C_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  ap_uint<32> data_split[16];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L2
  // io_L1
  // pe
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1)
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // latency
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
        // latency
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            BRAM_C_t16 in_data;
            BRAM_C_t1 out_data;
            in_data = local_BRAM_C[4*c4 + c7][(4 * c5 + c6) / 16];
            for (ap_uint<5> n = 0; n < 16; n++) {
            #pragma HLS UNROLL
              data_split[n] = in_data(31, 0);
              in_data = in_data >> 32;
            }
            int split_idx = (4*c5 + c6) % 16;
            union {unsigned int ui; float ut;} u;
            u.ui = (unsigned int)data_split[split_idx];
            out_data = u.ut;
            fifo_BRAM_C_local_out.write(out_data);
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_IO_L2_in_boundary(BRAM_C_t16 *BRAM_C, hls::stream<float> &fifo_BRAM_C_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  BRAM_C_t16 local_BRAM_C_ping[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_C_ping core=RAM_2P_BRAM
  BRAM_C_t16 local_BRAM_C_pong[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_C_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c0, c0_prev;
  int c1, c1_prev;
  int c2, c2_prev;
  /* Variable Declaration */

  {
    for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
      for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
        for (ap_uint<3> c2 = 1; c2 <= 3; c2 += 1) {
          // array
          {
            if (arb == 0) {
              BRAM_C_IO_L2_in_inter_trans_boundary(
                /* array */ BRAM_C, 
                /* array */ local_BRAM_C_pong, 
                /* fifo */ fifo_BRAM_C_in, 
                /* enable */ inter_trans_en
              );
              BRAM_C_IO_L2_in_intra_trans(
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ BRAM_C, 
                /* array */ local_BRAM_C_ping, 
                /* fifo */ fifo_BRAM_C_local_out, 
                /* enable */ intra_trans_en
              );
            } else {
              BRAM_C_IO_L2_in_inter_trans_boundary(
                /* array */ BRAM_C, 
                /* array */ local_BRAM_C_ping, 
                /* fifo */ fifo_BRAM_C_in, 
                /* enable */ inter_trans_en
              );
              BRAM_C_IO_L2_in_intra_trans(
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ BRAM_C, 
                /* array */ local_BRAM_C_pong, 
                /* fifo */ fifo_BRAM_C_local_out, 
                /* enable */ intra_trans_en
              );
            }
            intra_trans_en = 1;
            arb = !arb;
            c0_prev = c0;
            c1_prev = c1;
            c2_prev = c2;
          }
        }
    if (arb == 0) {
      BRAM_C_IO_L2_in_intra_trans(
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ BRAM_C, 
        /* array */ local_BRAM_C_ping, 
        /* fifo */ fifo_BRAM_C_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      BRAM_C_IO_L2_in_intra_trans(
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ BRAM_C, 
        /* array */ local_BRAM_C_pong, 
        /* fifo */ fifo_BRAM_C_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void PE(int idx, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_B_in, hls::stream<float> &fifo_BRAM_C_in, hls::stream<float> &fifo_BRAM_C_out, hls::stream<float> &fifo_BRAM_C_drain_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  BRAM_A_t1 local_BRAM_A[1][1];
  #pragma HLS ARRAY_PARTITION variable=local_BRAM_A dim=0 complete
  BRAM_B_t1 local_BRAM_B[1][1];
  #pragma HLS ARRAY_PARTITION variable=local_BRAM_B dim=0 complete
  BRAM_C_t1 local_BRAM_C[16][16];
  #pragma HLS RESOURCE variable=local_BRAM_C core=RAM_2P_BRAM
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
      for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
        // array
        // pe
        for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1)
          for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
            // latency
            for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
              // latency
              for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
              #pragma HLS PIPELINE II=1
                {
                  local_BRAM_A[0][0] = fifo_BRAM_A_in.read();
                  local_BRAM_B[0][0] = fifo_BRAM_B_in.read();
                  if (p0 + 16 * c2 >= 1)
                    local_BRAM_C[4*c4 + c7][4*c5 + c6] = fifo_BRAM_C_in.read();
                  local_BRAM_C[4*c4 + c7][4*c5 + c6] = (local_BRAM_C[4*c4 + c7][4*c5 + c6] + (local_BRAM_A[0][0] * local_BRAM_B[0][0]));
                  if (p0 == 15 && c2 == 3) {
                    fifo_BRAM_C_drain_out.write(local_BRAM_C[4*c4 + c7][4*c5 + c6]);
                  } else {
                    fifo_BRAM_C_out.write(local_BRAM_C[4*c4 + c7][4*c5 + c6]);
                  }
                }
              }
            }
          }
      }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_B_in, hls::stream<float> &fifo_BRAM_C_in, hls::stream<float> &fifo_BRAM_C_out, hls::stream<float> &fifo_BRAM_C_drain_out)
 {
  PE(
    /* module id */ idx, 
    /* fifo */ fifo_BRAM_A_in, 
    /* fifo */ fifo_BRAM_B_in, 
    /* fifo */ fifo_BRAM_C_in, 
    /* fifo */ fifo_BRAM_C_out, 
    /* fifo */ fifo_BRAM_C_drain_out);
}
/* Module Definition */

/* Module Definition */
void BRAM_C_PE_dummy_out(int idx, hls::stream<float> &fifo_BRAM_C_out) {
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
      for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1)
        if (p0 + 16 * c2 >= 1) {
          // array
          // pe
          for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1)
            for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
              // latency
              for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
                // latency
                for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
                #pragma HLS PIPELINE II=1
                  BRAM_C_t1 fifo_data = 0;
                  fifo_BRAM_C_out.write(fifo_data);
                }
              }
            }
        }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_IO_L2_out_intra_trans(int c0, int c1, int c2, BRAM_C_t16 *BRAM_C, BRAM_C_t16 local_BRAM_C[16][1], hls::stream<float> &fifo_BRAM_C_local_in, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  ap_uint<32> data_split[16];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L2
  // io_L1
  // pe
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1)
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // latency
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
        // latency
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            BRAM_C_t1 in_data;
            BRAM_C_t16 out_data;
            in_data = fifo_BRAM_C_local_in.read();
            int split_idx = (4*c5 + c6) % 16;
            out_data = local_BRAM_C[4*c4 + c7][(4 * c5 + c6) / 16];
            for (ap_uint<5> n = 0; n < 16; n++) {
            #pragma HLS UNROLL
              data_split[n] = out_data(31, 0);
              out_data = out_data >> 32;
            }
            union {unsigned int ui; float ut;} u;
            u.ut = in_data;
            data_split[split_idx] = ap_uint<32>(u.ui);
            out_data = (data_split[15], data_split[14], data_split[13], data_split[12], data_split[11], data_split[10], data_split[9], data_split[8], data_split[7], data_split[6], data_split[5], data_split[4], data_split[3], data_split[2], data_split[1], data_split[0]);            local_BRAM_C[4*c4 + c7][(4 * c5 + c6) / 16] = out_data;
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_IO_L2_out_inter_trans_boundary(int c0, int c1, BRAM_C_t16 *BRAM_C, BRAM_C_t16 local_BRAM_C[16][1], hls::stream<BRAM_C_t16> &fifo_BRAM_C_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  if (!inter_trans_en) return;

  // io_L2
  for (ap_uint<5> c3 = 0; c3 <= 15; c3 += 1) {
  #pragma HLS PIPELINE II=1
    // access_coalesce
    {
      BRAM_C_t16 in_data;
      BRAM_C_t16 out_data;
      in_data = local_BRAM_C[c3][0];
      out_data = in_data;
      BRAM_C[64*c0 + c1 + 4*c3] = out_data;
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_IO_L2_out_boundary(BRAM_C_t16 *BRAM_C, hls::stream<float> &fifo_BRAM_C_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  BRAM_C_t16 local_BRAM_C_ping[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_C_ping core=RAM_2P_BRAM
  BRAM_C_t16 local_BRAM_C_pong[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_C_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 0;
  bool intra_trans_en = 1;
  int c0, c0_prev;
  int c1, c1_prev;
  /* Variable Declaration */

  {
    for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
      for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
        for (ap_uint<3> c2 = 0; c2 <= 2; c2 += 1) {
          // array
          {
            if (arb == 0) {
              BRAM_C_IO_L2_out_intra_trans(
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ BRAM_C, 
                /* array */ local_BRAM_C_ping, 
                /* fifo */ fifo_BRAM_C_local_in, 
                /* enable */ intra_trans_en
              );
              BRAM_C_IO_L2_out_inter_trans_boundary(
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* array */ BRAM_C, 
                /* array */ local_BRAM_C_pong, 
                /* fifo */ fifo_BRAM_C_out, 
                /* enable */ inter_trans_en
              );
            } else {
              BRAM_C_IO_L2_out_intra_trans(
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ BRAM_C, 
                /* array */ local_BRAM_C_pong, 
                /* fifo */ fifo_BRAM_C_local_in, 
                /* enable */ intra_trans_en
              );
              BRAM_C_IO_L2_out_inter_trans_boundary(
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* array */ BRAM_C, 
                /* array */ local_BRAM_C_ping, 
                /* fifo */ fifo_BRAM_C_out, 
                /* enable */ inter_trans_en
              );
            }
            inter_trans_en = 1;
            arb = !arb;
            c0_prev = c0;
            c1_prev = c1;
          }
        }
    if (arb == 0) {
      BRAM_C_IO_L2_out_inter_trans_boundary(
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* array */ BRAM_C, 
        /* array */ local_BRAM_C_pong, 
        /* fifo */ fifo_BRAM_C_out, 
        /* enable */ inter_trans_en
      );
    } else {
      BRAM_C_IO_L2_out_inter_trans_boundary(
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* array */ BRAM_C, 
        /* array */ local_BRAM_C_ping, 
        /* fifo */ fifo_BRAM_C_out, 
        /* enable */ inter_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L1_out_intra_trans(int idx, int c0, int c1, BRAM_C_t4 local_BRAM_C[16][4], hls::stream<float> &fifo_BRAM_C_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */


  // io_L1
  // pe
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1)
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // latency
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
        // latency
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            BRAM_C_t1 in_data;
            BRAM_C_t4 out_data;
            in_data = fifo_BRAM_C_drain_local_in.read();
            int split_idx = (4*c5 + c6) % 4;
            out_data = local_BRAM_C[4*c4 + c7][(4 * c5 + c6) / 4];
            for (ap_uint<3> n = 0; n < 4; n++) {
            #pragma HLS UNROLL
              data_split[n] = out_data(31, 0);
              out_data = out_data >> 32;
            }
            union {unsigned int ui; float ut;} u;
            u.ut = in_data;
            data_split[split_idx] = ap_uint<32>(u.ui);
            out_data = (data_split[3], data_split[2], data_split[1], data_split[0]);            local_BRAM_C[4*c4 + c7][(4 * c5 + c6) / 4] = out_data;
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L1_out_inter_trans(int idx, int c0, int c1, BRAM_C_t4 local_BRAM_C[16][4], hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_in, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // io_L1
  if (p0 == 15) {
    for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      // access_coalesce
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      #pragma HLS PIPELINE II=1
        {
          BRAM_C_t4 in_data;
          BRAM_C_t4 out_data;
          in_data = local_BRAM_C[c4][c5];
          out_data = in_data;
          fifo_BRAM_C_drain_out.write(out_data);
        }
      }
    }
  } else {
    for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      // access_coalesce
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      #pragma HLS PIPELINE II=1
        {
          BRAM_C_t4 in_data;
          BRAM_C_t4 out_data;
          in_data = fifo_BRAM_C_drain_in.read();
          out_data = in_data;
          fifo_BRAM_C_drain_out.write(out_data);
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L1_out_inter_trans_boundary(int idx, int c0, int c1, BRAM_C_t4 local_BRAM_C[16][4], hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // io_L1
  for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
    // access_coalesce
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
    #pragma HLS PIPELINE II=1
      {
        BRAM_C_t4 in_data;
        BRAM_C_t4 out_data;
        in_data = local_BRAM_C[c4][c5];
        out_data = in_data;
        fifo_BRAM_C_drain_out.write(out_data);
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L1_out(int idx, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_in, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out, hls::stream<float> &fifo_BRAM_C_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  BRAM_C_t4 local_BRAM_C[16][4];
  #pragma HLS RESOURCE variable=local_BRAM_C core=RAM_2P_BRAM
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1) {
      // array
      // io_L2
      BRAM_C_drain_IO_L1_out_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0, 
        /* host iter */ c1, 
        /* array */ local_BRAM_C, 
        /* fifo */ fifo_BRAM_C_drain_local_in
      );
      BRAM_C_drain_IO_L1_out_inter_trans(
        /* module id */ idx, 
        /* host iter */ c0, 
        /* host iter */ c1, 
        /* array */ local_BRAM_C, 
        /* fifo */ fifo_BRAM_C_drain_in, 
        /* fifo */ fifo_BRAM_C_drain_out
      );
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L1_out_wrapper(int idx, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_in, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out, hls::stream<float> &fifo_BRAM_C_drain_local_in)
 {
  BRAM_C_drain_IO_L1_out(
    /* module id */ idx, 
    /* fifo */ fifo_BRAM_C_drain_in, 
    /* fifo */ fifo_BRAM_C_drain_out, 
    /* fifo */ fifo_BRAM_C_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L1_out_boundary(int idx, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out, hls::stream<float> &fifo_BRAM_C_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  BRAM_C_t4 local_BRAM_C[16][4];
  #pragma HLS RESOURCE variable=local_BRAM_C core=RAM_2P_BRAM
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1) {
      // array
      // io_L2
      BRAM_C_drain_IO_L1_out_intra_trans(
        /* module id */ idx, 
        /* host iter */ c0, 
        /* host iter */ c1, 
        /* array */ local_BRAM_C, 
        /* fifo */ fifo_BRAM_C_drain_local_in
      );
      BRAM_C_drain_IO_L1_out_inter_trans_boundary(
        /* module id */ idx, 
        /* host iter */ c0, 
        /* host iter */ c1, 
        /* array */ local_BRAM_C, 
        /* fifo */ fifo_BRAM_C_drain_out
      );
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L1_out_boundary_wrapper(int idx, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out, hls::stream<float> &fifo_BRAM_C_drain_local_in)
 {
  BRAM_C_drain_IO_L1_out_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_BRAM_C_drain_out, 
    /* fifo */ fifo_BRAM_C_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L2_out(BRAM_C_t16 *BRAM_C, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  BRAM_C_t4 data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1) {
      // array
      // io_L2
      // io_L1
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
        // access_coalesce
        for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
        #pragma HLS PIPELINE II=1
          {
            BRAM_C_t4 in_data;
            BRAM_C_t16 out_data;
            in_data = fifo_BRAM_C_drain_local_in.read();
            int split_idx = (256*c0 + 4*c1 + 16*c4 + c5) % 4;
            data_split[split_idx] = in_data;
            out_data = (data_split[3], data_split[2], data_split[1], data_split[0]);            if (c5 % 4 == 4 - 1 || c5 == 3) {
              BRAM_C[((16 * c0 + c4) * 64 + (16 * c1 + 4 * c5)) / 16] = out_data;
            }
          }
        }
      }
    }
}
/* Module Definition */

void kernel0(BRAM_A_t1 *BRAM_A, BRAM_B_t16 *BRAM_B, BRAM_C_t16 *BRAM_C_0, BRAM_C_t16 *BRAM_C_1, BRAM_C_t16 *BRAM_C_2)
{
#pragma HLS INTERFACE m_axi port=BRAM_A offset=slave bundle=gmem_BRAM_A
#pragma HLS INTERFACE m_axi port=BRAM_B offset=slave bundle=gmem_BRAM_B
#pragma HLS INTERFACE m_axi port=BRAM_C_0 offset=slave bundle=gmem_BRAM_C_0
#pragma HLS INTERFACE m_axi port=BRAM_C_1 offset=slave bundle=gmem_BRAM_C_1
#pragma HLS INTERFACE m_axi port=BRAM_C_2 offset=slave bundle=gmem_BRAM_C_2
#pragma HLS INTERFACE s_axilite port=BRAM_A bundle=control
#pragma HLS INTERFACE s_axilite port=BRAM_B bundle=control
#pragma HLS INTERFACE s_axilite port=BRAM_C_0 bundle=control
#pragma HLS INTERFACE s_axilite port=BRAM_C_1 bundle=control
#pragma HLS INTERFACE s_axilite port=BRAM_C_2 bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATAFLOW

  /* FIFO Declaration */
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_0;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_0 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_1;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_1 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_2;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_2 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_3;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_3 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_3 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_4;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_4 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_4 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_5;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_5 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_5 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_6;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_6 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_6 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_7;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_7 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_7 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_8;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_8 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_8 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_9;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_9 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_9 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_10;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_10 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_10 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_11;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_11 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_11 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_12;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_12 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_12 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_13;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_13 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_13 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_14;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_14 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_14 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_15;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_15 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_15 core=FIFO_SRL
  /* BRAM_A_IO_L1_in fifo */ hls::stream<float> fifo_BRAM_A_BRAM_A_IO_L1_in_16;
  #pragma HLS STREAM variable=fifo_BRAM_A_BRAM_A_IO_L1_in_16 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_BRAM_A_IO_L1_in_16 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_0;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_0 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_1;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_1 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_2;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_2 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_3;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_3 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_3 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_4;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_4 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_4 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_5;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_5 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_5 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_6;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_6 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_6 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_7;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_7 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_7 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_8;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_8 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_8 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_9;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_9 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_9 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_10;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_10 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_10 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_11;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_11 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_11 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_12;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_12 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_12 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_13;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_13 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_13 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_14;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_14 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_14 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_15;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_15 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_15 core=FIFO_SRL
  /* BRAM_B_IO_L1_in fifo */ hls::stream<BRAM_B_t4> fifo_BRAM_B_BRAM_B_IO_L1_in_16;
  #pragma HLS STREAM variable=fifo_BRAM_B_BRAM_B_IO_L1_in_16 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_BRAM_B_IO_L1_in_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_0;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_1;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_2;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_3;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_4;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_5;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_6;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_7;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_8;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_9;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_10;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_11;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_12;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_13;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_14;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_A_PE_15;
  #pragma HLS STREAM variable=fifo_BRAM_A_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_A_PE_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_0;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_1;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_2;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_3;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_4;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_5;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_6;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_7;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_8;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_9;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_10;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_11;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_12;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_13;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_14;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_B_PE_15;
  #pragma HLS STREAM variable=fifo_BRAM_B_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_B_PE_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_0;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_1;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_2;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_3;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_4;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_5;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_6;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_7;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_8;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_9;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_10;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_11;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_12;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_13;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_14;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_15;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_PE_16;
  #pragma HLS STREAM variable=fifo_BRAM_C_PE_16 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_PE_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_0;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_1;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_2;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_3;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_4;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_5;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_6;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_7;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_8;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_9;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_10;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_11;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_12;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_13;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_14;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_BRAM_C_drain_PE_15;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_PE_15 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_0;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_0 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_0 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_1;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_1 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_1 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_2;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_2 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_2 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_3;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_3 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_3 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_4;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_4 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_4 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_5;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_5 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_5 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_6;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_6 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_6 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_7;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_7 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_7 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_8;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_8 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_8 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_9;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_9 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_9 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_10;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_10 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_10 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_11;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_11 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_11 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_12;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_12 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_12 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_13;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_13 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_13 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_14;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_14 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_14 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_15;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_15 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_15 core=FIFO_SRL
  /* BRAM_C_drain_IO_L1_out fifo */ hls::stream<BRAM_C_t4> fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_16;
  #pragma HLS STREAM variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_16 depth=2
  #pragma HLS RESOURCE variable=fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_16 core=FIFO_SRL
  /* FIFO Declaration */

  /* Module Call */
  BRAM_A_IO_L2_in(
    /* array */ BRAM_A,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_0
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_0,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_1,
    /* fifo */ fifo_BRAM_A_PE_0
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_1,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_2,
    /* fifo */ fifo_BRAM_A_PE_1
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_2,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_3,
    /* fifo */ fifo_BRAM_A_PE_2
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_3,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_4,
    /* fifo */ fifo_BRAM_A_PE_3
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_4,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_5,
    /* fifo */ fifo_BRAM_A_PE_4
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_5,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_6,
    /* fifo */ fifo_BRAM_A_PE_5
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_6,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_7,
    /* fifo */ fifo_BRAM_A_PE_6
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_7,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_8,
    /* fifo */ fifo_BRAM_A_PE_7
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_8,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_9,
    /* fifo */ fifo_BRAM_A_PE_8
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_9,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_10,
    /* fifo */ fifo_BRAM_A_PE_9
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_10,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_11,
    /* fifo */ fifo_BRAM_A_PE_10
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_11,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_12,
    /* fifo */ fifo_BRAM_A_PE_11
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_12,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_13,
    /* fifo */ fifo_BRAM_A_PE_12
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_13,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_14,
    /* fifo */ fifo_BRAM_A_PE_13
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_14,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_15,
    /* fifo */ fifo_BRAM_A_PE_14
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_IO_L1_in_boundary_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_BRAM_A_BRAM_A_IO_L1_in_15,
    /* fifo */ fifo_BRAM_A_PE_15
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L2_in(
    /* array */ BRAM_B,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_0
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_0,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_1,
    /* fifo */ fifo_BRAM_B_PE_0
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_1,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_2,
    /* fifo */ fifo_BRAM_B_PE_1
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_2,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_3,
    /* fifo */ fifo_BRAM_B_PE_2
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_3,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_4,
    /* fifo */ fifo_BRAM_B_PE_3
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_4,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_5,
    /* fifo */ fifo_BRAM_B_PE_4
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_5,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_6,
    /* fifo */ fifo_BRAM_B_PE_5
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_6,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_7,
    /* fifo */ fifo_BRAM_B_PE_6
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_7,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_8,
    /* fifo */ fifo_BRAM_B_PE_7
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_8,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_9,
    /* fifo */ fifo_BRAM_B_PE_8
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_9,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_10,
    /* fifo */ fifo_BRAM_B_PE_9
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_10,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_11,
    /* fifo */ fifo_BRAM_B_PE_10
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_11,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_12,
    /* fifo */ fifo_BRAM_B_PE_11
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_12,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_13,
    /* fifo */ fifo_BRAM_B_PE_12
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_13,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_14,
    /* fifo */ fifo_BRAM_B_PE_13
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_14,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_15,
    /* fifo */ fifo_BRAM_B_PE_14
  );
  /* Module Call */

  /* Module Call */
  BRAM_B_IO_L1_in_boundary_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_15,
    /* fifo */ fifo_BRAM_B_PE_15
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_IO_L2_in_boundary(
    /* array */ BRAM_C_0,
    /* fifo */ fifo_BRAM_C_PE_0
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_PE_dummy_out(
    /* module id */ 0,
    /* fifo */ fifo_BRAM_C_PE_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_BRAM_A_PE_0,
    /* fifo */ fifo_BRAM_B_PE_0,
    /* fifo */ fifo_BRAM_C_PE_0,
    /* fifo */ fifo_BRAM_C_PE_1,
    /* fifo */ fifo_BRAM_C_drain_PE_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_BRAM_A_PE_1,
    /* fifo */ fifo_BRAM_B_PE_1,
    /* fifo */ fifo_BRAM_C_PE_1,
    /* fifo */ fifo_BRAM_C_PE_2,
    /* fifo */ fifo_BRAM_C_drain_PE_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_BRAM_A_PE_2,
    /* fifo */ fifo_BRAM_B_PE_2,
    /* fifo */ fifo_BRAM_C_PE_2,
    /* fifo */ fifo_BRAM_C_PE_3,
    /* fifo */ fifo_BRAM_C_drain_PE_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_BRAM_A_PE_3,
    /* fifo */ fifo_BRAM_B_PE_3,
    /* fifo */ fifo_BRAM_C_PE_3,
    /* fifo */ fifo_BRAM_C_PE_4,
    /* fifo */ fifo_BRAM_C_drain_PE_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_BRAM_A_PE_4,
    /* fifo */ fifo_BRAM_B_PE_4,
    /* fifo */ fifo_BRAM_C_PE_4,
    /* fifo */ fifo_BRAM_C_PE_5,
    /* fifo */ fifo_BRAM_C_drain_PE_4
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_BRAM_A_PE_5,
    /* fifo */ fifo_BRAM_B_PE_5,
    /* fifo */ fifo_BRAM_C_PE_5,
    /* fifo */ fifo_BRAM_C_PE_6,
    /* fifo */ fifo_BRAM_C_drain_PE_5
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_BRAM_A_PE_6,
    /* fifo */ fifo_BRAM_B_PE_6,
    /* fifo */ fifo_BRAM_C_PE_6,
    /* fifo */ fifo_BRAM_C_PE_7,
    /* fifo */ fifo_BRAM_C_drain_PE_6
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_BRAM_A_PE_7,
    /* fifo */ fifo_BRAM_B_PE_7,
    /* fifo */ fifo_BRAM_C_PE_7,
    /* fifo */ fifo_BRAM_C_PE_8,
    /* fifo */ fifo_BRAM_C_drain_PE_7
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_BRAM_A_PE_8,
    /* fifo */ fifo_BRAM_B_PE_8,
    /* fifo */ fifo_BRAM_C_PE_8,
    /* fifo */ fifo_BRAM_C_PE_9,
    /* fifo */ fifo_BRAM_C_drain_PE_8
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_BRAM_A_PE_9,
    /* fifo */ fifo_BRAM_B_PE_9,
    /* fifo */ fifo_BRAM_C_PE_9,
    /* fifo */ fifo_BRAM_C_PE_10,
    /* fifo */ fifo_BRAM_C_drain_PE_9
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_BRAM_A_PE_10,
    /* fifo */ fifo_BRAM_B_PE_10,
    /* fifo */ fifo_BRAM_C_PE_10,
    /* fifo */ fifo_BRAM_C_PE_11,
    /* fifo */ fifo_BRAM_C_drain_PE_10
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_BRAM_A_PE_11,
    /* fifo */ fifo_BRAM_B_PE_11,
    /* fifo */ fifo_BRAM_C_PE_11,
    /* fifo */ fifo_BRAM_C_PE_12,
    /* fifo */ fifo_BRAM_C_drain_PE_11
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_BRAM_A_PE_12,
    /* fifo */ fifo_BRAM_B_PE_12,
    /* fifo */ fifo_BRAM_C_PE_12,
    /* fifo */ fifo_BRAM_C_PE_13,
    /* fifo */ fifo_BRAM_C_drain_PE_12
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_BRAM_A_PE_13,
    /* fifo */ fifo_BRAM_B_PE_13,
    /* fifo */ fifo_BRAM_C_PE_13,
    /* fifo */ fifo_BRAM_C_PE_14,
    /* fifo */ fifo_BRAM_C_drain_PE_13
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_BRAM_A_PE_14,
    /* fifo */ fifo_BRAM_B_PE_14,
    /* fifo */ fifo_BRAM_C_PE_14,
    /* fifo */ fifo_BRAM_C_PE_15,
    /* fifo */ fifo_BRAM_C_drain_PE_14
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_BRAM_A_PE_15,
    /* fifo */ fifo_BRAM_B_PE_15,
    /* fifo */ fifo_BRAM_C_PE_15,
    /* fifo */ fifo_BRAM_C_PE_16,
    /* fifo */ fifo_BRAM_C_drain_PE_15
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_IO_L2_out_boundary(
    /* array */ BRAM_C_1,
    /* fifo */ fifo_BRAM_C_PE_1
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_15,
    /* fifo */ fifo_BRAM_C_drain_PE_15
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_15,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_14,
    /* fifo */ fifo_BRAM_C_drain_PE_14
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_14,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_13,
    /* fifo */ fifo_BRAM_C_drain_PE_13
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_13,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_12,
    /* fifo */ fifo_BRAM_C_drain_PE_12
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_12,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_11,
    /* fifo */ fifo_BRAM_C_drain_PE_11
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_11,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_10,
    /* fifo */ fifo_BRAM_C_drain_PE_10
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_10,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_9,
    /* fifo */ fifo_BRAM_C_drain_PE_9
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_9,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_8,
    /* fifo */ fifo_BRAM_C_drain_PE_8
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_8,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_7,
    /* fifo */ fifo_BRAM_C_drain_PE_7
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_7,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_6,
    /* fifo */ fifo_BRAM_C_drain_PE_6
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_6,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_5,
    /* fifo */ fifo_BRAM_C_drain_PE_5
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_5,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_4,
    /* fifo */ fifo_BRAM_C_drain_PE_4
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_4,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_3,
    /* fifo */ fifo_BRAM_C_drain_PE_3
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_3,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_2,
    /* fifo */ fifo_BRAM_C_drain_PE_2
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_2,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_1,
    /* fifo */ fifo_BRAM_C_drain_PE_1
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_1,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_0,
    /* fifo */ fifo_BRAM_C_drain_PE_0
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L2_out(
    /* array */ BRAM_C_2,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_0
  );
  /* Module Call */

}
