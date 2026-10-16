#!/usr/bin/env bash
# Run inside the build image with /work and a fresh writable /control mount.
# Controls only, NOT ForgeBench table inputs. StreamHLS's base pipeline rejects
# PolyBench's scalar alpha/beta arguments, so it uses an array-only smoke test.
set -euo pipefail
tool=${1:?scalehls, hida, or streamhls}
case "$tool" in
  scalehls|hida) frontend=/work/build/scale/bin/cgeist ;;
  streamhls) frontend=/work/build/stream-frontend/bin/cgeist ;;
  *) exit 2 ;;
esac
source=/work/tools/scalehls/samples/polybench/gemm/test_gemm.c
if [[ "$tool" == streamhls ]]; then source=/scripts/control_gemm.c; fi
set -x
"$frontend" "$source" \
  -function=test_gemm -S -memref-fullrank -raise-scf-to-affine -o /control/parsed.mlir
case "$tool" in
  scalehls)
    /work/build/scale/bin/scalehls-opt /control/parsed.mlir \
      '-scalehls-dse-pipeline=top-func=test_gemm target-spec=/work/tools/scalehls/samples/polybench/config.json' \
      -o /control/transformed.mlir
    /work/build/scale/bin/scalehls-translate /control/transformed.mlir \
      -scalehls-emit-hlscpp -o /control/emitted.cpp
    ;;
  hida)
    /work/build/hida/bin/scalehls-opt /control/parsed.mlir \
      '-hida-cpp-pipeline=top-func=test_gemm' -o /control/transformed.mlir
    /work/build/hida/bin/scalehls-translate /control/transformed.mlir \
      -scalehls-emit-hlscpp -emit-vitis-directives -o /control/emitted.cpp
    ;;
  streamhls)
    python3 /scripts/normalize_stream_dlti.py /control/parsed.mlir /control/compatible.mlir
    /work/build/streamhls/bin/streamhls-opt /control/compatible.mlir \
      '-streamhls-base-kernel-pipeline=top-func=test_gemm' -o /control/transformed.mlir
    /work/build/streamhls/bin/streamhls-translate /control/transformed.mlir \
      -emit-vivado-hls -allow-unregistered-dialect -vitis-hls-top-func=test_gemm -o /control/emitted.cpp
    ;;
esac
test -s /control/emitted.cpp
printf 'CONTROL PASS: float GEMM imported, transformed and emitted HLS C++.\n'
