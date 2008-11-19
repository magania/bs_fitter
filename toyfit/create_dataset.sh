#!/bin/bash

cat result_*.out  | grep -eA02 -eAll2 -eDeltaGamma -edelta_1 -edelta_2 -ephi_s -etau | sed -e 's/  */\t/g' |awk -f re_order.gak | sed -e 's/  */ /g'> data_set.txt
