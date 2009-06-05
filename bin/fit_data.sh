#!/bin/bash
if [ ! -f data ]; then
   echo "Falta data."
      exit 1
fi

if [ ! -f variables.txt ]; then
   echo "Falta variables.txt."
      exit 1
fi

if [[ ! (-f data_IIa && -f data_IIb && -f data_v14 && -f data_v15 && -f data_v16) ]] ; then
   awk '$8 == "v14" {print $1,$2,$3,$4,$5,$6,$7}' data > data_v14
   cp data_v14 data_IIa
   awk '$8 == "v15" {print $1,$2,$3,$4,$5,$6,$7}' data > data_v15
   awk '$8 == "v16" {print $1,$2,$3,$4,$5,$6,$7}' data  > data_v16
   cat data_v15 data_v16 > data_IIb
fi

if [[ -f et_parameters_IIa && -f et_parameters_IIb ]]; then
  echo "Usando et_parameters existentes."
else
  echo "Fiting et model..." 
  root -l -b -q et_fit.C > et_fit.out

  if [[ ! (-f et_parameters_IIa && -f et_parameters_IIb ) ]]; then
    echo "Archivos et_parameters no fueron creados correctamente."
    exit 1
  fi
fi

if [ -f parameters.txt ]; then
  echo "Usando parameters.txt existente."
else 
  cat initial_parameters.txt et_parameters_IIa et_parameters_IIb > parameters.txt
fi

./fit_subdata.sh v14 IIa
./fit_subdata.sh v15 IIb
./fit_subdata.sh v16 IIb
./bs fit --signal --bkg --resolution --efficiency --error --tag -j8 | tee fit.out
./bs plot --bkg --resolution --efficiency -p parameters_out.txt > plot_bkg.out
if [ ! -d plots_bkg ]; then
   mkdir plots_bkg
fi
mv mass.gif time.gif cpsi.gif ctheta.gif phi.gif plot_bkg.out plots_bkg

./bs plot --signal --bkg --resolution --efficiency -p parameters_out.txt > plot.out
