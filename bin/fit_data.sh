#!/bin/bash

TMVA_CUT=0.46

awk '$8 > '$TMVA_CUT' {print $1,$2,$3,$4,$5,$6,$7}' tofit.dat | awk -f d2p.awk > fit.dat
MASS_CUT=`root -b -q fit_et.cpp | tee fit_et.out | grep MASS`
#MASS_CUT=`echo MASS: 5.04951 5.2504 5.48037 5.74928`

MASS_LEFT=`echo $MASS_CUT | cut -d' ' -f 2`
MASS_LLEFT=`echo $MASS_CUT | cut -d' ' -f 3`
MASS_RRIGHT=`echo $MASS_CUT | cut -d' ' -f 4`
MASS_RIGHT=`echo $MASS_CUT | cut -d' ' -f 5`

echo $MASS_LEFT $MASS_LLEFT $MASS_RRIGHT $MASS_RIGHT

echo '_m = 5.3 L('$MASS_LEFT' - '$MASS_RIGHT')' > variables.txt
echo '_t =  5.0000 L(-2.5 - 12)' >> variables.txt
echo '_et = 0 L(0 - 1)' >> variables.txt

awk '$8 > '$TMVA_CUT' && '$MASS_LEFT' < $1 && $1 < '$MASS_RIGHT' {print $1,$2,$3,$4,$5,$6,$7}' tofit.dat | awk -f d2p.awk > fit.dat
awk '( $1 < '$MASS_LLEFT' || '$MASS_RRIGHT' < $1 )' fit.dat > fit_bkg.dat
#awk '$2 < 2 && ( $1 < '$MASS_LLEFT' || '$MASS_RRIGHT' < $1 )' fit.dat > fit_prompt.dat


