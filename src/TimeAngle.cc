#include "TimeAngle.h"

//ClassImp(TimeAngle) 
//;

TimeAngle::TimeAngle(){
 _basisExpCosh = 1;
 _basisExpSinh = 2;
 _basisExpCos  = 3;
 _basisExpSin  = 4;

Range r;
r.cpsi.min =-1;
r.cpsi.max =1;
r.ctheta.min =-1;
r.ctheta.max =1;
r.phi.min = -TMath::Pi();
r.phi.max = TMath::Pi();

Angles a;
Int_t code = F::getIntegralCode(kTRUE, kTRUE, kTRUE);

iF[0] = F::integralF1(code, a, r);
iF[1] = F::integralF2(code, a, r);
iF[2] = F::integralF3(code, a, r);
iF[3] = F::integralF4(code, a, r);
iF[4] = F::integralF5(code, a, r);
iF[5] = F::integralF6(code, a, r);

 std::cout << "Integrals "
      << iF[0] << ' ' << iF[1] << ' ' << iF[2] << ' '
	   << iF[3] << ' ' << iF[4] << ' ' << iF[5] << std::endl;
}

Double_t TimeAngle::eval(Double_t _t, Angles a, Parameters p, Bool_t isBs){
  Double_t value = coefficient(_basisExpCosh,a,p,isBs) * TMath::Exp(-_t/p.tau) * TMath::CosH(_t*p.DG/2)
    +  coefficient(_basisExpSinh,a,p,isBs) * TMath::Exp(-_t/p.tau) * TMath::SinH(_t*p.DG/2)
    +  coefficient(_basisExpCos,a,p,isBs) * TMath::Exp(-_t/p.tau) * TMath::Cos(_t*p.Dm)
    +  coefficient(_basisExpSin,a,p,isBs) * TMath::Exp(-_t/p.tau) * TMath::Sin(_t*p.Dm);
		    
  Double_t ivalue = icoefficient(_basisExpCosh,a,p,isBs) * (-4*p.tau)/(p.DG*p.DG*p.tau*p.tau-4)
    +  icoefficient(_basisExpSinh,a,p,isBs) * (-2*p.tau*p.DG)/(p.DG*p.DG*p.tau*p.tau-4)
    +  icoefficient(_basisExpCos,a,p,isBs) * p.tau/(p.Dm*p.Dm*p.tau*p.tau+1)
    +  icoefficient(_basisExpSin,a,p,isBs) * (p.tau*p.tau*p.Dm)/(p.Dm*p.Dm*p.tau*p.tau+1);

  return value/ivalue;
}

//_____________________________________________________________________________
Double_t TimeAngle::coefficient(Int_t basisIndex, Angles a, Parameters p, Bool_t _isBs)
{
  Double_t val =0;
  for (int i=1; i<=6; i++)
    if ( !_isBs && (basisIndex==_basisExpCos || basisIndex==_basisExpSin) )
      val -= coeficiente(basisIndex,i,p)*F::f(i,a);
    else
      val += coeficiente(basisIndex,i,p)*F::f(i,a);
  return val;
}

//_____________________________________________________________________________
Double_t TimeAngle::icoefficient(Int_t basisIndex, Angles a, Parameters p, Bool_t _isBs)
{
  Double_t val =0;
  for (int i=1; i<=6; i++)
    if ( !_isBs && (basisIndex==_basisExpCos || basisIndex==_basisExpSin) )
      val -= coeficiente(basisIndex,i,p)*iF[i-1];
    else
      val += coeficiente(basisIndex,i,p)*iF[i-1];
  return val;
}

//_____________________________________________________________________________
inline Double_t TimeAngle::coeficiente( Int_t basisIndex, Int_t fIndex, Parameters p)
{
  switch (fIndex) {
  case 1:
    if (basisIndex == _basisExpCosh) return  p.A02;
    if (basisIndex == _basisExpSinh) return -p.A02 * TMath::Abs(TMath::Cos(p.phi_s));
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return  p.A02 * TMath::Sin(p.phi_s);
    return 0;
  case 2:
    if (basisIndex == _basisExpCosh) return p.All2;
    if (basisIndex == _basisExpSinh) return - p.All2 * TMath::Abs(TMath::Cos(p.phi_s));
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return p.All2 * TMath::Sin(p.phi_s);
    return 0;
  case 3:
    if (basisIndex == _basisExpCosh) return  (1 - p.A02 - p.All2);
    if (basisIndex == _basisExpSinh) return  (1 - p.A02 - p.All2)* TMath::Abs(TMath::Cos(p.phi_s));
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return -(1 - p.A02 - p.All2)* TMath::Sin(p.phi_s);
    return 0;
  case 4:
    if (basisIndex == _basisExpCosh) return 0;
    if (basisIndex == _basisExpSinh) return -TMath::Sqrt(p.All2)*TMath::Sqrt(1-p.A02-p.All2) * TMath::Cos(p.delta_1)*TMath::Sin(p.phi_s);
    if (basisIndex == _basisExpCos)  return  TMath::Sqrt(p.All2)*TMath::Sqrt(1-p.A02-p.All2) * TMath::Sin(p.delta_1);
    if (basisIndex == _basisExpSin)  return -TMath::Sqrt(p.All2)*TMath::Sqrt(1-p.A02-p.All2) * TMath::Cos(p.delta_1)*TMath::Cos(p.phi_s);
    return 0;
  case 5:
    if (basisIndex == _basisExpCosh) return  TMath::Sqrt(p.A02)*TMath::Sqrt(p.All2)*TMath::Cos(p.delta_2-p.delta_1);
    if (basisIndex == _basisExpSinh) return -TMath::Sqrt(p.A02)*TMath::Sqrt(p.All2)*TMath::Cos(p.delta_2-p.delta_1) * TMath::Abs(TMath::Cos(p.phi_s));
    if (basisIndex == _basisExpCos)  return 0;
    if (basisIndex == _basisExpSin)  return  TMath::Sqrt(p.A02)*TMath::Sqrt(p.All2)*TMath::Cos(p.delta_2-p.delta_1) * TMath::Sin(p.phi_s);
    return 0;
  case 6:
    if (basisIndex == _basisExpCosh) return 0;
    if (basisIndex == _basisExpSinh) return -TMath::Sqrt(p.A02)*TMath::Sqrt(1-p.A02-p.All2) * TMath::Cos(p.delta_2)*TMath::Sin(p.phi_s);
    if (basisIndex == _basisExpCos)  return  TMath::Sqrt(p.A02)*TMath::Sqrt(1-p.A02-p.All2) * TMath::Sin(p.delta_2);
    if (basisIndex == _basisExpSin)  return -TMath::Sqrt(p.A02)*TMath::Sqrt(1-p.A02-p.All2) * TMath::Cos(p.delta_2)*TMath::Cos(p.phi_s);
    return 0;
  }
  return 0 ;
}
