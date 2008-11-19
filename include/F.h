#ifndef FHH
#define FHH

#include "TObject.h"
#include "TMath.h"
#include "TMathBase.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>

class Angles{
public:
  Double_t cpsi;
  Double_t ctheta;
  Double_t phi;
};

class Interval {
public:
  Double_t max;
  Double_t min;
};

class Range{
public:
  Interval cpsi;
  Interval ctheta;
  Interval phi;
};

class F{
public:
  static Double_t f(Int_t i, Angles angles);
  static Double_t f1(Angles angles) ;
  static Double_t f2(Angles angles) ;
  static Double_t f3(Angles angles) ;
  static Double_t f4(Angles angles) ;
  static Double_t f5(Angles angles) ;
  static Double_t f6(Angles angles) ;
  
  static Double_t integralF(Int_t i, Int_t code, Angles angles, Range range) ;
  static Double_t integralF1(Int_t code, Angles angles, Range range) ;
  static Double_t integralF2(Int_t code, Angles angles, Range range) ;
  static Double_t integralF3(Int_t code, Angles angles, Range range) ;
  static Double_t integralF4(Int_t code, Angles angles, Range range) ;
  static Double_t integralF5(Int_t code, Angles angles, Range range) ;
  static Double_t integralF6(Int_t code, Angles angles, Range range) ;

  static Int_t getIntegralCode(Bool_t cpsi, Bool_t ctheta, Bool_t phi) ;

  static Bool_t integrate_cpsi(Int_t code)  ;
  static Bool_t integrate_ctheta(Int_t code) ;
  static Bool_t integrate_phi(Int_t code) ;

protected:
  //  ClassDef(F,1) // B0s Time and Angular decay PDF

};

#endif
