{
gSystem->Load("../lib/libBsFitter.so");
gROOT->ProcessLine(".include ../include"); 
gROOT->ProcessLine(".X fit_et.C+");      
}
