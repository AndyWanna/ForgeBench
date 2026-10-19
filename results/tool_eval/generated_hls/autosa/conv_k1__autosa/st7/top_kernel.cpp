#include "top_kernel.h"
/* Module Definition */
void input_IO_L3_in(input_t2 *input, hls::stream<input_t2> &fifo_input_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    // io_L2
    for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1) {
      // io_L1
      for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
        for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
          // access_coalesce
          for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
          #pragma HLS PIPELINE II=1
            {
              input_t2 in_data;
              input_t2 out_data;
              in_data = input[14*c5 + 98*c6 + 7*c7 + c8];
              out_data = in_data;
              fifo_input_local_out.write(out_data);
            }
          }
        }
    }
  }
}
/* Module Definition */

/* Module Definition */
void input_IO_L2_in(int idx, hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_out, hls::stream<input_t2> &fifo_input_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1) {
    // io_L2
    if (c4 == p0) {
      for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1) {
        // io_L1
        for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
          for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
            // access_coalesce
            for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
            #pragma HLS PIPELINE II=1
              {
                input_t2 in_data;
                input_t2 out_data;
                in_data = fifo_input_in.read();
                out_data = in_data;
                fifo_input_local_out.write(out_data);
              }
            }
          }
      }
    } else {
      for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1) {
        // io_L1
        for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
          for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
            // access_coalesce
            for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
            #pragma HLS PIPELINE II=1
              {
                input_t2 in_data;
                input_t2 out_data;
                in_data = fifo_input_in.read();
                out_data = in_data;
                fifo_input_out.write(out_data);
              }
            }
          }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void input_IO_L2_in_boundary(int idx, hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1)
    if (c4 == p0) {
      // io_L2
      for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1) {
        // io_L1
        for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
          for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
            // access_coalesce
            for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
            #pragma HLS PIPELINE II=1
              {
                input_t2 in_data;
                input_t2 out_data;
                in_data = fifo_input_in.read();
                out_data = in_data;
                fifo_input_local_out.write(out_data);
              }
            }
          }
      }
    }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_intra_trans(int idx, int idy, input_t2 local_input[16][2][7], hls::stream<float> &fifo_input_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  ap_uint<32> data_split[2];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L1
  // pe
  for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
    for (ap_uint<5> c7 = 0; c7 <= 15; c7 += 1) {
      // latency
      for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
        // latency
        for (ap_uint<3> c9 = 0; c9 <= 3; c9 += 1) {
          // latency
          for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
          #pragma HLS PIPELINE II=1
            {
              input_t2 in_data;
              input_t1 out_data;
              in_data = local_input[c7][c10][(2 * c6 + c8) / 2];
              for (ap_uint<2> n = 0; n < 2; n++) {
              #pragma HLS UNROLL
                data_split[n] = in_data(31, 0);
                in_data = in_data >> 32;
              }
              int split_idx = (2*c6 + c8) % 2;
              union {unsigned int ui; float ut;} u;
              u.ui = (unsigned int)data_split[split_idx];
              out_data = u.ut;
              fifo_input_local_out.write(out_data);
            }
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_inter_trans(int idx, int idy, input_t2 local_input[16][2][7], hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<4> c5 = p1; c5 <= 6; c5 += 1) {
    // io_L1
    if (c5 == p1) {
      for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
        for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
          // access_coalesce
          for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
          #pragma HLS PIPELINE II=1
            {
              input_t2 in_data;
              input_t2 out_data;
              in_data = fifo_input_in.read();
              out_data = in_data;
              local_input[c6][c7][c8] = out_data;
            }
          }
        }
    } else {
      for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
        for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
          // access_coalesce
          for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
          #pragma HLS PIPELINE II=1
            {
              input_t2 in_data;
              input_t2 out_data;
              in_data = fifo_input_in.read();
              out_data = in_data;
              fifo_input_out.write(out_data);
            }
          }
        }
    }
  }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_inter_trans_boundary(int idx, int idy, input_t2 local_input[16][2][7], hls::stream<input_t2> &fifo_input_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<4> c5 = p1; c5 <= 6; c5 += 1)
    if (c5 == p1) {
      // io_L1
      for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
        for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
          // access_coalesce
          for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
          #pragma HLS PIPELINE II=1
            {
              input_t2 in_data;
              input_t2 out_data;
              in_data = fifo_input_in.read();
              out_data = in_data;
              local_input[c6][c7][c8] = out_data;
            }
          }
        }
    }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in(int idx, int idy, hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_out, hls::stream<float> &fifo_input_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  input_t2 local_input_ping[16][2][7];
  #pragma HLS RESOURCE variable=local_input_ping core=RAM_2P_BRAM
  input_t2 local_input_pong[16][2][7];
  #pragma HLS RESOURCE variable=local_input_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  /* Variable Declaration */

  {
    // array
    // io_L3
    // io_L2
    {
      if (arb == 0) {
        input_IO_L1_in_inter_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_input_pong, 
          /* fifo */ fifo_input_in, 
          /* fifo */ fifo_input_out, 
          /* enable */ inter_trans_en
        );
        input_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        input_IO_L1_in_inter_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_in, 
          /* fifo */ fifo_input_out, 
          /* enable */ inter_trans_en
        );
        input_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_input_pong, 
          /* fifo */ fifo_input_local_out, 
          /* enable */ intra_trans_en
        );
      }
      intra_trans_en = 1;
      arb = !arb;
    }
    if (arb == 0) {
      input_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* array */ local_input_ping, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      input_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* array */ local_input_pong, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_wrapper(int idx, int idy, hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_out, hls::stream<float> &fifo_input_local_out)
 {
  input_IO_L1_in(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_input_in, 
    /* fifo */ fifo_input_out, 
    /* fifo */ fifo_input_local_out);
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_boundary(int idx, int idy, hls::stream<input_t2> &fifo_input_in, hls::stream<float> &fifo_input_local_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  input_t2 local_input_ping[16][2][7];
  #pragma HLS RESOURCE variable=local_input_ping core=RAM_2P_BRAM
  input_t2 local_input_pong[16][2][7];
  #pragma HLS RESOURCE variable=local_input_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  /* Variable Declaration */

  {
    // array
    // io_L3
    // io_L2
    {
      if (arb == 0) {
        input_IO_L1_in_inter_trans_boundary(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_input_pong, 
          /* fifo */ fifo_input_in, 
          /* enable */ inter_trans_en
        );
        input_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        input_IO_L1_in_inter_trans_boundary(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_in, 
          /* enable */ inter_trans_en
        );
        input_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_input_pong, 
          /* fifo */ fifo_input_local_out, 
          /* enable */ intra_trans_en
        );
      }
      intra_trans_en = 1;
      arb = !arb;
    }
    if (arb == 0) {
      input_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* array */ local_input_ping, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      input_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* array */ local_input_pong, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_boundary_wrapper(int idx, int idy, hls::stream<input_t2> &fifo_input_in, hls::stream<float> &fifo_input_local_out)
 {
  input_IO_L1_in_boundary(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_input_in, 
    /* fifo */ fifo_input_local_out);
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L3_in(kernel_t1 *kernel, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    // io_L2
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
      for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          kernel_t1 in_data;
          kernel_t1 out_data;
          in_data = kernel[64*c4 + 16*c5 + c6];
          out_data = in_data;
          fifo_kernel_local_out.write(out_data);
        }
      }
  }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L2_in_intra_trans(int idx, float local_kernel[4][16][1][1], hls::stream<float> &fifo_kernel_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // array
  // io_L3
  // io_L2
  // io_L1
  // pe
  for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
    for (ap_uint<5> c7 = 0; c7 <= 15; c7 += 1) {
      // latency
      for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
        // latency
        for (ap_uint<3> c9 = 0; c9 <= 3; c9 += 1) {
          // latency
          for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
          #pragma HLS PIPELINE II=1
            {
              kernel_t1 in_data;
              kernel_t1 out_data;
              in_data = local_kernel[c9][c7][0][0];
              out_data = in_data;
              fifo_kernel_local_out.write(out_data);
            }
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L2_in_inter_trans(int idx, float local_kernel[4][16][1][1], hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1) {
    // io_L2
    if (c4 == p0) {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            kernel_t1 in_data;
            kernel_t1 out_data;
            in_data = fifo_kernel_in.read();
            out_data = in_data;
            local_kernel[c5][c6][0][0] = out_data;
          }
        }
    } else {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            kernel_t1 in_data;
            kernel_t1 out_data;
            in_data = fifo_kernel_in.read();
            out_data = in_data;
            fifo_kernel_out.write(out_data);
          }
        }
    }
  }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L2_in_inter_trans_boundary(int idx, float local_kernel[4][16][1][1], hls::stream<float> &fifo_kernel_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1)
    if (c4 == p0) {
      // io_L2
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            kernel_t1 in_data;
            kernel_t1 out_data;
            in_data = fifo_kernel_in.read();
            out_data = in_data;
            local_kernel[c5][c6][0][0] = out_data;
          }
        }
    }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L2_in(int idx, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  kernel_t1 local_kernel_ping[4][16][1][1];
  #pragma HLS RESOURCE variable=local_kernel_ping core=RAM_1P_BRAM
  float local_kernel_pong[4][16][1][1];
  #pragma HLS RESOURCE variable=local_kernel_pong core=RAM_1P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  /* Variable Declaration */

  {
    // array
    // io_L3
    {
      if (arb == 0) {
        kernel_IO_L2_in_inter_trans(
          /* module id */ idx, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_in, 
          /* fifo */ fifo_kernel_out, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L2_in_intra_trans(
          /* module id */ idx, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        kernel_IO_L2_in_inter_trans(
          /* module id */ idx, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_in, 
          /* fifo */ fifo_kernel_out, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L2_in_intra_trans(
          /* module id */ idx, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      }
      intra_trans_en = 1;
      arb = !arb;
    }
    if (arb == 0) {
      kernel_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* array */ local_kernel_ping, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      kernel_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* array */ local_kernel_pong, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L2_in_boundary(int idx, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  kernel_t1 local_kernel_ping[4][16][1][1];
  #pragma HLS RESOURCE variable=local_kernel_ping core=RAM_1P_BRAM
  float local_kernel_pong[4][16][1][1];
  #pragma HLS RESOURCE variable=local_kernel_pong core=RAM_1P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  /* Variable Declaration */

  {
    // array
    // io_L3
    {
      if (arb == 0) {
        kernel_IO_L2_in_inter_trans_boundary(
          /* module id */ idx, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_in, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L2_in_intra_trans(
          /* module id */ idx, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        kernel_IO_L2_in_inter_trans_boundary(
          /* module id */ idx, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_in, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L2_in_intra_trans(
          /* module id */ idx, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      }
      intra_trans_en = 1;
      arb = !arb;
    }
    if (arb == 0) {
      kernel_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* array */ local_kernel_ping, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      kernel_IO_L2_in_intra_trans(
        /* module id */ idx, 
        /* array */ local_kernel_pong, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void PE(int idx, int idy, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, hls::stream<float> &fifo_output_drain_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  input_t1 local_input[1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_input dim=0 complete
  kernel_t1 local_kernel[1][1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_kernel dim=0 complete
  output_t1 local_output[4][2][14];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // pe
  for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
    for (ap_uint<5> c7 = 0; c7 <= 15; c7 += 1) {
      // latency
      for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
        // latency
        for (ap_uint<3> c9 = 0; c9 <= 3; c9 += 1) {
          // latency
          for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
          #pragma HLS PIPELINE II=1
            {
              local_input[0][0][0] = fifo_input_in.read();
              local_kernel[0][0][0][0] = fifo_kernel_in.read();
              local_output[c9][c10][2*c6 + c8] = (local_output[c9][c10][2*c6 + c8] + (local_input[0][0][0] * local_kernel[0][0][0][0]));
              if (c7 == 15)
                fifo_output_drain_out.write(local_output[c9][c10][2*c6 + c8]);
              fifo_kernel_out.write(local_kernel[0][0][0][0]);
            }
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, int idy, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, hls::stream<float> &fifo_output_drain_out)
 {
  PE(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_input_in, 
    /* fifo */ fifo_kernel_in, 
    /* fifo */ fifo_kernel_out, 
    /* fifo */ fifo_output_drain_out);
}
/* Module Definition */

/* Module Definition */
void kernel_PE_dummy_in(int idx, int idy, hls::stream<float> &fifo_kernel_in) {
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  // array
  // pe
  for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
    for (ap_uint<5> c7 = 0; c7 <= 15; c7 += 1) {
      // latency
      for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
        // latency
        for (ap_uint<3> c9 = 0; c9 <= 3; c9 += 1) {
          // latency
          for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
          #pragma HLS PIPELINE II=1
            kernel_t1 fifo_data;
            fifo_data = fifo_kernel_in.read();
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_intra_trans(int idx, int idy, output_t2 local_output[4][2][7], hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  ap_uint<32> data_split[2];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */


  // io_L1
  // pe
  for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1) {
    // latency
    for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
      // latency
      for (ap_uint<3> c9 = 0; c9 <= 3; c9 += 1) {
        // latency
        for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
        #pragma HLS PIPELINE II=1
          {
            output_t1 in_data;
            output_t2 out_data;
            in_data = fifo_output_drain_local_in.read();
            int split_idx = (2*c6 + c8) % 2;
            out_data = local_output[c9][c10][(2 * c6 + c8) / 2];
            for (ap_uint<2> n = 0; n < 2; n++) {
            #pragma HLS UNROLL
              data_split[n] = out_data(31, 0);
              out_data = out_data >> 32;
            }
            union {unsigned int ui; float ut;} u;
            u.ut = in_data;
            data_split[split_idx] = ap_uint<32>(u.ui);
            out_data = (data_split[1], data_split[0]);            local_output[c9][c10][(2 * c6 + c8) / 2] = out_data;
          }
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_inter_trans(int idx, int idy, output_t2 local_output[4][2][7], hls::stream<output_t2> &fifo_output_drain_in, hls::stream<output_t2> &fifo_output_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<4> c5 = p1; c5 <= 6; c5 += 1) {
    // io_L1
    if (c5 == p1) {
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
        for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
          // access_coalesce
          for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
          #pragma HLS PIPELINE II=1
            {
              output_t2 in_data;
              output_t2 out_data;
              in_data = local_output[c6][c7][c8];
              out_data = in_data;
              fifo_output_drain_out.write(out_data);
            }
          }
        }
    } else {
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
        for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
          // access_coalesce
          for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
          #pragma HLS PIPELINE II=1
            {
              output_t2 in_data;
              output_t2 out_data;
              in_data = fifo_output_drain_in.read();
              out_data = in_data;
              fifo_output_drain_out.write(out_data);
            }
          }
        }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_inter_trans_boundary(int idx, int idy, output_t2 local_output[4][2][7], hls::stream<output_t2> &fifo_output_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<4> c5 = p1; c5 <= 6; c5 += 1)
    if (c5 == p1) {
      // io_L1
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
        for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
          // access_coalesce
          for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
          #pragma HLS PIPELINE II=1
            {
              output_t2 in_data;
              output_t2 out_data;
              in_data = local_output[c6][c7][c8];
              out_data = in_data;
              fifo_output_drain_out.write(out_data);
            }
          }
        }
    }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out(int idx, int idy, hls::stream<output_t2> &fifo_output_drain_in, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  output_t2 local_output[4][2][7];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // io_L3
  // io_L2
  output_drain_IO_L1_out_intra_trans(
    /* module id */ idx, 
    /* module id */ idy, 
    /* array */ local_output, 
    /* fifo */ fifo_output_drain_local_in
  );
  output_drain_IO_L1_out_inter_trans(
    /* module id */ idx, 
    /* module id */ idy, 
    /* array */ local_output, 
    /* fifo */ fifo_output_drain_in, 
    /* fifo */ fifo_output_drain_out
  );
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_wrapper(int idx, int idy, hls::stream<output_t2> &fifo_output_drain_in, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in)
 {
  output_drain_IO_L1_out(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_output_drain_in, 
    /* fifo */ fifo_output_drain_out, 
    /* fifo */ fifo_output_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_boundary(int idx, int idy, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  output_t2 local_output[4][2][7];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // io_L3
  // io_L2
  output_drain_IO_L1_out_intra_trans(
    /* module id */ idx, 
    /* module id */ idy, 
    /* array */ local_output, 
    /* fifo */ fifo_output_drain_local_in
  );
  output_drain_IO_L1_out_inter_trans_boundary(
    /* module id */ idx, 
    /* module id */ idy, 
    /* array */ local_output, 
    /* fifo */ fifo_output_drain_out
  );
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_boundary_wrapper(int idx, int idy, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in)
 {
  output_drain_IO_L1_out_boundary(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_output_drain_out, 
    /* fifo */ fifo_output_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L2_out(int idx, hls::stream<output_t2> &fifo_output_drain_in, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<output_t2> &fifo_output_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1) {
    // io_L2
    if (c4 == p0) {
      for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1) {
        // io_L1
        for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
          for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
            // access_coalesce
            for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
            #pragma HLS PIPELINE II=1
              {
                output_t2 in_data;
                output_t2 out_data;
                in_data = fifo_output_drain_local_in.read();
                out_data = in_data;
                fifo_output_drain_out.write(out_data);
              }
            }
          }
      }
    } else {
      for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1) {
        // io_L1
        for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
          for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
            // access_coalesce
            for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
            #pragma HLS PIPELINE II=1
              {
                output_t2 in_data;
                output_t2 out_data;
                in_data = fifo_output_drain_in.read();
                out_data = in_data;
                fifo_output_drain_out.write(out_data);
              }
            }
          }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L2_out_boundary(int idx, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<output_t2> &fifo_output_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1)
    if (c4 == p0) {
      // io_L2
      for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1) {
        // io_L1
        for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
          for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
            // access_coalesce
            for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
            #pragma HLS PIPELINE II=1
              {
                output_t2 in_data;
                output_t2 out_data;
                in_data = fifo_output_drain_local_in.read();
                out_data = in_data;
                fifo_output_drain_out.write(out_data);
              }
            }
          }
      }
    }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L3_out(output_t2 *output, hls::stream<output_t2> &fifo_output_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    // io_L2
    for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1) {
      // io_L1
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
        for (ap_uint<2> c7 = 0; c7 <= 1; c7 += 1) {
          // access_coalesce
          for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
          #pragma HLS PIPELINE II=1
            {
              output_t2 in_data;
              output_t2 out_data;
              in_data = fifo_output_drain_local_in.read();
              out_data = in_data;
              output[392*c4 + 14*c5 + 98*c6 + 7*c7 + c8] = out_data;
            }
          }
        }
    }
  }
}
/* Module Definition */

void kernel0(input_t2 *input, kernel_t1 *kernel, output_t2 *output)
{
#pragma HLS INTERFACE m_axi port=input offset=slave bundle=gmem_input
#pragma HLS INTERFACE m_axi port=kernel offset=slave bundle=gmem_kernel
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem_output
#pragma HLS INTERFACE s_axilite port=input bundle=control
#pragma HLS INTERFACE s_axilite port=kernel bundle=control
#pragma HLS INTERFACE s_axilite port=output bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATAFLOW

  /* FIFO Declaration */
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_0 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_1 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_2 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_3 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_0_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_0_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_0_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_0_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_0_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_0_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_0_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_0_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_0_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_0_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_0_5;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_0_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_0_5 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_0_6;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_0_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_0_6 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_0_7;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_0_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_0_7 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_1_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_1_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_1_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_1_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_1_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_1_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_1_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_1_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_1_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_1_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_1_5;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_1_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_1_5 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_1_6;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_1_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_1_6 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_1_7;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_1_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_1_7 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_2_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_2_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_2_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_2_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_2_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_2_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_2_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_2_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_2_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_2_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_2_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_2_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_2_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_2_5;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_2_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_2_5 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_2_6;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_2_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_2_6 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_2_7;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_2_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_2_7 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_3_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_3_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_3_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_3_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_3_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_3_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_3_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_3_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_3_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_3_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_3_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_3_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_3_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_3_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_3_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_3_5;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_3_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_3_5 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_3_6;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_3_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_3_6 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_3_7;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_3_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_3_7 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_0 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_1 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_2 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_3 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_0;
  #pragma HLS STREAM variable=fifo_input_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_0;
  #pragma HLS STREAM variable=fifo_input_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_0;
  #pragma HLS STREAM variable=fifo_input_PE_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_0;
  #pragma HLS STREAM variable=fifo_input_PE_3_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_4_0;
  #pragma HLS STREAM variable=fifo_input_PE_4_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_4_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_5_0;
  #pragma HLS STREAM variable=fifo_input_PE_5_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_5_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_6_0;
  #pragma HLS STREAM variable=fifo_input_PE_6_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_6_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_1;
  #pragma HLS STREAM variable=fifo_input_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_1;
  #pragma HLS STREAM variable=fifo_input_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_1;
  #pragma HLS STREAM variable=fifo_input_PE_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_1;
  #pragma HLS STREAM variable=fifo_input_PE_3_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_4_1;
  #pragma HLS STREAM variable=fifo_input_PE_4_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_4_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_5_1;
  #pragma HLS STREAM variable=fifo_input_PE_5_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_5_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_6_1;
  #pragma HLS STREAM variable=fifo_input_PE_6_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_6_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_2;
  #pragma HLS STREAM variable=fifo_input_PE_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_2;
  #pragma HLS STREAM variable=fifo_input_PE_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_2;
  #pragma HLS STREAM variable=fifo_input_PE_2_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_2;
  #pragma HLS STREAM variable=fifo_input_PE_3_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_4_2;
  #pragma HLS STREAM variable=fifo_input_PE_4_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_4_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_5_2;
  #pragma HLS STREAM variable=fifo_input_PE_5_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_5_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_6_2;
  #pragma HLS STREAM variable=fifo_input_PE_6_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_6_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_3;
  #pragma HLS STREAM variable=fifo_input_PE_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_3;
  #pragma HLS STREAM variable=fifo_input_PE_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_3;
  #pragma HLS STREAM variable=fifo_input_PE_2_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_3;
  #pragma HLS STREAM variable=fifo_input_PE_3_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_4_3;
  #pragma HLS STREAM variable=fifo_input_PE_4_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_4_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_5_3;
  #pragma HLS STREAM variable=fifo_input_PE_5_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_5_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_6_3;
  #pragma HLS STREAM variable=fifo_input_PE_6_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_6_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_0;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_0;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_0;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_0;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_4_0;
  #pragma HLS STREAM variable=fifo_kernel_PE_4_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_4_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_5_0;
  #pragma HLS STREAM variable=fifo_kernel_PE_5_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_5_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_6_0;
  #pragma HLS STREAM variable=fifo_kernel_PE_6_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_6_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_7_0;
  #pragma HLS STREAM variable=fifo_kernel_PE_7_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_7_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_1;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_1;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_1;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_1;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_4_1;
  #pragma HLS STREAM variable=fifo_kernel_PE_4_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_4_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_5_1;
  #pragma HLS STREAM variable=fifo_kernel_PE_5_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_5_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_6_1;
  #pragma HLS STREAM variable=fifo_kernel_PE_6_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_6_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_7_1;
  #pragma HLS STREAM variable=fifo_kernel_PE_7_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_7_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_2;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_2;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_2;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_2;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_4_2;
  #pragma HLS STREAM variable=fifo_kernel_PE_4_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_4_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_5_2;
  #pragma HLS STREAM variable=fifo_kernel_PE_5_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_5_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_6_2;
  #pragma HLS STREAM variable=fifo_kernel_PE_6_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_6_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_7_2;
  #pragma HLS STREAM variable=fifo_kernel_PE_7_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_7_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_3;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_3;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_3;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_3;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_4_3;
  #pragma HLS STREAM variable=fifo_kernel_PE_4_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_4_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_5_3;
  #pragma HLS STREAM variable=fifo_kernel_PE_5_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_5_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_6_3;
  #pragma HLS STREAM variable=fifo_kernel_PE_6_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_6_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_7_3;
  #pragma HLS STREAM variable=fifo_kernel_PE_7_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_7_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_0;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_0;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_0;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_0;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_4_0;
  #pragma HLS STREAM variable=fifo_output_drain_PE_4_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_4_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_5_0;
  #pragma HLS STREAM variable=fifo_output_drain_PE_5_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_5_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_6_0;
  #pragma HLS STREAM variable=fifo_output_drain_PE_6_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_6_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_1;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_1;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_1;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_1;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_4_1;
  #pragma HLS STREAM variable=fifo_output_drain_PE_4_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_4_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_5_1;
  #pragma HLS STREAM variable=fifo_output_drain_PE_5_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_5_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_6_1;
  #pragma HLS STREAM variable=fifo_output_drain_PE_6_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_6_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_2;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_2;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_2;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_2;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_4_2;
  #pragma HLS STREAM variable=fifo_output_drain_PE_4_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_4_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_5_2;
  #pragma HLS STREAM variable=fifo_output_drain_PE_5_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_5_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_6_2;
  #pragma HLS STREAM variable=fifo_output_drain_PE_6_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_6_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_3;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_3;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_3;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_3;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_4_3;
  #pragma HLS STREAM variable=fifo_output_drain_PE_4_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_4_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_5_3;
  #pragma HLS STREAM variable=fifo_output_drain_PE_5_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_5_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_6_3;
  #pragma HLS STREAM variable=fifo_output_drain_PE_6_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_6_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_0_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_0_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_0_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_0_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_0_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_0_5;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0_5 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_0_6;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0_6 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_0_7;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0_7 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_1_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_1_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_1_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_1_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_1_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_1_5;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1_5 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_1_6;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1_6 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_1_7;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1_7 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_2_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_2_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_2_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_2_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_2_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_2_5;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2_5 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_2_6;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2_6 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_2_7;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2_7 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_3_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_3_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_3_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_3_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_3_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_3_5;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3_5 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_3_6;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3_6 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_3_7;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3_7 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_0 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_1 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_2 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_3 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_4 core=FIFO_SRL
  /* FIFO Declaration */

  /* Module Call */
  input_IO_L3_in(
    /* array */ input,
    /* fifo */ fifo_input_input_IO_L2_in_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L2_in_0,
    /* fifo */ fifo_input_input_IO_L2_in_1,
    /* fifo */ fifo_input_input_IO_L1_in_0_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L2_in_1,
    /* fifo */ fifo_input_input_IO_L2_in_2,
    /* fifo */ fifo_input_input_IO_L1_in_1_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L2_in_2,
    /* fifo */ fifo_input_input_IO_L2_in_3,
    /* fifo */ fifo_input_input_IO_L1_in_2_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in_boundary(
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L2_in_3,
    /* fifo */ fifo_input_input_IO_L1_in_3_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_0_0,
    /* fifo */ fifo_input_input_IO_L1_in_0_1,
    /* fifo */ fifo_input_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_0_1,
    /* fifo */ fifo_input_input_IO_L1_in_0_2,
    /* fifo */ fifo_input_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_0_2,
    /* fifo */ fifo_input_input_IO_L1_in_0_3,
    /* fifo */ fifo_input_PE_2_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_0_3,
    /* fifo */ fifo_input_input_IO_L1_in_0_4,
    /* fifo */ fifo_input_PE_3_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 4,
    /* fifo */ fifo_input_input_IO_L1_in_0_4,
    /* fifo */ fifo_input_input_IO_L1_in_0_5,
    /* fifo */ fifo_input_PE_4_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 5,
    /* fifo */ fifo_input_input_IO_L1_in_0_5,
    /* fifo */ fifo_input_input_IO_L1_in_0_6,
    /* fifo */ fifo_input_PE_5_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 0,
    /* module id */ 6,
    /* fifo */ fifo_input_input_IO_L1_in_0_6,
    /* fifo */ fifo_input_PE_6_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_1_0,
    /* fifo */ fifo_input_input_IO_L1_in_1_1,
    /* fifo */ fifo_input_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_1_1,
    /* fifo */ fifo_input_input_IO_L1_in_1_2,
    /* fifo */ fifo_input_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_1_2,
    /* fifo */ fifo_input_input_IO_L1_in_1_3,
    /* fifo */ fifo_input_PE_2_1
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_1_3,
    /* fifo */ fifo_input_input_IO_L1_in_1_4,
    /* fifo */ fifo_input_PE_3_1
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 4,
    /* fifo */ fifo_input_input_IO_L1_in_1_4,
    /* fifo */ fifo_input_input_IO_L1_in_1_5,
    /* fifo */ fifo_input_PE_4_1
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 5,
    /* fifo */ fifo_input_input_IO_L1_in_1_5,
    /* fifo */ fifo_input_input_IO_L1_in_1_6,
    /* fifo */ fifo_input_PE_5_1
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 1,
    /* module id */ 6,
    /* fifo */ fifo_input_input_IO_L1_in_1_6,
    /* fifo */ fifo_input_PE_6_1
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_2_0,
    /* fifo */ fifo_input_input_IO_L1_in_2_1,
    /* fifo */ fifo_input_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_2_1,
    /* fifo */ fifo_input_input_IO_L1_in_2_2,
    /* fifo */ fifo_input_PE_1_2
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_2_2,
    /* fifo */ fifo_input_input_IO_L1_in_2_3,
    /* fifo */ fifo_input_PE_2_2
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_2_3,
    /* fifo */ fifo_input_input_IO_L1_in_2_4,
    /* fifo */ fifo_input_PE_3_2
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 4,
    /* fifo */ fifo_input_input_IO_L1_in_2_4,
    /* fifo */ fifo_input_input_IO_L1_in_2_5,
    /* fifo */ fifo_input_PE_4_2
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 5,
    /* fifo */ fifo_input_input_IO_L1_in_2_5,
    /* fifo */ fifo_input_input_IO_L1_in_2_6,
    /* fifo */ fifo_input_PE_5_2
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 2,
    /* module id */ 6,
    /* fifo */ fifo_input_input_IO_L1_in_2_6,
    /* fifo */ fifo_input_PE_6_2
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_3_0,
    /* fifo */ fifo_input_input_IO_L1_in_3_1,
    /* fifo */ fifo_input_PE_0_3
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_3_1,
    /* fifo */ fifo_input_input_IO_L1_in_3_2,
    /* fifo */ fifo_input_PE_1_3
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_3_2,
    /* fifo */ fifo_input_input_IO_L1_in_3_3,
    /* fifo */ fifo_input_PE_2_3
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_3_3,
    /* fifo */ fifo_input_input_IO_L1_in_3_4,
    /* fifo */ fifo_input_PE_3_3
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 4,
    /* fifo */ fifo_input_input_IO_L1_in_3_4,
    /* fifo */ fifo_input_input_IO_L1_in_3_5,
    /* fifo */ fifo_input_PE_4_3
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 5,
    /* fifo */ fifo_input_input_IO_L1_in_3_5,
    /* fifo */ fifo_input_input_IO_L1_in_3_6,
    /* fifo */ fifo_input_PE_5_3
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 3,
    /* module id */ 6,
    /* fifo */ fifo_input_input_IO_L1_in_3_6,
    /* fifo */ fifo_input_PE_6_3
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L3_in(
    /* array */ kernel,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_0,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_1,
    /* fifo */ fifo_kernel_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_1,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_2,
    /* fifo */ fifo_kernel_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_2,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_3,
    /* fifo */ fifo_kernel_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in_boundary(
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_3,
    /* fifo */ fifo_kernel_PE_0_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_0_0,
    /* fifo */ fifo_kernel_PE_0_0,
    /* fifo */ fifo_kernel_PE_1_0,
    /* fifo */ fifo_output_drain_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_0_1,
    /* fifo */ fifo_kernel_PE_0_1,
    /* fifo */ fifo_kernel_PE_1_1,
    /* fifo */ fifo_output_drain_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_0_2,
    /* fifo */ fifo_kernel_PE_0_2,
    /* fifo */ fifo_kernel_PE_1_2,
    /* fifo */ fifo_output_drain_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_0_3,
    /* fifo */ fifo_kernel_PE_0_3,
    /* fifo */ fifo_kernel_PE_1_3,
    /* fifo */ fifo_output_drain_PE_0_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_1_0,
    /* fifo */ fifo_kernel_PE_1_0,
    /* fifo */ fifo_kernel_PE_2_0,
    /* fifo */ fifo_output_drain_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_1_1,
    /* fifo */ fifo_kernel_PE_1_1,
    /* fifo */ fifo_kernel_PE_2_1,
    /* fifo */ fifo_output_drain_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_1_2,
    /* fifo */ fifo_kernel_PE_1_2,
    /* fifo */ fifo_kernel_PE_2_2,
    /* fifo */ fifo_output_drain_PE_1_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_1_3,
    /* fifo */ fifo_kernel_PE_1_3,
    /* fifo */ fifo_kernel_PE_2_3,
    /* fifo */ fifo_output_drain_PE_1_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_2_0,
    /* fifo */ fifo_kernel_PE_2_0,
    /* fifo */ fifo_kernel_PE_3_0,
    /* fifo */ fifo_output_drain_PE_2_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_2_1,
    /* fifo */ fifo_kernel_PE_2_1,
    /* fifo */ fifo_kernel_PE_3_1,
    /* fifo */ fifo_output_drain_PE_2_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_2_2,
    /* fifo */ fifo_kernel_PE_2_2,
    /* fifo */ fifo_kernel_PE_3_2,
    /* fifo */ fifo_output_drain_PE_2_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_2_3,
    /* fifo */ fifo_kernel_PE_2_3,
    /* fifo */ fifo_kernel_PE_3_3,
    /* fifo */ fifo_output_drain_PE_2_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_3_0,
    /* fifo */ fifo_kernel_PE_3_0,
    /* fifo */ fifo_kernel_PE_4_0,
    /* fifo */ fifo_output_drain_PE_3_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_3_1,
    /* fifo */ fifo_kernel_PE_3_1,
    /* fifo */ fifo_kernel_PE_4_1,
    /* fifo */ fifo_output_drain_PE_3_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_3_2,
    /* fifo */ fifo_kernel_PE_3_2,
    /* fifo */ fifo_kernel_PE_4_2,
    /* fifo */ fifo_output_drain_PE_3_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_3_3,
    /* fifo */ fifo_kernel_PE_3_3,
    /* fifo */ fifo_kernel_PE_4_3,
    /* fifo */ fifo_output_drain_PE_3_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 4,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_4_0,
    /* fifo */ fifo_kernel_PE_4_0,
    /* fifo */ fifo_kernel_PE_5_0,
    /* fifo */ fifo_output_drain_PE_4_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 4,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_4_1,
    /* fifo */ fifo_kernel_PE_4_1,
    /* fifo */ fifo_kernel_PE_5_1,
    /* fifo */ fifo_output_drain_PE_4_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 4,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_4_2,
    /* fifo */ fifo_kernel_PE_4_2,
    /* fifo */ fifo_kernel_PE_5_2,
    /* fifo */ fifo_output_drain_PE_4_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 4,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_4_3,
    /* fifo */ fifo_kernel_PE_4_3,
    /* fifo */ fifo_kernel_PE_5_3,
    /* fifo */ fifo_output_drain_PE_4_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 5,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_5_0,
    /* fifo */ fifo_kernel_PE_5_0,
    /* fifo */ fifo_kernel_PE_6_0,
    /* fifo */ fifo_output_drain_PE_5_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 5,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_5_1,
    /* fifo */ fifo_kernel_PE_5_1,
    /* fifo */ fifo_kernel_PE_6_1,
    /* fifo */ fifo_output_drain_PE_5_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 5,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_5_2,
    /* fifo */ fifo_kernel_PE_5_2,
    /* fifo */ fifo_kernel_PE_6_2,
    /* fifo */ fifo_output_drain_PE_5_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 5,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_5_3,
    /* fifo */ fifo_kernel_PE_5_3,
    /* fifo */ fifo_kernel_PE_6_3,
    /* fifo */ fifo_output_drain_PE_5_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 6,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_6_0,
    /* fifo */ fifo_kernel_PE_6_0,
    /* fifo */ fifo_kernel_PE_7_0,
    /* fifo */ fifo_output_drain_PE_6_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 6,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_6_1,
    /* fifo */ fifo_kernel_PE_6_1,
    /* fifo */ fifo_kernel_PE_7_1,
    /* fifo */ fifo_output_drain_PE_6_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 6,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_6_2,
    /* fifo */ fifo_kernel_PE_6_2,
    /* fifo */ fifo_kernel_PE_7_2,
    /* fifo */ fifo_output_drain_PE_6_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 6,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_6_3,
    /* fifo */ fifo_kernel_PE_6_3,
    /* fifo */ fifo_kernel_PE_7_3,
    /* fifo */ fifo_output_drain_PE_6_3
  );
  /* Module Call */

  /* Module Call */
  kernel_PE_dummy_in(
    /* module id */ 6,
    /* module id */ 0,
    /* fifo */ fifo_kernel_PE_7_0
  );
  /* Module Call */

  /* Module Call */
  kernel_PE_dummy_in(
    /* module id */ 6,
    /* module id */ 1,
    /* fifo */ fifo_kernel_PE_7_1
  );
  /* Module Call */

  /* Module Call */
  kernel_PE_dummy_in(
    /* module id */ 6,
    /* module id */ 2,
    /* fifo */ fifo_kernel_PE_7_2
  );
  /* Module Call */

  /* Module Call */
  kernel_PE_dummy_in(
    /* module id */ 6,
    /* module id */ 3,
    /* fifo */ fifo_kernel_PE_7_3
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 0,
    /* module id */ 6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_6,
    /* fifo */ fifo_output_drain_PE_6_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* module id */ 5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_5,
    /* fifo */ fifo_output_drain_PE_5_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* module id */ 4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_4,
    /* fifo */ fifo_output_drain_PE_4_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* module id */ 3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_3,
    /* fifo */ fifo_output_drain_PE_3_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* module id */ 2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_2,
    /* fifo */ fifo_output_drain_PE_2_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_1,
    /* fifo */ fifo_output_drain_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_0,
    /* fifo */ fifo_output_drain_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 1,
    /* module id */ 6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_6,
    /* fifo */ fifo_output_drain_PE_6_1
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* module id */ 5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_5,
    /* fifo */ fifo_output_drain_PE_5_1
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* module id */ 4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_4,
    /* fifo */ fifo_output_drain_PE_4_1
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* module id */ 3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_3,
    /* fifo */ fifo_output_drain_PE_3_1
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* module id */ 2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_2,
    /* fifo */ fifo_output_drain_PE_2_1
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_1,
    /* fifo */ fifo_output_drain_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_0,
    /* fifo */ fifo_output_drain_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 2,
    /* module id */ 6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_6,
    /* fifo */ fifo_output_drain_PE_6_2
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 2,
    /* module id */ 5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_5,
    /* fifo */ fifo_output_drain_PE_5_2
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 2,
    /* module id */ 4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_4,
    /* fifo */ fifo_output_drain_PE_4_2
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 2,
    /* module id */ 3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_3,
    /* fifo */ fifo_output_drain_PE_3_2
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 2,
    /* module id */ 2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_2,
    /* fifo */ fifo_output_drain_PE_2_2
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 2,
    /* module id */ 1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_1,
    /* fifo */ fifo_output_drain_PE_1_2
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 2,
    /* module id */ 0,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_0,
    /* fifo */ fifo_output_drain_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 3,
    /* module id */ 6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_6,
    /* fifo */ fifo_output_drain_PE_6_3
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 3,
    /* module id */ 5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_5,
    /* fifo */ fifo_output_drain_PE_5_3
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 3,
    /* module id */ 4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_4,
    /* fifo */ fifo_output_drain_PE_4_3
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 3,
    /* module id */ 3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_3,
    /* fifo */ fifo_output_drain_PE_3_3
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 3,
    /* module id */ 2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_2,
    /* fifo */ fifo_output_drain_PE_2_3
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 3,
    /* module id */ 1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_1,
    /* fifo */ fifo_output_drain_PE_1_3
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 3,
    /* module id */ 0,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_0,
    /* fifo */ fifo_output_drain_PE_0_3
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out_boundary(
    /* module id */ 3,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 2,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_3,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 1,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_2,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 0,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_1,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_0,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L3_out(
    /* array */ output,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_0
  );
  /* Module Call */

}
