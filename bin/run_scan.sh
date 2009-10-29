#!/bin/bash

DIR=`pwd`

mkdir scan
mkdir scan/out

for t in `seq -3.15 0.05 3.16`; do 
 if [ ! -f scan/phi_$t ]; then
   echo "#!/bin/bash" > /tmp/job.sh
   echo "PHI=$t" >> /tmp/job.sh 
   echo "DIR=$DIR" >> /tmp/job.sh
   echo . /home/magania/local/setup.sh >> /tmp/job.sh
   cat scan.sh >> /tmp/job.sh
   echo send $t
   /usr/bin/qsub -q LONG@timber-clued0 -l nodes=1 -k oe -m ae /tmp/job.sh 
 fi
done
