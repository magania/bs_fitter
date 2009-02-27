#include <RooAbsRealLValue.h>

#include "RooFit.h"
#include "Riostream.h"
#include "TMath.h"
#include "RooRealVar.h"
#include "RooRandom.h"
#include "TRandom3.h"

#include "RooBsTimeAngle.h"
#include "Efficiency.h"

templateClassImp(RooBsTimeAngle)
;


//_____________________________________________________________________________
template<class TA, class TAI>
RooBsTimeAngle<TA, TAI>::RooBsTimeAngle(const char *name, const char *title,
            RooRealVar& t, RooRealVar& cpsi, RooRealVar& ctheta, RooRealVar& phi, RooRealVar& p,
            RooAbsReal& A02, RooAbsReal& All2, RooAbsReal& Ap2,
            RooRealVar& DG, RooRealVar& tau, RooRealVar& Dm,
            RooRealVar& phi_s, RooRealVar& delta_1, RooRealVar& delta_2,
            const RooResolutionModel& model, TA &ta) :
  RooAbsAnaConvPdf(name,title,model,t),
  _t("_t", "time", this, t),
  _angles(this, cpsi, ctheta, phi, ta, true),
  _p("_p", "p", this, p),
  _A02("A02", "|A_0(0)|^2", this, A02),
  _All2("All2", "|A_#parallell(0)|^2", this, All2),
  _Ap2("All2", "|A_#perpendicular(0)|^2", this, Ap2),
  _DG("DG", "#Delta#Gamma", this, DG),
  _tau("tau", "#tau", this, tau),
  _Dm("Dm", "#Delta m", this, Dm),
  _phi_s("phi_s", "#phi_s", this, phi_s),
  _delta_1("delta_1", "#Delta_1", this, delta_1),
  _delta_2("delta_2", "#Delta_2", this, delta_2),
  _aleatorio(0)
{
  // Constructor
  _basisExpCosh = declareBasis("exp(-@0/@1)*cosh(@0*@2/2)", RooArgList(tau,DG)) ;
  _basisExpSinh = declareBasis("exp(-@0/@1)*sinh(@0*@2/2)", RooArgList(tau,DG)) ;
  _basisExpCos  = declareBasis("exp(-@0/@1)*cos(@0*@2)", RooArgList(tau,Dm)) ;
  _basisExpSin  = declareBasis("exp(-@0/@1)*sin(@0*@2)", RooArgList(tau,Dm)) ;
}

//_____________________________________________________________________________
template<class TA, class TAI>
RooBsTimeAngle<TA, TAI>::RooBsTimeAngle(const RooBsTimeAngle& other, const char* name) :
  RooAbsAnaConvPdf(other,name),
  _t("_t", this, other._t),
  _angles(this, other._angles),
  _p("_p",this, other._p),
  _A02("A02", this, other._A02),
  _All2("All2", this, other._All2),
  _Ap2("Ap2", this, other._Ap2),
  _DG("DG", this, other._DG),
  _tau("tau", this, other._tau),
  _Dm("Dm", this, other._Dm),
  _phi_s("phi_s", this, other._phi_s),
  _delta_1("delta_1", this, other._delta_1),
  _delta_2("delta_2", this, other._delta_2),
  _aleatorio(other._aleatorio),
  _basisExpCosh(other._basisExpCosh),
  _basisExpSinh(other._basisExpSinh),
  _basisExpCos(other._basisExpCos),
  _basisExpSin(other._basisExpSin)
{
}

//_____________________________________________________________________________
template<class TA, class TAI>
RooBsTimeAngle<TA, TAI>::~RooBsTimeAngle()
{
  // Destructor
}

//_____________________________________________________________________________
template<class TA, class TAI>
Double_t RooBsTimeAngle<TA, TAI>::coefficient(Int_t basisIndex) const {
    Double_t val = 0;
    for (int i = 1; i <= 6; i++){
	double old_val = val;
        val += coeficiente(basisIndex, i) * _angles.fe(i);
	if (val > 1000000)
	cout << "Coeficiente " << basisIndex << ' ' << i << ' ' << coeficiente(basisIndex, i)  << ' ' << _angles.fe(i) << endl;
    }

    if (basisIndex == _basisExpCos || basisIndex == _basisExpSin)
        return (2 * _p - 1) * val;
    else
        return val;
}

//_____________________________________________________________________________
template<class TA, class TAI>
inline Double_t RooBsTimeAngle<TA, TAI>::coeficiente( Int_t basisIndex, Int_t fIndex) const
{
  switch (fIndex) {
  case 1:
    if (basisIndex == _basisExpCosh) return  _A02;
    if (basisIndex == _basisExpSinh) return -_A02 * TMath::Cos(_phi_s);
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return  _A02 * TMath::Sin(_phi_s);
    return 0;
  case 2:
    if (basisIndex == _basisExpCosh) return _All2;
    if (basisIndex == _basisExpSinh) return - _All2 * TMath::Cos(_phi_s);
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return _All2 * TMath::Sin(_phi_s);
    return 0;
  case 3:
    if (basisIndex == _basisExpCosh) return  _Ap2;
    if (basisIndex == _basisExpSinh) return  _Ap2* TMath::Cos(_phi_s);
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return -_Ap2* TMath::Sin(_phi_s);
    return 0;
  case 4:
    if (basisIndex == _basisExpCosh) return 0;
    if (basisIndex == _basisExpSinh) return -TMath::Sqrt(_All2)*TMath::Sqrt(_Ap2) * TMath::Cos(_delta_1)*TMath::Sin(_phi_s);
    if (basisIndex == _basisExpCos)  return  TMath::Sqrt(_All2)*TMath::Sqrt(_Ap2) * TMath::Sin(_delta_1);
    if (basisIndex == _basisExpSin)  return -TMath::Sqrt(_All2)*TMath::Sqrt(_Ap2) * TMath::Cos(_delta_1)*TMath::Cos(_phi_s);
    return 0;
  case 5:
    if (basisIndex == _basisExpCosh) return  TMath::Sqrt(_A02)*TMath::Sqrt(_All2)*TMath::Cos(_delta_2-_delta_1);
    if (basisIndex == _basisExpSinh) return -TMath::Sqrt(_A02)*TMath::Sqrt(_All2)*TMath::Cos(_delta_2-_delta_1) * TMath::Cos(_phi_s);
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return  TMath::Sqrt(_A02)*TMath::Sqrt(_All2)*TMath::Cos(_delta_2-_delta_1) * TMath::Sin(_phi_s);
    return 0;
  case 6:
    if (basisIndex == _basisExpCosh) return 0;
    if (basisIndex == _basisExpSinh) return -TMath::Sqrt(_A02)*TMath::Sqrt(_Ap2) * TMath::Cos(_delta_2)*TMath::Sin(_phi_s);
    if (basisIndex == _basisExpCos)  return  TMath::Sqrt(_A02)*TMath::Sqrt(_Ap2) * TMath::Sin(_delta_2);
    if (basisIndex == _basisExpSin)  return -TMath::Sqrt(_A02)*TMath::Sqrt(_Ap2) * TMath::Cos(_delta_2)*TMath::Cos(_phi_s);
    return 0;
  }
  return 0 ;
}

//_____________________________________________________________________________
template<class TA, class TAI>
Int_t RooBsTimeAngle<TA, TAI>::getCoefAnalyticalIntegral(Int_t /*code*/, RooArgSet& allVars, RooArgSet& analVars, const char* /* rangeName*/) const
{
//    cout << "Integrate .." << endl;
//    allVars.Print();
  if (matchArgs(allVars, analVars, _angles.cpsiProxy(),_angles.cthetaProxy(),_angles.phiProxy())) return _angles.getIntegralCode(kTRUE , kTRUE , kTRUE );
  if (matchArgs(allVars, analVars,                     _angles.cthetaProxy(),_angles.phiProxy())) return _angles.getIntegralCode(kFALSE, kTRUE , kTRUE );
  if (matchArgs(allVars, analVars, _angles.cpsiProxy(),                      _angles.phiProxy())) return _angles.getIntegralCode(kTRUE , kFALSE, kTRUE );
  if (matchArgs(allVars, analVars, _angles.cpsiProxy(),_angles.cthetaProxy()                   )) return _angles.getIntegralCode(kTRUE , kTRUE , kFALSE);
  if (matchArgs(allVars, analVars, _angles.cpsiProxy()                                         )) return _angles.getIntegralCode(kTRUE , kFALSE, kFALSE);
  if (matchArgs(allVars, analVars,                     _angles.cthetaProxy()                   )) return _angles.getIntegralCode(kFALSE, kTRUE , kFALSE);
  if (matchArgs(allVars, analVars,                                           _angles.phiProxy())) return _angles.getIntegralCode(kFALSE, kFALSE, kTRUE );

 return 0;
}


//_____________________________________________________________________________
template<class TA, class TAI>
Double_t RooBsTimeAngle<TA, TAI>::coefAnalyticalIntegral(Int_t basisIndex, Int_t code, const char* range) const
{
    Double_t val = 0;
    for (int i = 1; i <= 6; i++) {
        val += coeficiente(basisIndex, i) * _angles.int_fe(i, code, range);
        if (val > 1000000)
        cout << "Coeficiente " << basisIndex << ' ' << i << ' ' << coeficiente(basisIndex, i)  << ' ' << _angles.int_fe(i,code,range) << endl;
}
//    cout << _p << ' ' << val << ' ' << (2*_p-1)*val<<endl;
    if (basisIndex == _basisExpCos || basisIndex == _basisExpSin)
        return (2 * _p - 1) * val;
    else
        return val;
}


//_____________________________________________________________________________
template<class TA, class TAI>
Int_t RooBsTimeAngle<TA, TAI>::getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK) const
{
  if (matchArgs(directVars,generateVars,_t, _angles.phiProxy(), _angles.cthetaProxy(), _angles.cpsiProxy())) return 1;
  if (staticInitOK)
      if( matchArgs(directVars,generateVars,_t) ) return 2;
  return 0;
}


//_____________________________________________________________________________
template<class TA, class TAI>
void RooBsTimeAngle<TA, TAI>::initGenerator(Int_t code)
{
  if ( code == 2 ) {
    _C_Cosh =  coefficient(_basisExpCosh);
    _C_Sinh =  coefficient(_basisExpSinh);
    _C_Cos =  coefficient(_basisExpCos);
    _C_Sin =  coefficient(_basisExpSin);
  }
}

//_____________________________________________________________________________
template<class TA, class TAI>
void RooBsTimeAngle<TA, TAI>::generateEvent(Int_t code) {
    Double_t max = 2.1;
    Double_t DG = TMath::Abs(_DG);
    Double_t tau_H = _tau / (1 - 0.5 * _tau * DG);
    Double_t value = 0;

    while (1) {
        _t = -tau_H * log(RooRandom::uniform(&_aleatorio));
        switch (code) {
            case 1:
                _angles.cpsi(RooRandom::uniform(&_aleatorio) * (_angles.cpsiMax() - _angles.cpsiMin()) + _angles.cpsiMin());
                _angles.ctheta(RooRandom::uniform(&_aleatorio) * (_angles.cthetaMax() - _angles.cthetaMin()) + _angles.cthetaMin());
                _angles.phi(RooRandom::uniform(&_aleatorio) * (_angles.phiMax() - _angles.phiMin()) + _angles.phiMin());

                value = (coefficient(_basisExpCosh) * cosh(_DG * _t / 2)
                        + coefficient(_basisExpSinh) * sinh(_DG * _t / 2)
                        + coefficient(_basisExpCos) * cos(_Dm * _t)
                        + coefficient(_basisExpSin) * sin(_Dm * _t)) * exp(-0.5 * DG * _t);
                break;
            case 2:
                value = (_C_Cosh * cosh(_DG * _t / 2)
                        + _C_Sinh * sinh(_DG * _t / 2)
                        + _C_Cos * cos(_Dm * _t)
                        + _C_Sin * sin(_Dm * _t)) * exp(-0.5 * DG * _t);
                cout << "WW: Using cached values ... " << endl;
                break;
        }

        if (value > max) {
            cout << "EE: Value > max " << value << endl;
            max = value * 1.05;
        }


        Double_t rand = RooRandom::uniform(&_aleatorio) * max;
//        cout << _angles.cpsi() << ' ' << _angles.ctheta() << ' ' << _angles.phi() << endl;
//        cout << rand << ' ' << value << endl;
        if (rand < value)
            break;
    }
}
