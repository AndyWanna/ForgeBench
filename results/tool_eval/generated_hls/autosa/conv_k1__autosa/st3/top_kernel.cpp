#include "top_kernel.h"
/* Module Definition */
void input_IO_L2_in(input_t2 *input, hls::stream<input_t2> &fifo_input_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L2
  for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
    // io_L1
    for (ap_uint<5> c6 = 0; c6 <= 13; c6 += 1) {
      // access_coalesce
      for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
      #pragma HLS PIPELINE II=1
        {
          input_t2 in_data;
          input_t2 out_data;
          in_data = input[98*c4 + 7*c6 + c7];
          out_data = in_data;
          fifo_input_local_out.write(out_data);
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_intra_trans(int idx, input_t2 local_input[1][14][7], hls::stream<float> &fifo_input_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[2];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // array
  // io_L2
  // io_L1
  // pe
  for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1)
    for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
      for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        // latency
        for (ap_uint<3> c8 = 0; c8 <= 3; c8 += 1) {
          // latency
          for (ap_uint<2> c9 = 0; c9 <= 1; c9 += 1) {
            // latency
            for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
            #pragma HLS PIPELINE II=1
              {
                input_t2 in_data;
                input_t1 out_data;
                in_data = local_input[0][2*c6 + c9][(2 * c5 + c10) / 2];
                for (ap_uint<2> n = 0; n < 2; n++) {
                #pragma HLS UNROLL
                  data_split[n] = in_data(31, 0);
                  in_data = in_data >> 32;
                }
                int split_idx = (c10 + 2*c5) % 2;
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
void input_IO_L1_in_inter_trans(int idx, input_t2 local_input[1][14][7], hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c4 = p0; c4 <= 15; c4 += 1) {
    // io_L1
    if (c4 == p0) {
      for (ap_uint<5> c6 = 0; c6 <= 13; c6 += 1) {
        // access_coalesce
        for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            input_t2 in_data;
            input_t2 out_data;
            in_data = fifo_input_in.read();
            out_data = in_data;
            local_input[0][c6][c7] = out_data;
          }
        }
      }
    } else {
      for (ap_uint<5> c6 = 0; c6 <= 13; c6 += 1) {
        // access_coalesce
        for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
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
void input_IO_L1_in_inter_trans_boundary(int idx, input_t2 local_input[1][14][7], hls::stream<input_t2> &fifo_input_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c4 = p0; c4 <= 15; c4 += 1)
    if (c4 == p0) {
      // io_L1
      for (ap_uint<5> c6 = 0; c6 <= 13; c6 += 1) {
        // access_coalesce
        for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            input_t2 in_data;
            input_t2 out_data;
            in_data = fifo_input_in.read();
            out_data = in_data;
            local_input[0][c6][c7] = out_data;
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in(int idx, hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_out, hls::stream<float> &fifo_input_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
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
    // io_L2
    {
      if (arb == 0) {
        input_IO_L1_in_inter_trans(
          /* module id */ idx, 
          /* array */ local_input_pong, 
          /* fifo */ fifo_input_in, 
          /* fifo */ fifo_input_out, 
          /* enable */ inter_trans_en
        );
        input_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        input_IO_L1_in_inter_trans(
          /* module id */ idx, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_in, 
          /* fifo */ fifo_input_out, 
          /* enable */ inter_trans_en
        );
        input_IO_L1_in_intra_trans(
          /* module id */ idx, 
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
        /* array */ local_input_ping, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      input_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* array */ local_input_pong, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_wrapper(int idx, hls::stream<input_t2> &fifo_input_in, hls::stream<input_t2> &fifo_input_out, hls::stream<float> &fifo_input_local_out)
 {
  input_IO_L1_in(
    /* module id */ idx, 
    /* fifo */ fifo_input_in, 
    /* fifo */ fifo_input_out, 
    /* fifo */ fifo_input_local_out);
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_boundary(int idx, hls::stream<input_t2> &fifo_input_in, hls::stream<float> &fifo_input_local_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
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
    // io_L2
    {
      if (arb == 0) {
        input_IO_L1_in_inter_trans_boundary(
          /* module id */ idx, 
          /* array */ local_input_pong, 
          /* fifo */ fifo_input_in, 
          /* enable */ inter_trans_en
        );
        input_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        input_IO_L1_in_inter_trans_boundary(
          /* module id */ idx, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_in, 
          /* enable */ inter_trans_en
        );
        input_IO_L1_in_intra_trans(
          /* module id */ idx, 
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
        /* array */ local_input_ping, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      input_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* array */ local_input_pong, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void input_IO_L1_in_boundary_wrapper(int idx, hls::stream<input_t2> &fifo_input_in, hls::stream<float> &fifo_input_local_out)
 {
  input_IO_L1_in_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_input_in, 
    /* fifo */ fifo_input_local_out);
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L2_in(kernel_t1 *kernel, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L2
  for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1) {
    // io_L1
    for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1) {
    #pragma HLS PIPELINE II=1
      // access_coalesce
      {
        kernel_t1 in_data;
        kernel_t1 out_data;
        in_data = kernel[c4 + 16*c5];
        out_data = in_data;
        fifo_kernel_local_out.write(out_data);
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L1_in_intra_trans(int idx, float local_kernel[16][1][1][1], hls::stream<float> &fifo_kernel_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // array
  // io_L2
  // io_L1
  // pe
  for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1)
    for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
      for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        // latency
        for (ap_uint<3> c8 = 0; c8 <= 3; c8 += 1) {
          // latency
          for (ap_uint<2> c9 = 0; c9 <= 1; c9 += 1) {
            // latency
            for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
            #pragma HLS PIPELINE II=1
              {
                kernel_t1 in_data;
                kernel_t1 out_data;
                in_data = local_kernel[4*c7 + c8][0][0][0];
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
void kernel_IO_L1_in_inter_trans(int idx, float local_kernel[16][1][1][1], hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c4 = p0; c4 <= 15; c4 += 1) {
    // io_L1
    if (c4 == p0) {
      for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          kernel_t1 in_data;
          kernel_t1 out_data;
          in_data = fifo_kernel_in.read();
          out_data = in_data;
          local_kernel[c5][0][0][0] = out_data;
        }
      }
    } else {
      for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1) {
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
void kernel_IO_L1_in_inter_trans_boundary(int idx, float local_kernel[16][1][1][1], hls::stream<float> &fifo_kernel_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c4 = p0; c4 <= 15; c4 += 1)
    if (c4 == p0) {
      // io_L1
      for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          kernel_t1 in_data;
          kernel_t1 out_data;
          in_data = fifo_kernel_in.read();
          out_data = in_data;
          local_kernel[c5][0][0][0] = out_data;
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L1_in(int idx, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  kernel_t1 local_kernel_ping[16][1][1][1];
  #pragma HLS RESOURCE variable=local_kernel_ping core=RAM_1P_BRAM
  float local_kernel_pong[16][1][1][1];
  #pragma HLS RESOURCE variable=local_kernel_pong core=RAM_1P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  /* Variable Declaration */

  {
    // array
    // io_L2
    {
      if (arb == 0) {
        kernel_IO_L1_in_inter_trans(
          /* module id */ idx, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_in, 
          /* fifo */ fifo_kernel_out, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        kernel_IO_L1_in_inter_trans(
          /* module id */ idx, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_in, 
          /* fifo */ fifo_kernel_out, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L1_in_intra_trans(
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
      kernel_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* array */ local_kernel_ping, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      kernel_IO_L1_in_intra_trans(
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
void kernel_IO_L1_in_wrapper(int idx, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, hls::stream<float> &fifo_kernel_local_out)
 {
  kernel_IO_L1_in(
    /* module id */ idx, 
    /* fifo */ fifo_kernel_in, 
    /* fifo */ fifo_kernel_out, 
    /* fifo */ fifo_kernel_local_out);
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L1_in_boundary(int idx, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  kernel_t1 local_kernel_ping[16][1][1][1];
  #pragma HLS RESOURCE variable=local_kernel_ping core=RAM_1P_BRAM
  float local_kernel_pong[16][1][1][1];
  #pragma HLS RESOURCE variable=local_kernel_pong core=RAM_1P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  /* Variable Declaration */

  {
    // array
    // io_L2
    {
      if (arb == 0) {
        kernel_IO_L1_in_inter_trans_boundary(
          /* module id */ idx, 
          /* array */ local_kernel_pong, 
          /* fifo */ fifo_kernel_in, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L1_in_intra_trans(
          /* module id */ idx, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        kernel_IO_L1_in_inter_trans_boundary(
          /* module id */ idx, 
          /* array */ local_kernel_ping, 
          /* fifo */ fifo_kernel_in, 
          /* enable */ inter_trans_en
        );
        kernel_IO_L1_in_intra_trans(
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
      kernel_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* array */ local_kernel_ping, 
        /* fifo */ fifo_kernel_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      kernel_IO_L1_in_intra_trans(
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
void kernel_IO_L1_in_boundary_wrapper(int idx, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_local_out)
 {
  kernel_IO_L1_in_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_kernel_in, 
    /* fifo */ fifo_kernel_local_out);
}
/* Module Definition */

/* Module Definition */
void PE(int idx, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_drain_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  input_t1 local_input[1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_input dim=0 complete
  kernel_t1 local_kernel[1][1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_kernel dim=0 complete
  output_t1 local_output[16][14][14];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // pe
  for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1)
    for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
      for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        // latency
        for (ap_uint<3> c8 = 0; c8 <= 3; c8 += 1) {
          // latency
          for (ap_uint<2> c9 = 0; c9 <= 1; c9 += 1) {
            // latency
            for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
            #pragma HLS PIPELINE II=1
              {
                local_input[0][0][0] = fifo_input_in.read();
                local_kernel[0][0][0][0] = fifo_kernel_in.read();
                if (p0 >= 1)
                  local_output[4*c7 + c8][2*c6 + c9][c10 + 2*c5] = fifo_output_in.read();
                local_output[4*c7 + c8][2*c6 + c9][c10 + 2*c5] = (local_output[4*c7 + c8][2*c6 + c9][c10 + 2*c5] + (local_input[0][0][0] * local_kernel[0][0][0][0]));
                if (p0 == 15) {
                  fifo_output_drain_out.write(local_output[4*c7 + c8][2*c6 + c9][c10 + 2*c5]);
                } else {
                  fifo_output_out.write(local_output[4*c7 + c8][2*c6 + c9][c10 + 2*c5]);
                }
              }
            }
          }
        }
      }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_drain_out)
 {
  PE(
    /* module id */ idx, 
    /* fifo */ fifo_input_in, 
    /* fifo */ fifo_kernel_in, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_output_out, 
    /* fifo */ fifo_output_drain_out);
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_intra_trans(int idx, output_t2 local_output[16][14][7], hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[2];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */


  // io_L1
  // pe
  for (ap_uint<4> c5 = 0; c5 <= 6; c5 += 1)
    for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1)
      for (ap_uint<3> c7 = 0; c7 <= 3; c7 += 1) {
        // latency
        for (ap_uint<3> c8 = 0; c8 <= 3; c8 += 1) {
          // latency
          for (ap_uint<2> c9 = 0; c9 <= 1; c9 += 1) {
            // latency
            for (ap_uint<2> c10 = 0; c10 <= 1; c10 += 1) {
            #pragma HLS PIPELINE II=1
              {
                output_t1 in_data;
                output_t2 out_data;
                in_data = fifo_output_drain_local_in.read();
                int split_idx = (c10 + 2*c5) % 2;
                out_data = local_output[4*c7 + c8][2*c6 + c9][(2 * c5 + c10) / 2];
                for (ap_uint<2> n = 0; n < 2; n++) {
                #pragma HLS UNROLL
                  data_split[n] = out_data(31, 0);
                  out_data = out_data >> 32;
                }
                union {unsigned int ui; float ut;} u;
                u.ut = in_data;
                data_split[split_idx] = ap_uint<32>(u.ui);
                out_data = (data_split[1], data_split[0]);                local_output[4*c7 + c8][2*c6 + c9][(2 * c5 + c10) / 2] = out_data;
              }
            }
          }
        }
      }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_inter_trans(int idx, output_t2 local_output[16][14][7], hls::stream<output_t2> &fifo_output_drain_in, hls::stream<output_t2> &fifo_output_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // io_L1
  if (p0 == 15) {
    for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1)
      for (ap_uint<5> c6 = 0; c6 <= 13; c6 += 1) {
        // access_coalesce
        for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            output_t2 in_data;
            output_t2 out_data;
            in_data = local_output[c5][c6][c7];
            out_data = in_data;
            fifo_output_drain_out.write(out_data);
          }
        }
      }
  } else {
    for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1)
      for (ap_uint<5> c6 = 0; c6 <= 13; c6 += 1) {
        // access_coalesce
        for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
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
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_inter_trans_boundary(int idx, output_t2 local_output[16][14][7], hls::stream<output_t2> &fifo_output_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // io_L1
  for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1)
    for (ap_uint<5> c6 = 0; c6 <= 13; c6 += 1) {
      // access_coalesce
      for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
      #pragma HLS PIPELINE II=1
        {
          output_t2 in_data;
          output_t2 out_data;
          in_data = local_output[c5][c6][c7];
          out_data = in_data;
          fifo_output_drain_out.write(out_data);
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out(int idx, hls::stream<output_t2> &fifo_output_drain_in, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t2 local_output[16][14][7];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // io_L2
  output_drain_IO_L1_out_intra_trans(
    /* module id */ idx, 
    /* array */ local_output, 
    /* fifo */ fifo_output_drain_local_in
  );
  output_drain_IO_L1_out_inter_trans(
    /* module id */ idx, 
    /* array */ local_output, 
    /* fifo */ fifo_output_drain_in, 
    /* fifo */ fifo_output_drain_out
  );
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_wrapper(int idx, hls::stream<output_t2> &fifo_output_drain_in, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in)
 {
  output_drain_IO_L1_out(
    /* module id */ idx, 
    /* fifo */ fifo_output_drain_in, 
    /* fifo */ fifo_output_drain_out, 
    /* fifo */ fifo_output_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_boundary(int idx, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t2 local_output[16][14][7];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // io_L2
  output_drain_IO_L1_out_intra_trans(
    /* module id */ idx, 
    /* array */ local_output, 
    /* fifo */ fifo_output_drain_local_in
  );
  output_drain_IO_L1_out_inter_trans_boundary(
    /* module id */ idx, 
    /* array */ local_output, 
    /* fifo */ fifo_output_drain_out
  );
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_boundary_wrapper(int idx, hls::stream<output_t2> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in)
 {
  output_drain_IO_L1_out_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_output_drain_out, 
    /* fifo */ fifo_output_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L2_out(output_t2 *output, hls::stream<output_t2> &fifo_output_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L2
  // io_L1
  for (ap_uint<5> c5 = 0; c5 <= 15; c5 += 1)
    for (ap_uint<5> c6 = 0; c6 <= 13; c6 += 1) {
      // access_coalesce
      for (ap_uint<4> c7 = 0; c7 <= 6; c7 += 1) {
      #pragma HLS PIPELINE II=1
        {
          output_t2 in_data;
          output_t2 out_data;
          in_data = fifo_output_drain_local_in.read();
          out_data = in_data;
          output[98*c5 + 7*c6 + c7] = out_data;
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
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_0;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_0 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_1;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_1 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_2;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_2 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_3;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_3 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_4;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_4 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_5;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_5 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_6;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_6 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_7;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_7 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_8;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_8 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_8 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_9;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_9 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_9 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_10;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_10 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_10 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_11;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_11 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_11 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_12;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_12 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_12 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_13;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_13 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_13 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_14;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_14 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_14 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_15;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_15 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_15 core=FIFO_SRL
  /* input_IO_L1_in fifo */ hls::stream<input_t2> fifo_input_input_IO_L1_in_16;
  #pragma HLS STREAM variable=fifo_input_input_IO_L1_in_16 depth=2
  #pragma HLS RESOURCE variable=fifo_input_input_IO_L1_in_16 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_0;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_0 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_1;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_1 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_2;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_2 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_3;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_3 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_4;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_4 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_5;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_5 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_5 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_6;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_6 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_6 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_7;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_7 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_7 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_8;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_8 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_8 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_9;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_9 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_9 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_10;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_10 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_10 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_11;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_11 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_11 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_12;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_12 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_12 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_13;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_13 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_13 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_14;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_14 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_14 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_15;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_15 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_15 core=FIFO_SRL
  /* kernel_IO_L1_in fifo */ hls::stream<float> fifo_kernel_kernel_IO_L1_in_16;
  #pragma HLS STREAM variable=fifo_kernel_kernel_IO_L1_in_16 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_kernel_IO_L1_in_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_0;
  #pragma HLS STREAM variable=fifo_input_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_1;
  #pragma HLS STREAM variable=fifo_input_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_2;
  #pragma HLS STREAM variable=fifo_input_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_3;
  #pragma HLS STREAM variable=fifo_input_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_4;
  #pragma HLS STREAM variable=fifo_input_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_5;
  #pragma HLS STREAM variable=fifo_input_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_6;
  #pragma HLS STREAM variable=fifo_input_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_7;
  #pragma HLS STREAM variable=fifo_input_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_8;
  #pragma HLS STREAM variable=fifo_input_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_9;
  #pragma HLS STREAM variable=fifo_input_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_10;
  #pragma HLS STREAM variable=fifo_input_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_11;
  #pragma HLS STREAM variable=fifo_input_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_12;
  #pragma HLS STREAM variable=fifo_input_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_13;
  #pragma HLS STREAM variable=fifo_input_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_14;
  #pragma HLS STREAM variable=fifo_input_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_input_PE_15;
  #pragma HLS STREAM variable=fifo_input_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_input_PE_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_0;
  #pragma HLS STREAM variable=fifo_kernel_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_1;
  #pragma HLS STREAM variable=fifo_kernel_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_2;
  #pragma HLS STREAM variable=fifo_kernel_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_3;
  #pragma HLS STREAM variable=fifo_kernel_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_4;
  #pragma HLS STREAM variable=fifo_kernel_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_5;
  #pragma HLS STREAM variable=fifo_kernel_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_6;
  #pragma HLS STREAM variable=fifo_kernel_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_7;
  #pragma HLS STREAM variable=fifo_kernel_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_8;
  #pragma HLS STREAM variable=fifo_kernel_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_9;
  #pragma HLS STREAM variable=fifo_kernel_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_10;
  #pragma HLS STREAM variable=fifo_kernel_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_11;
  #pragma HLS STREAM variable=fifo_kernel_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_12;
  #pragma HLS STREAM variable=fifo_kernel_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_13;
  #pragma HLS STREAM variable=fifo_kernel_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_14;
  #pragma HLS STREAM variable=fifo_kernel_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_kernel_PE_15;
  #pragma HLS STREAM variable=fifo_kernel_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_kernel_PE_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0;
  #pragma HLS STREAM variable=fifo_output_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1;
  #pragma HLS STREAM variable=fifo_output_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_2;
  #pragma HLS STREAM variable=fifo_output_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_3;
  #pragma HLS STREAM variable=fifo_output_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_4;
  #pragma HLS STREAM variable=fifo_output_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_5;
  #pragma HLS STREAM variable=fifo_output_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_6;
  #pragma HLS STREAM variable=fifo_output_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_7;
  #pragma HLS STREAM variable=fifo_output_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_8;
  #pragma HLS STREAM variable=fifo_output_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_9;
  #pragma HLS STREAM variable=fifo_output_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_10;
  #pragma HLS STREAM variable=fifo_output_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_11;
  #pragma HLS STREAM variable=fifo_output_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_12;
  #pragma HLS STREAM variable=fifo_output_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_13;
  #pragma HLS STREAM variable=fifo_output_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_14;
  #pragma HLS STREAM variable=fifo_output_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_15;
  #pragma HLS STREAM variable=fifo_output_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_16;
  #pragma HLS STREAM variable=fifo_output_PE_16 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_0;
  #pragma HLS STREAM variable=fifo_output_drain_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_1;
  #pragma HLS STREAM variable=fifo_output_drain_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_2;
  #pragma HLS STREAM variable=fifo_output_drain_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_3;
  #pragma HLS STREAM variable=fifo_output_drain_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_4;
  #pragma HLS STREAM variable=fifo_output_drain_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_5;
  #pragma HLS STREAM variable=fifo_output_drain_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_6;
  #pragma HLS STREAM variable=fifo_output_drain_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_7;
  #pragma HLS STREAM variable=fifo_output_drain_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_8;
  #pragma HLS STREAM variable=fifo_output_drain_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_9;
  #pragma HLS STREAM variable=fifo_output_drain_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_10;
  #pragma HLS STREAM variable=fifo_output_drain_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_11;
  #pragma HLS STREAM variable=fifo_output_drain_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_12;
  #pragma HLS STREAM variable=fifo_output_drain_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_13;
  #pragma HLS STREAM variable=fifo_output_drain_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_14;
  #pragma HLS STREAM variable=fifo_output_drain_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_drain_PE_15;
  #pragma HLS STREAM variable=fifo_output_drain_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_PE_15 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_4 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_5;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_5 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_6;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_6 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_7;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_7 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_8;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_8 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_9;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_9 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_10;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_10 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_11;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_11 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_12;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_12 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_13;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_13 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_14;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_14 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_15;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_15 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t2> fifo_output_drain_output_drain_IO_L1_out_16;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_16 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_16 core=FIFO_SRL
  /* FIFO Declaration */

  /* Module Call */
  input_IO_L2_in(
    /* array */ input,
    /* fifo */ fifo_input_input_IO_L1_in_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_input_input_IO_L1_in_0,
    /* fifo */ fifo_input_input_IO_L1_in_1,
    /* fifo */ fifo_input_PE_0
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_input_input_IO_L1_in_1,
    /* fifo */ fifo_input_input_IO_L1_in_2,
    /* fifo */ fifo_input_PE_1
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_input_input_IO_L1_in_2,
    /* fifo */ fifo_input_input_IO_L1_in_3,
    /* fifo */ fifo_input_PE_2
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_input_input_IO_L1_in_3,
    /* fifo */ fifo_input_input_IO_L1_in_4,
    /* fifo */ fifo_input_PE_3
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_input_input_IO_L1_in_4,
    /* fifo */ fifo_input_input_IO_L1_in_5,
    /* fifo */ fifo_input_PE_4
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_input_input_IO_L1_in_5,
    /* fifo */ fifo_input_input_IO_L1_in_6,
    /* fifo */ fifo_input_PE_5
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_input_input_IO_L1_in_6,
    /* fifo */ fifo_input_input_IO_L1_in_7,
    /* fifo */ fifo_input_PE_6
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_input_input_IO_L1_in_7,
    /* fifo */ fifo_input_input_IO_L1_in_8,
    /* fifo */ fifo_input_PE_7
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_input_input_IO_L1_in_8,
    /* fifo */ fifo_input_input_IO_L1_in_9,
    /* fifo */ fifo_input_PE_8
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_input_input_IO_L1_in_9,
    /* fifo */ fifo_input_input_IO_L1_in_10,
    /* fifo */ fifo_input_PE_9
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_input_input_IO_L1_in_10,
    /* fifo */ fifo_input_input_IO_L1_in_11,
    /* fifo */ fifo_input_PE_10
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_input_input_IO_L1_in_11,
    /* fifo */ fifo_input_input_IO_L1_in_12,
    /* fifo */ fifo_input_PE_11
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_input_input_IO_L1_in_12,
    /* fifo */ fifo_input_input_IO_L1_in_13,
    /* fifo */ fifo_input_PE_12
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_input_input_IO_L1_in_13,
    /* fifo */ fifo_input_input_IO_L1_in_14,
    /* fifo */ fifo_input_PE_13
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_input_input_IO_L1_in_14,
    /* fifo */ fifo_input_input_IO_L1_in_15,
    /* fifo */ fifo_input_PE_14
  );
  /* Module Call */

  /* Module Call */
  input_IO_L1_in_boundary_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_input_input_IO_L1_in_15,
    /* fifo */ fifo_input_PE_15
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L2_in(
    /* array */ kernel,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_0,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1,
    /* fifo */ fifo_kernel_PE_0
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_1,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2,
    /* fifo */ fifo_kernel_PE_1
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_2,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3,
    /* fifo */ fifo_kernel_PE_2
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4,
    /* fifo */ fifo_kernel_PE_3
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_4,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5,
    /* fifo */ fifo_kernel_PE_4
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_5,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6,
    /* fifo */ fifo_kernel_PE_5
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_6,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7,
    /* fifo */ fifo_kernel_PE_6
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_7,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8,
    /* fifo */ fifo_kernel_PE_7
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_8,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9,
    /* fifo */ fifo_kernel_PE_8
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_9,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10,
    /* fifo */ fifo_kernel_PE_9
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_10,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11,
    /* fifo */ fifo_kernel_PE_10
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_11,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12,
    /* fifo */ fifo_kernel_PE_11
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_12,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13,
    /* fifo */ fifo_kernel_PE_12
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_13,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14,
    /* fifo */ fifo_kernel_PE_13
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_14,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15,
    /* fifo */ fifo_kernel_PE_14
  );
  /* Module Call */

  /* Module Call */
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_15,
    /* fifo */ fifo_kernel_PE_15
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_input_PE_0,
    /* fifo */ fifo_kernel_PE_0,
    /* fifo */ fifo_output_PE_0,
    /* fifo */ fifo_output_PE_1,
    /* fifo */ fifo_output_drain_PE_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_input_PE_1,
    /* fifo */ fifo_kernel_PE_1,
    /* fifo */ fifo_output_PE_1,
    /* fifo */ fifo_output_PE_2,
    /* fifo */ fifo_output_drain_PE_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_input_PE_2,
    /* fifo */ fifo_kernel_PE_2,
    /* fifo */ fifo_output_PE_2,
    /* fifo */ fifo_output_PE_3,
    /* fifo */ fifo_output_drain_PE_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_input_PE_3,
    /* fifo */ fifo_kernel_PE_3,
    /* fifo */ fifo_output_PE_3,
    /* fifo */ fifo_output_PE_4,
    /* fifo */ fifo_output_drain_PE_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_input_PE_4,
    /* fifo */ fifo_kernel_PE_4,
    /* fifo */ fifo_output_PE_4,
    /* fifo */ fifo_output_PE_5,
    /* fifo */ fifo_output_drain_PE_4
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_input_PE_5,
    /* fifo */ fifo_kernel_PE_5,
    /* fifo */ fifo_output_PE_5,
    /* fifo */ fifo_output_PE_6,
    /* fifo */ fifo_output_drain_PE_5
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_input_PE_6,
    /* fifo */ fifo_kernel_PE_6,
    /* fifo */ fifo_output_PE_6,
    /* fifo */ fifo_output_PE_7,
    /* fifo */ fifo_output_drain_PE_6
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_input_PE_7,
    /* fifo */ fifo_kernel_PE_7,
    /* fifo */ fifo_output_PE_7,
    /* fifo */ fifo_output_PE_8,
    /* fifo */ fifo_output_drain_PE_7
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_input_PE_8,
    /* fifo */ fifo_kernel_PE_8,
    /* fifo */ fifo_output_PE_8,
    /* fifo */ fifo_output_PE_9,
    /* fifo */ fifo_output_drain_PE_8
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_input_PE_9,
    /* fifo */ fifo_kernel_PE_9,
    /* fifo */ fifo_output_PE_9,
    /* fifo */ fifo_output_PE_10,
    /* fifo */ fifo_output_drain_PE_9
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_input_PE_10,
    /* fifo */ fifo_kernel_PE_10,
    /* fifo */ fifo_output_PE_10,
    /* fifo */ fifo_output_PE_11,
    /* fifo */ fifo_output_drain_PE_10
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_input_PE_11,
    /* fifo */ fifo_kernel_PE_11,
    /* fifo */ fifo_output_PE_11,
    /* fifo */ fifo_output_PE_12,
    /* fifo */ fifo_output_drain_PE_11
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_input_PE_12,
    /* fifo */ fifo_kernel_PE_12,
    /* fifo */ fifo_output_PE_12,
    /* fifo */ fifo_output_PE_13,
    /* fifo */ fifo_output_drain_PE_12
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_input_PE_13,
    /* fifo */ fifo_kernel_PE_13,
    /* fifo */ fifo_output_PE_13,
    /* fifo */ fifo_output_PE_14,
    /* fifo */ fifo_output_drain_PE_13
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_input_PE_14,
    /* fifo */ fifo_kernel_PE_14,
    /* fifo */ fifo_output_PE_14,
    /* fifo */ fifo_output_PE_15,
    /* fifo */ fifo_output_drain_PE_14
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_input_PE_15,
    /* fifo */ fifo_kernel_PE_15,
    /* fifo */ fifo_output_PE_15,
    /* fifo */ fifo_output_PE_16,
    /* fifo */ fifo_output_drain_PE_15
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15,
    /* fifo */ fifo_output_drain_PE_15
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_15,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_14,
    /* fifo */ fifo_output_drain_PE_14
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_14,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_13,
    /* fifo */ fifo_output_drain_PE_13
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_13,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_12,
    /* fifo */ fifo_output_drain_PE_12
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_12,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_11,
    /* fifo */ fifo_output_drain_PE_11
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_11,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_10,
    /* fifo */ fifo_output_drain_PE_10
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_10,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_9,
    /* fifo */ fifo_output_drain_PE_9
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_9,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_8,
    /* fifo */ fifo_output_drain_PE_8
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_8,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_7,
    /* fifo */ fifo_output_drain_PE_7
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_7,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_6,
    /* fifo */ fifo_output_drain_PE_6
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_6,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_5,
    /* fifo */ fifo_output_drain_PE_5
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_5,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_4,
    /* fifo */ fifo_output_drain_PE_4
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_4,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3,
    /* fifo */ fifo_output_drain_PE_3
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_3,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2,
    /* fifo */ fifo_output_drain_PE_2
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_2,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1,
    /* fifo */ fifo_output_drain_PE_1
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_1,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0,
    /* fifo */ fifo_output_drain_PE_0
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L2_out(
    /* array */ output,
    /* fifo */ fifo_output_drain_output_drain_IO_L1_out_0
  );
  /* Module Call */

}
