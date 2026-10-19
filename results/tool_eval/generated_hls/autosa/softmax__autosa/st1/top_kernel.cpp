#include "top_kernel.h"
typedef float data_t;
/* Module Definition */
void output_IO_L2_in(output_t1 *output, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
    // array
    // io_L2
    for (ap_uint<5> c2 = 0; c2 <= 15; c2 += 1) {
      // io_L1
      for (ap_uint<4> c3 = 0; c3 <= 7; c3 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          output_t1 in_data;
          output_t1 out_data;
          in_data = output[16*c1 + c2 + 128*c3];
          out_data = in_data;
          fifo_output_local_out.write(out_data);
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_intra_trans(int idx, int c1, float local_output[8][1], hls::stream<float> &fifo_output_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L1
  // pe
  for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
    // latency
    for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    #pragma HLS PIPELINE II=1
      {
        output_t1 in_data;
        output_t1 out_data;
        in_data = local_output[4*c3 + c4][0];
        out_data = in_data;
        fifo_output_local_out.write(out_data);
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_inter_trans(int idx, int c1, float local_output[8][1], hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c2 = p0; c2 <= 15; c2 += 1) {
    // io_L1
    if (c2 == p0) {
      for (ap_uint<4> c3 = 0; c3 <= 7; c3 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          output_t1 in_data;
          output_t1 out_data;
          in_data = fifo_output_in.read();
          out_data = in_data;
          local_output[c3][0] = out_data;
        }
      }
    } else {
      for (ap_uint<4> c3 = 0; c3 <= 7; c3 += 1) {
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
void output_IO_L1_in_inter_trans_boundary(int idx, int c1, float local_output[8][1], hls::stream<float> &fifo_output_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<5> c2 = p0; c2 <= 15; c2 += 1)
    if (c2 == p0) {
      // io_L1
      for (ap_uint<4> c3 = 0; c3 <= 7; c3 += 1) {
      #pragma HLS PIPELINE II=1
        // access_coalesce
        {
          output_t1 in_data;
          output_t1 out_data;
          in_data = fifo_output_in.read();
          out_data = in_data;
          local_output[c3][0] = out_data;
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t1 local_output_ping[8][1];
  #pragma HLS ARRAY_PARTITION variable=local_output_ping dim=0 complete
  float local_output_pong[8][1];
  #pragma HLS ARRAY_PARTITION variable=local_output_pong dim=0 complete
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c1, c1_prev;
  /* Variable Declaration */

  {
    for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
      // array
      // io_L2
      {
        if (arb == 0) {
          output_IO_L1_in_inter_trans(
            /* module id */ idx, 
            /* host iter */ c1, 
            /* array */ local_output_pong, 
            /* fifo */ fifo_output_in, 
            /* fifo */ fifo_output_out, 
            /* enable */ inter_trans_en
          );
          output_IO_L1_in_intra_trans(
            /* module id */ idx, 
            /* host iter */ c1_prev, 
            /* array */ local_output_ping, 
            /* fifo */ fifo_output_local_out, 
            /* enable */ intra_trans_en
          );
        } else {
          output_IO_L1_in_inter_trans(
            /* module id */ idx, 
            /* host iter */ c1, 
            /* array */ local_output_ping, 
            /* fifo */ fifo_output_in, 
            /* fifo */ fifo_output_out, 
            /* enable */ inter_trans_en
          );
          output_IO_L1_in_intra_trans(
            /* module id */ idx, 
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
        /* host iter */ c1_prev, 
        /* array */ local_output_ping, 
        /* fifo */ fifo_output_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      output_IO_L1_in_intra_trans(
        /* module id */ idx, 
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
void output_IO_L1_in_wrapper(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_out, hls::stream<float> &fifo_output_local_out)
 {
  output_IO_L1_in(
    /* module id */ idx, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_output_out, 
    /* fifo */ fifo_output_local_out);
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_boundary(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t1 local_output_ping[8][1];
  #pragma HLS ARRAY_PARTITION variable=local_output_ping dim=0 complete
  float local_output_pong[8][1];
  #pragma HLS ARRAY_PARTITION variable=local_output_pong dim=0 complete
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c1, c1_prev;
  /* Variable Declaration */

  {
    for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
      // array
      // io_L2
      {
        if (arb == 0) {
          output_IO_L1_in_inter_trans_boundary(
            /* module id */ idx, 
            /* host iter */ c1, 
            /* array */ local_output_pong, 
            /* fifo */ fifo_output_in, 
            /* enable */ inter_trans_en
          );
          output_IO_L1_in_intra_trans(
            /* module id */ idx, 
            /* host iter */ c1_prev, 
            /* array */ local_output_ping, 
            /* fifo */ fifo_output_local_out, 
            /* enable */ intra_trans_en
          );
        } else {
          output_IO_L1_in_inter_trans_boundary(
            /* module id */ idx, 
            /* host iter */ c1, 
            /* array */ local_output_ping, 
            /* fifo */ fifo_output_in, 
            /* enable */ inter_trans_en
          );
          output_IO_L1_in_intra_trans(
            /* module id */ idx, 
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
        /* host iter */ c1_prev, 
        /* array */ local_output_ping, 
        /* fifo */ fifo_output_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      output_IO_L1_in_intra_trans(
        /* module id */ idx, 
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
void output_IO_L1_in_boundary_wrapper(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_output_local_out)
 {
  output_IO_L1_in_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_output_local_out);
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_in_intra_trans(int c1, sum_t8 *sum, sum_t8 local_sum[1], hls::stream<float> &fifo_sum_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  ap_uint<32> data_split[8];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L2
  // io_L1
  // pe
  for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
    // latency
    for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    #pragma HLS PIPELINE II=1
      {
        sum_t8 in_data;
        sum_t1 out_data;
        in_data = local_sum[(4 * c3 + c4) / 8];
        for (ap_uint<4> n = 0; n < 8; n++) {
        #pragma HLS UNROLL
          data_split[n] = in_data(31, 0);
          in_data = in_data >> 32;
        }
        int split_idx = (4*c3 + c4) % 8;
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
void sum_IO_L2_in_boundary(sum_t8 *sum, hls::stream<float> &fifo_sum_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  sum_t8 local_sum_ping[1];
  #pragma HLS RESOURCE variable=local_sum_ping core=RAM_2P_BRAM
  sum_t8 local_sum_pong[1];
  #pragma HLS RESOURCE variable=local_sum_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 1;
  bool intra_trans_en = 0;
  int c1, c1_prev;
  /* Variable Declaration */

  {
    for (ap_uint<4> c1 = 1; c1 <= 7; c1 += 1) {
      // array
      {
        if (arb == 0) {
          sum_IO_L2_in_inter_trans_boundary(
            /* array */ sum, 
            /* array */ local_sum_pong, 
            /* fifo */ fifo_sum_in, 
            /* enable */ inter_trans_en
          );
          sum_IO_L2_in_intra_trans(
            /* host iter */ c1_prev, 
            /* array */ sum, 
            /* array */ local_sum_ping, 
            /* fifo */ fifo_sum_local_out, 
            /* enable */ intra_trans_en
          );
        } else {
          sum_IO_L2_in_inter_trans_boundary(
            /* array */ sum, 
            /* array */ local_sum_ping, 
            /* fifo */ fifo_sum_in, 
            /* enable */ inter_trans_en
          );
          sum_IO_L2_in_intra_trans(
            /* host iter */ c1_prev, 
            /* array */ sum, 
            /* array */ local_sum_pong, 
            /* fifo */ fifo_sum_local_out, 
            /* enable */ intra_trans_en
          );
        }
        intra_trans_en = 1;
        arb = !arb;
        c1_prev = c1;
      }
    }
    if (arb == 0) {
      sum_IO_L2_in_intra_trans(
        /* host iter */ c1_prev, 
        /* array */ sum, 
        /* array */ local_sum_ping, 
        /* fifo */ fifo_sum_local_out, 
        /* enable */ intra_trans_en
      );
    } else {
      sum_IO_L2_in_intra_trans(
        /* host iter */ c1_prev, 
        /* array */ sum, 
        /* array */ local_sum_pong, 
        /* fifo */ fifo_sum_local_out, 
        /* enable */ intra_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void PE(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_sum_in, hls::stream<float> &fifo_sum_out, hls::stream<float> &fifo_sum_drain_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t1 local_output[1][1];
  #pragma HLS ARRAY_PARTITION variable=local_output dim=0 complete
  sum_t1 local_sum[8];
  #pragma HLS ARRAY_PARTITION variable=local_sum dim=0 complete
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
    // array
    // pe
    for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
      // latency
      for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
      #pragma HLS PIPELINE II=1
        {
          local_output[0][0] = fifo_output_in.read();
          if (p0 + 16 * c1 >= 1)
            local_sum[4*c3 + c4] = fifo_sum_in.read();
          local_sum[4*c3 + c4] = (local_sum[4*c3 + c4] + local_output[0][0]);
          if (p0 == 15 && c1 == 7) {
            fifo_sum_drain_out.write(local_sum[4*c3 + c4]);
          } else {
            fifo_sum_out.write(local_sum[4*c3 + c4]);
          }
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_sum_in, hls::stream<float> &fifo_sum_out, hls::stream<float> &fifo_sum_drain_out)
 {
  PE(
    /* module id */ idx, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_sum_in, 
    /* fifo */ fifo_sum_out, 
    /* fifo */ fifo_sum_drain_out);
}
/* Module Definition */

/* Module Definition */
void sum_PE_dummy_out(int idx, hls::stream<float> &fifo_sum_out) {
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1)
    if (p0 + 16 * c1 >= 1) {
      // array
      // pe
      for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
        // latency
        for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
        #pragma HLS PIPELINE II=1
          sum_t1 fifo_data = 0;
          fifo_sum_out.write(fifo_data);
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_out_intra_trans(int c1, sum_t8 *sum, sum_t8 local_sum[1], hls::stream<float> &fifo_sum_local_in, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  ap_uint<32> data_split[8];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L2
  // io_L1
  // pe
  for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
    // latency
    for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    #pragma HLS PIPELINE II=1
      {
        sum_t1 in_data;
        sum_t8 out_data;
        in_data = fifo_sum_local_in.read();
        int split_idx = (4*c3 + c4) % 8;
        out_data = local_sum[(4 * c3 + c4) / 8];
        for (ap_uint<4> n = 0; n < 8; n++) {
        #pragma HLS UNROLL
          data_split[n] = out_data(31, 0);
          out_data = out_data >> 32;
        }
        union {unsigned int ui; float ut;} u;
        u.ut = in_data;
        data_split[split_idx] = ap_uint<32>(u.ui);
        out_data = (data_split[7], data_split[6], data_split[5], data_split[4], data_split[3], data_split[2], data_split[1], data_split[0]);        local_sum[(4 * c3 + c4) / 8] = out_data;
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_out_inter_trans_boundary(sum_t8 *sum, sum_t8 local_sum[1], hls::stream<sum_t8> &fifo_sum_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  if (!inter_trans_en) return;

  // io_L2
  // access_coalesce
  // hls_pipeline
  {
    sum_t8 in_data;
    sum_t8 out_data;
    in_data = local_sum[0];
    out_data = in_data;
    sum[0] = out_data;
  }
}
/* Module Definition */

/* Module Definition */
void sum_IO_L2_out_boundary(sum_t8 *sum, hls::stream<float> &fifo_sum_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  sum_t8 local_sum_ping[1];
  #pragma HLS RESOURCE variable=local_sum_ping core=RAM_2P_BRAM
  sum_t8 local_sum_pong[1];
  #pragma HLS RESOURCE variable=local_sum_pong core=RAM_2P_BRAM
  bool arb = 0;
  bool inter_trans_en = 0;
  bool intra_trans_en = 1;
  /* Variable Declaration */

  {
    for (ap_uint<4> c1 = 0; c1 <= 6; c1 += 1) {
      // array
      {
        if (arb == 0) {
          sum_IO_L2_out_intra_trans(
            /* host iter */ c1, 
            /* array */ sum, 
            /* array */ local_sum_ping, 
            /* fifo */ fifo_sum_local_in, 
            /* enable */ intra_trans_en
          );
          sum_IO_L2_out_inter_trans_boundary(
            /* array */ sum, 
            /* array */ local_sum_pong, 
            /* fifo */ fifo_sum_out, 
            /* enable */ inter_trans_en
          );
        } else {
          sum_IO_L2_out_intra_trans(
            /* host iter */ c1, 
            /* array */ sum, 
            /* array */ local_sum_pong, 
            /* fifo */ fifo_sum_local_in, 
            /* enable */ intra_trans_en
          );
          sum_IO_L2_out_inter_trans_boundary(
            /* array */ sum, 
            /* array */ local_sum_ping, 
            /* fifo */ fifo_sum_out, 
            /* enable */ inter_trans_en
          );
        }
        inter_trans_en = 1;
        arb = !arb;
      }
    }
    if (arb == 0) {
      sum_IO_L2_out_inter_trans_boundary(
        /* array */ sum, 
        /* array */ local_sum_pong, 
        /* fifo */ fifo_sum_out, 
        /* enable */ inter_trans_en
      );
    } else {
      sum_IO_L2_out_inter_trans_boundary(
        /* array */ sum, 
        /* array */ local_sum_ping, 
        /* fifo */ fifo_sum_out, 
        /* enable */ inter_trans_en
      );
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_intra_trans(int idx, sum_t4 local_sum[2], hls::stream<float> &fifo_sum_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */


  // io_L1
  // pe
  for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
    // latency
    for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    #pragma HLS PIPELINE II=1
      {
        sum_t1 in_data;
        sum_t4 out_data;
        in_data = fifo_sum_drain_local_in.read();
        int split_idx = (4*c3 + c4) % 4;
        out_data = local_sum[(4 * c3 + c4) / 4];
        for (ap_uint<3> n = 0; n < 4; n++) {
        #pragma HLS UNROLL
          data_split[n] = out_data(31, 0);
          out_data = out_data >> 32;
        }
        union {unsigned int ui; float ut;} u;
        u.ut = in_data;
        data_split[split_idx] = ap_uint<32>(u.ui);
        out_data = (data_split[3], data_split[2], data_split[1], data_split[0]);        local_sum[(4 * c3 + c4) / 4] = out_data;
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_inter_trans(int idx, sum_t4 local_sum[2], hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // io_L1
  if (p0 == 15) {
    // access_coalesce
    for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
    #pragma HLS PIPELINE II=1
      {
        sum_t4 in_data;
        sum_t4 out_data;
        in_data = local_sum[c3];
        out_data = in_data;
        fifo_sum_drain_out.write(out_data);
      }
    }
  } else {
    // access_coalesce
    for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
    #pragma HLS PIPELINE II=1
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
void sum_drain_IO_L1_out_inter_trans_boundary(int idx, sum_t4 local_sum[2], hls::stream<sum_t4> &fifo_sum_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  // io_L1
  // access_coalesce
  for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
  #pragma HLS PIPELINE II=1
    {
      sum_t4 in_data;
      sum_t4 out_data;
      in_data = local_sum[c3];
      out_data = in_data;
      fifo_sum_drain_out.write(out_data);
    }
  }
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out(int idx, hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  sum_t4 local_sum[2];
  #pragma HLS RESOURCE variable=local_sum core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // io_L2
  sum_drain_IO_L1_out_intra_trans(
    /* module id */ idx, 
    /* array */ local_sum, 
    /* fifo */ fifo_sum_drain_local_in
  );
  sum_drain_IO_L1_out_inter_trans(
    /* module id */ idx, 
    /* array */ local_sum, 
    /* fifo */ fifo_sum_drain_in, 
    /* fifo */ fifo_sum_drain_out
  );
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_wrapper(int idx, hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in)
 {
  sum_drain_IO_L1_out(
    /* module id */ idx, 
    /* fifo */ fifo_sum_drain_in, 
    /* fifo */ fifo_sum_drain_out, 
    /* fifo */ fifo_sum_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_boundary(int idx, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  sum_t4 local_sum[2];
  #pragma HLS RESOURCE variable=local_sum core=RAM_2P_BRAM
  /* Variable Declaration */

  // array
  // io_L2
  sum_drain_IO_L1_out_intra_trans(
    /* module id */ idx, 
    /* array */ local_sum, 
    /* fifo */ fifo_sum_drain_local_in
  );
  sum_drain_IO_L1_out_inter_trans_boundary(
    /* module id */ idx, 
    /* array */ local_sum, 
    /* fifo */ fifo_sum_drain_out
  );
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_boundary_wrapper(int idx, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in)
 {
  sum_drain_IO_L1_out_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_sum_drain_out, 
    /* fifo */ fifo_sum_drain_local_in);
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L2_out(sum_t8 *sum, hls::stream<sum_t4> &fifo_sum_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  sum_t4 data_split[2];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  // array
  // io_L2
  // io_L1
  // access_coalesce
  for (ap_uint<2> c3 = 0; c3 <= 1; c3 += 1) {
  #pragma HLS PIPELINE II=1
    {
      sum_t4 in_data;
      sum_t8 out_data;
      in_data = fifo_sum_drain_local_in.read();
      int split_idx = (c3) % 2;
      data_split[split_idx] = in_data;
      out_data = (data_split[1], data_split[0]);      if (c3 % 2 == 2 - 1 || c3 == 1) {
        sum[4 * c3 / 8] = out_data;
      }
    }
  }
}
/* Module Definition */

void kernel0(output_t1 *output, sum_t8 *sum_0, sum_t8 *sum_1, sum_t8 *sum_2)
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
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_2 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_3;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_3 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_3 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_4;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_4 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_4 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_5;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_5 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_5 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_6;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_6 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_6 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_7;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_7 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_7 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_8;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_8 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_8 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_9;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_9 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_9 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_10;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_10 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_10 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_11;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_11 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_11 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_12;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_12 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_12 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_13;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_13 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_13 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_14;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_14 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_14 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_15;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_15 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_15 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<float> fifo_output_output_IO_L1_in_16;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_16 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_16 core=FIFO_SRL
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
  /* PE fifo */ hls::stream<float> fifo_sum_PE_0;
  #pragma HLS STREAM variable=fifo_sum_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_1;
  #pragma HLS STREAM variable=fifo_sum_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_2;
  #pragma HLS STREAM variable=fifo_sum_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_3;
  #pragma HLS STREAM variable=fifo_sum_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_4;
  #pragma HLS STREAM variable=fifo_sum_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_5;
  #pragma HLS STREAM variable=fifo_sum_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_6;
  #pragma HLS STREAM variable=fifo_sum_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_7;
  #pragma HLS STREAM variable=fifo_sum_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_8;
  #pragma HLS STREAM variable=fifo_sum_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_9;
  #pragma HLS STREAM variable=fifo_sum_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_10;
  #pragma HLS STREAM variable=fifo_sum_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_11;
  #pragma HLS STREAM variable=fifo_sum_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_12;
  #pragma HLS STREAM variable=fifo_sum_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_13;
  #pragma HLS STREAM variable=fifo_sum_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_14;
  #pragma HLS STREAM variable=fifo_sum_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_15;
  #pragma HLS STREAM variable=fifo_sum_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_15 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_PE_16;
  #pragma HLS STREAM variable=fifo_sum_PE_16 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_PE_16 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_2;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_3;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_3 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_3 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_4;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_4 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_4 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_5;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_5 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_5 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_6;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_6 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_6 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_7;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_7 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_7 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_8;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_8 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_8 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_9;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_9 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_9 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_10;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_10 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_10 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_11;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_11 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_11 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_12;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_12 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_12 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_13;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_13 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_13 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_14;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_14 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_14 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_15;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_15 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_15 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_2 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_3;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_3 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_3 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_4;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_4 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_4 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_5;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_5 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_5 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_6;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_6 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_6 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_7;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_7 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_7 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_8;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_8 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_8 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_9;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_9 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_9 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_10;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_10 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_10 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_11;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_11 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_11 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_12;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_12 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_12 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_13;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_13 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_13 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_14;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_14 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_14 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_15;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_15 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_15 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_16;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_16 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_16 core=FIFO_SRL
  /* FIFO Declaration */

  /* Module Call */
  output_IO_L2_in(
    /* array */ output,
    /* fifo */ fifo_output_output_IO_L1_in_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_output_output_IO_L1_in_15,
    /* fifo */ fifo_output_PE_15
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_output_output_IO_L1_in_14,
    /* fifo */ fifo_output_output_IO_L1_in_15,
    /* fifo */ fifo_output_PE_14
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_output_output_IO_L1_in_13,
    /* fifo */ fifo_output_output_IO_L1_in_14,
    /* fifo */ fifo_output_PE_13
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_output_output_IO_L1_in_12,
    /* fifo */ fifo_output_output_IO_L1_in_13,
    /* fifo */ fifo_output_PE_12
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_output_output_IO_L1_in_11,
    /* fifo */ fifo_output_output_IO_L1_in_12,
    /* fifo */ fifo_output_PE_11
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_output_output_IO_L1_in_10,
    /* fifo */ fifo_output_output_IO_L1_in_11,
    /* fifo */ fifo_output_PE_10
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_output_output_IO_L1_in_9,
    /* fifo */ fifo_output_output_IO_L1_in_10,
    /* fifo */ fifo_output_PE_9
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_output_output_IO_L1_in_8,
    /* fifo */ fifo_output_output_IO_L1_in_9,
    /* fifo */ fifo_output_PE_8
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_output_output_IO_L1_in_7,
    /* fifo */ fifo_output_output_IO_L1_in_8,
    /* fifo */ fifo_output_PE_7
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_output_output_IO_L1_in_6,
    /* fifo */ fifo_output_output_IO_L1_in_7,
    /* fifo */ fifo_output_PE_6
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_output_output_IO_L1_in_5,
    /* fifo */ fifo_output_output_IO_L1_in_6,
    /* fifo */ fifo_output_PE_5
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_output_output_IO_L1_in_4,
    /* fifo */ fifo_output_output_IO_L1_in_5,
    /* fifo */ fifo_output_PE_4
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_output_output_IO_L1_in_3,
    /* fifo */ fifo_output_output_IO_L1_in_4,
    /* fifo */ fifo_output_PE_3
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_output_output_IO_L1_in_2,
    /* fifo */ fifo_output_output_IO_L1_in_3,
    /* fifo */ fifo_output_PE_2
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_1,
    /* fifo */ fifo_output_output_IO_L1_in_2,
    /* fifo */ fifo_output_PE_1
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_output_output_IO_L1_in_0,
    /* fifo */ fifo_output_output_IO_L1_in_1,
    /* fifo */ fifo_output_PE_0
  );
  /* Module Call */

  /* Module Call */
  sum_IO_L2_in_boundary(
    /* array */ sum_0,
    /* fifo */ fifo_sum_PE_0
  );
  /* Module Call */

  /* Module Call */
  sum_PE_dummy_out(
    /* module id */ 0,
    /* fifo */ fifo_sum_PE_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_output_PE_0,
    /* fifo */ fifo_sum_PE_0,
    /* fifo */ fifo_sum_PE_1,
    /* fifo */ fifo_sum_drain_PE_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_output_PE_1,
    /* fifo */ fifo_sum_PE_1,
    /* fifo */ fifo_sum_PE_2,
    /* fifo */ fifo_sum_drain_PE_1
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_output_PE_2,
    /* fifo */ fifo_sum_PE_2,
    /* fifo */ fifo_sum_PE_3,
    /* fifo */ fifo_sum_drain_PE_2
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_output_PE_3,
    /* fifo */ fifo_sum_PE_3,
    /* fifo */ fifo_sum_PE_4,
    /* fifo */ fifo_sum_drain_PE_3
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_output_PE_4,
    /* fifo */ fifo_sum_PE_4,
    /* fifo */ fifo_sum_PE_5,
    /* fifo */ fifo_sum_drain_PE_4
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_output_PE_5,
    /* fifo */ fifo_sum_PE_5,
    /* fifo */ fifo_sum_PE_6,
    /* fifo */ fifo_sum_drain_PE_5
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_output_PE_6,
    /* fifo */ fifo_sum_PE_6,
    /* fifo */ fifo_sum_PE_7,
    /* fifo */ fifo_sum_drain_PE_6
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_output_PE_7,
    /* fifo */ fifo_sum_PE_7,
    /* fifo */ fifo_sum_PE_8,
    /* fifo */ fifo_sum_drain_PE_7
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_output_PE_8,
    /* fifo */ fifo_sum_PE_8,
    /* fifo */ fifo_sum_PE_9,
    /* fifo */ fifo_sum_drain_PE_8
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_output_PE_9,
    /* fifo */ fifo_sum_PE_9,
    /* fifo */ fifo_sum_PE_10,
    /* fifo */ fifo_sum_drain_PE_9
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_output_PE_10,
    /* fifo */ fifo_sum_PE_10,
    /* fifo */ fifo_sum_PE_11,
    /* fifo */ fifo_sum_drain_PE_10
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_output_PE_11,
    /* fifo */ fifo_sum_PE_11,
    /* fifo */ fifo_sum_PE_12,
    /* fifo */ fifo_sum_drain_PE_11
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_output_PE_12,
    /* fifo */ fifo_sum_PE_12,
    /* fifo */ fifo_sum_PE_13,
    /* fifo */ fifo_sum_drain_PE_12
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_output_PE_13,
    /* fifo */ fifo_sum_PE_13,
    /* fifo */ fifo_sum_PE_14,
    /* fifo */ fifo_sum_drain_PE_13
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_output_PE_14,
    /* fifo */ fifo_sum_PE_14,
    /* fifo */ fifo_sum_PE_15,
    /* fifo */ fifo_sum_drain_PE_14
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_output_PE_15,
    /* fifo */ fifo_sum_PE_15,
    /* fifo */ fifo_sum_PE_16,
    /* fifo */ fifo_sum_drain_PE_15
  );
  /* Module Call */

  /* Module Call */
  sum_IO_L2_out_boundary(
    /* array */ sum_1,
    /* fifo */ fifo_sum_PE_1
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 15,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_15,
    /* fifo */ fifo_sum_drain_PE_15
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 14,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_15,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_14,
    /* fifo */ fifo_sum_drain_PE_14
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 13,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_14,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_13,
    /* fifo */ fifo_sum_drain_PE_13
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 12,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_13,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_12,
    /* fifo */ fifo_sum_drain_PE_12
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 11,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_12,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_11,
    /* fifo */ fifo_sum_drain_PE_11
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 10,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_11,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_10,
    /* fifo */ fifo_sum_drain_PE_10
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 9,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_10,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_9,
    /* fifo */ fifo_sum_drain_PE_9
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 8,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_9,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_8,
    /* fifo */ fifo_sum_drain_PE_8
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 7,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_8,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_7,
    /* fifo */ fifo_sum_drain_PE_7
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 6,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_7,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_6,
    /* fifo */ fifo_sum_drain_PE_6
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 5,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_6,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_5,
    /* fifo */ fifo_sum_drain_PE_5
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 4,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_5,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_4,
    /* fifo */ fifo_sum_drain_PE_4
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 3,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_4,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_3,
    /* fifo */ fifo_sum_drain_PE_3
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 2,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_3,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_2,
    /* fifo */ fifo_sum_drain_PE_2
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_2,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_1,
    /* fifo */ fifo_sum_drain_PE_1
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_1,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_0,
    /* fifo */ fifo_sum_drain_PE_0
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L2_out(
    /* array */ sum_2,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_0
  );
  /* Module Call */

}
