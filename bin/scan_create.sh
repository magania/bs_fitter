let x=0; while [ "$x" -lt "63" ] ; do grep -v Phi_s parameters.txt > ../to_fit/in/$x.txt; echo Phi_s = `echo -3.1+0.1*$x|bc -l` >> ../to_fit/in/$x.txt ; grep Phi_s ../to_fit/in/$x.txt; let x=x+1  ; done
