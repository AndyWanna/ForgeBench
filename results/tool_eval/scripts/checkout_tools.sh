#!/usr/bin/env bash
# Fetch the exact revisions used for the fresh evaluation. No global git config.
set -euo pipefail
tool_eval_work=${1:?Usage: checkout_tools.sh EXTERNAL_WORK_DIRECTORY}
mkdir -p "$tool_eval_work/tools"
tool_eval_work=$(cd "$tool_eval_work" && pwd)
fetch_pin() {
  local name=$1 url=$2 commit=$3
  local dest="$tool_eval_work/tools/$name"
  if [[ -e "$dest" ]]; then
    [[ "$(git -C "$dest" rev-parse HEAD)" == "$commit" ]] || {
      printf 'Existing checkout does not match pin: %s\n' "$name" >&2
      return 1
    }
    return
  fi
  git init "$dest"
  git -C "$dest" remote add origin "$url"
  git -C "$dest" fetch --depth 1 origin "$commit"
  git -C "$dest" checkout --detach FETCH_HEAD
}
fetch_pin scalehls https://github.com/UIUC-ChenLab/scalehls.git b69f334c9c122fad3759fcf388d3f039689b6fc0
fetch_pin hida https://github.com/UIUC-ChenLab/ScaleHLS-HIDA.git fd79f6c946593c290d6d285c2a2efcab72fbe9c6
fetch_pin autosa https://github.com/UCLA-VAST/AutoSA.git b61a1b4132d631600696feba59eb606acb34d304
fetch_pin streamhls https://github.com/UCLA-VAST/Stream-HLS.git 00c6219a82fc710492f4eb44e4f34836dfb19e08
fetch_pin stream-polygeist https://github.com/llvm/Polygeist.git 77c04bb2a7a2406ca9480bcc9e729b07d2c8d077
for name in scalehls hida; do
  git -C "$tool_eval_work/tools/$name" \
    -c submodule.polygeist.url=https://github.com/hanchenye/Polygeist.git \
    submodule update --init --depth 1 polygeist
done
git -C "$tool_eval_work/tools/scalehls/polygeist" submodule update --init --depth 1 llvm-project
git -C "$tool_eval_work/tools/streamhls" submodule update --init --depth 1 extern/llvm-project
git -C "$tool_eval_work/tools/stream-polygeist" submodule update --init --depth 1 llvm-project
# Rename HIDA's CMake targets, not output filenames, to avoid import collisions.
patch_dir=$(cd "$(dirname "$0")/../patches/build" && pwd)
if git -C "$tool_eval_work/tools/hida" apply --check "$patch_dir/hida-target-names.patch"; then
  git -C "$tool_eval_work/tools/hida" apply "$patch_dir/hida-target-names.patch"
else
  git -C "$tool_eval_work/tools/hida" apply --reverse --check "$patch_dir/hida-target-names.patch"
fi
