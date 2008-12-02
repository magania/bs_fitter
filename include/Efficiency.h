/* 
 * File:   Efficiency.h
 * Author: magania
 *
 * Created on November 24, 2008, 9:34 PM
 */

#ifndef _EFFICIENCY_H
#define	_EFFICIENCY_H

#include "RooRealVar.h"
#include "RooArgSet.h"
#include <fstream>

class Efficiency {
public:
    Efficiency();
    void setEfficiency(const char* efficiency);
    void writeEfficiency(const char* file);
    RooArgSet* getAs();
    RooArgSet* getBs();
    RooArgSet* getCs();
    
    RooRealVar a1;
    RooRealVar a2;
    RooRealVar a3;
    RooRealVar a4;
    RooRealVar a5;
    RooRealVar a6;
    RooRealVar a7;
    RooRealVar a8;
    RooRealVar a9;

    RooRealVar b1;
    RooRealVar b2;
    RooRealVar b3;
    RooRealVar b4;
    RooRealVar b5;
    RooRealVar b6;
    RooRealVar b7;
    RooRealVar b8;
    RooRealVar b9;

    RooRealVar c1;
    RooRealVar c2;
    RooRealVar c3;
    RooRealVar c4;
    RooRealVar c5;
    RooRealVar c6;
    RooRealVar c7;
    RooRealVar c8;
    RooRealVar c9;

    RooArgSet As;
    RooArgSet Bs;
    RooArgSet Cs;
};


#endif	/* _EFFICIENCY_H */

