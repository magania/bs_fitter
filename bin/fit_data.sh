#!/bin/bash
if [ ! -f data ]; then
   echo "data file is missing."
      exit 1
fi

if [ ! -f variables.txt ]; then
   echo "File variables.txt is missing"
      exit 1
fi

if [[ ! (-f data_IIa && -f data_IIb && -f data_Mu && -f data_DMu) ]] ; then
   awk '$8 == "IIa_Mu" || $8 == "IIa_DMu" {print $1,$2,$3,$4,$5,$6,$7}' data > data_IIa
   awk '$8 == "IIb_Mu" || $8 == "IIb_DMu" {print $1,$2,$3,$4,$5,$6,$7}' data > data_IIb
   awk '$8 == "IIa_Mu" || $8 == "IIb_Mu" {print $1,$2,$3,$4,$5,$6,$7,$8}' data > data_Mu
   awk '$8 == "IIa_DMu" || $8 == "IIb_DMu" {print $1,$2,$3,$4,$5,$6,$7,$8}' data > data_DMu
fi

if [[ -f et_parameters_IIa && -f et_parameters_IIb ]]; then
  echo "Using file et_parameters (not fitting again)."
else
  echo "Fiting et model..." 
  root -l -b -q et_fit.C > et_fit.out

  if [[ ! (-f et_parameters_IIa && -f et_parameters_IIb ) ]]; then
    echo "Fiting et model failed."
    exit 1
  fi
fi

exit
if [ -f parameters.txt ]; then
  echo "Using file parameters.txt."
else 
  cat initial_parameters.txt et_parameters_IIa et_parameters_IIb > parameters.txt
fi

exit 
./fit_subdata.sh v14 IIa
./fit_subdata.sh v15 IIb
./fit_subdata.sh v16 IIb

head  -n 18 initial_parameters.txt > parameters.txt
cat parameters_v14_out.txt | grep v14 | sed 's/S_v14/S_IIa/' >> parameters.txt 
cat parameters_v15_out.txt | grep v15 | sed 's/S_v15/S_IIb/' >> parameters.txt 
cat parameters_v16_out.txt | grep v16 >> parameters.txt 
cat et_parameters_IIa >> parameters.txt 
cat et_parameters_IIb >> parameters.txt

./bs-fit-multi -d data -j8 | tee multi.out
