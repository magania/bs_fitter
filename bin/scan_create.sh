#!/bin/bash

PARAMS_DIR=scan/in

x=0
while [ $x -lt 63 ] ; do
    OUT_FILE=$PARAMS_DIR/Phi_s_$x.txt
    grep -v Phi_s parameters_fit.txt > $OUT_FILE 
    echo Phi_s = `echo -3.1+0.1*$x|bc -l` >> $OUT_FILE
    grep Phi_s $OUT_FILE
    let x=x+1  
done

x=0
while [ $x -lt 41 ] ; do
    OUT_FILE=$PARAMS_DIR/DeltaGamma_$x.txt
    grep -v DeltaGamma parameters_fit.txt > $OUT_FILE 
    echo DeltaGamma = `echo 0.005*$x|bc -l` >> $OUT_FILE
    grep DeltaGamma $OUT_FILE
    let x=x+1  
done

x=0
while [ $x -lt 41 ] ; do
    OUT_FILE=$PARAMS_DIR/Tau_$x.txt
    grep -v Tau parameters_fit.txt > $OUT_FILE 
    echo Tau = `echo 1.3+0.005*$x|bc -l` >> $OUT_FILE
    grep Tau $OUT_FILE
    let x=x+1  
done
