/* 
 * File:   TransAnglesEfficiency.h
 * Author: magania
 *
 * Created on November 28, 2008, 8:33 PM
 */

#include "TransAngles.h"
#include "Efficiency.h"

#include <iostream>
#include "TMath.h"
#include "RooRealProxy.h"

#ifndef _TRANSANGLESEFFICIENCY_H
#define	_TRANSANGLESEFFICIENCY_H

class TransAnglesEfficiency : public TransAngles {
public:
    TransAnglesEfficiency(RooAbsReal* parent, RooRealVar& cpsi, RooRealVar& ctheta, RooRealVar& phi, Efficiency &e, Bool_t s_ok);
    TransAnglesEfficiency(RooAbsReal* parent, const TransAnglesEfficiency& other, const char* name = 0);

    Double_t fe(Int_t i) const;
    Double_t int_fe(Int_t i, Int_t code, const char* range) const;
    
private:
    Double_t ecpsi() const;
    Double_t ectheta() const;
    Double_t ephi() const;
    Double_t ChebyshevT(Int_t i, Double_t x) const;

    Double_t static_ok;
    Double_t int_1,int_2,int_3,int_4,int_5,int_6;

    RooRealProxy _a1;
    RooRealProxy _a2;
    RooRealProxy _a3;
    RooRealProxy _a4;
    RooRealProxy _a5;
    RooRealProxy _a6;
    RooRealProxy _a7;
    RooRealProxy _a8;
    RooRealProxy _a9;

    RooRealProxy _b1;
    RooRealProxy _b2;
    RooRealProxy _b3;
    RooRealProxy _b4;
    RooRealProxy _b5;
    RooRealProxy _b6;
    RooRealProxy _b7;
    RooRealProxy _b8;
    RooRealProxy _b9;

    RooRealProxy _c1;
    RooRealProxy _c2;
    RooRealProxy _c3;
    RooRealProxy _c4;
    RooRealProxy _c5;
    RooRealProxy _c6;
    RooRealProxy _c7;
    RooRealProxy _c8;
    RooRealProxy _c9;
};

#endif	/* _TRANSANGLESEFFICIENCY_H */

