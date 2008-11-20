#!/bin/bash

if [ -e data_set.txt ]
then mv data_set.txt data_set_old.txt
fi

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../bsfitter/lib
SAMPLES=0
while [ $SAMPLES -lt 1000 ]
do 
../../toy/dist/Debug/GNU-Linux-x86/toy -f- | tail -n+7 | ../bin/fitter -d- -r /tmp/result_1 -n > /dev/null&
PID1=$!
../../toy/dist/Debug/GNU-Linux-x86/toy -f- | tail -n+7 | ../bin/fitter -d- -r /tmp/result_2 -n > /dev/null&
PID2=$!
../../toy/dist/Debug/GNU-Linux-x86/toy -f- | tail -n+7 | ../bin/fitter -d- -r /tmp/result_3 -n > /dev/null&
PID3=$!
../../toy/dist/Debug/GNU-Linux-x86/toy -f- | tail -n+7 | ../bin/fitter -d- -r /tmp/result_4 -n > /dev/null&
PID4=$!
echo $PID1 $PID2 $PID3 $PID4
wait $PID1 $PID2 $PID3 $PID4
cat /tmp/result_* | grep -eA02 -eAll2 -eDeltaGamma -edelta_1 -edelta_2 -ephi_s -etau | sed -e 's/  */\t/g' |awk -f re_order.gak | sed -e 's/  */ /g'>> data_set.txt
let SAMPLES=`cat data_set.txt|wc -l`
done
