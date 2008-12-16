#!/bin/bash

cd /prj_root/2677/ckm_write/magania/local/root/
. bin/thisroot.sh
cd /scratch/
cd $PBS_JOBID
cp ~/desktop/src/bsfitter/bin/* .
let count=`ls ~/desktop/src/bsfitter/to_fit/in/ | wc -l`
while [ $count -gt 0 ] ;do 
    myfile=`ls ~/desktop/src/bsfitter/to_fit/in/* |head -n1`
    if mv $myfile parameters.txt ; then 
	rm fit.txt
	./bs fit
	while [ 1 ]
	  do let num=$RANDOM 
	  if [ ! -f ~/desktop/src/bsfitter/to_fit/out/$num.txt ]; then  break
	  fi
	done
	if mv fit.txt ~/desktop/src/bsfitter/to_fit/out/$num.txt; then
	    echo one more done.
	else
	    mv parameters.txt $myfile
	    break
	fi
    fi
    let count=`ls ~/desktop/src/bsfitter/to_fit/in/ | wc -l`
done
