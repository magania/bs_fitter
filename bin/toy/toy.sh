#!/bin/bash
#function get_root {
# wget ftp://root.cern.ch/root/root_v5.22.00.Linux-slc5-gcc3.4.tar.gz -O root.tar.gz
# wget http://www-clued0.fnal.gov/~magania/root_v5.22.00.Linux-slc5-gcc3.4.tar.gz -O root.tar.gz
#wget ftp://root.cern.ch/root/root_v5.22.00.Linux-slc4_amd64-gcc3.4.tar.gz -O root.tar.gz
#}

#get_root
#tar xzf root.tar.gz
#. root/bin/thisroot.sh
#which root

#. /home/magania/local/root/bin/thisroot.sh
chmod u+x BS
ls -l BS
arch
gcc --version
ldd BS
export ROOTSYS=/tmp

./BS generate --signal --bkg --resolution --error --efficiency -n 63278
./BS fit --signal --bkg --resolution --error --efficiency -o $1
