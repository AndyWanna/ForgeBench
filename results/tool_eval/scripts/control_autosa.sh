#!/usr/bin/env bash
# Inside the pinned AutoSA image. /control is a fresh writable bind mount.
set -euo pipefail
cd /usr/src/docker_autosa
mkdir -p /control/src /control/latency_est /control/resource_est /control/tuning
./autosa ./autosa_tests/mm/kernel.c \
  --config=./autosa_config/autosa_config.json \
  --target=autosa_hls_c --output-dir=/control \
  '--sa-sizes={kernel[]->space_time[3];kernel[]->array_part[16,16,16];kernel[]->latency[8,8];kernel[]->simd[2]}' \
  --simd-info=./autosa_tests/mm/simd_info.json --host-serialize --hls
test -s /control/src/kernel_kernel.cpp
printf 'CONTROL PASS: upstream matrix multiplication emitted HLS C++.\n'
