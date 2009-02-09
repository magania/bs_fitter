#!/bin/bash

FIT_DIR=~/desktop/bsfit/data
PARAMS_DIR=$FIT_DIR/scan/in
OUT_DIR=$FIT_DIR/scan/out

cd /prj_root/2677/ckm_write/magania/local/root/
. bin/thisroot.sh
cd /scratch/
cd $PBS_JOBID
cp $FIT_DIR/* .
let count=`ls $PARAMS_DIR | wc -l`
while [ $count -gt 0 ] ;do 
    myfile=`ls -rt $PARAMS_DIR/ |head -n1`
    if mv $PARAMS_DIR/$myfile parameters.txt ; then 
	rm fit.txt
	./bs fit
#	while [ 1 ]
#	  do let num=$RANDOM 
#	  if [ ! -f $OUT_DIR/$num.txt ]; then  break
#	  fi
#	done
	if mv fit.txt $OUT_DIR/$myfile.txt; then
	    echo One more done: $myfile.
	else
	    mv parameters.txt $PARAMS_DIR/$myfile
	    break
	fi
    fi
    let count=`ls $PARAMS_DIR | wc -l`
done
