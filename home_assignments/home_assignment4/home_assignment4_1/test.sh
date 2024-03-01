#!/bin/bash

set -euo pipefail

for f in "the" "Alice" "To be, or not to be"
do
    echo Testing against "\"${f}\""
    diff <(./main "${f}" ./books/*.txt) <(grep -Fc "${f}" ./books/*.txt) \
        && echo PASSED         \
        || (echo FAILED; false)
done

echo All tests passed.

