#!/bin/bash

VARIABLES="A0 A1 DeltaGamma Delta_1 Delta_2 Phi_s Tau"

for t in $VARIABLES ; do grep $t fit.* | sed 's/  */\t/g' | cut -f4,6 > $t ;done
paste $VARIABLES > fits.dat
rm $VARIABLES

awk '{dg=$5;phis=$11}; (dg>0 && phis>-1.570796327 && phis<1.570796327) || ( dg<0 && (phis<-1.570796327 || phis>1.570796327) )' fits.dat > fits2.dat

awk '{dg=$5;phis=$11;delta1=$7;delta2=$9}; dg<0 && (phis<-1.570796327 || phis>1.570796327) {dg=-dg;phis=3.141592654-phis;delta1=3.141592654-delta1;delta2=3.141592654-delta2}; {print $1,$2,$3,$4,dg,$6,delta1,$8,delta2,$10,phis,$12,$13,$14}' fits2.dat > fits3.dat

awk '{phis=$11;delta1=$7;delta2=$9}; phis<3.141592654 && phis>-3.141592654 && delta1>-2*3.141592654 && delta1<2*3.141592654 && delta2>-3.141592654 && delta2<3*3.141592654' fits3.dat > fits4.dat

awk '{phis=$5;delta1=$7; delta2=$9}; delta1<-3.141592654 {delta1=2*3.141592654+delta1}; delta1>3.141592654 {delta1=-2*3.141592654+delta1};  delta2<0 {delta2=2*3.141592654+delta2}; delta2>2*3.141592654 {delta2=-2*3.141592654+delta2}; phis<-1.570796327 {phis=3.141592654+phis}; phis>1.570796327 {phis=-3.141592654+phis}; {print $1,$2,$3,$4,phis,$6,delta1,$8,delta2,$10,$11,$12,$13,$14}' fits4.dat > fits5.dat

