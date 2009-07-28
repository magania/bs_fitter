#!/bin/bash
if [ ! -f data ]; then
   echo "Falta data."
      exit 1
fi

if [ ! -f variables.txt ]; then
   echo "Falta variables.txt."
      exit 1
fi

if [ -f parameters.txt ]; then
  echo "Usando parameters.txt existente."
else 
  cat initial_parameters_mc.txt > parameters.txt
fi

./bs-fit-signal -nv14 -d data -j8 | tee fit.out

