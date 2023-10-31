#!/bin/bash

set -euo pipefail

for ifile in tests/input*.txt
do
    ofile=${ifile/input/output}
    echo Testing against ${ifile} and ${ofile}...
    ./main < ${ifile}          \
        | diff - ${ofile}      \
        && echo PASSED         \
        || (echo FAILED; false)
done

echo All tests passed.

