#!/bin/bash
set -euxo pipefail

build_dir=$1
env=$2
shift 2

# Generate initial coverage report
lcov -i -d $build_dir/$env/ -c -o lcov.info.initial

# Run tests
$@

lcov -d $build_dir/$env/ -c -o lcov.info
lcov --add-tracefile lcov.info.initial -a lcov.info -o lcov.info.merged
lcov --remove lcov.info.merged '/usr/include/*' '*.pio/*' '*/tool-unity/*' '*/test/*' '*/MockArduino/*' -o lcov.info.cleaned
genhtml -p $PWD -o build/coverage/ --demangle-cpp lcov.info.cleaned
