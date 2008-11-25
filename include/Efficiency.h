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
#include "RooChebychev.h"
#include "RooProdPdf.h"
#include "TransAngles.h"


class Efficiency {
public:
    Efficiency();
    void setEfficiency(const char* eff);
    Double_t getPhi(Int_t i);
    Double_t getPhi1();
    Double_t getPhi2();
    Double_t getPhi3();
    Double_t getPhi4();
    Double_t getPhi5();
    Double_t getPhi6();

    Double_t getVal(Double_t cpsi, Double_t ctheta, Double_t phi) ;
    
private:
    RooRealVar _cpsi;
    RooRealVar _ctheta;
    RooRealVar _phi;
    
    RooRealVar _cpsi_1;
    RooRealVar _cpsi_2;
    RooRealVar _cpsi_3;
    RooRealVar _cpsi_4;
    RooRealVar _cpsi_5;
    RooRealVar _cpsi_6;
    RooRealVar _cpsi_7;

    RooRealVar _cth_1;
    RooRealVar _cth_2;
    RooRealVar _cth_3;
    RooRealVar _cth_4;
    RooRealVar _cth_5;
    RooRealVar _cth_6;

    RooRealVar _phi_1;
    RooRealVar _phi_2;
    RooRealVar _phi_3;
    RooRealVar _phi_4;
    RooRealVar _phi_5;
    RooRealVar _phi_6;

    RooChebychev _eff_cpsi;
    RooChebychev _eff_ctheta;
    RooChebychev _eff_phi;
    
    RooArgSet _coef_cpsi;
    RooArgSet _coef_ctheta;
    RooArgSet _coef_phi;
    RooProdPdf _efficiency;

    RooRealVar _Phi1;
    RooRealVar _Phi2;
    RooRealVar _Phi3;
    RooRealVar _Phi4;
    RooRealVar _Phi5;
    RooRealVar _Phi6;
    RooArgSet _Phis;
    
    
};


#endif	/* _EFFICIENCY_H */

