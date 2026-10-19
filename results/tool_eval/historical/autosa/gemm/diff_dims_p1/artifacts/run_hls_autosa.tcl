# Auto-generated TCL file for HLS
open_project -reset project_1

set_top kernel0

add_files  top_gemm_new_ijk_bias_kernel.cpp
add_files -tb top_gemm_new_ijk_bias_host.cpp
add_files top_gemm_new_ijk_bias_kernel.h

open_solution "solution1"

set_part xczu9eg-ffvb1156-2-e

create_clock -period 10 -name default
config_compile -name_max_length 50
csim_design

csynth_design

cosim_design

export_design -format ip_catalog

exit