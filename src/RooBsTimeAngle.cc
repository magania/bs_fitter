
#include <RooAbsRealLValue.h>

#include "RooFit.h"

#include "Riostream.h"
#include "TMath.h"
#include "RooRealVar.h"
#include "RooRandom.h"
#include "TRandom3.h"

#include "RooBsTimeAngle.h"

ClassImp(RooBsTimeAngle) 
;


//_____________________________________________________________________________
RooBsTimeAngle::RooBsTimeAngle(const char *name, const char *title, Bool_t isBs,
			       RooRealVar& t, RooRealVar& cpsi, RooRealVar& ctheta,
			       RooRealVar& phi, RooRealVar& A02, RooRealVar& All2,
			       RooRealVar& DG, RooRealVar& tau, RooRealVar& Dm,
			       RooRealVar& phi_s, RooRealVar& delta_1, RooRealVar& delta_2,
			       const RooResolutionModel& model) :
  RooAbsAnaConvPdf(name,title,model,t), 
  _isBs(isBs),
  _t("_t", "time", this, t),
  _angles(this,cpsi, ctheta, phi),
  _A02("A02", "|A_0(0)|^2", this, A02),
  _All2("All2", "|A_#parallell(0)|^2", this, All2),
  _DG("DG", "#Delta#Gamma", this, DG),
  _tau("tau", "#tau", this, tau),
  _Dm("Dm", "#Delta m", this, Dm),
  _phi_s("phi_s", "#phi_s", this, phi_s),
  _delta_1("delta_1", "#Delta_1", this, delta_1),
  _delta_2("delta_2", "#Delta_2", this, delta_2),
  _aleatorio(0)
{
//    std::cout << "RANGE: " << cpsi.getMax() << " " << cpsi.getMin() << std::endl;
  _usePhi = kFALSE;
  // Constructor
  _basisExpCosh = declareBasis("exp(-@0/@1)*cosh(@0*@2/2)", RooArgList(tau,DG)) ;
  _basisExpSinh = declareBasis("exp(-@0/@1)*sinh(@0*@2/2)", RooArgList(tau,DG)) ;
  _basisExpCos  = declareBasis("exp(-@0/@1)*cos(@0*@2)", RooArgList(tau,Dm)) ;
  _basisExpSin  = declareBasis("exp(-@0/@1)*sin(@0*@2)", RooArgList(tau,Dm)) ;
}

//_____________________________________________________________________________
RooBsTimeAngle::RooBsTimeAngle(const char *name, const char *title, Bool_t isBs,
			       RooRealVar& t, RooRealVar& cpsi, RooRealVar& ctheta,
			       RooRealVar& phi, RooRealVar& A02, RooRealVar& All2,
			       RooRealVar& DG, RooRealVar& tau, RooRealVar& Dm,
			       RooRealVar& phi_s, RooRealVar& delta_1, RooRealVar& delta_2,
			       const RooResolutionModel& model,
			       Double_t& phi_1, Double_t& phi_2, Double_t& phi_3, 
			       Double_t& phi_4, Double_t& phi_5, Double_t& phi_6) :
  RooAbsAnaConvPdf(name,title,model,t), 
  _isBs(isBs),
  _t("_t", "time", this, t),
  _angles(this,cpsi, ctheta, phi),
  _A02("A02", "|A_0(0)|^2", this, A02),
  _All2("All2", "|A_#parallell(0)|^2", this, All2),
  _DG("DG", "#Delta#Gamma", this, DG),
  _tau("tau", "#tau", this, tau),
  _Dm("Dm", "#Delta m", this, Dm),
  _phi_s("phi_s", "#phi_s", this, phi_s),
  _delta_1("delta_1", "#Delta_1", this, delta_1),
  _delta_2("delta_2", "#Delta_2", this, delta_2),
  _aleatorio(0),
  _Phi_1(phi_1),
  _Phi_2(phi_2),
  _Phi_3(phi_3),
  _Phi_4(phi_4),
  _Phi_5(phi_5),
  _Phi_6(phi_6)
{
  _usePhi = kTRUE;
  // Constructor
  _basisExpCosh = declareBasis("exp(-@0/@1)*cosh(@0*@2/2)", RooArgList(tau,DG)) ;
  _basisExpSinh = declareBasis("exp(-@0/@1)*sinh(@0*@2/2)", RooArgList(tau,DG)) ;
  _basisExpCos  = declareBasis("exp(-@0/@1)*cos(@0*@2)", RooArgList(tau,Dm)) ;
  _basisExpSin  = declareBasis("exp(-@0/@1)*sin(@0*@2)", RooArgList(tau,Dm)) ;
}

//_____________________________________________________________________________
RooBsTimeAngle::RooBsTimeAngle(const RooBsTimeAngle& other, const char* name) : 
  RooAbsAnaConvPdf(other,name), 
  _isBs(other._isBs),
  _t("_t", this, other._t),
  _angles(this,other._angles),
  _A02("A02", this, other._A02),
  _All2("All2", this, other._All2),
  _DG("DG", this, other._DG),
  _tau("tau", this, other._tau),
  _Dm("Dm", this, other._Dm),
  _phi_s("phi_s", this, other._phi_s),
  _delta_1("delta_1", this, other._delta_1),
  _delta_2("delta_2", this, other._delta_2),
  _aleatorio(other._aleatorio),
  _Phi_1(other._Phi_1),
  _Phi_2(other._Phi_2),
  _Phi_3(other._Phi_3),
  _Phi_4(other._Phi_4),
  _Phi_5(other._Phi_5),
  _Phi_6(other._Phi_6),
  _usePhi(other._usePhi),
  _basisExpCosh(other._basisExpCosh),
  _basisExpSinh(other._basisExpSinh),
  _basisExpCos(other._basisExpCos),
  _basisExpSin(other._basisExpSin)
{
  // Copy constructor
}



//_____________________________________________________________________________
RooBsTimeAngle::~RooBsTimeAngle()
{
  // Destructor
}

//_____________________________________________________________________________
Double_t RooBsTimeAngle::coefficient(Int_t basisIndex) const 
{
  Double_t val =0;
  for (int i=1; i<=6; i++)
    if ( !_isBs && (basisIndex==_basisExpCos || basisIndex==_basisExpSin) )
      val -= coeficiente(basisIndex,i)*_angles.f(i);
    else
      val += coeficiente(basisIndex,i)*_angles.f(i);
  return val;
}

//_____________________________________________________________________________
inline Double_t RooBsTimeAngle::coeficiente( Int_t basisIndex, Int_t fIndex) const
{
  switch (fIndex) {
  case 1:
    if (basisIndex == _basisExpCosh) return  _A02;
    if (basisIndex == _basisExpSinh) return -_A02 * TMath::Abs(cos(_phi_s));
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return  _A02 * sin(_phi_s);
    return 0;
  case 2:
    if (basisIndex == _basisExpCosh) return _All2;
    if (basisIndex == _basisExpSinh) return - _All2 * TMath::Abs(cos(_phi_s));
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return _All2 * sin(_phi_s);
    return 0;
  case 3:
    if (basisIndex == _basisExpCosh) return  (1 - _A02 - _All2);
    if (basisIndex == _basisExpSinh) return  (1 - _A02 - _All2)* TMath::Abs(cos(_phi_s));
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return -(1 - _A02 - _All2)* sin(_phi_s);
    return 0;
  case 4:
    if (basisIndex == _basisExpCosh) return 0;
    if (basisIndex == _basisExpSinh) return -TMath::Sqrt(_All2)*TMath::Sqrt(1-_A02-_All2) * cos(_delta_1)*sin(_phi_s);
    if (basisIndex == _basisExpCos)  return  TMath::Sqrt(_All2)*TMath::Sqrt(1-_A02-_All2) * sin(_delta_1);
    if (basisIndex == _basisExpSin)  return -TMath::Sqrt(_All2)*TMath::Sqrt(1-_A02-_All2) * cos(_delta_1)*cos(_phi_s);
    return 0;
  case 5:
    if (basisIndex == _basisExpCosh) return  TMath::Sqrt(_A02)*TMath::Sqrt(_All2)*cos(_delta_2-_delta_1);
    if (basisIndex == _basisExpSinh) return -TMath::Sqrt(_A02)*TMath::Sqrt(_All2)*cos(_delta_2-_delta_1) * TMath::Abs(cos(_phi_s));
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return  TMath::Sqrt(_A02)*TMath::Sqrt(_All2)*cos(_delta_2-_delta_1) * sin(_phi_s);
    return 0;
  case 6:
    if (basisIndex == _basisExpCosh) return 0;
    if (basisIndex == _basisExpSinh) return -TMath::Sqrt(_A02)*TMath::Sqrt(1-_A02-_All2) * cos(_delta_2)*sin(_phi_s);
    if (basisIndex == _basisExpCos)  return  TMath::Sqrt(_A02)*TMath::Sqrt(1-_A02-_All2) * sin(_delta_2);
    if (basisIndex == _basisExpSin)  return -TMath::Sqrt(_A02)*TMath::Sqrt(1-_A02-_All2) * cos(_delta_2)*cos(_phi_s);
    return 0;
  }
  return 0 ;
}

//_____________________________________________________________________________
Int_t RooBsTimeAngle::getCoefAnalyticalIntegral(Int_t /*code*/, RooArgSet& allVars, RooArgSet& analVars, const char* /* rangeName*/) const 
{
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
Double_t RooBsTimeAngle::coefAnalyticalIntegral(Int_t basisIndex, Int_t code, const char* range) const 
{  
  Double_t f_integral[6];

  if ( _usePhi ){
    f_integral[0] = _Phi_1;
    f_integral[1] = _Phi_2;
    f_integral[2] = _Phi_3;
    f_integral[3] = _Phi_4;
    f_integral[4] = _Phi_5;
    f_integral[5] = _Phi_6;
  } else {
    for (int i=0; i<6 ; i++)
      f_integral[i] = _angles.integralF(i+1, code, range);

//    if (f_integral[5] !=0)
//      cout << "INTEGRAL: " << code << ' ' 
//	 << f_integral[0] << ' '
//	 << f_integral[1] << ' '
//	 << f_integral[2] << ' '
//	 << f_integral[3] << ' '
//	 << f_integral[4] << ' '
//	 << f_integral[5] << endl;
  }

  Double_t val =0;
  for (int i=0; i<6; i++)
    if ( !_isBs && (basisIndex==_basisExpCos || basisIndex==_basisExpSin) )
      val -= coeficiente(basisIndex,i+1)*f_integral[i];
    else
      val += coeficiente(basisIndex,i+1)*f_integral[i];
  return val;
}



//_____________________________________________________________________________
Int_t RooBsTimeAngle::getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK) const
{
  if (matchArgs(directVars,generateVars,_t, _angles.phiProxy(), _angles.cthetaProxy(), _angles.cpsiProxy())) return 1;
  if (staticInitOK)
      if( matchArgs(directVars,generateVars,_t) ) return 2;
  return 0;
}


//_____________________________________________________________________________
void RooBsTimeAngle::initGenerator(Int_t code)
{
  if ( code == 2 ) {
    _C_Cosh =  coefficient(_basisExpCosh);
    _C_Sinh =  coefficient(_basisExpSinh);
    _C_Cos =  coefficient(_basisExpCos);
    _C_Sin =  coefficient(_basisExpSin);
  }
}

//_____________________________________________________________________________
void RooBsTimeAngle::generateEvent(Int_t code)
{
  Double_t max = 2;
  Double_t DG = TMath::Abs(_DG);
  Double_t tau_H = _tau/(1-0.5*_tau*DG);
  Double_t value = 0;

  while (1){
    _t = -tau_H*log(RooRandom::uniform(&_aleatorio));
    switch (code){
    case 1:
      _angles.cpsi( RooRandom::uniform(&_aleatorio) * (_angles.cpsiMax()-_angles.cpsiMin()) + _angles.cpsiMin() );
      _angles.ctheta( RooRandom::uniform(&_aleatorio) * (_angles.cthetaMax()-_angles.cthetaMin()) + _angles.cthetaMin() );
      _angles.phi( RooRandom::uniform(&_aleatorio) * (_angles.phiMax()-_angles.phiMin()) + _angles.phiMin() );
      
      value = (coefficient(_basisExpCosh) * cosh(_DG*_t/2)
	+  coefficient(_basisExpSinh) * sinh(_DG*_t/2)
	+  coefficient(_basisExpCos) * cos(_Dm*_t)
	+  coefficient(_basisExpSin) * sin(_Dm*_t)) * exp(-0.5*DG*_t);
      break;
    case 2:
      value = (_C_Cosh * cosh(_DG*_t/2)
	+  _C_Sinh * sinh(_DG*_t/2)
	+  _C_Cos * cos(_Dm*_t)
	+  _C_Sin * sin(_Dm*_t)) * exp(-0.5*DG*_t);
      cout << "WW: Using cached values ... " << endl;
      break;
    }

    if (value > max){
      cout << "EE: Value > max " << value << endl;
      max = value*1.05;
    }

    Double_t rand = RooRandom::uniform(&_aleatorio)*max;
    if ( rand < value )
      break;
  }
}
