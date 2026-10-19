#!/usr/bin/env bash
# Run inside Dockerfile.build, with downloaded source trees at /work/tools.
# Use Release + assertions; omit Python bindings and unrelated LLVM backends.
set -euo pipefail
mode=${1:?scale or stream}
jobs=${JOBS:-24}
case "$mode" in
  scale)
    cmake -G Ninja -S /work/tools/scalehls/polygeist/llvm-project/llvm -B /work/build/scale \
      -DLLVM_ENABLE_PROJECTS='mlir;clang' -DLLVM_TARGETS_TO_BUILD=X86 \
      -DLLVM_EXTERNAL_PROJECTS='polygeist;scalehls' \
      -DLLVM_EXTERNAL_POLYGEIST_SOURCE_DIR=/work/tools/scalehls/polygeist \
      -DLLVM_EXTERNAL_SCALEHLS_SOURCE_DIR=/work/tools/scalehls \
      -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DLLVM_USE_LINKER=lld -DLLVM_PARALLEL_LINK_JOBS= \
      -DLLVM_INCLUDE_BENCHMARKS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF
    cmake --build /work/build/scale -j "$jobs" --target cgeist clang-resource-headers scalehls-opt scalehls-translate FileCheck
    cmake -G Ninja -S /work/tools/hida -B /work/build/hida \
      -DMLIR_DIR=/work/build/scale/lib/cmake/mlir \
      -DLLVM_DIR=/work/build/scale/lib/cmake/llvm \
      -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DLLVM_USE_LINKER=lld -DLLVM_PARALLEL_LINK_JOBS=2
    cmake --build /work/build/hida -j "$jobs" --target hida-opt hida-translate
    ;;
  stream)
    cmake -G Ninja -S /work/tools/streamhls/extern/llvm-project/llvm -B /work/build/stream-llvm \
      -DLLVM_ENABLE_PROJECTS='mlir;clang' -DLLVM_TARGETS_TO_BUILD=X86 \
      -DLLVM_EXTERNAL_PROJECTS= \
      -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DLLVM_USE_LINKER=lld -DLLVM_PARALLEL_LINK_JOBS= \
      -DLLVM_INCLUDE_BENCHMARKS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF
    cmake --build /work/build/stream-llvm -j "$jobs" --target mlir-opt mlir-translate MLIRTestToLLVMIRTranslation FileCheck
    cmake -G Ninja -S /work/tools/streamhls -B /work/build/streamhls \
      -DMLIR_DIR=/work/build/stream-llvm/lib/cmake/mlir \
      -DLLVM_DIR=/work/build/stream-llvm/lib/cmake/llvm \
      -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_DEBUG='-O1 -g0' -DCMAKE_C_FLAGS_DEBUG='-O1 -g0' -DLLVM_ENABLE_ASSERTIONS=ON \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DLLVM_USE_LINKER= -DLLVM_PARALLEL_LINK_JOBS=2
    cmake --build /work/build/streamhls -j "$jobs"
    ;;
  stream-frontend)
    cmake -G Ninja -S /work/tools/stream-polygeist/llvm-project/llvm -B /work/build/stream-frontend \
      -DLLVM_ENABLE_PROJECTS='mlir;clang' -DLLVM_TARGETS_TO_BUILD=X86 \
      -DLLVM_EXTERNAL_PROJECTS=polygeist \
      -DLLVM_EXTERNAL_POLYGEIST_SOURCE_DIR=/work/tools/stream-polygeist \
      -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
      -DLLVM_USE_LINKER=lld -DLLVM_PARALLEL_LINK_JOBS= \
      -DLLVM_INCLUDE_BENCHMARKS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF
    cmake --build /work/build/stream-frontend -j "$jobs" --target cgeist clang-resource-headers
    ;;
  *) exit 2 ;;
esac
