#ifndef TRANSANGLES
#define TRANSANGLES

#include "RooRealProxy.h"

class TransAngles{
public:
  TransAngles(RooAbsReal* parent, RooRealVar& cpsi, RooRealVar& ctheta, RooRealVar& phi);
  TransAngles(RooAbsReal* parent, const TransAngles& other, const char* name=0);

  //  virtual TObject* clone(const char* newname) const { return new TransAngles(*this,newname) ; }
  //  virtual ~TransAngles();  

  Double_t cpsi() const {return (Double_t) _cpsi;}
  Double_t ctheta() const {return (Double_t) _ctheta;}
  Double_t phi() const {return (Double_t) _phi;}

  const RooRealProxy& cpsiProxy() const {return  _cpsi;}
  const RooRealProxy& cthetaProxy() const {return  _ctheta;}
  const RooRealProxy& phiProxy() const {return _phi;}

  void cpsi(Double_t cpsi) {_cpsi = cpsi;}
  void ctheta(Double_t ctheta) {_ctheta = ctheta;}
  void phi(Double_t phi) {_phi = phi;}

  Double_t cpsiMax() { return _cpsi.max(); }
  Double_t cthetaMax() {return _ctheta.max();}
  Double_t phiMax() {return _phi.max();}

  Double_t cpsiMin(){ return _cpsi.min(); }
  Double_t cthetaMin(){ return _ctheta.min(); }
  Double_t phiMin(){return _phi.min(); }
  
  
  Double_t f(Int_t i) const;
  Double_t f1() const;
  Double_t f2() const;
  Double_t f3() const;
  Double_t f4() const;
  Double_t f5() const;
  Double_t f6() const;
  
  Double_t integralF(Int_t i, Int_t code, const char* range) const;
  Double_t integralF1(Int_t code, const char* range) const;
  Double_t integralF2(Int_t code, const char* range) const;
  Double_t integralF3(Int_t code, const char* range) const;
  Double_t integralF4(Int_t code, const char* range) const;
  Double_t integralF5(Int_t code, const char* range) const;
  Double_t integralF6(Int_t code, const char* range) const;

  //  Int_t getCoefAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars) const ;
  Int_t getIntegralCode(Bool_t _cpsi, Bool_t _ctheta, Bool_t _phi) const;

private:
  bool integrate_cpsi(Int_t code) const ;
  bool integrate_ctheta(Int_t code) const;
  bool integrate_phi(Int_t code) const;

protected:
  RooRealProxy _cpsi;
  RooRealProxy _ctheta;
  RooRealProxy _phi;

  //  ClassDef(TransAngles,1) // Transversity Angles Functions
};

#endif
