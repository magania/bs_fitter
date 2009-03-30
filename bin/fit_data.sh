#!/bin/bash
if [ ! -f variables.txt ]; then
   echo "Falta variables.txt."
   exit 1
fi

if [ -f et_parameters.txt ]; then
  echo "Usando et_parameters.txt existente."
else
  echo "Fiting et model..." 
  root -l -b -q fit_et.C > fit_et.out
  if [ ! -d plots_et ]; then
     mkdir plots_et
  fi

  mv fit_et.out et_sig.gif et_bkg.gif mass_cut.gif plots_et

  if [ ! -f et_parameters.txt ]; then
    echo "Archivo et_parameters.txt no fue creado correctamente."
    exit 1
  fi
fi

if [ -f parameters.txt ]; then
  echo "Usando parameters.txt existente."
else 
  cat initial_parameters.txt et_parameters.txt > parameters.txt
fi

./bs fit --signal --bkg --resolution --efficiency --error --tag -j8 | tee fit.out
./bs plot --bkg --resolution --efficiency -p parameters_out.txt > plot_bkg.out
if [ ! -d plots_bkg ]; then
   mkdir plots_bkg
fi
mv mass.gif time.gif cpsi.gif ctheta.gif phi.gif plot_bkg.out plots_bkg

./bs plot --signal --bkg --resolution --efficiency -p parameters_out.txt > plot.out
