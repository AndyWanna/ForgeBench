#!/usr/bin/env bash
# Run inside the build image with the completed build tree mounted at /work.
set -euo pipefail
set -x
clang --version
ld.lld --version
cmake --version
ninja --version
python3 --version
dpkg-query -W build-essential clang-14 lld-14 cmake ninja-build python3 python3-numpy libgmp-dev libboost-dev libzstd-dev zlib1g-dev libtinfo-dev
/work/build/scale/bin/cgeist --version
/work/build/scale/bin/scalehls-opt --version
/work/build/hida/bin/scalehls-opt --version
/work/build/stream-frontend/bin/cgeist --version
/work/build/streamhls/bin/streamhls-opt --version
