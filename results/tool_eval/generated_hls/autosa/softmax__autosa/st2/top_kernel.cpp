#include "top_kernel.h"
typedef float data_t;
/* Module Definition */
void output_IO_L3_in(output_t1 *output, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
    // array
    // io_L3
    for (ap_uint<5> c2 = 0; c2 <= 15; c2 += 1) {
      // io_L2
      for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
        // io_L1
        for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
        #pragma HLS PIPELINE II=1
          // access_coalesce
          {
            output_t1 in_data;
            output_t1 out_data;
            in_data = output[16*c1 + c2 + 512*c3 + 128*c4];
            out_data = in_data;
            fifo_output_local_out.write(out_data);
          }
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L2_in(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
    // array
    // io_L3
    for (ap_uint<5> c2 = p0; c2 <= 15; c2 += 1) {
      // io_L2
      if (c2 == p0) {
        for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
          // io_L1
          for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
          #pragma HLS PIPELINE II=1
            // access_coalesce
            {
              output_t1 in_data;
              output_t1 out_data;
              in_data = fifo_output_in.read();
              out_data = in_data;
              fifo_output_local_out.write(out_data);
            }
          }
        }
      } else {
        for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
          // io_L1
          for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
          #pragma HLS PIPELINE II=1
            // access_coalesce
            {
              output_t1 in_data;
              output_t1 out_data;
              in_data = fifo_output_in.read();
              out_data = in_data;
              fifo_output_out.write(out_data);
            }
          }
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L2_in_boundary(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
    // array
    // io_L3
    for (ap_uint<5> c2 = p0; c2 <= 15; c2 += 1)
      if (c2 == p0) {
        // io_L2
        for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
          // io_L1
          for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
          #pragma HLS PIPELINE II=1
            // access_coalesce
            {
              output_t1 in_data;
              output_t1 out_data;
              in_data = fifo_output_in.read();
              out_data = in_data;
              fifo_output_local_out.write(out_data);
            }
          }
        }
      }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_intra_trans(int idx, int idy, int c1, float local_output[4][1], hls::stream<float> &fifo_output_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L1
  // pe
  // latency
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
  #pragma HLS PIPELINE II=1
    {
      output_t1 in_data;
      output_t1 out_data;
      in_data = local_output[c4][0];
      out_data = in_data;
      fifo_output_local_out.write(out_data);
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_inter_trans(int idx, int idy, int c1, float local_output[4][1], hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c3 = p1; c3 <= 1; c3 += 1) {
    // io_L1
    if (c3 == p1) {
      for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          output_t1 in_data;
          output_t1 out_data;
          in_data = fifo_output_in.read();
          out_data = in_data;
          local_output[c4][0] = out_data;
        }
      }
    } else {
      for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          output_t1 in_data;
          output_t1 out_data;
          in_data = fifo_output_in.read();
          out_data = in_data;
          fifo_output_out.write(out_data);
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_inter_trans_boundary(int idx, int idy, int c1, float local_output[4][1], hls::stream<float> &fifo_output_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c3 = p1; c3 <= 1; c3 += 1)
    if (c3 == p1) {
      // io_L1
      for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          output_t1 in_data;
          output_t1 out_data;
          in_data = fifo_output_in.read();
          out_data = in_data;
          local_output[c4][0] = out_data;
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in(int idx, int idy, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  output_t1 local_output_ping[4][1];
  #pragma HLS ARRAY_PARTITION variable=local_output_ping dim=0 complete
  float local_output_pong[4][1];
  #pragma HLS ARRAY_PARTITION variable=local_output_pong dim=0 complete
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c1, c1_prev;
  /* Variable Declaration */

  {
    for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
      // array
      // io_L3
      // io_L2
      {
        if (arb == 0) {
          output_IO_L1_in_inter_trans(
            /* module id */ idx, 
            /* module id */ idy, 
            /* host iter */ c1, 
            /* array */ local_output_pong, 
            /* fifo */ fifo_output_in, 
            /* fifo */ fifo_output_out, 
            /* enable */ inter_trans_en
          );
          output_IO_L1_in_intra_trans(
            /* module id */ idx, 
            /* module id */ idy, 
            /* host iter */ c1_prev, 
            /* array */ local_output_ping, 
            /* fifo */ fifo_output_local_out, 
            /* enable */ intra_trans_en
          );
        } else {
          output_IO_L1_in_inter_trans(
            /* module id */ idx, 
            /* module id */ idy, 
            /* host iter */ c1, 
            /* array */ local_output_ping, 
            /* fifo */ fifo_output_in, 
            /* fifo */ fifo_output_out, 
            /* enable */ inter_trans_en
          );
          output_IO_L1_in_intra_trans(
            /* module id */ idx, 
            /* module id */ idy, 
            /* host iter */ c1_prev, 
            /* array */ local_output_pong, 
            /* fifo */ fifo_output_local_out, 
            /* enable */ intra_trans_en
          );
        }
        intra_trans_en = 1;
        arb = !arb;
        c1_prev = c1;
      }
    }
    if (arb == 0) {
      output_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* host iter */ c1_prev, 
        /* array */ local_output_ping, 
        /* fifo */ fifo_output_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      output_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* host iter */ c1_prev, 
        /* array */ local_output_pong, 
        /* fifo */ fifo_output_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_wrapper(int idx, int idy, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_local_out)
 {
  output_IO_L1_in(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_output_out, 
    /* fifo */ fifo_output_local_out);
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_boundary(int idx, int idy, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  output_t1 local_output_ping[4][1];
  #pragma HLS ARRAY_PARTITION variable=local_output_ping dim=0 complete
  float local_output_pong[4][1];
  #pragma HLS ARRAY_PARTITION variable=local_output_pong dim=0 complete
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c1, c1_prev;
  /* Variable Declaration */

  {
    for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
      // array
      // io_L3
      // io_L2
      {
        if (arb == 0) {
          output_IO_L1_in_inter_trans_boundary(
            /* module id */ idx, 
            /* module id */ idy, 
            /* host iter */ c1, 
            /* array */ local_output_pong, 
            /* fifo */ fifo_output_in, 
            /* enable */ inter_trans_en
          );
          output_IO_L1_in_intra_trans(
            /* module id */ idx, 
            /* module id */ idy, 
            /* host iter */ c1_prev, 
            /* array */ local_output_ping, 
            /* fifo */ fifo_output_local_out, 
            /* enable */ intra_trans_en
          );
        } else {
          output_IO_L1_in_inter_trans_boundary(
            /* module id */ idx, 
            /* module id */ idy, 
            /* host iter */ c1, 
            /* array */ local_output_ping, 
            /* fifo */ fifo_output_in, 
            /* enable */ inter_trans_en
          );
          output_IO_L1_in_intra_trans(
            /* module id */ idx, 
            /* module id */ idy, 
            /* host iter */ c1_prev, 
            /* array */ local_output_pong, 
            /* fifo */ fifo_output_local_out, 
            /* enable */ intra_trans_en
          );
        }
        intra_trans_en = 1;
        arb = !arb;
        c1_prev = c1;
      }
    }
    if (arb == 0) {
      output_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* host iter */ c1_prev, 
        /* array */ local_output_ping, 
        /* fifo */ fifo_output_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      output_IO_L1_in_intra_trans(
        /* module id */ idx, 
        /* module id */ idy, 
        /* host iter */ c1_prev, 
        /* array */ local_output_pong, 
        /* fifo */ fifo_output_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_boundary_wrapper(int idx, int idy, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_local_out)
 {
  output_IO_L1_in_boundary(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_output_local_out);
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_in_intra_trans(int idx, sum_t0 local_sum[4], hls::stream<float> &fifo_sum_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  for (ap_uint<4> c1 = 1; c1 <= 7; c1 += 1) {
    // array
    // io_L3
    // io_L2
    // io_L1
    // pe
    // latency
    for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    #pragma HLS PIPELINE II=1
      {
        sum_t4 in_data;
        sum_t1 out_data;
        in_data = local_sum[c4 / 4];
        for (ap_uint<3> n = 0; n < 4; n++) {
        #pragma HLS UNROLL
          data_split[n] = in_data(31, 0);
          in_data = in_data >> 32;
        }
        int split_idx = (c4) % 4;
        union {unsigned int ui; float ut;} u;
        u.ui = (unsigned int)data_split[split_idx];
        out_data = u.ut;
        fifo_sum_local_out.write(out_data);
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_in(int idx, hls::stream<sum_t0> &fifo_sum_in, hls::stream<sum_t0> &fifo_sum_out, hls::stream<float> &fifo_sum_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  sum_t0 local_sum_ping[4];
  #pragma HLS RESOURCE variable=local_sum_ping core=RAM_2P_BRAM
  sum_t0 local_sum_pong[4];
  #pragma HLS RESOURCE variable=local_sum_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  /* Variable Declaration */

  if (arb == 0) {
    sum_IO_L2_in_intra_trans(
      /* module id */ idx, 
      /* array */ local_sum_ping, 
      /* fifo */ fifo_sum_local_out, 
      /* enable */ intra_trans_en
    );
  } else {
    sum_IO_L2_in_intra_trans(
      /* module id */ idx, 
      /* array */ local_sum_pong, 
      /* fifo */ fifo_sum_local_out, 
      /* enable */ intra_trans_en
    );
  }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_in_boundary(int idx, hls::stream<sum_t0> &fifo_sum_in, hls::stream<float> &fifo_sum_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  sum_t0 local_sum_ping[4];
  #pragma HLS RESOURCE variable=local_sum_ping core=RAM_2P_BRAM
  sum_t0 local_sum_pong[4];
  #pragma HLS RESOURCE variable=local_sum_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  /* Variable Declaration */

  if (arb == 0) {
    sum_IO_L2_in_intra_trans(
      /* module id */ idx, 
      /* array */ local_sum_ping, 
      /* fifo */ fifo_sum_local_out, 
      /* enable */ intra_trans_en
    );
  } else {
    sum_IO_L2_in_intra_trans(
      /* module id */ idx, 
      /* array */ local_sum_pong, 
      /* fifo */ fifo_sum_local_out, 
      /* enable */ intra_trans_en
    );
  }
}
/* Module Definition */

/* Module Definition */
void PE(int idx, int idy, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_sum_in, hls::stream<float> &fifo_sum_out, hls::stream<float> &fifo_sum_drain_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  output_t1 local_output[1][1];
  #pragma HLS ARRAY_PARTITION variable=local_output dim=0 complete
  sum_t1 local_sum[4];
  #pragma HLS ARRAY_PARTITION variable=local_sum dim=0 complete
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
    // array
    // pe
    // latency
    for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    #pragma HLS PIPELINE II=1
      {
        local_output[0][0] = fifo_output_in.read();
        if (p1 + 16 * c1 >= 1)
          local_sum[c4] = fifo_sum_in.read();
        local_sum[c4] = (local_sum[c4] + local_output[0][0]);
        if (p1 == 15 && c1 == 7) {
          fifo_sum_drain_out.write(local_sum[c4]);
        } else {
          fifo_sum_out.write(local_sum[c4]);
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, int idy, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_sum_in, hls::stream<float> &fifo_sum_out, hls::stream<float> &fifo_sum_drain_out)
 {
  PE(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_sum_in, 
    /* fifo */ fifo_sum_out, 
    /* fifo */ fifo_sum_drain_out);
}
/* Module Definition */

/* Module Definition */
void sum_PE_dummy_out(int idx, int idy, hls::stream<float> &fifo_sum_out) {
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1)
    if (p1 + 16 * c1 >= 1) {
      // array
      // pe
      // latency
      for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
      #pragma HLS PIPELINE II=1
        sum_t1 fifo_data = 0;
        fifo_sum_out.write(fifo_data);
      }
    }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_out_intra_trans(int idx, sum_t4 local_sum[1], hls::stream<float> &fifo_sum_local_in, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  for (ap_uint<4> c1 = 0; c1 <= 6; c1 += 1) {
    // array
    // io_L3
    // io_L2
    // io_L1
    // pe
    // latency
    for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    #pragma HLS PIPELINE II=1
      {
        sum_t1 in_data;
        sum_t4 out_data;
        in_data = fifo_sum_local_in.read();
        int split_idx = (c4) % 4;
        out_data = local_sum[c4 / 4];
        for (ap_uint<3> n = 0; n < 4; n++) {
        #pragma HLS UNROLL
          data_split[n] = out_data(31, 0);
          out_data = out_data >> 32;
        }
        union {unsigned int ui; float ut;} u;
        u.ut = in_data;
        data_split[split_idx] = ap_uint<32>(u.ui);
        out_data = (data_split[3], data_split[2], data_split[1], data_split[0]);        local_sum[c4 / 4] = out_data;
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_out_inter_trans(int idx, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_in, hls::stream<sum_t4> &fifo_sum_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c2 = p0; c2 <= 1; c2 += 1) {
  #pragma HLS PIPELINE II=1
    // io_L2
    if (c2 == p0) {
      // access_coalesce
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = local_sum[0];
        out_data = in_data;
        fifo_sum_out.write(out_data);
      }
    } else {
      // access_coalesce
      // hls_pipeline
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = fifo_sum_in.read();
        out_data = in_data;
        fifo_sum_out.write(out_data);
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_out_inter_trans_boundary(int idx, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c2 = p0; c2 <= 1; c2 += 1)
  #pragma HLS PIPELINE II=1
    if (c2 == p0) {
      // io_L2
      // access_coalesce
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = local_sum[0];
        out_data = in_data;
        fifo_sum_out.write(out_data);
      }
    }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_out(int idx, hls::stream<sum_t4> &fifo_sum_in, hls::stream<sum_t4> &fifo_sum_out, hls::stream<float> &fifo_sum_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  sum_t4 local_sum_ping[1];
  #pragma HLS RESOURCE variable=local_sum_ping core=RAM_2P_BRAM
  sum_t4 local_sum_pong[1];
  #pragma HLS RESOURCE variable=local_sum_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 0;
  bool intra_trans_en = 1;
  /* Variable Declaration */

  {
    // array
    // io_L3
    {
      if (arb == 0) {
        sum_IO_L2_out_intra_trans(
          /* module id */ idx, 
          /* array */ local_sum_ping, 
          /* fifo */ fifo_sum_local_in, 
          /* enable */ intra_trans_en
        );
        sum_IO_L2_out_inter_trans(
          /* module id */ idx, 
          /* array */ local_sum_pong, 
          /* fifo */ fifo_sum_in, 
          /* fifo */ fifo_sum_out, 
          /* enable */ inter_trans_en
        );
      } else {
        sum_IO_L2_out_intra_trans(
          /* module id */ idx, 
          /* array */ local_sum_pong, 
          /* fifo */ fifo_sum_local_in, 
          /* enable */ intra_trans_en
        );
        sum_IO_L2_out_inter_trans(
          /* module id */ idx, 
          /* array */ local_sum_ping, 
          /* fifo */ fifo_sum_in, 
          /* fifo */ fifo_sum_out, 
          /* enable */ inter_trans_en
        );
      }
      inter_trans_en = 1;
      arb = !arb;
    }
    if (arb == 0) {
      sum_IO_L2_out_inter_trans(
        /* module id */ idx, 
        /* array */ local_sum_pong, 
        /* fifo */ fifo_sum_in, 
        /* fifo */ fifo_sum_out, 
        /* enable */ inter_trans_en
      );
    } else {
      sum_IO_L2_out_inter_trans(
        /* module id */ idx, 
        /* array */ local_sum_ping, 
        /* fifo */ fifo_sum_in, 
        /* fifo */ fifo_sum_out, 
        /* enable */ inter_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_out_boundary(int idx, hls::stream<sum_t4> &fifo_sum_out, hls::stream<float> &fifo_sum_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  sum_t4 local_sum_ping[1];
  #pragma HLS RESOURCE variable=local_sum_ping core=RAM_2P_BRAM
  sum_t4 local_sum_pong[1];
  #pragma HLS RESOURCE variable=local_sum_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 0;
  bool intra_trans_en = 1;
  /* Variable Declaration */

  {
    // array
    // io_L3
    {
      if (arb == 0) {
        sum_IO_L2_out_intra_trans(
          /* module id */ idx, 
          /* array */ local_sum_ping, 
          /* fifo */ fifo_sum_local_in, 
          /* enable */ intra_trans_en
        );
        sum_IO_L2_out_inter_trans_boundary(
          /* module id */ idx, 
          /* array */ local_sum_pong, 
          /* fifo */ fifo_sum_out, 
          /* enable */ inter_trans_en
        );
      } else {
        sum_IO_L2_out_intra_trans(
          /* module id */ idx, 
          /* array */ local_sum_pong, 
          /* fifo */ fifo_sum_local_in, 
          /* enable */ intra_trans_en
        );
        sum_IO_L2_out_inter_trans_boundary(
          /* module id */ idx, 
          /* array */ local_sum_ping, 
          /* fifo */ fifo_sum_out, 
          /* enable */ inter_trans_en
        );
      }
      inter_trans_en = 1;
      arb = !arb;
    }
    if (arb == 0) {
      sum_IO_L2_out_inter_trans_boundary(
        /* module id */ idx, 
        /* array */ local_sum_pong, 
        /* fifo */ fifo_sum_out, 
        /* enable */ inter_trans_en
      );
    } else {
      sum_IO_L2_out_inter_trans_boundary(
        /* module id */ idx, 
        /* array */ local_sum_ping, 
        /* fifo */ fifo_sum_out, 
        /* enable */ inter_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L3_out(sum_t4 *sum, hls::stream<sum_t4> &fifo_sum_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L3
  for (ap_uint<2> c2 = 0; c2 <= 1; c2 += 1) {
  #pragma HLS PIPELINE II=1
    // io_L2
    // access_coalesce
    {
      sum_t4 in_data;
      sum_t4 out_data;
      in_data = fifo_sum_local_in.read();
      out_data = in_data;
      sum[c2] = out_data;
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_intra_trans(int idx, int idy, sum_t4 local_sum[1], hls::stream<float> &fifo_sum_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */


  // io_L1
  // pe
  // latency
  for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
  #pragma HLS PIPELINE II=1
    {
      sum_t1 in_data;
      sum_t4 out_data;
      in_data = fifo_sum_drain_local_in.read();
      int split_idx = (c4) % 4;
      out_data = local_sum[c4 / 4];
      for (ap_uint<3> n = 0; n < 4; n++) {
      #pragma HLS UNROLL
        data_split[n] = out_data(31, 0);
        out_data = out_data >> 32;
      }
      union {unsigned int ui; float ut;} u;
      u.ut = in_data;
      data_split[split_idx] = ap_uint<32>(u.ui);
      out_data = (data_split[3], data_split[2], data_split[1], data_split[0]);      local_sum[c4 / 4] = out_data;
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_inter_trans(int idx, int idy, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<2> c3 = p1; c3 <= 1; c3 += 1) {
  #pragma HLS PIPELINE II=1
    // io_L1
    if (c3 == p1) {
      // access_coalesce
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = local_sum[0];
        out_data = in_data;
        fifo_sum_drain_out.write(out_data);
      }
    } else {
      // access_coalesce
      // hls_pipeline
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = fifo_sum_drain_in.read();
        out_data = in_data;
        fifo_sum_drain_out.write(out_data);
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_inter_trans_boundary(int idx, int idy, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  /* Variable Declaration */

  for (ap_uint<2> c3 = p1; c3 <= 1; c3 += 1)
  #pragma HLS PIPELINE II=1
    if (c3 == p1) {
      // io_L1
      // access_coalesce
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = local_sum[0];
        out_data = in_data;
        fifo_sum_drain_out.write(out_data);
      }
    }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out(int idx, int idy, hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  sum_t4 local_sum[1];
  #pragma HLS RESOURCE variable=local_sum core=RAM_2P_BRAM
  /* Variable Declaration */

  if (p0 == 15) {
    // array
    // io_L3
    // io_L2
    sum_drain_IO_L1_out_intra_trans(
      /* module id */ idx, 
      /* module id */ idy, 
      /* array */ local_sum, 
      /* fifo */ fifo_sum_drain_local_in
    );
    sum_drain_IO_L1_out_inter_trans(
      /* module id */ idx, 
      /* module id */ idy, 
      /* array */ local_sum, 
      /* fifo */ fifo_sum_drain_in, 
      /* fifo */ fifo_sum_drain_out
    );
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_wrapper(int idx, int idy, hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in)
 {
  sum_drain_IO_L1_out(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_sum_drain_in, 
    /* fifo */ fifo_sum_drain_out, 
    /* fifo */ fifo_sum_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_boundary(int idx, int idy, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx, p1 = idy; // module id
  sum_t4 local_sum[1];
  #pragma HLS RESOURCE variable=local_sum core=RAM_2P_BRAM
  /* Variable Declaration */

  if (p0 == 15) {
    // array
    // io_L3
    // io_L2
    sum_drain_IO_L1_out_intra_trans(
      /* module id */ idx, 
      /* module id */ idy, 
      /* array */ local_sum, 
      /* fifo */ fifo_sum_drain_local_in
    );
    sum_drain_IO_L1_out_inter_trans_boundary(
      /* module id */ idx, 
      /* module id */ idy, 
      /* array */ local_sum, 
      /* fifo */ fifo_sum_drain_out
    );
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_boundary_wrapper(int idx, int idy, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in)
 {
  sum_drain_IO_L1_out_boundary(
    /* module id */ idx, 
    /* module id */ idy, 
    /* fifo */ fifo_sum_drain_out, 
    /* fifo */ fifo_sum_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L2_out(int idx, hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<sum_t4> &fifo_sum_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // array
  // io_L3
  // io_L2
  if (p0 == 15) {
    for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
    #pragma HLS PIPELINE II=1
      // io_L1
      // access_coalesce
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = fifo_sum_drain_local_in.read();
        out_data = in_data;
        fifo_sum_drain_out.write(out_data);
      }
    }
  } else {
    for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
    #pragma HLS PIPELINE II=1
      // io_L1
      // access_coalesce
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = fifo_sum_drain_in.read();
        out_data = in_data;
        fifo_sum_drain_out.write(out_data);
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L2_out_boundary(int idx, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<sum_t4> &fifo_sum_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (p0 == 15) {
    // array
    // io_L3
    // io_L2
    for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
    #pragma HLS PIPELINE II=1
      // io_L1
      // access_coalesce
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = fifo_sum_drain_local_in.read();
        out_data = in_data;
        fifo_sum_drain_out.write(out_data);
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L3_out(sum_t4 *sum, hls::stream<sum_t4> &fifo_sum_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L3
  // io_L2
  for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
  #pragma HLS PIPELINE II=1
    // io_L1
    // access_coalesce
    {
      sum_t4 in_data;
      sum_t4 out_data;
      in_data = fifo_sum_drain_local_in.read();
      out_data = in_data;
      sum[c3] = out_data;
    }
  }
}
/* Module Definition */

void kernel0(output_t1 *output, sum_t4 *sum_0, sum_t4 *sum_1, sum_t4 *sum_2)
{
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem_output
#pragma HLS INTERFACE m_axi port=sum_0 offset=slave bundle=gmem_sum_0
#pragma HLS INTERFACE m_axi port=sum_1 offset=slave bundle=gmem_sum_1
#pragma HLS INTERFACE m_axi port=sum_2 offset=slave bundle=gmem_sum_2
#pragma HLS INTERFACE s_axilite port=output bundle=control
#pragma HLS INTERFACE s_axilite port=sum_0 bundle=control
#pragma HLS INTERFACE s_axilite port=sum_1 bundle=control
#pragma HLS INTERFACE s_axilite port=sum_2 bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATAFLOW

  /* FIFO Declaration */
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_0 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_1 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_2 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_3;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_3 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_4;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_4 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_5;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_5 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_6;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_6 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_7;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_7 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_8;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_8 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_9;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_9 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_10;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_10 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_11;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_11 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_12;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_12 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_13;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_13 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_14;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_14 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_15;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_15 core=FIFO_SRL
  /* output_IO_L2_in fifo */ hls::stream<float> fifo_output_output_IO_L2_in_16;
  #pragma HLS STREAM variable=fifo_output_output_IO_L2_in_16 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L2_in_16 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_0_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_0_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_0_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_0_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_0_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_0_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_1_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_1_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_1_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_1_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_1_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_1_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_2_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_2_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_2_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_2_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_2_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_2_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_2_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_3_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_3_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_3_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_3_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_3_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_3_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_3_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_3_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_3_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_4_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_4_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_4_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_4_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_4_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_4_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_4_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_4_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_4_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_5_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_5_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_5_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_5_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_5_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_5_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_5_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_5_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_5_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_6_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_6_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_6_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_6_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_6_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_6_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_6_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_6_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_6_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_7_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_7_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_7_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_7_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_7_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_7_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_7_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_7_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_7_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_8_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_8_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_8_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_8_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_8_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_8_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_8_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_8_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_8_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_9_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_9_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_9_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_9_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_9_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_9_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_9_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_9_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_9_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_10_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_10_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_10_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_10_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_10_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_10_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_10_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_10_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_10_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_11_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_11_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_11_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_11_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_11_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_11_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_11_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_11_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_11_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_12_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_12_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_12_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_12_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_12_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_12_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_12_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_12_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_12_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_13_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_13_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_13_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_13_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_13_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_13_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_13_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_13_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_13_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_14_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_14_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_14_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_14_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_14_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_14_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_14_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_14_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_14_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_15_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_15_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_15_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_15_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_15_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_15_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_15_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_15_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_15_2 core=FIFO_SRL
  /* sum_IO_L2_in fifo */ hls::stream<sum_t0> fifo_sum_sum_IO_L2_in_0;
  #pragma HLS STREAM variable=fifo_sum_sum_IO_L2_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_sum_IO_L2_in_0 core=FIFO_SRL
  /* sum_IO_L2_in fifo */ hls::stream<sum_t0> fifo_sum_sum_IO_L2_in_1;
  #pragma HLS STREAM variable=fifo_sum_sum_IO_L2_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_sum_IO_L2_in_1 core=FIFO_SRL
  /* sum_IO_L2_in fifo */ hls::stream<sum_t0> fifo_sum_sum_IO_L2_in_2;
  #pragma HLS STREAM variable=fifo_sum_sum_IO_L2_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_sum_IO_L2_in_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_0;
  #pragma HLS STREAM variable=fifo_output_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_0;
  #pragma HLS STREAM variable=fifo_output_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_1;
  #pragma HLS STREAM variable=fifo_output_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_1;
  #pragma HLS STREAM variable=fifo_output_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_2;
  #pragma HLS STREAM variable=fifo_output_PE_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_2;
  #pragma HLS STREAM variable=fifo_output_PE_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_3;
  #pragma HLS STREAM variable=fifo_output_PE_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_3;
  #pragma HLS STREAM variable=fifo_output_PE_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_4;
  #pragma HLS STREAM variable=fifo_output_PE_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_4;
  #pragma HLS STREAM variable=fifo_output_PE_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_5;
  #pragma HLS STREAM variable=fifo_output_PE_0_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_5;
  #pragma HLS STREAM variable=fifo_output_PE_1_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_6;
  #pragma HLS STREAM variable=fifo_output_PE_0_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_6;
  #pragma HLS STREAM variable=fifo_output_PE_1_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_7;
  #pragma HLS STREAM variable=fifo_output_PE_0_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_7;
  #pragma HLS STREAM variable=fifo_output_PE_1_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_8;
  #pragma HLS STREAM variable=fifo_output_PE_0_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_8;
  #pragma HLS STREAM variable=fifo_output_PE_1_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_9;
  #pragma HLS STREAM variable=fifo_output_PE_0_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_9;
  #pragma HLS STREAM variable=fifo_output_PE_1_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_10;
  #pragma HLS STREAM variable=fifo_output_PE_0_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_10;
  #pragma HLS STREAM variable=fifo_output_PE_1_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_11;
  #pragma HLS STREAM variable=fifo_output_PE_0_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_11;
  #pragma HLS STREAM variable=fifo_output_PE_1_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_12;
  #pragma HLS STREAM variable=fifo_output_PE_0_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_12;
  #pragma HLS STREAM variable=fifo_output_PE_1_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_13;
  #pragma HLS STREAM variable=fifo_output_PE_0_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_13;
  #pragma HLS STREAM variable=fifo_output_PE_1_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_14;
  #pragma HLS STREAM variable=fifo_output_PE_0_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_14;
  #pragma HLS STREAM variable=fifo_output_PE_1_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0_15;
  #pragma HLS STREAM variable=fifo_output_PE_0_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1_15;
  #pragma HLS STREAM variable=fifo_output_PE_1_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_0;
  #pragma HLS STREAM variable=fifo_sum_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_1;
  #pragma HLS STREAM variable=fifo_sum_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_2;
  #pragma HLS STREAM variable=fifo_sum_PE_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_3;
  #pragma HLS STREAM variable=fifo_sum_PE_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_4;
  #pragma HLS STREAM variable=fifo_sum_PE_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_5;
  #pragma HLS STREAM variable=fifo_sum_PE_0_5 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_6;
  #pragma HLS STREAM variable=fifo_sum_PE_0_6 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_7;
  #pragma HLS STREAM variable=fifo_sum_PE_0_7 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_8;
  #pragma HLS STREAM variable=fifo_sum_PE_0_8 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_9;
  #pragma HLS STREAM variable=fifo_sum_PE_0_9 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_10;
  #pragma HLS STREAM variable=fifo_sum_PE_0_10 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_11;
  #pragma HLS STREAM variable=fifo_sum_PE_0_11 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_12;
  #pragma HLS STREAM variable=fifo_sum_PE_0_12 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_13;
  #pragma HLS STREAM variable=fifo_sum_PE_0_13 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_14;
  #pragma HLS STREAM variable=fifo_sum_PE_0_14 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_15;
  #pragma HLS STREAM variable=fifo_sum_PE_0_15 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0_16;
  #pragma HLS STREAM variable=fifo_sum_PE_0_16 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_0;
  #pragma HLS STREAM variable=fifo_sum_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_1;
  #pragma HLS STREAM variable=fifo_sum_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_2;
  #pragma HLS STREAM variable=fifo_sum_PE_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_3;
  #pragma HLS STREAM variable=fifo_sum_PE_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_4;
  #pragma HLS STREAM variable=fifo_sum_PE_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_5;
  #pragma HLS STREAM variable=fifo_sum_PE_1_5 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_6;
  #pragma HLS STREAM variable=fifo_sum_PE_1_6 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_7;
  #pragma HLS STREAM variable=fifo_sum_PE_1_7 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_8;
  #pragma HLS STREAM variable=fifo_sum_PE_1_8 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_9;
  #pragma HLS STREAM variable=fifo_sum_PE_1_9 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_10;
  #pragma HLS STREAM variable=fifo_sum_PE_1_10 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_11;
  #pragma HLS STREAM variable=fifo_sum_PE_1_11 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_12;
  #pragma HLS STREAM variable=fifo_sum_PE_1_12 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_13;
  #pragma HLS STREAM variable=fifo_sum_PE_1_13 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_14;
  #pragma HLS STREAM variable=fifo_sum_PE_1_14 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_15;
  #pragma HLS STREAM variable=fifo_sum_PE_1_15 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1_16;
  #pragma HLS STREAM variable=fifo_sum_PE_1_16 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_0;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_0;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_1;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_1;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_2;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_2;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_3;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_3 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_3;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_3 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_4;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_4 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_4;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_4 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_5;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_5 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_5;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_5 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_6;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_6 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_6;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_6 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_7;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_7 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_7;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_7 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_8;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_8 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_8;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_8 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_9;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_9 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_9;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_9 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_10;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_10 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_10;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_10 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_11;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_11 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_11;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_11 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_12;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_12 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_12;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_12 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_13;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_13 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_13;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_13 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_14;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_14 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_14;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_14 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0_15;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0_15 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1_15;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1_15 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1_15 core=FIFO_SRL
  /* sum_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_sum_IO_L2_out_0;
  #pragma HLS STREAM variable=fifo_sum_sum_IO_L2_out_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_sum_IO_L2_out_0 core=FIFO_SRL
  /* sum_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_sum_IO_L2_out_1;
  #pragma HLS STREAM variable=fifo_sum_sum_IO_L2_out_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_sum_IO_L2_out_1 core=FIFO_SRL
  /* sum_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_sum_IO_L2_out_2;
  #pragma HLS STREAM variable=fifo_sum_sum_IO_L2_out_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_sum_IO_L2_out_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_0_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_0_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_0_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_0_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_0_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_0_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_0_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_0_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_0_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_1_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_1_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_1_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_1_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_1_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_1_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_1_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_1_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_1_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_2_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_2_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_2_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_2_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_2_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_2_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_2_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_2_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_2_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_3_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_3_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_3_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_3_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_3_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_3_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_3_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_3_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_3_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_4_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_4_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_4_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_4_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_4_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_4_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_4_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_4_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_4_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_5_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_5_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_5_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_5_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_5_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_5_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_5_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_5_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_5_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_6_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_6_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_6_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_6_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_6_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_6_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_6_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_6_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_6_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_7_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_7_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_7_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_7_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_7_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_7_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_7_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_7_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_7_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_8_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_8_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_8_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_8_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_8_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_8_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_8_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_8_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_8_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_9_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_9_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_9_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_9_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_9_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_9_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_9_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_9_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_9_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_10_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_10_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_10_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_10_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_10_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_10_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_10_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_10_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_10_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_11_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_11_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_11_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_11_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_11_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_11_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_11_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_11_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_11_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_12_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_12_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_12_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_12_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_12_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_12_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_12_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_12_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_12_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_13_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_13_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_13_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_13_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_13_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_13_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_13_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_13_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_13_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_14_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_14_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_14_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_14_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_14_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_14_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_14_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_14_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_14_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_15_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_15_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_15_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_15_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_15_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_15_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_15_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_15_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_15_2 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_0 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_1 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_2 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_3;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_3 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_3 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_4;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_4 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_4 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_5;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_5 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_5 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_6;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_6 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_6 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_7;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_7 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_7 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_8;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_8 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_8 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_9;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_9 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_9 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_10;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_10 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_10 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_11;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_11 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_11 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_12;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_12 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_12 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_13;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_13 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_13 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_14;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_14 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_14 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_15;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_15 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_15 core=FIFO_SRL
  /* sum_drain_IO_L2_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L2_out_16;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L2_out_16 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L2_out_16 core=FIFO_SRL
  /* FIFO Declaration */

  /* Module Call */
  output_IO_L2_in_boundary(
    /* module id */ 15,
    /* fifo */ fifo_output_output_IO_L2_in_15,
    /* fifo */ fifo_output_output_IO_L1_in_15_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 14,
    /* fifo */ fifo_output_output_IO_L2_in_14,
    /* fifo */ fifo_output_output_IO_L2_in_15,
    /* fifo */ fifo_output_output_IO_L1_in_14_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 13,
    /* fifo */ fifo_output_output_IO_L2_in_13,
    /* fifo */ fifo_output_output_IO_L2_in_14,
    /* fifo */ fifo_output_output_IO_L1_in_13_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 12,
    /* fifo */ fifo_output_output_IO_L2_in_12,
    /* fifo */ fifo_output_output_IO_L2_in_13,
    /* fifo */ fifo_output_output_IO_L1_in_12_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 11,
    /* fifo */ fifo_output_output_IO_L2_in_11,
    /* fifo */ fifo_output_output_IO_L2_in_12,
    /* fifo */ fifo_output_output_IO_L1_in_11_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 10,
    /* fifo */ fifo_output_output_IO_L2_in_10,
    /* fifo */ fifo_output_output_IO_L2_in_11,
    /* fifo */ fifo_output_output_IO_L1_in_10_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 9,
    /* fifo */ fifo_output_output_IO_L2_in_9,
    /* fifo */ fifo_output_output_IO_L2_in_10,
    /* fifo */ fifo_output_output_IO_L1_in_9_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 8,
    /* fifo */ fifo_output_output_IO_L2_in_8,
    /* fifo */ fifo_output_output_IO_L2_in_9,
    /* fifo */ fifo_output_output_IO_L1_in_8_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 7,
    /* fifo */ fifo_output_output_IO_L2_in_7,
    /* fifo */ fifo_output_output_IO_L2_in_8,
    /* fifo */ fifo_output_output_IO_L1_in_7_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 6,
    /* fifo */ fifo_output_output_IO_L2_in_6,
    /* fifo */ fifo_output_output_IO_L2_in_7,
    /* fifo */ fifo_output_output_IO_L1_in_6_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 5,
    /* fifo */ fifo_output_output_IO_L2_in_5,
    /* fifo */ fifo_output_output_IO_L2_in_6,
    /* fifo */ fifo_output_output_IO_L1_in_5_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 4,
    /* fifo */ fifo_output_output_IO_L2_in_4,
    /* fifo */ fifo_output_output_IO_L2_in_5,
    /* fifo */ fifo_output_output_IO_L1_in_4_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 3,
    /* fifo */ fifo_output_output_IO_L2_in_3,
    /* fifo */ fifo_output_output_IO_L2_in_4,
    /* fifo */ fifo_output_output_IO_L1_in_3_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 2,
    /* fifo */ fifo_output_output_IO_L2_in_2,
    /* fifo */ fifo_output_output_IO_L2_in_3,
    /* fifo */ fifo_output_output_IO_L1_in_2_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L2_in_1,
    /* fifo */ fifo_output_output_IO_L2_in_2,
    /* fifo */ fifo_output_output_IO_L1_in_1_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L2_in(
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L2_in_0,
    /* fifo */ fifo_output_output_IO_L2_in_1,
    /* fifo */ fifo_output_output_IO_L1_in_0_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L3_in(
    /* array */ output,
    /* fifo */ fifo_output_output_IO_L2_in_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_0_1,
    /* fifo */ fifo_output_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_0_0,
    /* fifo */ fifo_output_output_IO_L1_in_0_1,
    /* fifo */ fifo_output_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_1_1,
    /* fifo */ fifo_output_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_1_0,
    /* fifo */ fifo_output_output_IO_L1_in_1_1,
    /* fifo */ fifo_output_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 2,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_2_1,
    /* fifo */ fifo_output_PE_1_2
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 2,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_2_0,
    /* fifo */ fifo_output_output_IO_L1_in_2_1,
    /* fifo */ fifo_output_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 3,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_3_1,
    /* fifo */ fifo_output_PE_1_3
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 3,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_3_0,
    /* fifo */ fifo_output_output_IO_L1_in_3_1,
    /* fifo */ fifo_output_PE_0_3
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 4,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_4_1,
    /* fifo */ fifo_output_PE_1_4
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 4,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_4_0,
    /* fifo */ fifo_output_output_IO_L1_in_4_1,
    /* fifo */ fifo_output_PE_0_4
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 5,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_5_1,
    /* fifo */ fifo_output_PE_1_5
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 5,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_5_0,
    /* fifo */ fifo_output_output_IO_L1_in_5_1,
    /* fifo */ fifo_output_PE_0_5
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 6,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_6_1,
    /* fifo */ fifo_output_PE_1_6
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 6,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_6_0,
    /* fifo */ fifo_output_output_IO_L1_in_6_1,
    /* fifo */ fifo_output_PE_0_6
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 7,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_7_1,
    /* fifo */ fifo_output_PE_1_7
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 7,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_7_0,
    /* fifo */ fifo_output_output_IO_L1_in_7_1,
    /* fifo */ fifo_output_PE_0_7
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 8,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_8_1,
    /* fifo */ fifo_output_PE_1_8
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 8,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_8_0,
    /* fifo */ fifo_output_output_IO_L1_in_8_1,
    /* fifo */ fifo_output_PE_0_8
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 9,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_9_1,
    /* fifo */ fifo_output_PE_1_9
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 9,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_9_0,
    /* fifo */ fifo_output_output_IO_L1_in_9_1,
    /* fifo */ fifo_output_PE_0_9
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 10,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_10_1,
    /* fifo */ fifo_output_PE_1_10
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 10,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_10_0,
    /* fifo */ fifo_output_output_IO_L1_in_10_1,
    /* fifo */ fifo_output_PE_0_10
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 11,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_11_1,
    /* fifo */ fifo_output_PE_1_11
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 11,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_11_0,
    /* fifo */ fifo_output_output_IO_L1_in_11_1,
    /* fifo */ fifo_output_PE_0_11
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 12,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_12_1,
    /* fifo */ fifo_output_PE_1_12
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 12,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_12_0,
    /* fifo */ fifo_output_output_IO_L1_in_12_1,
    /* fifo */ fifo_output_PE_0_12
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 13,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_13_1,
    /* fifo */ fifo_output_PE_1_13
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 13,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_13_0,
    /* fifo */ fifo_output_output_IO_L1_in_13_1,
    /* fifo */ fifo_output_PE_0_13
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 14,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_14_1,
    /* fifo */ fifo_output_PE_1_14
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 14,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_14_0,
    /* fifo */ fifo_output_output_IO_L1_in_14_1,
    /* fifo */ fifo_output_PE_0_14
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 15,
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_15_1,
    /* fifo */ fifo_output_PE_1_15
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 15,
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_15_0,
    /* fifo */ fifo_output_output_IO_L1_in_15_1,
    /* fifo */ fifo_output_PE_0_15
  );
  /* Module Call */

  /* Module Call */
  sum_IO_L3_in(
    /* array */ sum_0,
    /* fifo */ fifo_sum_sum_IO_L2_in_0
  );
  /* Module Call */

  /* Module Call */
  sum_IO_L2_in(
    /* module id */ 0,
    /* fifo */ fifo_sum_sum_IO_L2_in_0,
    /* fifo */ fifo_sum_sum_IO_L2_in_1,
    /* fifo */ fifo_sum_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  sum_IO_L2_in_boundary(
    /* module id */ 1,
    /* fifo */ fifo_sum_sum_IO_L2_in_1,
    /* fifo */ fifo_sum_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  sum_PE_dummy_out(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_sum_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  sum_PE_dummy_out(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_sum_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 0,
    /* fifo */ fifo_output_PE_0_0,
    /* fifo */ fifo_sum_PE_0_0,
    /* fifo */ fifo_sum_PE_0_1,
    /* fifo */ fifo_sum_drain_PE_0_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 1,
    /* fifo */ fifo_output_PE_0_1,
    /* fifo */ fifo_sum_PE_0_1,
    /* fifo */ fifo_sum_PE_0_2,
    /* fifo */ fifo_sum_drain_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 2,
    /* fifo */ fifo_output_PE_0_2,
    /* fifo */ fifo_sum_PE_0_2,
    /* fifo */ fifo_sum_PE_0_3,
    /* fifo */ fifo_sum_drain_PE_0_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 3,
    /* fifo */ fifo_output_PE_0_3,
    /* fifo */ fifo_sum_PE_0_3,
    /* fifo */ fifo_sum_PE_0_4,
    /* fifo */ fifo_sum_drain_PE_0_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 4,
    /* fifo */ fifo_output_PE_0_4,
    /* fifo */ fifo_sum_PE_0_4,
    /* fifo */ fifo_sum_PE_0_5,
    /* fifo */ fifo_sum_drain_PE_0_4
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 5,
    /* fifo */ fifo_output_PE_0_5,
    /* fifo */ fifo_sum_PE_0_5,
    /* fifo */ fifo_sum_PE_0_6,
    /* fifo */ fifo_sum_drain_PE_0_5
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 6,
    /* fifo */ fifo_output_PE_0_6,
    /* fifo */ fifo_sum_PE_0_6,
    /* fifo */ fifo_sum_PE_0_7,
    /* fifo */ fifo_sum_drain_PE_0_6
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 7,
    /* fifo */ fifo_output_PE_0_7,
    /* fifo */ fifo_sum_PE_0_7,
    /* fifo */ fifo_sum_PE_0_8,
    /* fifo */ fifo_sum_drain_PE_0_7
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 8,
    /* fifo */ fifo_output_PE_0_8,
    /* fifo */ fifo_sum_PE_0_8,
    /* fifo */ fifo_sum_PE_0_9,
    /* fifo */ fifo_sum_drain_PE_0_8
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 9,
    /* fifo */ fifo_output_PE_0_9,
    /* fifo */ fifo_sum_PE_0_9,
    /* fifo */ fifo_sum_PE_0_10,
    /* fifo */ fifo_sum_drain_PE_0_9
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 10,
    /* fifo */ fifo_output_PE_0_10,
    /* fifo */ fifo_sum_PE_0_10,
    /* fifo */ fifo_sum_PE_0_11,
    /* fifo */ fifo_sum_drain_PE_0_10
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 11,
    /* fifo */ fifo_output_PE_0_11,
    /* fifo */ fifo_sum_PE_0_11,
    /* fifo */ fifo_sum_PE_0_12,
    /* fifo */ fifo_sum_drain_PE_0_11
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 12,
    /* fifo */ fifo_output_PE_0_12,
    /* fifo */ fifo_sum_PE_0_12,
    /* fifo */ fifo_sum_PE_0_13,
    /* fifo */ fifo_sum_drain_PE_0_12
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 13,
    /* fifo */ fifo_output_PE_0_13,
    /* fifo */ fifo_sum_PE_0_13,
    /* fifo */ fifo_sum_PE_0_14,
    /* fifo */ fifo_sum_drain_PE_0_13
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 14,
    /* fifo */ fifo_output_PE_0_14,
    /* fifo */ fifo_sum_PE_0_14,
    /* fifo */ fifo_sum_PE_0_15,
    /* fifo */ fifo_sum_drain_PE_0_14
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* module id */ 15,
    /* fifo */ fifo_output_PE_0_15,
    /* fifo */ fifo_sum_PE_0_15,
    /* fifo */ fifo_sum_PE_0_16,
    /* fifo */ fifo_sum_drain_PE_0_15
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 0,
    /* fifo */ fifo_output_PE_1_0,
    /* fifo */ fifo_sum_PE_1_0,
    /* fifo */ fifo_sum_PE_1_1,
    /* fifo */ fifo_sum_drain_PE_1_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 1,
    /* fifo */ fifo_output_PE_1_1,
    /* fifo */ fifo_sum_PE_1_1,
    /* fifo */ fifo_sum_PE_1_2,
    /* fifo */ fifo_sum_drain_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 2,
    /* fifo */ fifo_output_PE_1_2,
    /* fifo */ fifo_sum_PE_1_2,
    /* fifo */ fifo_sum_PE_1_3,
    /* fifo */ fifo_sum_drain_PE_1_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 3,
    /* fifo */ fifo_output_PE_1_3,
    /* fifo */ fifo_sum_PE_1_3,
    /* fifo */ fifo_sum_PE_1_4,
    /* fifo */ fifo_sum_drain_PE_1_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 4,
    /* fifo */ fifo_output_PE_1_4,
    /* fifo */ fifo_sum_PE_1_4,
    /* fifo */ fifo_sum_PE_1_5,
    /* fifo */ fifo_sum_drain_PE_1_4
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 5,
    /* fifo */ fifo_output_PE_1_5,
    /* fifo */ fifo_sum_PE_1_5,
    /* fifo */ fifo_sum_PE_1_6,
    /* fifo */ fifo_sum_drain_PE_1_5
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 6,
    /* fifo */ fifo_output_PE_1_6,
    /* fifo */ fifo_sum_PE_1_6,
    /* fifo */ fifo_sum_PE_1_7,
    /* fifo */ fifo_sum_drain_PE_1_6
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 7,
    /* fifo */ fifo_output_PE_1_7,
    /* fifo */ fifo_sum_PE_1_7,
    /* fifo */ fifo_sum_PE_1_8,
    /* fifo */ fifo_sum_drain_PE_1_7
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 8,
    /* fifo */ fifo_output_PE_1_8,
    /* fifo */ fifo_sum_PE_1_8,
    /* fifo */ fifo_sum_PE_1_9,
    /* fifo */ fifo_sum_drain_PE_1_8
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 9,
    /* fifo */ fifo_output_PE_1_9,
    /* fifo */ fifo_sum_PE_1_9,
    /* fifo */ fifo_sum_PE_1_10,
    /* fifo */ fifo_sum_drain_PE_1_9
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 10,
    /* fifo */ fifo_output_PE_1_10,
    /* fifo */ fifo_sum_PE_1_10,
    /* fifo */ fifo_sum_PE_1_11,
    /* fifo */ fifo_sum_drain_PE_1_10
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 11,
    /* fifo */ fifo_output_PE_1_11,
    /* fifo */ fifo_sum_PE_1_11,
    /* fifo */ fifo_sum_PE_1_12,
    /* fifo */ fifo_sum_drain_PE_1_11
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 12,
    /* fifo */ fifo_output_PE_1_12,
    /* fifo */ fifo_sum_PE_1_12,
    /* fifo */ fifo_sum_PE_1_13,
    /* fifo */ fifo_sum_drain_PE_1_12
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 13,
    /* fifo */ fifo_output_PE_1_13,
    /* fifo */ fifo_sum_PE_1_13,
    /* fifo */ fifo_sum_PE_1_14,
    /* fifo */ fifo_sum_drain_PE_1_13
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 14,
    /* fifo */ fifo_output_PE_1_14,
    /* fifo */ fifo_sum_PE_1_14,
    /* fifo */ fifo_sum_PE_1_15,
    /* fifo */ fifo_sum_drain_PE_1_14
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* module id */ 15,
    /* fifo */ fifo_output_PE_1_15,
    /* fifo */ fifo_sum_PE_1_15,
    /* fifo */ fifo_sum_PE_1_16,
    /* fifo */ fifo_sum_drain_PE_1_15
  );
  /* Module Call */

  /* Module Call */
  sum_IO_L2_out_boundary(
    /* module id */ 1,
    /* fifo */ fifo_sum_sum_IO_L2_out_1,
    /* fifo */ fifo_sum_PE_1_1
  );
  /* Module Call */

  /* Module Call */
  sum_IO_L2_out(
    /* module id */ 0,
    /* fifo */ fifo_sum_sum_IO_L2_out_1,
    /* fifo */ fifo_sum_sum_IO_L2_out_0,
    /* fifo */ fifo_sum_PE_0_1
  );
  /* Module Call */

  /* Module Call */
  sum_IO_L3_out(
    /* array */ sum_1,
    /* fifo */ fifo_sum_sum_IO_L2_out_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 15,
    /* module id */ 1,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_15_1,
    /* fifo */ fifo_sum_drain_PE_1_15
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 15,
    /* module id */ 0,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_15_1,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_15_0,
    /* fifo */ fifo_sum_drain_PE_0_15
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out_boundary(
    /* module id */ 15,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_15,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_15_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 14,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_15,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_14,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_14_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 13,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_14,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_13,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_13_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 12,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_13,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_12,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_12_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 11,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_12,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_11,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_11_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 10,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_11,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_10,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_10_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 9,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_10,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_9,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_9_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 8,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_9,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_8,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_8_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 7,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_8,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_7,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_7_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 6,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_7,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_6,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_6_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 5,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_6,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_5,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_5_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 4,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_5,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_4,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_4_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 3,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_4,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_3,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_3_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 2,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_3,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_2,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_2_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 1,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_2,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_1,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_1_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* module id */ 0,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_1,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_0,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_0_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L3_out(
    /* array */ sum_2,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L2_out_0
  );
  /* Module Call */

}
