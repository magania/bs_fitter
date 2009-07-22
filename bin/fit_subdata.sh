#!/bin/bash
./bs-fit -n$1 -e$2 -j8 -o fit_$1.out -d data_$1 | tee fit_$1.out
mv parameters_out.txt parameters_${1}_out.txt
mv mass.png mass_${1}.png
mv time.png time_${1}.png
mv cpsi.png cpsi_${1}.png
mv ctheta.png ctheta_${1}.png
mv phi.png phi_${1}.png
