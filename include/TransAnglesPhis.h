/* 
 * File:   TransAnglesPhis.h
 * Author: magania
 *
 * Created on November 28, 2008, 11:30 PM
 */

#include "TransAngles.h"
#include "Phis.h"

#ifndef _TRANSANGLESPHIS_H
#define	_TRANSANGLESPHIS_H

class TransAnglesPhis : public TransAngles {
public:
    TransAnglesPhis(RooAbsReal* parent, RooRealVar& cpsi, RooRealVar& ctheta, RooRealVar& phi, Phis &phis);
    TransAnglesPhis(RooAbsReal* parent, const TransAnglesPhis& other, const char* name = 0);

    Double_t fe(Int_t i) const;
    Double_t int_fe(Int_t i, Int_t code, const char* range) const;
    
private:
    RooRealProxy _Phi1;
    RooRealProxy _Phi2;
    RooRealProxy _Phi3;
    RooRealProxy _Phi4;
    RooRealProxy _Phi5;
    RooRealProxy _Phi6;
};


#endif	/* _TRANSANGLESPHIS_H */

