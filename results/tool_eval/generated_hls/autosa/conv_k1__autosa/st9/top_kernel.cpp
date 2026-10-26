#include "top_kernel.h"
/* Module Definition */
void input_IO_L3_in(input_t2 *input, hls::stream<input_t2> &fifo_input_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
    // io_L2
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // io_L1
      for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
        // access_coalesce
        for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
        #pragma HLS PIPELINE II=1
          {
            input_t2 in_data;
            input_t2 out_data;
            in_data = input[98*c4 + 7*c7 + c8];
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
  for (ap_uint<5> c4 = p0; c4 <= 15; c4 += 1) {
    // io_L2
    if (c4 == p0) {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
        // io_L1
        for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
        // io_L1
        for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
  for (ap_uint<5> c4 = p0; c4 <= 15; c4 += 1)
    if (c4 == p0) {
      // io_L2
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
        // io_L1
        for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
void input_IO_L1_in_intra_trans(int idx, int idy, input_t2 local_input[1][14][7], hls::stream<float> &fifo_input_local_out, bool intra_trans_en) {
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
    for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
      // latency
      for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
        // latency
        for (ap_uint<2> c9 = 0; c9 <= 1; c9 += 1) {
          // latency
          for (ap_uint<3> c10 = 0; c10 <= 3; c10 += 1) {
          #pragma HLS PIPELINE II=1
            {
              input_t2 in_data;
              input_t1 out_data;
              in_data = local_input[0][2*c7 + c8][(2 * c6 + c9) / 2];
              for (ap_uint<2> n = 0; n < 2; n++) {
              #pragma HLS UNROLL
                data_split[n] = in_data(31, 0);
                in_data = in_data >> 32;
              }
              int split_idx = (2*c6 + c9) % 2;
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
void input_IO_L1_in_inter_trans(int idx, int idy, input_t2 local_input[1][14][7], hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c5 = p1; c5 <= 3; c5 += 1) {
    // io_L1
    if (c5 == p1) {
      for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
        // access_coalesce
        for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
        #pragma HLS PIPELINE II=1
          {
            input_t2 in_data;
            input_t2 out_data;
            in_data = fifo_input_in.read();
            out_data = in_data;
            local_input[0][c7][c8] = out_data;
          }
        }
      }
    } else {
      for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
void input_IO_L1_in_inter_trans_boundary(int idx, int idy, input_t2 local_input[1][14][7], hls::stream<input_t2> &fifo_input_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c5 = p1; c5 <= 3; c5 += 1)
    if (c5 == p1) {
      // io_L1
      for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
        // access_coalesce
        for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
        #pragma HLS PIPELINE II=1
          {
            input_t2 in_data;
            input_t2 out_data;
            in_data = fifo_input_in.read();
            out_data = in_data;
            local_input[0][c7][c8] = out_data;
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
  input_t2 local_input_ping[1][14][7];
  #pragma HLS RESOURCE variable=local_input_ping core=RAM_2P_BRAM
  input_t2 local_input_pong[1][14][7];
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
  input_t2 local_input_ping[1][14][7];
  #pragma HLS RESOURCE variable=local_input_ping core=RAM_2P_BRAM
  input_t2 local_input_pong[1][14][7];
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
  for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
    // io_L2
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // io_L1
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          kernel_t1 in_data;
          kernel_t1 out_data;
          in_data = kernel[c4 + 64*c5 + 16*c6];
          out_data = in_data;
          fifo_kernel_local_out.write(out_data);
        }
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
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<5> c4 = p0; c4 <= 15; c4 += 1) {
    // io_L2
    if (c4 == p0) {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
        // io_L1
        for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            kernel_t1 in_data;
            kernel_t1 out_data;
            in_data = fifo_kernel_in.read();
            out_data = in_data;
            fifo_kernel_local_out.write(out_data);
          }
        }
      }
    } else {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
        // io_L1
        for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
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
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L2_in_boundary(int idx, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<5> c4 = p0; c4 <= 15; c4 += 1)
    if (c4 == p0) {
      // io_L2
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
        // io_L1
        for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            kernel_t1 in_data;
            kernel_t1 out_data;
            in_data = fifo_kernel_in.read();
            out_data = in_data;
            fifo_kernel_local_out.write(out_data);
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L1_in_intra_trans(int idx, int idy, float local_kernel[4][1][1][1], hls::stream<float> &fifo_kernel_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L1
  // pe
  for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
    for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
      // latency
      for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
        // latency
        for (ap_uint<2> c9 = 0; c9 <= 1; c9 += 1) {
          // latency
          for (ap_uint<3> c10 = 0; c10 <= 3; c10 += 1) {
          #pragma HLS PIPELINE II=1
            {
              kernel_t1 in_data;
              kernel_t1 out_data;
              in_data = local_kernel[c10][0][0][0];
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
void kernel_IO_L1_in_inter_trans(int idx, int idy, float local_kernel[4][1][1][1], hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c5 = p1; c5 <= 3; c5 += 1) {
    // io_L1
    if (c5 == p1) {
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          kernel_t1 in_data;
          kernel_t1 out_data;
          in_data = fifo_kernel_in.read();
          out_data = in_data;
          local_kernel[c6][0][0][0] = out_data;
        }
      }
    } else {
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
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
void kernel_IO_L1_in_inter_trans_boundary(int idx, int idy, float local_kernel[4][1][1][1], hls::stream<float> &fifo_kernel_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c5 = p1; c5 <= 3; c5 += 1)
    if (c5 == p1) {
      // io_L1
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          kernel_t1 in_data;
          kernel_t1 out_data;
          in_data = fifo_kernel_in.read();
          out_data = in_data;
          local_kernel[c6][0][0][0] = out_data;
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L1_in(int idx, int idy, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  kernel_t1 local_kernel_ping[4][1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_kernel_ping dim=0 complete
  float local_kernel_pong[4][1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_kernel_pong dim=0 complete
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
        kernel_IO_L1_in_inter_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_in, 
          /* fifo */ fifo_kernel_out, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        kernel_IO_L1_in_inter_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_in, 
          /* fifo */ fifo_kernel_out, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      }
      intra_trans_en = 1;
      arb = !arb;
    }
    if (arb == 0) {
      kernel_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* array */ local_kernel_ping, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      kernel_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* array */ local_kernel_pong, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L1_in_wrapper(int idx, int idy, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, hls::stream<float> &fifo_kernel_local_out)
 {
  kernel_IO_L1_in(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_kernel_in, 
    /* fifo */ fifo_kernel_out, 
    /* fifo */ fifo_kernel_local_out);
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L1_in_boundary(int idx, int idy, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  kernel_t1 local_kernel_ping[4][1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_kernel_ping dim=0 complete
  float local_kernel_pong[4][1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_kernel_pong dim=0 complete
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
        kernel_IO_L1_in_inter_trans_boundary(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_in, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        kernel_IO_L1_in_inter_trans_boundary(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_in, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* module id */ idy, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      }
      intra_trans_en = 1;
      arb = !arb;
    }
    if (arb == 0) {
      kernel_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* array */ local_kernel_ping, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      kernel_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* array */ local_kernel_pong, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L1_in_boundary_wrapper(int idx, int idy, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_local_out)
 {
  kernel_IO_L1_in_boundary(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_kernel_in, 
    /* fifo */ fifo_kernel_local_out);
}
/* Module Definition */

/* Module Definition */
void PE(int idx, int idy, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_drain_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  input_t1 local_input[1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_input dim=0 complete
  kernel_t1 local_kernel[1][1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_kernel dim=0 complete
  output_t1 local_output[4][14][14];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // pe
  for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
    for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
      // latency
      for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
        // latency
        for (ap_uint<2> c9 = 0; c9 <= 1; c9 += 1) {
          // latency
          for (ap_uint<3> c10 = 0; c10 <= 3; c10 += 1) {
          #pragma HLS PIPELINE II=1
            {
              local_input[0][0][0] = fifo_input_in.read();
              local_kernel[0][0][0][0] = fifo_kernel_in.read();
              if (p1 >= 1)
                local_output[c10][2*c7 + c8][2*c6 + c9] = fifo_output_in.read();
              local_output[c10][2*c7 + c8][2*c6 + c9] = (local_output[c10][2*c7 + c8][2*c6 + c9] + (local_input[0][0][0] * local_kernel[0][0][0][0]));
              if (p1 == 15) {
                fifo_output_drain_out.write(local_output[c10][2*c7 + c8][2*c6 + c9]);
              } else {
                fifo_output_out.write(local_output[c10][2*c7 + c8][2*c6 + c9]);
              }
            }
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, int idy, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_drain_out)
 {
  PE(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_input_in, 
    /* fifo */ fifo_kernel_in, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_output_out, 
    /* fifo */ fifo_output_drain_out);
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_intra_trans(int idx, int idy, output_t2 local_output[4][14][7], hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  ap_uint<32> data_split[2];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */


  // io_L1
  // pe
  for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
    for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
      // latency
      for (ap_uint<2> c8 = 0; c8 <= 1; c8 += 1) {
        // latency
        for (ap_uint<2> c9 = 0; c9 <= 1; c9 += 1) {
          // latency
          for (ap_uint<3> c10 = 0; c10 <= 3; c10 += 1) {
          #pragma HLS PIPELINE II=1
            {
              output_t1 in_data;
              output_t2 out_data;
              in_data = fifo_output_drain_local_in.read();
              int split_idx = (2*c6 + c9) % 2;
              out_data = local_output[c10][2*c7 + c8][(2 * c6 + c9) / 2];
              for (ap_uint<2> n = 0; n < 2; n++) {
              #pragma HLS UNROLL
                data_split[n] = out_data(31, 0);
                out_data = out_data >> 32;
              }
              union {unsigned int ui; float ut;} u;
              u.ut = in_data;
              data_split[split_idx] = ap_uint<32>(u.ui);
              out_data = (data_split[1], data_split[0]);              local_output[c10][2*c7 + c8][(2 * c6 + c9) / 2] = out_data;
            }
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_inter_trans(int idx, int idy, output_t2 local_output[4][14][7], hls::stream<output_t2> &fifo_output_drain_in, hls::stream<output_t2> &fifo_output_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<3> c5 = p1; c5 <= 3; c5 += 1) {
    // io_L1
    if (c5 == p1) {
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
        for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
        for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
void output_drain_IO_L1_out_inter_trans_boundary(int idx, int idy, output_t2 local_output[4][14][7], hls::stream<output_t2> &fifo_output_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<3> c5 = p1; c5 <= 3; c5 += 1)
    if (c5 == p1) {
      // io_L1
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
        for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
  output_t2 local_output[4][14][7];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  if (p0 == 15) {
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
  output_t2 local_output[4][14][7];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  if (p0 == 15) {
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
  // io_L2
  if (p0 == 15) {
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // io_L1
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
        for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // io_L1
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
        for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
void output_drain_IO_L2_out_boundary(int idx, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<output_t2> &fifo_output_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (p0 == 15) {
    // array
    // io_L3
    // io_L2
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
      // io_L1
      for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
        for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
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
  // io_L2
  for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1) {
    // io_L1
    for (ap_uint<3> c6 = 0; c6 <= 3; c6 += 1)
      for (ap_uint<5> c7 = 0; c7 <= 13; c7 += 1) {
        // access_coalesce
        for (ap_uint<4> c8 = 0; c8 <= 6; c8 += 1) {
        #pragma HLS PIPELINE II=1
          {
            output_t2 in_data;
            output_t2 out_data;
            in_data = fifo_output_drain_local_in.read();
            out_data = in_data;
            output[392*c5 + 98*c6 + 7*c7 + c8] = out_data;
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
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_5;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_5 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_6;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_6 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_7;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_7 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_8;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_8 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_8 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_9;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_9 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_9 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_10;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_10 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_10 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_11;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_11 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_11 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_12;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_12 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_12 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_13;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_13 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_13 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_14;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_14 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_14 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_15;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_15 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_15 core=FIFO_SRL
  /* input_IO_L2_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L2_in_16;
  #pragma HLS STREAM variable=fifo_input_input_IO_L2_in_16 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L2_in_16 core=FIFO_SRL
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
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_4_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_4_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_4_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_4_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_4_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_4_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_4_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_4_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_4_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_4_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_4_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_4_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_4_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_4_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_4_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_5_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_5_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_5_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_5_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_5_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_5_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_5_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_5_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_5_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_5_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_5_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_5_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_5_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_5_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_5_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_6_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_6_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_6_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_6_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_6_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_6_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_6_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_6_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_6_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_6_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_6_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_6_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_6_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_6_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_6_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_7_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_7_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_7_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_7_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_7_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_7_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_7_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_7_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_7_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_7_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_7_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_7_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_7_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_7_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_7_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_8_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_8_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_8_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_8_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_8_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_8_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_8_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_8_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_8_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_8_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_8_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_8_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_8_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_8_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_8_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_9_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_9_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_9_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_9_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_9_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_9_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_9_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_9_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_9_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_9_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_9_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_9_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_9_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_9_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_9_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_10_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_10_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_10_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_10_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_10_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_10_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_10_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_10_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_10_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_10_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_10_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_10_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_10_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_10_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_10_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_11_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_11_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_11_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_11_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_11_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_11_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_11_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_11_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_11_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_11_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_11_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_11_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_11_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_11_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_11_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_12_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_12_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_12_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_12_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_12_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_12_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_12_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_12_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_12_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_12_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_12_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_12_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_12_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_12_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_12_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_13_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_13_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_13_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_13_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_13_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_13_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_13_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_13_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_13_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_13_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_13_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_13_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_13_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_13_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_13_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_14_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_14_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_14_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_14_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_14_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_14_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_14_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_14_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_14_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_14_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_14_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_14_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_14_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_14_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_14_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_15_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_15_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_15_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_15_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_15_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_15_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_15_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_15_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_15_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_15_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_15_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_15_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_15_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_15_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_15_4 core=FIFO_SRL
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
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_5;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_5 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_5 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_6;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_6 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_6 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_7;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_7 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_7 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_8;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_8 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_8 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_9;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_9 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_9 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_10;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_10 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_10 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_11;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_11 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_11 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_12;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_12 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_12 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_13;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_13 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_13 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_14;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_14 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_14 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_15;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_15 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_15 core=FIFO_SRL
  /* kernel_IO_L2_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L2_in_16;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L2_in_16 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L2_in_16 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_0_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_0_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_0_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_0_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_0_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_0_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_0_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_0_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_0_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_0_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_1_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_1_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_1_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_1_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_1_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_1_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_1_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_1_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_1_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_1_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_2_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_2_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_2_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_2_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_2_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_2_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_2_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_2_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_2_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_2_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_2_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_2_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_2_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_3_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_3_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_3_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_3_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_3_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_3_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_3_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_3_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_3_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_3_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_3_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_3_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_3_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_3_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_3_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_4_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_4_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_4_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_4_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_4_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_4_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_4_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_4_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_4_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_4_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_4_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_4_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_4_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_4_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_4_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_5_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_5_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_5_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_5_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_5_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_5_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_5_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_5_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_5_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_5_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_5_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_5_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_5_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_5_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_5_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_6_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_6_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_6_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_6_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_6_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_6_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_6_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_6_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_6_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_6_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_6_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_6_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_6_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_6_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_6_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_7_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_7_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_7_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_7_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_7_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_7_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_7_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_7_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_7_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_7_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_7_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_7_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_7_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_7_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_7_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_8_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_8_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_8_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_8_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_8_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_8_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_8_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_8_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_8_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_8_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_8_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_8_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_8_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_8_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_8_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_9_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_9_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_9_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_9_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_9_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_9_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_9_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_9_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_9_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_9_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_9_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_9_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_9_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_9_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_9_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_10_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_10_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_10_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_10_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_10_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_10_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_10_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_10_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_10_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_10_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_10_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_10_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_10_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_10_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_10_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_11_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_11_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_11_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_11_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_11_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_11_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_11_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_11_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_11_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_11_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_11_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_11_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_11_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_11_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_11_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_12_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_12_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_12_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_12_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_12_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_12_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_12_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_12_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_12_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_12_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_12_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_12_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_12_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_12_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_12_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_13_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_13_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_13_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_13_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_13_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_13_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_13_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_13_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_13_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_13_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_13_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_13_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_13_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_13_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_13_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_14_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_14_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_14_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_14_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_14_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_14_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_14_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_14_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_14_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_14_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_14_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_14_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_14_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_14_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_14_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_15_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_15_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_15_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_15_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_15_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_15_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_15_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_15_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_15_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_15_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_15_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_15_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_15_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_15_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_15_4 core=FIFO_SRL
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
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_4;
  #pragma HLS STREAM variable=fifo_input_PE_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_4;
  #pragma HLS STREAM variable=fifo_input_PE_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_4;
  #pragma HLS STREAM variable=fifo_input_PE_2_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_4;
  #pragma HLS STREAM variable=fifo_input_PE_3_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_5;
  #pragma HLS STREAM variable=fifo_input_PE_0_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_5;
  #pragma HLS STREAM variable=fifo_input_PE_1_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_5;
  #pragma HLS STREAM variable=fifo_input_PE_2_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_5;
  #pragma HLS STREAM variable=fifo_input_PE_3_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_6;
  #pragma HLS STREAM variable=fifo_input_PE_0_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_6;
  #pragma HLS STREAM variable=fifo_input_PE_1_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_6;
  #pragma HLS STREAM variable=fifo_input_PE_2_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_6;
  #pragma HLS STREAM variable=fifo_input_PE_3_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_7;
  #pragma HLS STREAM variable=fifo_input_PE_0_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_7;
  #pragma HLS STREAM variable=fifo_input_PE_1_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_7;
  #pragma HLS STREAM variable=fifo_input_PE_2_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_7;
  #pragma HLS STREAM variable=fifo_input_PE_3_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_8;
  #pragma HLS STREAM variable=fifo_input_PE_0_8 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_8;
  #pragma HLS STREAM variable=fifo_input_PE_1_8 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_8;
  #pragma HLS STREAM variable=fifo_input_PE_2_8 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_8;
  #pragma HLS STREAM variable=fifo_input_PE_3_8 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_9;
  #pragma HLS STREAM variable=fifo_input_PE_0_9 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_9;
  #pragma HLS STREAM variable=fifo_input_PE_1_9 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_9;
  #pragma HLS STREAM variable=fifo_input_PE_2_9 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_9;
  #pragma HLS STREAM variable=fifo_input_PE_3_9 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_10;
  #pragma HLS STREAM variable=fifo_input_PE_0_10 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_10;
  #pragma HLS STREAM variable=fifo_input_PE_1_10 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_10;
  #pragma HLS STREAM variable=fifo_input_PE_2_10 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_10;
  #pragma HLS STREAM variable=fifo_input_PE_3_10 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_11;
  #pragma HLS STREAM variable=fifo_input_PE_0_11 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_11;
  #pragma HLS STREAM variable=fifo_input_PE_1_11 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_11;
  #pragma HLS STREAM variable=fifo_input_PE_2_11 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_11;
  #pragma HLS STREAM variable=fifo_input_PE_3_11 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_12;
  #pragma HLS STREAM variable=fifo_input_PE_0_12 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_12;
  #pragma HLS STREAM variable=fifo_input_PE_1_12 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_12;
  #pragma HLS STREAM variable=fifo_input_PE_2_12 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_12;
  #pragma HLS STREAM variable=fifo_input_PE_3_12 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_13;
  #pragma HLS STREAM variable=fifo_input_PE_0_13 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_13;
  #pragma HLS STREAM variable=fifo_input_PE_1_13 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_13;
  #pragma HLS STREAM variable=fifo_input_PE_2_13 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_13;
  #pragma HLS STREAM variable=fifo_input_PE_3_13 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_14;
  #pragma HLS STREAM variable=fifo_input_PE_0_14 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_14;
  #pragma HLS STREAM variable=fifo_input_PE_1_14 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_14;
  #pragma HLS STREAM variable=fifo_input_PE_2_14 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_14;
  #pragma HLS STREAM variable=fifo_input_PE_3_14 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0_15;
  #pragma HLS STREAM variable=fifo_input_PE_0_15 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1_15;
  #pragma HLS STREAM variable=fifo_input_PE_1_15 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2_15;
  #pragma HLS STREAM variable=fifo_input_PE_2_15 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3_15;
  #pragma HLS STREAM variable=fifo_input_PE_3_15 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3_15 core=FIFO_SRL
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
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_4;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_4;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_4;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_4;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_5;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_5 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_5;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_5 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_5;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_5 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_5;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_5 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_6;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_6 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_6;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_6 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_6;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_6 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_6;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_6 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_7;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_7 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_7;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_7 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_7;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_7 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_7;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_7 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_8;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_8 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_8;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_8 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_8;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_8 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_8;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_8 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_9;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_9 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_9;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_9 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_9;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_9 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_9;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_9 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_10;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_10 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_10;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_10 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_10;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_10 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_10;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_10 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_11;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_11 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_11;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_11 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_11;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_11 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_11;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_11 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_12;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_12 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_12;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_12 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_12;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_12 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_12;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_12 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_13;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_13 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_13;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_13 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_13;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_13 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_13;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_13 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_14;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_14 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_14;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_14 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_14;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_14 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_14;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_14 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0_15;
  #pragma HLS STREAM variable=fifo_kernel_PE_0_15 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1_15;
  #pragma HLS STREAM variable=fifo_kernel_PE_1_15 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2_15;
  #pragma HLS STREAM variable=fifo_kernel_PE_2_15 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3_15;
  #pragma HLS STREAM variable=fifo_kernel_PE_3_15 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_0;
  #pragma HLS STREAM variable=fifo_output_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_1;
  #pragma HLS STREAM variable=fifo_output_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_2;
  #pragma HLS STREAM variable=fifo_output_PE_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_3;
  #pragma HLS STREAM variable=fifo_output_PE_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_4;
  #pragma HLS STREAM variable=fifo_output_PE_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_5;
  #pragma HLS STREAM variable=fifo_output_PE_0_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_6;
  #pragma HLS STREAM variable=fifo_output_PE_0_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_7;
  #pragma HLS STREAM variable=fifo_output_PE_0_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_8;
  #pragma HLS STREAM variable=fifo_output_PE_0_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_9;
  #pragma HLS STREAM variable=fifo_output_PE_0_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_10;
  #pragma HLS STREAM variable=fifo_output_PE_0_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_11;
  #pragma HLS STREAM variable=fifo_output_PE_0_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_12;
  #pragma HLS STREAM variable=fifo_output_PE_0_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_13;
  #pragma HLS STREAM variable=fifo_output_PE_0_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_14;
  #pragma HLS STREAM variable=fifo_output_PE_0_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_15;
  #pragma HLS STREAM variable=fifo_output_PE_0_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_16;
  #pragma HLS STREAM variable=fifo_output_PE_0_16 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_0;
  #pragma HLS STREAM variable=fifo_output_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_1;
  #pragma HLS STREAM variable=fifo_output_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_2;
  #pragma HLS STREAM variable=fifo_output_PE_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_3;
  #pragma HLS STREAM variable=fifo_output_PE_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_4;
  #pragma HLS STREAM variable=fifo_output_PE_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_5;
  #pragma HLS STREAM variable=fifo_output_PE_1_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_6;
  #pragma HLS STREAM variable=fifo_output_PE_1_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_7;
  #pragma HLS STREAM variable=fifo_output_PE_1_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_8;
  #pragma HLS STREAM variable=fifo_output_PE_1_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_9;
  #pragma HLS STREAM variable=fifo_output_PE_1_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_10;
  #pragma HLS STREAM variable=fifo_output_PE_1_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_11;
  #pragma HLS STREAM variable=fifo_output_PE_1_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_12;
  #pragma HLS STREAM variable=fifo_output_PE_1_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_13;
  #pragma HLS STREAM variable=fifo_output_PE_1_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_14;
  #pragma HLS STREAM variable=fifo_output_PE_1_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_15;
  #pragma HLS STREAM variable=fifo_output_PE_1_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_16;
  #pragma HLS STREAM variable=fifo_output_PE_1_16 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_0;
  #pragma HLS STREAM variable=fifo_output_PE_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_1;
  #pragma HLS STREAM variable=fifo_output_PE_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_2;
  #pragma HLS STREAM variable=fifo_output_PE_2_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_3;
  #pragma HLS STREAM variable=fifo_output_PE_2_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_4;
  #pragma HLS STREAM variable=fifo_output_PE_2_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_5;
  #pragma HLS STREAM variable=fifo_output_PE_2_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_6;
  #pragma HLS STREAM variable=fifo_output_PE_2_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_7;
  #pragma HLS STREAM variable=fifo_output_PE_2_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_8;
  #pragma HLS STREAM variable=fifo_output_PE_2_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_9;
  #pragma HLS STREAM variable=fifo_output_PE_2_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_10;
  #pragma HLS STREAM variable=fifo_output_PE_2_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_11;
  #pragma HLS STREAM variable=fifo_output_PE_2_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_12;
  #pragma HLS STREAM variable=fifo_output_PE_2_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_13;
  #pragma HLS STREAM variable=fifo_output_PE_2_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_14;
  #pragma HLS STREAM variable=fifo_output_PE_2_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_15;
  #pragma HLS STREAM variable=fifo_output_PE_2_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2_16;
  #pragma HLS STREAM variable=fifo_output_PE_2_16 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_0;
  #pragma HLS STREAM variable=fifo_output_PE_3_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_1;
  #pragma HLS STREAM variable=fifo_output_PE_3_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_2;
  #pragma HLS STREAM variable=fifo_output_PE_3_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_3;
  #pragma HLS STREAM variable=fifo_output_PE_3_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_4;
  #pragma HLS STREAM variable=fifo_output_PE_3_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_5;
  #pragma HLS STREAM variable=fifo_output_PE_3_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_6;
  #pragma HLS STREAM variable=fifo_output_PE_3_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_7;
  #pragma HLS STREAM variable=fifo_output_PE_3_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_8;
  #pragma HLS STREAM variable=fifo_output_PE_3_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_9;
  #pragma HLS STREAM variable=fifo_output_PE_3_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_10;
  #pragma HLS STREAM variable=fifo_output_PE_3_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_11;
  #pragma HLS STREAM variable=fifo_output_PE_3_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_12;
  #pragma HLS STREAM variable=fifo_output_PE_3_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_13;
  #pragma HLS STREAM variable=fifo_output_PE_3_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_14;
  #pragma HLS STREAM variable=fifo_output_PE_3_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_15;
  #pragma HLS STREAM variable=fifo_output_PE_3_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3_16;
  #pragma HLS STREAM variable=fifo_output_PE_3_16 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3_16 core=FIFO_SRL
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
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_4;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_4;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_4;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_4;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_5;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_5;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_5;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_5;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_6;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_6;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_6;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_6;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_7;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_7;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_7;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_7;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_8;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_8;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_8;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_8;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_9;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_9;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_9;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_9;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_10;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_10;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_10;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_10;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_11;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_11;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_11;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_11;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_12;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_12;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_12;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_12;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_13;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_13;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_13;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_13;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_14;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_14;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_14;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_14;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0_15;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1_15;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2_15;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3_15;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3_15 core=FIFO_SRL
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
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_4_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_4_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_4_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_4_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_4_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_4_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_4_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_4_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_4_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_4_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_4_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_4_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_4_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_4_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_4_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_5_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_5_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_5_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_5_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_5_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_5_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_5_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_5_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_5_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_5_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_5_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_5_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_5_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_5_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_5_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_6_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_6_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_6_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_6_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_6_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_6_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_6_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_6_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_6_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_6_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_6_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_6_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_6_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_6_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_6_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_7_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_7_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_7_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_7_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_7_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_7_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_7_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_7_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_7_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_7_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_7_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_7_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_7_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_7_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_7_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_8_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_8_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_8_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_8_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_8_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_8_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_8_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_8_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_8_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_8_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_8_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_8_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_8_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_8_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_8_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_9_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_9_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_9_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_9_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_9_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_9_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_9_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_9_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_9_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_9_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_9_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_9_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_9_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_9_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_9_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_10_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_10_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_10_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_10_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_10_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_10_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_10_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_10_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_10_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_10_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_10_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_10_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_10_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_10_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_10_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_11_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_11_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_11_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_11_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_11_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_11_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_11_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_11_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_11_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_11_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_11_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_11_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_11_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_11_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_11_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_12_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_12_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_12_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_12_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_12_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_12_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_12_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_12_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_12_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_12_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_12_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_12_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_12_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_12_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_12_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_13_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_13_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_13_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_13_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_13_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_13_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_13_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_13_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_13_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_13_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_13_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_13_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_13_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_13_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_13_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_14_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_14_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_14_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_14_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_14_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_14_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_14_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_14_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_14_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_14_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_14_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_14_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_14_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_14_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_14_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_15_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_15_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_15_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_15_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_15_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_15_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_15_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_15_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_15_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_15_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_15_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_15_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_15_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_15_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_15_4 core=FIFO_SRL
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
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_5;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_5 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_6;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_6 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_7;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_7 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_8;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_8 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_9;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_9 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_10;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_10 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_11;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_11 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_12;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_12 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_13;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_13 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_14;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_14 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_15;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_15 core=FIFO_SRL
  /* output_drain_IO_L2_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L2_out_16;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L2_out_16 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L2_out_16 core=FIFO_SRL
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
  input_IO_L2_in(
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L2_in_3,
    /* fifo */ fifo_input_input_IO_L2_in_4,
    /* fifo */ fifo_input_input_IO_L1_in_3_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 4,
    /* fifo */ fifo_input_input_IO_L2_in_4,
    /* fifo */ fifo_input_input_IO_L2_in_5,
    /* fifo */ fifo_input_input_IO_L1_in_4_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 5,
    /* fifo */ fifo_input_input_IO_L2_in_5,
    /* fifo */ fifo_input_input_IO_L2_in_6,
    /* fifo */ fifo_input_input_IO_L1_in_5_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 6,
    /* fifo */ fifo_input_input_IO_L2_in_6,
    /* fifo */ fifo_input_input_IO_L2_in_7,
    /* fifo */ fifo_input_input_IO_L1_in_6_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 7,
    /* fifo */ fifo_input_input_IO_L2_in_7,
    /* fifo */ fifo_input_input_IO_L2_in_8,
    /* fifo */ fifo_input_input_IO_L1_in_7_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 8,
    /* fifo */ fifo_input_input_IO_L2_in_8,
    /* fifo */ fifo_input_input_IO_L2_in_9,
    /* fifo */ fifo_input_input_IO_L1_in_8_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 9,
    /* fifo */ fifo_input_input_IO_L2_in_9,
    /* fifo */ fifo_input_input_IO_L2_in_10,
    /* fifo */ fifo_input_input_IO_L1_in_9_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 10,
    /* fifo */ fifo_input_input_IO_L2_in_10,
    /* fifo */ fifo_input_input_IO_L2_in_11,
    /* fifo */ fifo_input_input_IO_L1_in_10_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 11,
    /* fifo */ fifo_input_input_IO_L2_in_11,
    /* fifo */ fifo_input_input_IO_L2_in_12,
    /* fifo */ fifo_input_input_IO_L1_in_11_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 12,
    /* fifo */ fifo_input_input_IO_L2_in_12,
    /* fifo */ fifo_input_input_IO_L2_in_13,
    /* fifo */ fifo_input_input_IO_L1_in_12_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 13,
    /* fifo */ fifo_input_input_IO_L2_in_13,
    /* fifo */ fifo_input_input_IO_L2_in_14,
    /* fifo */ fifo_input_input_IO_L1_in_13_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in(
    /* module id */ 14,
    /* fifo */ fifo_input_input_IO_L2_in_14,
    /* fifo */ fifo_input_input_IO_L2_in_15,
    /* fifo */ fifo_input_input_IO_L1_in_14_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L2_in_boundary(
    /* module id */ 15,
    /* fifo */ fifo_input_input_IO_L2_in_15,
    /* fifo */ fifo_input_input_IO_L1_in_15_0
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
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 0,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_0_3,
    /* fifo */ fifo_input_PE_3_0
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
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 1,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_1_3,
    /* fifo */ fifo_input_PE_3_1
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
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 2,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_2_3,
    /* fifo */ fifo_input_PE_3_2
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
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 3,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_3_3,
    /* fifo */ fifo_input_PE_3_3
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 4,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_4_0,
    /* fifo */ fifo_input_input_IO_L1_in_4_1,
    /* fifo */ fifo_input_PE_0_4
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 4,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_4_1,
    /* fifo */ fifo_input_input_IO_L1_in_4_2,
    /* fifo */ fifo_input_PE_1_4
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 4,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_4_2,
    /* fifo */ fifo_input_input_IO_L1_in_4_3,
    /* fifo */ fifo_input_PE_2_4
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 4,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_4_3,
    /* fifo */ fifo_input_PE_3_4
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 5,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_5_0,
    /* fifo */ fifo_input_input_IO_L1_in_5_1,
    /* fifo */ fifo_input_PE_0_5
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 5,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_5_1,
    /* fifo */ fifo_input_input_IO_L1_in_5_2,
    /* fifo */ fifo_input_PE_1_5
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 5,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_5_2,
    /* fifo */ fifo_input_input_IO_L1_in_5_3,
    /* fifo */ fifo_input_PE_2_5
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 5,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_5_3,
    /* fifo */ fifo_input_PE_3_5
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 6,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_6_0,
    /* fifo */ fifo_input_input_IO_L1_in_6_1,
    /* fifo */ fifo_input_PE_0_6
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 6,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_6_1,
    /* fifo */ fifo_input_input_IO_L1_in_6_2,
    /* fifo */ fifo_input_PE_1_6
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 6,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_6_2,
    /* fifo */ fifo_input_input_IO_L1_in_6_3,
    /* fifo */ fifo_input_PE_2_6
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 6,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_6_3,
    /* fifo */ fifo_input_PE_3_6
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 7,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_7_0,
    /* fifo */ fifo_input_input_IO_L1_in_7_1,
    /* fifo */ fifo_input_PE_0_7
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 7,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_7_1,
    /* fifo */ fifo_input_input_IO_L1_in_7_2,
    /* fifo */ fifo_input_PE_1_7
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 7,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_7_2,
    /* fifo */ fifo_input_input_IO_L1_in_7_3,
    /* fifo */ fifo_input_PE_2_7
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 7,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_7_3,
    /* fifo */ fifo_input_PE_3_7
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 8,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_8_0,
    /* fifo */ fifo_input_input_IO_L1_in_8_1,
    /* fifo */ fifo_input_PE_0_8
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 8,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_8_1,
    /* fifo */ fifo_input_input_IO_L1_in_8_2,
    /* fifo */ fifo_input_PE_1_8
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 8,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_8_2,
    /* fifo */ fifo_input_input_IO_L1_in_8_3,
    /* fifo */ fifo_input_PE_2_8
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 8,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_8_3,
    /* fifo */ fifo_input_PE_3_8
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 9,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_9_0,
    /* fifo */ fifo_input_input_IO_L1_in_9_1,
    /* fifo */ fifo_input_PE_0_9
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 9,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_9_1,
    /* fifo */ fifo_input_input_IO_L1_in_9_2,
    /* fifo */ fifo_input_PE_1_9
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 9,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_9_2,
    /* fifo */ fifo_input_input_IO_L1_in_9_3,
    /* fifo */ fifo_input_PE_2_9
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 9,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_9_3,
    /* fifo */ fifo_input_PE_3_9
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 10,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_10_0,
    /* fifo */ fifo_input_input_IO_L1_in_10_1,
    /* fifo */ fifo_input_PE_0_10
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 10,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_10_1,
    /* fifo */ fifo_input_input_IO_L1_in_10_2,
    /* fifo */ fifo_input_PE_1_10
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 10,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_10_2,
    /* fifo */ fifo_input_input_IO_L1_in_10_3,
    /* fifo */ fifo_input_PE_2_10
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 10,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_10_3,
    /* fifo */ fifo_input_PE_3_10
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 11,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_11_0,
    /* fifo */ fifo_input_input_IO_L1_in_11_1,
    /* fifo */ fifo_input_PE_0_11
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 11,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_11_1,
    /* fifo */ fifo_input_input_IO_L1_in_11_2,
    /* fifo */ fifo_input_PE_1_11
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 11,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_11_2,
    /* fifo */ fifo_input_input_IO_L1_in_11_3,
    /* fifo */ fifo_input_PE_2_11
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 11,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_11_3,
    /* fifo */ fifo_input_PE_3_11
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 12,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_12_0,
    /* fifo */ fifo_input_input_IO_L1_in_12_1,
    /* fifo */ fifo_input_PE_0_12
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 12,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_12_1,
    /* fifo */ fifo_input_input_IO_L1_in_12_2,
    /* fifo */ fifo_input_PE_1_12
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 12,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_12_2,
    /* fifo */ fifo_input_input_IO_L1_in_12_3,
    /* fifo */ fifo_input_PE_2_12
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 12,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_12_3,
    /* fifo */ fifo_input_PE_3_12
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 13,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_13_0,
    /* fifo */ fifo_input_input_IO_L1_in_13_1,
    /* fifo */ fifo_input_PE_0_13
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 13,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_13_1,
    /* fifo */ fifo_input_input_IO_L1_in_13_2,
    /* fifo */ fifo_input_PE_1_13
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 13,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_13_2,
    /* fifo */ fifo_input_input_IO_L1_in_13_3,
    /* fifo */ fifo_input_PE_2_13
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 13,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_13_3,
    /* fifo */ fifo_input_PE_3_13
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 14,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_14_0,
    /* fifo */ fifo_input_input_IO_L1_in_14_1,
    /* fifo */ fifo_input_PE_0_14
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 14,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_14_1,
    /* fifo */ fifo_input_input_IO_L1_in_14_2,
    /* fifo */ fifo_input_PE_1_14
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 14,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_14_2,
    /* fifo */ fifo_input_input_IO_L1_in_14_3,
    /* fifo */ fifo_input_PE_2_14
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 14,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_14_3,
    /* fifo */ fifo_input_PE_3_14
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 15,
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_15_0,
    /* fifo */ fifo_input_input_IO_L1_in_15_1,
    /* fifo */ fifo_input_PE_0_15
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 15,
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_15_1,
    /* fifo */ fifo_input_input_IO_L1_in_15_2,
    /* fifo */ fifo_input_PE_1_15
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 15,
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_15_2,
    /* fifo */ fifo_input_input_IO_L1_in_15_3,
    /* fifo */ fifo_input_PE_2_15
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 15,
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_15_3,
    /* fifo */ fifo_input_PE_3_15
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
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_1,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_2,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_3,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_4,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 4,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_4,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_5,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 5,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_5,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_6,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 6,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_6,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_7,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 7,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_7,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_8,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 8,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_8,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_9,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 9,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_9,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_10,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 10,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_10,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_11,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 11,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_11,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_12,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 12,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_12,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_13,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 13,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_13,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_14,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* module id */ 14,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_14,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_15,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in_boundary(
    /* module id */ 15,
    /* fifo */ fifo_kernel_kernel_IO_L2_in_15,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0_1,
    /* fifo */ fifo_kernel_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0_2,
    /* fifo */ fifo_kernel_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0_3,
    /* fifo */ fifo_kernel_PE_2_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 0,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0_3,
    /* fifo */ fifo_kernel_PE_3_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1_1,
    /* fifo */ fifo_kernel_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1_2,
    /* fifo */ fifo_kernel_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1_3,
    /* fifo */ fifo_kernel_PE_2_1
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 1,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1_3,
    /* fifo */ fifo_kernel_PE_3_1
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2_1,
    /* fifo */ fifo_kernel_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2_2,
    /* fifo */ fifo_kernel_PE_1_2
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2_3,
    /* fifo */ fifo_kernel_PE_2_2
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 2,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2_3,
    /* fifo */ fifo_kernel_PE_3_2
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3_1,
    /* fifo */ fifo_kernel_PE_0_3
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3_2,
    /* fifo */ fifo_kernel_PE_1_3
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3_3,
    /* fifo */ fifo_kernel_PE_2_3
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 3,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3_3,
    /* fifo */ fifo_kernel_PE_3_3
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 4,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4_1,
    /* fifo */ fifo_kernel_PE_0_4
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 4,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4_2,
    /* fifo */ fifo_kernel_PE_1_4
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 4,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4_3,
    /* fifo */ fifo_kernel_PE_2_4
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 4,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4_3,
    /* fifo */ fifo_kernel_PE_3_4
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 5,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5_1,
    /* fifo */ fifo_kernel_PE_0_5
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 5,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5_2,
    /* fifo */ fifo_kernel_PE_1_5
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 5,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5_3,
    /* fifo */ fifo_kernel_PE_2_5
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 5,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5_3,
    /* fifo */ fifo_kernel_PE_3_5
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 6,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6_1,
    /* fifo */ fifo_kernel_PE_0_6
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 6,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6_2,
    /* fifo */ fifo_kernel_PE_1_6
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 6,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6_3,
    /* fifo */ fifo_kernel_PE_2_6
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 6,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6_3,
    /* fifo */ fifo_kernel_PE_3_6
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 7,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7_1,
    /* fifo */ fifo_kernel_PE_0_7
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 7,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7_2,
    /* fifo */ fifo_kernel_PE_1_7
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 7,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7_3,
    /* fifo */ fifo_kernel_PE_2_7
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 7,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7_3,
    /* fifo */ fifo_kernel_PE_3_7
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 8,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8_1,
    /* fifo */ fifo_kernel_PE_0_8
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 8,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8_2,
    /* fifo */ fifo_kernel_PE_1_8
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 8,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8_3,
    /* fifo */ fifo_kernel_PE_2_8
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 8,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8_3,
    /* fifo */ fifo_kernel_PE_3_8
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 9,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9_1,
    /* fifo */ fifo_kernel_PE_0_9
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 9,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9_2,
    /* fifo */ fifo_kernel_PE_1_9
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 9,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9_3,
    /* fifo */ fifo_kernel_PE_2_9
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 9,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9_3,
    /* fifo */ fifo_kernel_PE_3_9
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 10,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10_1,
    /* fifo */ fifo_kernel_PE_0_10
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 10,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10_2,
    /* fifo */ fifo_kernel_PE_1_10
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 10,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10_3,
    /* fifo */ fifo_kernel_PE_2_10
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 10,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10_3,
    /* fifo */ fifo_kernel_PE_3_10
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 11,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11_1,
    /* fifo */ fifo_kernel_PE_0_11
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 11,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11_2,
    /* fifo */ fifo_kernel_PE_1_11
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 11,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11_3,
    /* fifo */ fifo_kernel_PE_2_11
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 11,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11_3,
    /* fifo */ fifo_kernel_PE_3_11
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 12,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12_1,
    /* fifo */ fifo_kernel_PE_0_12
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 12,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12_2,
    /* fifo */ fifo_kernel_PE_1_12
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 12,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12_3,
    /* fifo */ fifo_kernel_PE_2_12
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 12,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12_3,
    /* fifo */ fifo_kernel_PE_3_12
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 13,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13_1,
    /* fifo */ fifo_kernel_PE_0_13
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 13,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13_2,
    /* fifo */ fifo_kernel_PE_1_13
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 13,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13_3,
    /* fifo */ fifo_kernel_PE_2_13
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 13,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13_3,
    /* fifo */ fifo_kernel_PE_3_13
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 14,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14_1,
    /* fifo */ fifo_kernel_PE_0_14
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 14,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14_2,
    /* fifo */ fifo_kernel_PE_1_14
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 14,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14_3,
    /* fifo */ fifo_kernel_PE_2_14
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 14,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14_3,
    /* fifo */ fifo_kernel_PE_3_14
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 15,
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15_1,
    /* fifo */ fifo_kernel_PE_0_15
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 15,
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15_2,
    /* fifo */ fifo_kernel_PE_1_15
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 15,
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15_3,
    /* fifo */ fifo_kernel_PE_2_15
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 15,
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15_3,
    /* fifo */ fifo_kernel_PE_3_15
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_0_0,
    /* fifo */ fifo_kernel_PE_0_0,
    /* fifo */ fifo_output_PE_0_0,
    /* fifo */ fifo_output_PE_0_1,
    /* fifo */ fifo_output_drain_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_0_1,
    /* fifo */ fifo_kernel_PE_0_1,
    /* fifo */ fifo_output_PE_0_1,
    /* fifo */ fifo_output_PE_0_2,
    /* fifo */ fifo_output_drain_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_0_2,
    /* fifo */ fifo_kernel_PE_0_2,
    /* fifo */ fifo_output_PE_0_2,
    /* fifo */ fifo_output_PE_0_3,
    /* fifo */ fifo_output_drain_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_0_3,
    /* fifo */ fifo_kernel_PE_0_3,
    /* fifo */ fifo_output_PE_0_3,
    /* fifo */ fifo_output_PE_0_4,
    /* fifo */ fifo_output_drain_PE_0_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 4,
    /* fifo */ fifo_input_PE_0_4,
    /* fifo */ fifo_kernel_PE_0_4,
    /* fifo */ fifo_output_PE_0_4,
    /* fifo */ fifo_output_PE_0_5,
    /* fifo */ fifo_output_drain_PE_0_4
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 5,
    /* fifo */ fifo_input_PE_0_5,
    /* fifo */ fifo_kernel_PE_0_5,
    /* fifo */ fifo_output_PE_0_5,
    /* fifo */ fifo_output_PE_0_6,
    /* fifo */ fifo_output_drain_PE_0_5
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 6,
    /* fifo */ fifo_input_PE_0_6,
    /* fifo */ fifo_kernel_PE_0_6,
    /* fifo */ fifo_output_PE_0_6,
    /* fifo */ fifo_output_PE_0_7,
    /* fifo */ fifo_output_drain_PE_0_6
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 7,
    /* fifo */ fifo_input_PE_0_7,
    /* fifo */ fifo_kernel_PE_0_7,
    /* fifo */ fifo_output_PE_0_7,
    /* fifo */ fifo_output_PE_0_8,
    /* fifo */ fifo_output_drain_PE_0_7
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 8,
    /* fifo */ fifo_input_PE_0_8,
    /* fifo */ fifo_kernel_PE_0_8,
    /* fifo */ fifo_output_PE_0_8,
    /* fifo */ fifo_output_PE_0_9,
    /* fifo */ fifo_output_drain_PE_0_8
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 9,
    /* fifo */ fifo_input_PE_0_9,
    /* fifo */ fifo_kernel_PE_0_9,
    /* fifo */ fifo_output_PE_0_9,
    /* fifo */ fifo_output_PE_0_10,
    /* fifo */ fifo_output_drain_PE_0_9
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 10,
    /* fifo */ fifo_input_PE_0_10,
    /* fifo */ fifo_kernel_PE_0_10,
    /* fifo */ fifo_output_PE_0_10,
    /* fifo */ fifo_output_PE_0_11,
    /* fifo */ fifo_output_drain_PE_0_10
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 11,
    /* fifo */ fifo_input_PE_0_11,
    /* fifo */ fifo_kernel_PE_0_11,
    /* fifo */ fifo_output_PE_0_11,
    /* fifo */ fifo_output_PE_0_12,
    /* fifo */ fifo_output_drain_PE_0_11
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 12,
    /* fifo */ fifo_input_PE_0_12,
    /* fifo */ fifo_kernel_PE_0_12,
    /* fifo */ fifo_output_PE_0_12,
    /* fifo */ fifo_output_PE_0_13,
    /* fifo */ fifo_output_drain_PE_0_12
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 13,
    /* fifo */ fifo_input_PE_0_13,
    /* fifo */ fifo_kernel_PE_0_13,
    /* fifo */ fifo_output_PE_0_13,
    /* fifo */ fifo_output_PE_0_14,
    /* fifo */ fifo_output_drain_PE_0_13
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 14,
    /* fifo */ fifo_input_PE_0_14,
    /* fifo */ fifo_kernel_PE_0_14,
    /* fifo */ fifo_output_PE_0_14,
    /* fifo */ fifo_output_PE_0_15,
    /* fifo */ fifo_output_drain_PE_0_14
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 15,
    /* fifo */ fifo_input_PE_0_15,
    /* fifo */ fifo_kernel_PE_0_15,
    /* fifo */ fifo_output_PE_0_15,
    /* fifo */ fifo_output_PE_0_16,
    /* fifo */ fifo_output_drain_PE_0_15
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_1_0,
    /* fifo */ fifo_kernel_PE_1_0,
    /* fifo */ fifo_output_PE_1_0,
    /* fifo */ fifo_output_PE_1_1,
    /* fifo */ fifo_output_drain_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_1_1,
    /* fifo */ fifo_kernel_PE_1_1,
    /* fifo */ fifo_output_PE_1_1,
    /* fifo */ fifo_output_PE_1_2,
    /* fifo */ fifo_output_drain_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_1_2,
    /* fifo */ fifo_kernel_PE_1_2,
    /* fifo */ fifo_output_PE_1_2,
    /* fifo */ fifo_output_PE_1_3,
    /* fifo */ fifo_output_drain_PE_1_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_1_3,
    /* fifo */ fifo_kernel_PE_1_3,
    /* fifo */ fifo_output_PE_1_3,
    /* fifo */ fifo_output_PE_1_4,
    /* fifo */ fifo_output_drain_PE_1_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 4,
    /* fifo */ fifo_input_PE_1_4,
    /* fifo */ fifo_kernel_PE_1_4,
    /* fifo */ fifo_output_PE_1_4,
    /* fifo */ fifo_output_PE_1_5,
    /* fifo */ fifo_output_drain_PE_1_4
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 5,
    /* fifo */ fifo_input_PE_1_5,
    /* fifo */ fifo_kernel_PE_1_5,
    /* fifo */ fifo_output_PE_1_5,
    /* fifo */ fifo_output_PE_1_6,
    /* fifo */ fifo_output_drain_PE_1_5
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 6,
    /* fifo */ fifo_input_PE_1_6,
    /* fifo */ fifo_kernel_PE_1_6,
    /* fifo */ fifo_output_PE_1_6,
    /* fifo */ fifo_output_PE_1_7,
    /* fifo */ fifo_output_drain_PE_1_6
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 7,
    /* fifo */ fifo_input_PE_1_7,
    /* fifo */ fifo_kernel_PE_1_7,
    /* fifo */ fifo_output_PE_1_7,
    /* fifo */ fifo_output_PE_1_8,
    /* fifo */ fifo_output_drain_PE_1_7
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 8,
    /* fifo */ fifo_input_PE_1_8,
    /* fifo */ fifo_kernel_PE_1_8,
    /* fifo */ fifo_output_PE_1_8,
    /* fifo */ fifo_output_PE_1_9,
    /* fifo */ fifo_output_drain_PE_1_8
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 9,
    /* fifo */ fifo_input_PE_1_9,
    /* fifo */ fifo_kernel_PE_1_9,
    /* fifo */ fifo_output_PE_1_9,
    /* fifo */ fifo_output_PE_1_10,
    /* fifo */ fifo_output_drain_PE_1_9
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 10,
    /* fifo */ fifo_input_PE_1_10,
    /* fifo */ fifo_kernel_PE_1_10,
    /* fifo */ fifo_output_PE_1_10,
    /* fifo */ fifo_output_PE_1_11,
    /* fifo */ fifo_output_drain_PE_1_10
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 11,
    /* fifo */ fifo_input_PE_1_11,
    /* fifo */ fifo_kernel_PE_1_11,
    /* fifo */ fifo_output_PE_1_11,
    /* fifo */ fifo_output_PE_1_12,
    /* fifo */ fifo_output_drain_PE_1_11
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 12,
    /* fifo */ fifo_input_PE_1_12,
    /* fifo */ fifo_kernel_PE_1_12,
    /* fifo */ fifo_output_PE_1_12,
    /* fifo */ fifo_output_PE_1_13,
    /* fifo */ fifo_output_drain_PE_1_12
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 13,
    /* fifo */ fifo_input_PE_1_13,
    /* fifo */ fifo_kernel_PE_1_13,
    /* fifo */ fifo_output_PE_1_13,
    /* fifo */ fifo_output_PE_1_14,
    /* fifo */ fifo_output_drain_PE_1_13
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 14,
    /* fifo */ fifo_input_PE_1_14,
    /* fifo */ fifo_kernel_PE_1_14,
    /* fifo */ fifo_output_PE_1_14,
    /* fifo */ fifo_output_PE_1_15,
    /* fifo */ fifo_output_drain_PE_1_14
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 15,
    /* fifo */ fifo_input_PE_1_15,
    /* fifo */ fifo_kernel_PE_1_15,
    /* fifo */ fifo_output_PE_1_15,
    /* fifo */ fifo_output_PE_1_16,
    /* fifo */ fifo_output_drain_PE_1_15
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_2_0,
    /* fifo */ fifo_kernel_PE_2_0,
    /* fifo */ fifo_output_PE_2_0,
    /* fifo */ fifo_output_PE_2_1,
    /* fifo */ fifo_output_drain_PE_2_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_2_1,
    /* fifo */ fifo_kernel_PE_2_1,
    /* fifo */ fifo_output_PE_2_1,
    /* fifo */ fifo_output_PE_2_2,
    /* fifo */ fifo_output_drain_PE_2_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_2_2,
    /* fifo */ fifo_kernel_PE_2_2,
    /* fifo */ fifo_output_PE_2_2,
    /* fifo */ fifo_output_PE_2_3,
    /* fifo */ fifo_output_drain_PE_2_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_2_3,
    /* fifo */ fifo_kernel_PE_2_3,
    /* fifo */ fifo_output_PE_2_3,
    /* fifo */ fifo_output_PE_2_4,
    /* fifo */ fifo_output_drain_PE_2_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 4,
    /* fifo */ fifo_input_PE_2_4,
    /* fifo */ fifo_kernel_PE_2_4,
    /* fifo */ fifo_output_PE_2_4,
    /* fifo */ fifo_output_PE_2_5,
    /* fifo */ fifo_output_drain_PE_2_4
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 5,
    /* fifo */ fifo_input_PE_2_5,
    /* fifo */ fifo_kernel_PE_2_5,
    /* fifo */ fifo_output_PE_2_5,
    /* fifo */ fifo_output_PE_2_6,
    /* fifo */ fifo_output_drain_PE_2_5
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 6,
    /* fifo */ fifo_input_PE_2_6,
    /* fifo */ fifo_kernel_PE_2_6,
    /* fifo */ fifo_output_PE_2_6,
    /* fifo */ fifo_output_PE_2_7,
    /* fifo */ fifo_output_drain_PE_2_6
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 7,
    /* fifo */ fifo_input_PE_2_7,
    /* fifo */ fifo_kernel_PE_2_7,
    /* fifo */ fifo_output_PE_2_7,
    /* fifo */ fifo_output_PE_2_8,
    /* fifo */ fifo_output_drain_PE_2_7
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 8,
    /* fifo */ fifo_input_PE_2_8,
    /* fifo */ fifo_kernel_PE_2_8,
    /* fifo */ fifo_output_PE_2_8,
    /* fifo */ fifo_output_PE_2_9,
    /* fifo */ fifo_output_drain_PE_2_8
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 9,
    /* fifo */ fifo_input_PE_2_9,
    /* fifo */ fifo_kernel_PE_2_9,
    /* fifo */ fifo_output_PE_2_9,
    /* fifo */ fifo_output_PE_2_10,
    /* fifo */ fifo_output_drain_PE_2_9
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 10,
    /* fifo */ fifo_input_PE_2_10,
    /* fifo */ fifo_kernel_PE_2_10,
    /* fifo */ fifo_output_PE_2_10,
    /* fifo */ fifo_output_PE_2_11,
    /* fifo */ fifo_output_drain_PE_2_10
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 11,
    /* fifo */ fifo_input_PE_2_11,
    /* fifo */ fifo_kernel_PE_2_11,
    /* fifo */ fifo_output_PE_2_11,
    /* fifo */ fifo_output_PE_2_12,
    /* fifo */ fifo_output_drain_PE_2_11
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 12,
    /* fifo */ fifo_input_PE_2_12,
    /* fifo */ fifo_kernel_PE_2_12,
    /* fifo */ fifo_output_PE_2_12,
    /* fifo */ fifo_output_PE_2_13,
    /* fifo */ fifo_output_drain_PE_2_12
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 13,
    /* fifo */ fifo_input_PE_2_13,
    /* fifo */ fifo_kernel_PE_2_13,
    /* fifo */ fifo_output_PE_2_13,
    /* fifo */ fifo_output_PE_2_14,
    /* fifo */ fifo_output_drain_PE_2_13
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 14,
    /* fifo */ fifo_input_PE_2_14,
    /* fifo */ fifo_kernel_PE_2_14,
    /* fifo */ fifo_output_PE_2_14,
    /* fifo */ fifo_output_PE_2_15,
    /* fifo */ fifo_output_drain_PE_2_14
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* module id */ 15,
    /* fifo */ fifo_input_PE_2_15,
    /* fifo */ fifo_kernel_PE_2_15,
    /* fifo */ fifo_output_PE_2_15,
    /* fifo */ fifo_output_PE_2_16,
    /* fifo */ fifo_output_drain_PE_2_15
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 0,
    /* fifo */ fifo_input_PE_3_0,
    /* fifo */ fifo_kernel_PE_3_0,
    /* fifo */ fifo_output_PE_3_0,
    /* fifo */ fifo_output_PE_3_1,
    /* fifo */ fifo_output_drain_PE_3_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 1,
    /* fifo */ fifo_input_PE_3_1,
    /* fifo */ fifo_kernel_PE_3_1,
    /* fifo */ fifo_output_PE_3_1,
    /* fifo */ fifo_output_PE_3_2,
    /* fifo */ fifo_output_drain_PE_3_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 2,
    /* fifo */ fifo_input_PE_3_2,
    /* fifo */ fifo_kernel_PE_3_2,
    /* fifo */ fifo_output_PE_3_2,
    /* fifo */ fifo_output_PE_3_3,
    /* fifo */ fifo_output_drain_PE_3_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 3,
    /* fifo */ fifo_input_PE_3_3,
    /* fifo */ fifo_kernel_PE_3_3,
    /* fifo */ fifo_output_PE_3_3,
    /* fifo */ fifo_output_PE_3_4,
    /* fifo */ fifo_output_drain_PE_3_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 4,
    /* fifo */ fifo_input_PE_3_4,
    /* fifo */ fifo_kernel_PE_3_4,
    /* fifo */ fifo_output_PE_3_4,
    /* fifo */ fifo_output_PE_3_5,
    /* fifo */ fifo_output_drain_PE_3_4
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 5,
    /* fifo */ fifo_input_PE_3_5,
    /* fifo */ fifo_kernel_PE_3_5,
    /* fifo */ fifo_output_PE_3_5,
    /* fifo */ fifo_output_PE_3_6,
    /* fifo */ fifo_output_drain_PE_3_5
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 6,
    /* fifo */ fifo_input_PE_3_6,
    /* fifo */ fifo_kernel_PE_3_6,
    /* fifo */ fifo_output_PE_3_6,
    /* fifo */ fifo_output_PE_3_7,
    /* fifo */ fifo_output_drain_PE_3_6
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 7,
    /* fifo */ fifo_input_PE_3_7,
    /* fifo */ fifo_kernel_PE_3_7,
    /* fifo */ fifo_output_PE_3_7,
    /* fifo */ fifo_output_PE_3_8,
    /* fifo */ fifo_output_drain_PE_3_7
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 8,
    /* fifo */ fifo_input_PE_3_8,
    /* fifo */ fifo_kernel_PE_3_8,
    /* fifo */ fifo_output_PE_3_8,
    /* fifo */ fifo_output_PE_3_9,
    /* fifo */ fifo_output_drain_PE_3_8
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 9,
    /* fifo */ fifo_input_PE_3_9,
    /* fifo */ fifo_kernel_PE_3_9,
    /* fifo */ fifo_output_PE_3_9,
    /* fifo */ fifo_output_PE_3_10,
    /* fifo */ fifo_output_drain_PE_3_9
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 10,
    /* fifo */ fifo_input_PE_3_10,
    /* fifo */ fifo_kernel_PE_3_10,
    /* fifo */ fifo_output_PE_3_10,
    /* fifo */ fifo_output_PE_3_11,
    /* fifo */ fifo_output_drain_PE_3_10
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 11,
    /* fifo */ fifo_input_PE_3_11,
    /* fifo */ fifo_kernel_PE_3_11,
    /* fifo */ fifo_output_PE_3_11,
    /* fifo */ fifo_output_PE_3_12,
    /* fifo */ fifo_output_drain_PE_3_11
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 12,
    /* fifo */ fifo_input_PE_3_12,
    /* fifo */ fifo_kernel_PE_3_12,
    /* fifo */ fifo_output_PE_3_12,
    /* fifo */ fifo_output_PE_3_13,
    /* fifo */ fifo_output_drain_PE_3_12
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 13,
    /* fifo */ fifo_input_PE_3_13,
    /* fifo */ fifo_kernel_PE_3_13,
    /* fifo */ fifo_output_PE_3_13,
    /* fifo */ fifo_output_PE_3_14,
    /* fifo */ fifo_output_drain_PE_3_13
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 14,
    /* fifo */ fifo_input_PE_3_14,
    /* fifo */ fifo_kernel_PE_3_14,
    /* fifo */ fifo_output_PE_3_14,
    /* fifo */ fifo_output_PE_3_15,
    /* fifo */ fifo_output_drain_PE_3_14
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* module id */ 15,
    /* fifo */ fifo_input_PE_3_15,
    /* fifo */ fifo_kernel_PE_3_15,
    /* fifo */ fifo_output_PE_3_15,
    /* fifo */ fifo_output_PE_3_16,
    /* fifo */ fifo_output_drain_PE_3_15
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 15,
    /* module id */ 3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15_3,
    /* fifo */ fifo_output_drain_PE_3_15
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 15,
    /* module id */ 2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15_3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15_2,
    /* fifo */ fifo_output_drain_PE_2_15
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 15,
    /* module id */ 1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15_2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15_1,
    /* fifo */ fifo_output_drain_PE_1_15
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 15,
    /* module id */ 0,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15_1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15_0,
    /* fifo */ fifo_output_drain_PE_0_15
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out_boundary(
    /* module id */ 15,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_15,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 14,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_15,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_14,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_14_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 13,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_14,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_13,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_13_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 12,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_13,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_12,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_12_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 11,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_12,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_11,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_11_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 10,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_11,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_10,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_10_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 9,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_10,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_9,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_9_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 8,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_9,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_8,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_8_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 7,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_8,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_7,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_7_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 6,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_7,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_6_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 5,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_6,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_5_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 4,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_5,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_4_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* module id */ 3,
    /* fifo */ fifo_output_drain_output_drain_IO_L2_out_4,
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
