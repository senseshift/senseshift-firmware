#!/bin/bash
set -euxo pipefail

build_dir=$1
env=$2
shift 2

# Run tests
$@

lcov -d $build_dir/$env/ -c -o lcov.info
lcov --remove lcov.info '/usr/include/*' '*.platformio/*' '*.pio/*' '*/tool-unity/*' '*/test/*' '*/MockArduino/*' -o lcov.info.cleaned
genhtml -p $PWD -o build/coverage/ --demangle-cpp lcov.info.cleaned
