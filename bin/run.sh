#!/bin/bash

cd bin
if [ -f fitter.out ]; 
then mv fitter.out fitter_old.out
fi
./fitter | tee fitter.out
display	mass.gif&
display time.gif&
display cpsi.gif&
display ctheta.gif&
display phi.gif&
