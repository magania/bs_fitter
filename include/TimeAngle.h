#ifndef TIMEANGLE
#define TIMEANGLE

#include "TObject.h"
#include "F.h"
#include <iostream>

class Parameters{
public:
  Double_t A02;
  Double_t All2;
  Double_t DG;
  Double_t tau;
  Double_t Dm;
  Double_t phi_s;
  Double_t delta_1;
  Double_t delta_2;
};

class TimeAngle{
public:  
  TimeAngle();
  Double_t eval(Double_t _t, Angles a, Parameters p, Bool_t isBs);

private:
  Double_t iF[6];
  Double_t _basisExpCosh;
  Double_t _basisExpSinh;
  Double_t _basisExpCos;
  Double_t _basisExpSin;

protected:
  Double_t coefficient(Int_t basisIndex, Angles a, Parameters p, Bool_t _isBs);
  Double_t icoefficient(Int_t basisIndex, Angles a, Parameters p, Bool_t _isBs);
  inline Double_t coeficiente( Int_t basisIndex, Int_t fIndex, Parameters params);
  
  //  ClassDef(TimeAngle,1) // B0s Time and Angular decay PDF
};

#endif
