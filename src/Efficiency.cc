#include "Efficiency.h"

Efficiency::Efficiency() :
_cpsi("_cpsi", "cos psi", 0),
_ctheta("_ctheta", "cos theta", 0),
_phi("_phi", "phi",0),
_cpsi_1("_cpsi_1", "a1 eff cos(#psi)", 0),
_cpsi_2("_cpsi_2", "a2 eff cos(#psi)", 0),
_cpsi_3("_cpsi_3", "a3 eff cos(#psi)", 0),
_cpsi_4("_cpsi_4", "a4 eff cos(#psi)", 0),
_cpsi_5("_cpsi_5", "a5 eff cos(#psi)", 0),
_cpsi_6("_cpsi_6", "a6 eff cos(#psi)", 0),
_cpsi_7("_cpsi_7", "a7 eff cos(#psi)", 0),
_cth_1("_cth_1", "a1 eff cos(#theta)", 0),
_cth_2("_cth_2", "a2 eff cos(#theta)", 0),
_cth_3("_cth_3", "a3 eff cos(#theta)", 0),
_cth_4("_cth_4", "a4 eff cos(#theta)", 0),
_cth_5("_cth_5", "a5 eff cos(#theta)", 0),
_cth_6("_cth_6", "a6 eff cos(#theta)", 0),
_phi_1("_phi_1", "a1 eff #Phi", 0),
_phi_2("_phi_2", "a2 eff #Phi", 0),
_phi_3("_phi_3", "a3 eff #Phi", 0),
_phi_4("_phi_4", "a4 eff #Phi", 0),
_phi_5("_phi_5", "a5 eff #Phi", 0),
_phi_6("_phi_6", "a6 eff #Phi", 0),
_coef_cpsi(_cpsi_1, _cpsi_2, _cpsi_3, _cpsi_4, _cpsi_5, _cpsi_6, _cpsi_7),
_coef_ctheta(_cth_1, _cth_2, _cth_3, _cth_4, _cth_5, _cth_6),
_coef_phi(_phi_1, _phi_2, _phi_3, _phi_4, _phi_5, _phi_6),
_eff_cpsi("_eff_cpsi", "cos(#psi) efficiency", _cpsi, 
        RooArgList(_cpsi_1,_cpsi_2,_cpsi_3,_cpsi_4,_cpsi_5,_cpsi_6,_cpsi_7)),
_eff_ctheta("_eff_ctheta", "cos(#theta) efficiency", _ctheta,
        RooArgList(_cth_1,_cth_2,_cth_3,_cth_4,_cth_5,_cth_6)),
_eff_phi("_eff_phi", "#Phi efficiency", _phi, 
        RooArgList(_phi_1,_phi_2,_phi_3,_phi_4,_phi_5,_phi_6)),
_efficiency("_efficiency", "efficiency", RooArgSet(_eff_cpsi, _eff_ctheta, _eff_phi)),
_Phi1("_Phi1", "#Phi1", 0),
_Phi2("_Phi2", "#Phi2", 0),
_Phi3("_Phi3", "#Phi3", 0),
_Phi4("_Phi4", "#Phi4", 0),
_Phi5("_Phi5", "#Phi5", 0),
_Phi6("_Phi6", "#Phi6", 0),
_Phis(_Phi1, _Phi2, _Phi3, _Phi4, _Phi5, _Phi6)
{
}

void Efficiency::setEfficiency(const char* efficiency) {
    _coef_cpsi.readFromFile(efficiency);
    _coef_ctheta.readFromFile(efficiency);
    _coef_phi.readFromFile(efficiency);
    _Phis.readFromFile(efficiency);
}

Double_t Efficiency::getPhi(Int_t i) {
    switch (i){
        case 1: return getPhi1();
        case 2: return getPhi2();
        case 3: return getPhi3();
        case 4: return getPhi4();
        case 5: return getPhi5();
        case 6: return getPhi6();
        default: 
            cout << "EE: i=" << i << " incorrecto" << endl;
            exit(1);
    }
}

Double_t Efficiency::getPhi1() {
    return _Phi1.getVal();
}

Double_t Efficiency::getPhi2() {
    return _Phi2.getVal();
}

Double_t Efficiency::getPhi3() {
    return _Phi3.getVal();
}

Double_t Efficiency::getPhi4() {
    return _Phi4.getVal();
}

Double_t Efficiency::getPhi5() {
    return _Phi5.getVal();
}

Double_t Efficiency::getPhi6() {
    return _Phi6.getVal();
}

Double_t Efficiency::getVal(Double_t cpsi, Double_t ctheta, Double_t phi) {
/*    cout << "phis " << getPhi1() << ' '
            << getPhi2() << ' '
            << getPhi3() << ' '
            << getPhi4() << ' '
            << getPhi5() << ' '
            << getPhi6() << endl;
    cout << "eff: " << _efficiency.getVal() << endl;*/
    _cpsi = cpsi; _ctheta =ctheta; _phi =phi;
    return _efficiency.getVal();
}
