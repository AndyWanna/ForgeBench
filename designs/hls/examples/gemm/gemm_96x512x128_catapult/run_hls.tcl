# Auto-generated TCL file for Catapult HLS
options defaults
options set Input/TargetPlatform x86_64

project new -name catapult_top

solution file add top.cpp
solution file add top.h

go analyze

directive set -OPT_CONST_MULTS simple_one_adder
solution library add mgc_Xilinx-ZYNQ-uplus-2_beh -- -rtlsyntool Vivado -manufacturer Xilinx -family ZYNQ-uplus -speed -2 -part xczu9eg-ffvb1156-2-e
solution library add Xilinx_RAMS
directive set -PROTOTYPE_ROM false

solution design set top -top
go compile
go libraries

directive set -CLOCKS {clk {-CLOCK_PERIOD 10 -CLOCK_EDGE rising -CLOCK_UNCERTAINTY 0.0 -CLOCK_HIGH_TIME 5.0 -RESET_SYNC_NAME rst -RESET_ASYNC_NAME arst_n -RESET_KIND sync -RESET_SYNC_ACTIVE high -RESET_ASYNC_ACTIVE low -ENABLE_ACTIVE high}}

go assembly
go extract

project save
exit