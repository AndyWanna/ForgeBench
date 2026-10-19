#include "top_kernel.h"
/* Module Definition */
void input_IO_L2_in_intra_trans(input_t2 *input, input_t2 local_input[16][14][7], hls::stream<float> &fifo_input_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  ap_uint<32> data_split[2];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L2
  // io_L1
  // pe
  for (ap_uint<3> c5 = 0; c5 <= 2; c5 += 1)
    for (ap_uint<3> c6 = 0; c6 <= 2; c6 += 1)
      for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1)
        for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1)
          for (ap_uint<5> c9 = 0; c9 <= 15; c9 += 1) {
            // latency
            for (ap_uint<3> c10 = 0; c10 <= 3; c10 += 1) {
              // latency
              for (ap_uint<3> c11 = 0; c11 <= 3; c11 += 1) {
                // latency
                for (ap_uint<3> c12 = 0; c12 <= 3; c12 += 1) {
                #pragma HLS PIPELINE II=1
                  {
                    input_t2 in_data;
                    input_t1 out_data;
                    in_data = local_input[c9][c11 + 4*c5 + c7][(4 * c6 + c8 + c10) / 2];
                    for (ap_uint<2> n = 0; n < 2; n++) {
                    #pragma HLS UNROLL
                      data_split[n] = in_data(31, 0);
                      in_data = in_data >> 32;
                    }
                    int split_idx = (c10 + 4*c6 + c8) % 2;
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
void input_IO_L2_in_inter_trans_boundary(input_t2 *input, input_t2 local_input[16][14][7], hls::stream<input_t2> &fifo_input_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  if (!inter_trans_en) return;

  // io_L2
  for (ap_uint<5> c4 = 0; c4 <= 15; c4 += 1)
    for (ap_uint<5> c5 = 0; c5 <= 13; c5 += 1) {
      // access_coalesce
      for (ap_uint<4> c6 = 0; c6 <= 6; c6 += 1) {
      #pragma HLS PIPELINE II=1
        {
          input_t2 in_data;
          input_t2 out_data;
          in_data = input[98*c4 + 7*c5 + c6];
          out_data = in_data;
          local_input[c4][c5][c6] = out_data;
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void input_IO_L2_in_boundary(input_t2 *input, hls::stream<float> &fifo_input_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  input_t2 local_input_ping[16][14][7];
  #pragma HLS RESOURCE variable=local_input_ping core=RAM_2P_BRAM
  input_t2 local_input_pong[16][14][7];
  #pragma HLS RESOURCE variable=local_input_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  /* Variable Declaration */

  {
    // array
    {
      if (arb == 0) {
        input_IO_L2_in_inter_trans_boundary(
          /* array */ input, 
          /* array */ local_input_pong, 
          /* fifo */ fifo_input_in, 
          /* enable */ inter_trans_en
        );
        input_IO_L2_in_intra_trans(
          /* array */ input, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_local_out, 
          /* enable */ intra_trans_en
        );
      } else {
        input_IO_L2_in_inter_trans_boundary(
          /* array */ input, 
          /* array */ local_input_ping, 
          /* fifo */ fifo_input_in, 
          /* enable */ inter_trans_en
        );
        input_IO_L2_in_intra_trans(
          /* array */ input, 
          /* array */ local_input_pong, 
          /* fifo */ fifo_input_local_out, 
          /* enable */ intra_trans_en
        );
      }
      intra_trans_en = 1;
      arb = !arb;
    }
    if (arb == 0) {
      input_IO_L2_in_intra_trans(
        /* array */ input, 
        /* array */ local_input_ping, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      input_IO_L2_in_intra_trans(
        /* array */ input, 
        /* array */ local_input_pong, 
        /* fifo */ fifo_input_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L2_in(kernel_t1 *kernel, hls::stream<float> &fifo_kernel_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L2
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    // io_L1
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
      for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
        for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
          // access_coalesce
          for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1) {
          #pragma HLS PIPELINE II=1
            {
              kernel_t1 in_data;
              kernel_t1 out_data;
              in_data = kernel[576*c4 + 144*c5 + 9*c6 + 3*c7 + c8];
              out_data = in_data;
              fifo_kernel_local_out.write(out_data);
            }
          }
        }
  }
}
/* Module Definition */

/* Module Definition */
void kernel_IO_L1_in_intra_trans(int idx, float local_kernel[4][16][3][3], hls::stream<float> &fifo_kernel_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // array
  // io_L2
  // io_L1
  // pe
  for (ap_uint<3> c5 = 0; c5 <= 2; c5 += 1)
    for (ap_uint<3> c6 = 0; c6 <= 2; c6 += 1)
      for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1)
        for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1)
          for (ap_uint<5> c9 = 0; c9 <= 15; c9 += 1) {
            // latency
            for (ap_uint<3> c10 = 0; c10 <= 3; c10 += 1) {
              // latency
              for (ap_uint<3> c11 = 0; c11 <= 3; c11 += 1) {
                // latency
                for (ap_uint<3> c12 = 0; c12 <= 3; c12 += 1) {
                #pragma HLS PIPELINE II=1
                  {
                    kernel_t1 in_data;
                    kernel_t1 out_data;
                    in_data = local_kernel[c12][c9][c7][c8];
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
void kernel_IO_L1_in_inter_trans(int idx, float local_kernel[4][16][3][3], hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_kernel_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1) {
    // io_L1
    if (c4 == p0) {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
          for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
            // access_coalesce
            for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1) {
            #pragma HLS PIPELINE II=1
              {
                kernel_t1 in_data;
                kernel_t1 out_data;
                in_data = fifo_kernel_in.read();
                out_data = in_data;
                local_kernel[c5][c6][c7][c8] = out_data;
              }
            }
          }
    } else {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
          for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
            // access_coalesce
            for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1) {
            #pragma HLS PIPELINE II=1
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
void kernel_IO_L1_in_inter_trans_boundary(int idx, float local_kernel[4][16][3][3], hls::stream<float> &fifo_kernel_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1)
    if (c4 == p0) {
      // io_L1
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<5> c6 = 0; c6 <= 15; c6 += 1)
          for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
            // access_coalesce
            for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1) {
            #pragma HLS PIPELINE II=1
              {
                kernel_t1 in_data;
                kernel_t1 out_data;
                in_data = fifo_kernel_in.read();
                out_data = in_data;
                local_kernel[c5][c6][c7][c8] = out_data;
              }
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
  kernel_t1 local_kernel_ping[4][16][3][3];
  #pragma HLS RESOURCE variable=local_kernel_ping core=RAM_1P_BRAM
  float local_kernel_pong[4][16][3][3];
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
  kernel_t1 local_kernel_ping[4][16][3][3];
  #pragma HLS RESOURCE variable=local_kernel_ping core=RAM_1P_BRAM
  float local_kernel_pong[4][16][3][3];
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
void PE(int idx, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_input_out, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_output_drain_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  input_t1 local_input[1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_input dim=0 complete
  kernel_t1 local_kernel[1][1][1][1];
  #pragma HLS ARRAY_PARTITION variable=local_kernel dim=0 complete
  output_t1 local_output[4][12][12];
  #pragma HLS RESOURCE variable=local_output core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // pe
  for (ap_uint<3> c5 = 0; c5 <= 2; c5 += 1)
    for (ap_uint<3> c6 = 0; c6 <= 2; c6 += 1)
      for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1)
        for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1)
          for (ap_uint<5> c9 = 0; c9 <= 15; c9 += 1) {
            // latency
            for (ap_uint<3> c10 = 0; c10 <= 3; c10 += 1) {
              // latency
              for (ap_uint<3> c11 = 0; c11 <= 3; c11 += 1) {
                // latency
                for (ap_uint<3> c12 = 0; c12 <= 3; c12 += 1) {
                #pragma HLS PIPELINE II=1
                  {
                    local_input[0][0][0] = fifo_input_in.read();
                    local_kernel[0][0][0][0] = fifo_kernel_in.read();
                    local_output[c12][c11 + 4*c5][c10 + 4*c6] = (local_output[c12][c11 + 4*c5][c10 + 4*c6] + (local_input[0][0][0] * local_kernel[0][0][0][0]));
                    if (c7 == 2 && c8 == 2 && c9 == 15)
                      fifo_output_drain_out.write(local_output[c12][c11 + 4*c5][c10 + 4*c6]);
                    fifo_input_out.write(local_input[0][0][0]);
                  }
                }
              }
            }
          }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, hls::stream<float> &fifo_input_in, hls::stream<float> &fifo_input_out, hls::stream<float> &fifo_kernel_in, hls::stream<float> &fifo_output_drain_out)
 {
  PE(
    /* module id */ idx, 
    /* fifo */ fifo_input_in, 
    /* fifo */ fifo_input_out, 
    /* fifo */ fifo_kernel_in, 
    /* fifo */ fifo_output_drain_out);
}
/* Module Definition */

/* Module Definition */
void input_PE_dummy_in(int idx, hls::stream<float> &fifo_input_in) {
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // array
  // pe
  for (ap_uint<3> c5 = 0; c5 <= 2; c5 += 1)
    for (ap_uint<3> c6 = 0; c6 <= 2; c6 += 1)
      for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1)
        for (ap_uint<3> c8 = 0; c8 <= 2; c8 += 1)
          for (ap_uint<5> c9 = 0; c9 <= 15; c9 += 1) {
            // latency
            for (ap_uint<3> c10 = 0; c10 <= 3; c10 += 1) {
              // latency
              for (ap_uint<3> c11 = 0; c11 <= 3; c11 += 1) {
                // latency
                for (ap_uint<3> c12 = 0; c12 <= 3; c12 += 1) {
                #pragma HLS PIPELINE II=1
                  input_t1 fifo_data;
                  fifo_data = fifo_input_in.read();
                }
              }
            }
          }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_intra_trans(int idx, output_t4 local_output[4][12][3], hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */


  // io_L1
  // pe
  for (ap_uint<3> c5 = 0; c5 <= 2; c5 += 1)
    for (ap_uint<3> c6 = 0; c6 <= 2; c6 += 1) {
      // latency
      for (ap_uint<3> c10 = 0; c10 <= 3; c10 += 1) {
        // latency
        for (ap_uint<3> c11 = 0; c11 <= 3; c11 += 1) {
          // latency
          for (ap_uint<3> c12 = 0; c12 <= 3; c12 += 1) {
          #pragma HLS PIPELINE II=1
            {
              output_t1 in_data;
              output_t4 out_data;
              in_data = fifo_output_drain_local_in.read();
              int split_idx = (c10 + 4*c6) % 4;
              out_data = local_output[c12][c11 + 4*c5][(4 * c6 + c10) / 4];
              for (ap_uint<3> n = 0; n < 4; n++) {
              #pragma HLS UNROLL
                data_split[n] = out_data(31, 0);
                out_data = out_data >> 32;
              }
              union {unsigned int ui; float ut;} u;
              u.ut = in_data;
              data_split[split_idx] = ap_uint<32>(u.ui);
              out_data = (data_split[3], data_split[2], data_split[1], data_split[0]);              local_output[c12][c11 + 4*c5][(4 * c6 + c10) / 4] = out_data;
            }
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_inter_trans(int idx, output_t4 local_output[4][12][3], hls::stream<output_t4> &fifo_output_drain_in, hls::stream<output_t4> &fifo_output_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1) {
    // io_L1
    if (c4 == p0) {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<5> c6 = 0; c6 <= 11; c6 += 1) {
          // access_coalesce
          for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
          #pragma HLS PIPELINE II=1
            {
              output_t4 in_data;
              output_t4 out_data;
              in_data = local_output[c5][c6][c7];
              out_data = in_data;
              fifo_output_drain_out.write(out_data);
            }
          }
        }
    } else {
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<5> c6 = 0; c6 <= 11; c6 += 1) {
          // access_coalesce
          for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
          #pragma HLS PIPELINE II=1
            {
              output_t4 in_data;
              output_t4 out_data;
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
void output_drain_IO_L1_out_inter_trans_boundary(int idx, output_t4 local_output[4][12][3], hls::stream<output_t4> &fifo_output_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<3> c4 = p0; c4 <= 3; c4 += 1)
    if (c4 == p0) {
      // io_L1
      for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
        for (ap_uint<5> c6 = 0; c6 <= 11; c6 += 1) {
          // access_coalesce
          for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
          #pragma HLS PIPELINE II=1
            {
              output_t4 in_data;
              output_t4 out_data;
              in_data = local_output[c5][c6][c7];
              out_data = in_data;
              fifo_output_drain_out.write(out_data);
            }
          }
        }
    }
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out(int idx, hls::stream<output_t4> &fifo_output_drain_in, hls::stream<output_t4> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t4 local_output[4][12][3];
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
void output_drain_IO_L1_out_wrapper(int idx, hls::stream<output_t4> &fifo_output_drain_in, hls::stream<output_t4> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in)
 {
  output_drain_IO_L1_out(
    /* module id */ idx, 
    /* fifo */ fifo_output_drain_in, 
    /* fifo */ fifo_output_drain_out, 
    /* fifo */ fifo_output_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L1_out_boundary(int idx, hls::stream<output_t4> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t4 local_output[4][12][3];
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
void output_drain_IO_L1_out_boundary_wrapper(int idx, hls::stream<output_t4> &fifo_output_drain_out, hls::stream<float> &fifo_output_drain_local_in)
 {
  output_drain_IO_L1_out_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_output_drain_out, 
    /* fifo */ fifo_output_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void output_drain_IO_L2_out(output_t4 *output, hls::stream<output_t4> &fifo_output_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L2
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    // io_L1
    for (ap_uint<3> c5 = 0; c5 <= 3; c5 += 1)
      for (ap_uint<5> c6 = 0; c6 <= 11; c6 += 1) {
        // access_coalesce
        for (ap_uint<3> c7 = 0; c7 <= 2; c7 += 1) {
        #pragma HLS PIPELINE II=1
          {
            output_t4 in_data;
            output_t4 out_data;
            in_data = fifo_output_drain_local_in.read();
            out_data = in_data;
            output[144*c4 + 36*c5 + 3*c6 + c7] = out_data;
          }
        }
      }
  }
}
/* Module Definition */

void kernel0(input_t2 *input, kernel_t1 *kernel, output_t4 *output)
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
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t4> fifo_output_drain_output_drain_IO_L1_out_0;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_0 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t4> fifo_output_drain_output_drain_IO_L1_out_1;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_1 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t4> fifo_output_drain_output_drain_IO_L1_out_2;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_2 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t4> fifo_output_drain_output_drain_IO_L1_out_3;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_3 core=FIFO_SRL
  /* output_drain_IO_L1_out fifo */ hls::stream<output_t4> fifo_output_drain_output_drain_IO_L1_out_4;
  #pragma HLS STREAM variable=fifo_output_drain_output_drain_IO_L1_out_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_drain_output_drain_IO_L1_out_4 core=FIFO_SRL
  /* FIFO Declaration */

  /* Module Call */
  input_IO_L2_in_boundary(
    /* array */ input,
    /* fifo */ fifo_input_PE_0
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
  kernel_IO_L1_in_boundary_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_kernel_kernel_IO_L1_in_3,
    /* fifo */ fifo_kernel_PE_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_input_PE_0,
    /* fifo */ fifo_input_PE_1,
    /* fifo */ fifo_kernel_PE_0,
    /* fifo */ fifo_output_drain_PE_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_input_PE_1,
    /* fifo */ fifo_input_PE_2,
    /* fifo */ fifo_kernel_PE_1,
    /* fifo */ fifo_output_drain_PE_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_input_PE_2,
    /* fifo */ fifo_input_PE_3,
    /* fifo */ fifo_kernel_PE_2,
    /* fifo */ fifo_output_drain_PE_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_input_PE_3,
    /* fifo */ fifo_input_PE_4,
    /* fifo */ fifo_kernel_PE_3,
    /* fifo */ fifo_output_drain_PE_3
  );
  /* Module Call */

  /* Module Call */
  input_PE_dummy_in(
    /* module id */ 3,
    /* fifo */ fifo_input_PE_4
  );
  /* Module Call */

  /* Module Call */
  output_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 3,
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
