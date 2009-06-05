#!/bin/bash
./bs-fit -n$1 -e$2 -j8 -o fit_$1.out -d data_$1 | tee fit_$1.out
mv parameters_out.txt parameters_${1}_out.txt
