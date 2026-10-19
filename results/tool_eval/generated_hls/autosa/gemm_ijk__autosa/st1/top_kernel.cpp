#include "top_kernel.h"
/* Module Definition */
void BRAM_A_IO_L2_in_intra_trans(int c0, int c1, int c2, BRAM_A_t16 *BRAM_A, BRAM_A_t16 local_BRAM_A[16][1], hls::stream<float> &fifo_BRAM_A_local_out, bool intra_trans_en) {
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
    for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1) {
      // latency
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
        // latency
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            BRAM_A_t16 in_data;
            BRAM_A_t1 out_data;
            in_data = local_BRAM_A[4*c4 + c6][c5 / 16];
            for (ap_uint<5> n = 0; n < 16; n++) {
            #pragma HLS UNROLL
              data_split[n] = in_data(31, 0);
              in_data = in_data >> 32;
            }
            int split_idx = (c5) % 16;
            union {unsigned int ui; float ut;} u;
            u.ui = (unsigned int)data_split[split_idx];
            out_data = u.ut;
            fifo_BRAM_A_local_out.write(out_data);
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void BRAM_A_IO_L2_in_inter_trans_boundary(int c0, int c1, int c2, BRAM_A_t16 *BRAM_A, BRAM_A_t16 local_BRAM_A[16][1], hls::stream<BRAM_A_t16> &fifo_BRAM_A_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  if (!inter_trans_en) return;

  // io_L2
  for (ap_uint<5> c3 = 0; c3 <= 15; c3 += 1) {
  #pragma HLS PIPELINE II=1
    // access_coalesce
    {
      BRAM_A_t16 in_data;
      BRAM_A_t16 out_data;
      in_data = BRAM_A[64*c0 + c2 + 4*c3];
      out_data = in_data;
      local_BRAM_A[c3][0] = out_data;
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_A_IO_L2_in_boundary(BRAM_A_t16 *BRAM_A, hls::stream<float> &fifo_BRAM_A_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  BRAM_A_t16 local_BRAM_A_ping[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_A_ping core=RAM_2P_BRAM
  BRAM_A_t16 local_BRAM_A_pong[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_A_pong core=RAM_2P_BRAM
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
          {
            if (arb == 0) {
              BRAM_A_IO_L2_in_inter_trans_boundary(
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ BRAM_A, 
                /* array */ local_BRAM_A_pong, 
                /* fifo */ fifo_BRAM_A_in, 
                /* enable */ inter_trans_en
              );
              BRAM_A_IO_L2_in_intra_trans(
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ BRAM_A, 
                /* array */ local_BRAM_A_ping, 
                /* fifo */ fifo_BRAM_A_local_out, 
                /* enable */ intra_trans_en
              );
            } else {
              BRAM_A_IO_L2_in_inter_trans_boundary(
                /* host iter */ c0, 
                /* host iter */ c1, 
                /* host iter */ c2, 
                /* array */ BRAM_A, 
                /* array */ local_BRAM_A_ping, 
                /* fifo */ fifo_BRAM_A_in, 
                /* enable */ inter_trans_en
              );
              BRAM_A_IO_L2_in_intra_trans(
                /* host iter */ c0_prev, 
                /* host iter */ c1_prev, 
                /* host iter */ c2_prev, 
                /* array */ BRAM_A, 
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
      BRAM_A_IO_L2_in_intra_trans(
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ BRAM_A, 
        /* array */ local_BRAM_A_ping, 
        /* fifo */ fifo_BRAM_A_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      BRAM_A_IO_L2_in_intra_trans(
        /* host iter */ c0_prev, 
        /* host iter */ c1_prev, 
        /* host iter */ c2_prev, 
        /* array */ BRAM_A, 
        /* array */ local_BRAM_A_pong, 
        /* fifo */ fifo_BRAM_A_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L2_in(BRAM_B_t4 *BRAM_B, hls::stream<BRAM_B_t4> &fifo_BRAM_B_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
      for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
        // array
        // io_L2
        for (ap_uint<3> c3 = 0; c3 <= 3; c3 += 1) {
          // io_L1
          for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
          #pragma HLS PIPELINE II=1
            // access_coalesce
            {
              BRAM_B_t4 in_data;
              BRAM_B_t4 out_data;
              in_data = BRAM_B[4*c1 + 256*c2 + c3 + 16*c4];
              out_data = in_data;
              fifo_BRAM_B_local_out.write(out_data);
            }
          }
        }
      }
}
/* Module Definition */

/* Module Definition */
void BRAM_B_IO_L1_in_intra_trans(int idx, int c0, int c1, int c2, BRAM_B_t4 local_BRAM_B[16][1], hls::stream<float> &fifo_BRAM_B_local_out, bool intra_trans_en) {
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
    for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1) {
      // latency
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
        // latency
        for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            BRAM_B_t4 in_data;
            BRAM_B_t1 out_data;
            in_data = local_BRAM_B[c5][c7 / 4];
            for (ap_uint<3> n = 0; n < 4; n++) {
            #pragma HLS UNROLL
              data_split[n] = in_data(31, 0);
              in_data = in_data >> 32;
            }
            int split_idx = (c7) % 4;
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
void BRAM_B_IO_L1_in_inter_trans(int idx, int c0, int c1, int c2, BRAM_B_t4 local_BRAM_B[16][1], hls::stream<BRAM_B_t4> &fifo_BRAM_B_in, hls::stream<BRAM_B_t4> &fifo_BRAM_B_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c3 = p0; c3 <= 3; c3 += 1) {
    // io_L1
    if (c3 == p0) {
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          BRAM_B_t4 in_data;
          BRAM_B_t4 out_data;
          in_data = fifo_BRAM_B_in.read();
          out_data = in_data;
          local_BRAM_B[c4][0] = out_data;
        }
      }
    } else {
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
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
void BRAM_B_IO_L1_in_inter_trans_boundary(int idx, int c0, int c1, int c2, BRAM_B_t4 local_BRAM_B[16][1], hls::stream<BRAM_B_t4> &fifo_BRAM_B_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c3 = p0; c3 <= 3; c3 += 1)
    if (c3 == p0) {
      // io_L1
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          BRAM_B_t4 in_data;
          BRAM_B_t4 out_data;
          in_data = fifo_BRAM_B_in.read();
          out_data = in_data;
          local_BRAM_B[c4][0] = out_data;
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
  BRAM_B_t4 local_BRAM_B_ping[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_B_ping core=RAM_2P_BRAM
  BRAM_B_t4 local_BRAM_B_pong[16][1];
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
  BRAM_B_t4 local_BRAM_B_ping[16][1];
  #pragma HLS RESOURCE variable=local_BRAM_B_ping core=RAM_2P_BRAM
  BRAM_B_t4 local_BRAM_B_pong[16][1];
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
void PE(int idx, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_out, hls::stream<float> &fifo_BRAM_B_in, hls::stream<float> &fifo_BRAM_C_drain_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  BRAM_A_t1 local_BRAM_A[1][1];
  #pragma HLS ARRAY_PARTITION variable=local_BRAM_A dim=0 complete
  BRAM_B_t1 local_BRAM_B[1][1];
  #pragma HLS ARRAY_PARTITION variable=local_BRAM_B dim=0 complete
  BRAM_C_t1 local_BRAM_C[16][4];
  #pragma HLS RESOURCE variable=local_BRAM_C core=RAM_2P_BRAM
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
      for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
        // array
        // pe
        for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1)
          for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1) {
            // latency
            for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
              // latency
              for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
              #pragma HLS PIPELINE II=1
                {
                  local_BRAM_A[0][0] = fifo_BRAM_A_in.read();
                  local_BRAM_B[0][0] = fifo_BRAM_B_in.read();
                  local_BRAM_C[4*c4 + c6][c7] = (local_BRAM_C[4*c4 + c6][c7] + (local_BRAM_A[0][0] * local_BRAM_B[0][0]));
                  if (c2 == 3 && c5 == 15)
                    fifo_BRAM_C_drain_out.write(local_BRAM_C[4*c4 + c6][c7]);
                  fifo_BRAM_A_out.write(local_BRAM_A[0][0]);
                }
              }
            }
          }
      }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, hls::stream<float> &fifo_BRAM_A_in, hls::stream<float> &fifo_BRAM_A_out, hls::stream<float> &fifo_BRAM_B_in, hls::stream<float> &fifo_BRAM_C_drain_out)
 {
  PE(
    /* module id */ idx, 
    /* fifo */ fifo_BRAM_A_in, 
    /* fifo */ fifo_BRAM_A_out, 
    /* fifo */ fifo_BRAM_B_in, 
    /* fifo */ fifo_BRAM_C_drain_out);
}
/* Module Definition */

/* Module Definition */
void BRAM_A_PE_dummy_in(int idx, hls::stream<float> &fifo_BRAM_A_in) {
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1)
      for (ap_uint<3> c2 = 0; c2 <= 3; c2 += 1) {
        // array
        // pe
        for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1)
          for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1) {
            // latency
            for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
              // latency
              for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
              #pragma HLS PIPELINE II=1
                BRAM_A_t1 fifo_data;
                fifo_data = fifo_BRAM_A_in.read();
              }
            }
          }
      }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L1_out_intra_trans(int idx, int c0, int c1, BRAM_C_t4 local_BRAM_C[16][1], hls::stream<float> &fifo_BRAM_C_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */


  // io_L1
  // pe
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    // latency
    for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
      // latency
      for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
      #pragma HLS PIPELINE II=1
        {
          BRAM_C_t1 in_data;
          BRAM_C_t4 out_data;
          in_data = fifo_BRAM_C_drain_local_in.read();
          int split_idx = (c7) % 4;
          out_data = local_BRAM_C[4*c4 + c6][c7 / 4];
          for (ap_uint<3> n = 0; n < 4; n++) {
          #pragma HLS UNROLL
            data_split[n] = out_data(31, 0);
            out_data = out_data >> 32;
          }
          union {unsigned int ui; float ut;} u;
          u.ut = in_data;
          data_split[split_idx] = ap_uint<32>(u.ui);
          out_data = (data_split[3], data_split[2], data_split[1], data_split[0]);          local_BRAM_C[4*c4 + c6][c7 / 4] = out_data;
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void BRAM_C_drain_IO_L1_out_inter_trans(int idx, int c0, int c1, BRAM_C_t4 local_BRAM_C[16][1], hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_in, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<3> c3 = p0; c3 <= 3; c3 += 1) {
    // io_L1
    if (c3 == p0) {
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          BRAM_C_t4 in_data;
          BRAM_C_t4 out_data;
          in_data = local_BRAM_C[c4][0];
          out_data = in_data;
          fifo_BRAM_C_drain_out.write(out_data);
        }
      }
    } else {
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
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
void BRAM_C_drain_IO_L1_out_inter_trans_boundary(int idx, int c0, int c1, BRAM_C_t4 local_BRAM_C[16][1], hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<3> c3 = p0; c3 <= 3; c3 += 1)
    if (c3 == p0) {
      // io_L1
      for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          BRAM_C_t4 in_data;
          BRAM_C_t4 out_data;
          in_data = local_BRAM_C[c4][0];
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
  BRAM_C_t4 local_BRAM_C[16][1];
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
  BRAM_C_t4 local_BRAM_C[16][1];
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
void BRAM_C_drain_IO_L2_out(BRAM_C_t4 *BRAM_C, hls::stream<BRAM_C_t4> &fifo_BRAM_C_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  for (ap_uint<3> c0 = 0; c0 <= 3; c0 += 1)
    for (ap_uint<3> c1 = 0; c1 <= 3; c1 += 1) {
      // array
      // io_L2
      for (ap_uint<3> c3 = 0; c3 <= 3; c3 += 1) {
        // io_L1
        for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            BRAM_C_t4 in_data;
            BRAM_C_t4 out_data;
            in_data = fifo_BRAM_C_drain_local_in.read();
            out_data = in_data;
            BRAM_C[256*c0 + 4*c1 + c3 + 16*c4] = out_data;
          }
        }
      }
    }
}
/* Module Definition */

void kernel0(BRAM_A_t16 *BRAM_A, BRAM_B_t4 *BRAM_B, BRAM_C_t4 *BRAM_C)
{
#pragma HLS INTERFACE m_axi port=BRAM_A offset=slave bundle=gmem_BRAM_A
#pragma HLS INTERFACE m_axi port=BRAM_B offset=slave bundle=gmem_BRAM_B
#pragma HLS INTERFACE m_axi port=BRAM_C offset=slave bundle=gmem_BRAM_C
#pragma HLS INTERFACE s_axilite port=BRAM_A bundle=control
#pragma HLS INTERFACE s_axilite port=BRAM_B bundle=control
#pragma HLS INTERFACE s_axilite port=BRAM_C bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATAFLOW

  /* FIFO Declaration */
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
  /* FIFO Declaration */

  /* Module Call */
  BRAM_A_IO_L2_in_boundary(
    /* array */ BRAM_A,
    /* fifo */ fifo_BRAM_A_PE_0
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
  BRAM_B_IO_L1_in_boundary_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_BRAM_B_BRAM_B_IO_L1_in_3,
    /* fifo */ fifo_BRAM_B_PE_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_BRAM_A_PE_0,
    /* fifo */ fifo_BRAM_A_PE_1,
    /* fifo */ fifo_BRAM_B_PE_0,
    /* fifo */ fifo_BRAM_C_drain_PE_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_BRAM_A_PE_1,
    /* fifo */ fifo_BRAM_A_PE_2,
    /* fifo */ fifo_BRAM_B_PE_1,
    /* fifo */ fifo_BRAM_C_drain_PE_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_BRAM_A_PE_2,
    /* fifo */ fifo_BRAM_A_PE_3,
    /* fifo */ fifo_BRAM_B_PE_2,
    /* fifo */ fifo_BRAM_C_drain_PE_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_BRAM_A_PE_3,
    /* fifo */ fifo_BRAM_A_PE_4,
    /* fifo */ fifo_BRAM_B_PE_3,
    /* fifo */ fifo_BRAM_C_drain_PE_3
  );
  /* Module Call */

  /* Module Call */
  BRAM_A_PE_dummy_in(
    /* module id */ 3,
    /* fifo */ fifo_BRAM_A_PE_4
  );
  /* Module Call */

  /* Module Call */
  BRAM_C_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 3,
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
    /* array */ BRAM_C,
    /* fifo */ fifo_BRAM_C_drain_BRAM_C_drain_IO_L1_out_0
  );
  /* Module Call */

}
