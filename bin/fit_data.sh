#!/bin/bash

T_CUT=2

echo "Fiting et model..." 
awk -f d2p.awk tofit.dat > fit.dat
MASS_CUT=`root -b -q fit_et.cpp | tee fit_et.out | grep MASS`
#MASS_CUT=`echo MASS: 5.04951 5.2504 5.48037 5.74928`
if [ ! -d plots_et ]; then
mkdir plots_et
fi
mv *.gif plots_et/

XS=`echo $MASS_CUT | cut -d' ' -f 2`
MASS_LEFT=`echo $MASS_CUT | cut -d' ' -f 4`
MASS_LLEFT=`echo $MASS_CUT | cut -d' ' -f 5`
MASS_RRIGHT=`echo $MASS_CUT | cut -d' ' -f 6`
MASS_RIGHT=`echo $MASS_CUT | cut -d' ' -f 7`

echo $XS $MASS_LEFT $MASS_LLEFT $MASS_RRIGHT $MASS_RIGHT

echo '_m = 5.3 L('$MASS_LEFT' - '$MASS_RIGHT')' > variables.txt
echo '_t =  5.0000 L(-1 - 9)' >> variables.txt
echo '_et = 0 L(0 - 1)' >> variables.txt

awk $MASS_LEFT' < $1 && $1 < '$MASS_RIGHT' {print $1,$2,$3,$4,$5,$6,$7}' fit.dat | awk -f d2p.awk > fit.tmp
mv fit.tmp fit.dat
awk '$1 < '$MASS_LLEFT' || '$MASS_RRIGHT' < $1' fit.dat > fit_bkg.dat
awk '$2 > '$T_CUT fit_bkg.dat > fit_noprompt.dat
awk '$2 < '$T_CUT fit_bkg.dat > fit_prompt.dat

echo "Fiting prompt ..."
./bs fit  --prompt-only -d fit_prompt.dat -p initial_parameters.txt > fit_prompt.out
mv parameters_out.txt parameters_prompt.txt
./bs plot --prompt-only -d fit_prompt.dat -p parameters_prompt.txt > /dev/null
if [ ! -d plots_prompt ]; then
mkdir plots_prompt
fi
mv *.gif fit_prompt.out plots_prompt/

echo "Fiting noprompt ..."
./bs fit  --noprompt-only -d fit_noprompt.dat -p initial_parameters.txt > fit_noprompt.out
mv parameters_out.txt parameters_noprompt.txt
./bs plot --noprompt-only -d fit_noprompt.dat -p parameters_noprompt.txt > /dev/null

if [ ! -d plots_noprompt ]; then
mkdir plots_noprompt
fi
mv *.gif fit_noprompt.out plots_noprompt/

cat parameters_prompt.txt | sed 's/C//g' > parameters.txt
grep -v S parameters_noprompt.txt | sed 's/C//g' >> parameters.txt
echo "xp =  0.7 L(0 - 1)" >> parameters.txt 

#echo "Fitting background ..."
#./bs fit  -d fit_bkg.dat --prompt-only --noprompt-only > fit_bkg.out
mv parameters.txt parameters_bkg.txt
#./bs plot -d fit_bkg.dat --prompt-only --noprompt-only -p parameters_bkg.txt> /dev/null
#if [ ! -d plots_bkg ]; then
#mkdir plots_bkg
#fi
#mv *.gif fit_bkg.out plots_bkg/

head -n 18 initial_parameters.txt | tail -n 16 > parameters.txt
cat parameters_bkg.txt >> parameters.txt
echo "xs = $XS L(0 - 1)" >> parameters.txt
cat et_parameters.txt >> parameters.txt 
