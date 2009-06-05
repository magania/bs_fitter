#!/bin/bash
./bs-fit -n$1 -e$2 -j8 -o fit_$1.out | tee fit_$1.out
mv parameters_out.txt parameters_v14_out.txt
