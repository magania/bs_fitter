#PHI=$1
#DIR=$2

SCAN_DIR=$DIR/scan
OUT_DIR=$DIR/scan/out
SAMPLE=fit.root
CUTS=""

cd /batch/$PBS_JOBID &&
cp -r $DIR/* . &&
grep -v Phi_s parameters_out.txt > parameters.txt &&
echo Phi_s = $PHI >> parameters.txt  &&
./bs-fit -j1 -d $SAMPLE  -n3 -a4 -o $SCAN_DIR/phi_$PHI > $OUT_DIR/phi_$PHI
