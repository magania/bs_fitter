#include "TransAnglesPhis.h"

TransAnglesPhis::TransAnglesPhis(RooAbsReal* parent, RooRealVar& cpsi, RooRealVar& ctheta, RooRealVar& phi, Phis &phis) :
TransAngles(parent, cpsi, ctheta, phi),
_Phi1("_Phi1", "#phi_1", parent, phis.Phi1),
_Phi2("_Phi2", "#phi_2", parent, phis.Phi2),
_Phi3("_Phi3", "#phi_3", parent, phis.Phi3),
_Phi4("_Phi4", "#phi_4", parent, phis.Phi4),
_Phi5("_Phi5", "#phi_5", parent, phis.Phi5),
_Phi6("_Phi6", "#phi_6", parent, phis.Phi6)
{
}

TransAnglesPhis::TransAnglesPhis(RooAbsReal* parent, const TransAnglesPhis& other, const char* name) :
TransAngles(parent, other, name),
_Phi1("_Phi1", parent, other._Phi1),
_Phi2("_Phi1", parent, other._Phi2),
_Phi3("_Phi1", parent, other._Phi3),
_Phi4("_Phi1", parent, other._Phi4),
_Phi5("_Phi1", parent, other._Phi5),
_Phi6("_Phi1", parent, other._Phi6)
{
}

Double_t TransAnglesPhis::fe(Int_t i) const{
    return f(i);
}

Double_t TransAnglesPhis::int_fe(Int_t i, Int_t code, const char* range) const{
    switch (i) {
        case 1:
            return _Phi1;
        case 2:
            return _Phi2;
        case 3:
            return _Phi3;
        case 4:
            return _Phi4;
        case 5:
            return _Phi5;
        case 6:
            return _Phi6;
        default:
            std::cout << "EE: Error index phi." << std::endl;
            return 0;
    }
}
