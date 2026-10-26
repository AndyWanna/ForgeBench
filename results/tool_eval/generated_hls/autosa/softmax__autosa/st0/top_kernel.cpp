#include "top_kernel.h"
typedef float data_t;
/* Module Definition */
void output_IO_L2_in(output_t16 *output, hls::stream<output_t4> &fifo_output_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  output_t4 data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
    // array
    // io_L2
    for (ap_uint<2> c2 = 0; c2 <= 1; c2 += 1) {
      // io_L1
      for (ap_uint<3> c3 = 0; c3 <= 3; c3 += 1) {
        // access_coalesce
        for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
        #pragma HLS PIPELINE II=1
          {
            output_t16 in_data;
            output_t4 out_data;
            if (c4 % 4 == 0) {
              in_data = output[((4 * c2 + c3) * 128 + (16 * c1 + 4 * c4)) / 16];
              for (ap_uint<3> n = 0; n < 4; n++) {
              #pragma HLS UNROLL
                data_split[n] = in_data(127, 0);
                in_data = in_data >> 128;
              }
            }
            int split_idx = (4*c1 + 128*c2 + 32*c3 + c4) % 4;
            out_data = data_split[split_idx];
            fifo_output_local_out.write(out_data);
          }
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_intra_trans(int idx, int c1, output_t4 local_output[4][4], hls::stream<float> &fifo_output_local_out, bool intra_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  ap_uint<32> data_split[4];
  #pragma HLS ARRAY_PARTITION variable=data_split complete
  /* Variable Declaration */

  if (!intra_trans_en) return;


  // io_L1
  // pe
  for (ap_uint<5> c3 = 0; c3 <= 15; c3 += 1) {
    // latency
    for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
    #pragma HLS PIPELINE II=1
      {
        output_t4 in_data;
        output_t1 out_data;
        in_data = local_output[c4][c3 / 4];
        for (ap_uint<3> n = 0; n < 4; n++) {
        #pragma HLS UNROLL
          data_split[n] = in_data(31, 0);
          in_data = in_data >> 32;
        }
        int split_idx = (c3) % 4;
        union {unsigned int ui; float ut;} u;
        u.ui = (unsigned int)data_split[split_idx];
        out_data = u.ut;
        fifo_output_local_out.write(out_data);
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_inter_trans(int idx, int c1, output_t4 local_output[4][4], hls::stream<output_t4> &fifo_output_in, hls::stream<output_t4> &fifo_output_out, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c2 = p0; c2 <= 1; c2 += 1) {
    // io_L1
    if (c2 == p0) {
      for (ap_uint<3> c3 = 0; c3 <= 3; c3 += 1) {
        // access_coalesce
        for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
        #pragma HLS PIPELINE II=1
          {
            output_t4 in_data;
            output_t4 out_data;
            in_data = fifo_output_in.read();
            out_data = in_data;
            local_output[c3][c4] = out_data;
          }
        }
      }
    } else {
      for (ap_uint<3> c3 = 0; c3 <= 3; c3 += 1) {
        // access_coalesce
        for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
        #pragma HLS PIPELINE II=1
          {
            output_t4 in_data;
            output_t4 out_data;
            in_data = fifo_output_in.read();
            out_data = in_data;
            fifo_output_out.write(out_data);
          }
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_inter_trans_boundary(int idx, int c1, output_t4 local_output[4][4], hls::stream<output_t4> &fifo_output_in, bool inter_trans_en) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  if (!inter_trans_en) return;

  for (ap_uint<2> c2 = p0; c2 <= 1; c2 += 1)
    if (c2 == p0) {
      // io_L1
      for (ap_uint<3> c3 = 0; c3 <= 3; c3 += 1) {
        // access_coalesce
        for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
        #pragma HLS PIPELINE II=1
          {
            output_t4 in_data;
            output_t4 out_data;
            in_data = fifo_output_in.read();
            out_data = in_data;
            local_output[c3][c4] = out_data;
          }
        }
      }
    }
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in(int idx, hls::stream<output_t4> &fifo_output_in, hls::stream<output_t4> &fifo_output_out, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t4 local_output_ping[4][4];
  #pragma HLS RESOURCE variable=local_output_ping core=RAM_2P_BRAM
  output_t4 local_output_pong[4][4];
  #pragma HLS RESOURCE variable=local_output_pong core=RAM_2P_BRAM
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
void output_IO_L1_in_wrapper(int idx, hls::stream<output_t4> &fifo_output_in, hls::stream<output_t4> &fifo_output_out, hls::stream<float> &fifo_output_local_out)
 {
  output_IO_L1_in(
    /* module id */ idx, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_output_out, 
    /* fifo */ fifo_output_local_out);
}
/* Module Definition */

/* Module Definition */
void output_IO_L1_in_boundary(int idx, hls::stream<output_t4> &fifo_output_in, hls::stream<float> &fifo_output_local_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t4 local_output_ping[4][4];
  #pragma HLS RESOURCE variable=local_output_ping core=RAM_2P_BRAM
  output_t4 local_output_pong[4][4];
  #pragma HLS RESOURCE variable=local_output_pong core=RAM_2P_BRAM
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
void output_IO_L1_in_boundary_wrapper(int idx, hls::stream<output_t4> &fifo_output_in, hls::stream<float> &fifo_output_local_out)
 {
  output_IO_L1_in_boundary(
    /* module id */ idx, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_output_local_out);
}
/* Module Definition */

/* Module Definition */
void PE(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_sum_drain_out) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  output_t1 local_output[1][1];
  #pragma HLS ARRAY_PARTITION variable=local_output dim=0 complete
  sum_t1 local_sum[4];
  #pragma HLS ARRAY_PARTITION variable=local_sum dim=0 complete
  /* Variable Declaration */

  for (ap_uint<4> c1 = 0; c1 <= 7; c1 += 1) {
    // array
    // pe
    for (ap_uint<5> c3 = 0; c3 <= 15; c3 += 1) {
      // latency
      for (ap_uint<3> c4 = 0; c4 <= 3; c4 += 1) {
      #pragma HLS PIPELINE II=1
        {
          local_output[0][0] = fifo_output_in.read();
          local_sum[c4] = (local_sum[c4] + local_output[0][0]);
          if (c1 == 7 && c3 == 15)
            fifo_sum_drain_out.write(local_sum[c4]);
        }
      }
    }
  }
}
/* Module Definition */

/* Module Definition */
void PE_wrapper(int idx, hls::stream<float> &fifo_output_in, hls::stream<float> &fifo_sum_drain_out)
 {
  PE(
    /* module id */ idx, 
    /* fifo */ fifo_output_in, 
    /* fifo */ fifo_sum_drain_out);
}
/* Module Definition */

/* Module Definition */
void sum_drain_IO_L1_out_intra_trans(int idx, sum_t4 local_sum[1], hls::stream<float> &fifo_sum_drain_local_in) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
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
void sum_drain_IO_L1_out_inter_trans(int idx, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<2> c2 = p0; c2 <= 1; c2 += 1) {
  #pragma HLS PIPELINE II=1
    // io_L1
    if (c2 == p0) {
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
void sum_drain_IO_L1_out_inter_trans_boundary(int idx, sum_t4 local_sum[1], hls::stream<sum_t4> &fifo_sum_drain_out) {
#pragma HLS INLINE
  /* Variable Declaration */
  int p0 = idx; // module id
  /* Variable Declaration */

  for (ap_uint<2> c2 = p0; c2 <= 1; c2 += 1)
  #pragma HLS PIPELINE II=1
    if (c2 == p0) {
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
void sum_drain_IO_L1_out(int idx, hls::stream<sum_t4> &fifo_sum_drain_in, hls::stream<sum_t4> &fifo_sum_drain_out, hls::stream<float> &fifo_sum_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  int p0 = idx; // module id
  sum_t4 local_sum[1];
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
  sum_t4 local_sum[1];
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
void sum_drain_IO_L2_out(sum_t4 *sum, hls::stream<sum_t4> &fifo_sum_drain_local_in) {
#pragma HLS INLINE OFF
  /* Variable Declaration */
  /* Variable Declaration */

  // array
  // io_L2
  for (ap_uint<2> c2 = 0; c2 <= 1; c2 += 1) {
  #pragma HLS PIPELINE II=1
    // io_L1
    // access_coalesce
    {
      sum_t4 in_data;
      sum_t4 out_data;
      in_data = fifo_sum_drain_local_in.read();
      out_data = in_data;
      sum[c2] = out_data;
    }
  }
}
/* Module Definition */

void kernel0(output_t16 *output, sum_t4 *sum)
{
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem_output
#pragma HLS INTERFACE m_axi port=sum offset=slave bundle=gmem_sum
#pragma HLS INTERFACE s_axilite port=output bundle=control
#pragma HLS INTERFACE s_axilite port=sum bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATAFLOW

  /* FIFO Declaration */
  /* output_IO_L1_in fifo */ hls::stream<output_t4> fifo_output_output_IO_L1_in_0;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_0 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<output_t4> fifo_output_output_IO_L1_in_1;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_1 core=FIFO_SRL
  /* output_IO_L1_in fifo */ hls::stream<output_t4> fifo_output_output_IO_L1_in_2;
  #pragma HLS STREAM variable=fifo_output_output_IO_L1_in_2 depth=2
  #pragma HLS RESOURCE variable=fifo_output_output_IO_L1_in_2 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_0;
  #pragma HLS STREAM variable=fifo_output_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_output_PE_1;
  #pragma HLS STREAM variable=fifo_output_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_output_PE_1 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_0;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_0 core=FIFO_SRL
  /* PE fifo */ hls::stream<float> fifo_sum_drain_PE_1;
  #pragma HLS STREAM variable=fifo_sum_drain_PE_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_PE_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_0;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_0 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_0 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_1;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_1 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_1 core=FIFO_SRL
  /* sum_drain_IO_L1_out fifo */ hls::stream<sum_t4> fifo_sum_drain_sum_drain_IO_L1_out_2;
  #pragma HLS STREAM variable=fifo_sum_drain_sum_drain_IO_L1_out_2 depth=2
  #pragma HLS RESOURCE variable=fifo_sum_drain_sum_drain_IO_L1_out_2 core=FIFO_SRL
  /* FIFO Declaration */

  /* Module Call */
  output_IO_L2_in(
    /* array */ output,
    /* fifo */ fifo_output_output_IO_L1_in_0
  );
  /* Module Call */

  /* Module Call */
  output_IO_L1_in_boundary_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_output_output_IO_L1_in_1,
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
  PE_wrapper(
    /* module id */ 0,
    /* fifo */ fifo_output_PE_0,
    /* fifo */ fifo_sum_drain_PE_0
  );
  /* Module Call */

  /* Module Call */
  PE_wrapper(
    /* module id */ 1,
    /* fifo */ fifo_output_PE_1,
    /* fifo */ fifo_sum_drain_PE_1
  );
  /* Module Call */

  /* Module Call */
  sum_drain_IO_L1_out_boundary_wrapper(
    /* module id */ 1,
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
    /* array */ sum,
    /* fifo */ fifo_sum_drain_sum_drain_IO_L1_out_0
  );
  /* Module Call */

}
