/* 
 * File:   Phis.h
 * Author: magania
 *
 * Created on November 28, 2008, 7:41 PM
 */

#ifndef _PHIS_H
#define	_PHIS_H

#include "RooRealVar.h"
#include "RooArgSet.h"

class Phis {
public:
    Phis();
    ~Phis();
    void setPhis(const char* file);
    void writePhis(const char* file);
    RooArgSet* getPhis();
    
    RooRealVar Phi1;
    RooRealVar Phi2;
    RooRealVar Phi3;
    RooRealVar Phi4;
    RooRealVar Phi5;
    RooRealVar Phi6;

    RooArgSet phis;
};

#endif	/* _PHIS_H */

