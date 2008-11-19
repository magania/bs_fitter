#include "F.h"

//ClassImp(F) 
//;

Double_t F::f(Int_t i, Angles angles)
{
  switch (i) {
  case 1: return f1(angles);
  case 2: return f2(angles);
  case 3: return f3(angles);
  case 4: return f4(angles);
  case 5: return f5(angles);
  case 6: return f6(angles);
  default: 
    std::cout << "F Functions index " << i << " incorrect." << std::endl;
    return 0;
  }
}

Double_t F::integralF(Int_t i, Int_t code, Angles angles, Range range)
{
  switch (i) {
  case 1: return integralF1(code, angles, range);
  case 2: return integralF2(code, angles, range);
  case 3: return integralF3(code, angles, range);
  case 4: return integralF4(code, angles, range);
  case 5: return integralF5(code, angles, range);
  case 6: return integralF6(code, angles, range);
  default: 
    std::cout << "F Functions intergal index " << i << " incorrect." << std::endl;
    return 0;
  }
}

Bool_t F::integrate_cpsi(Int_t code) {
  return (code == 1 ||            code==3 || code==4 || code==5);
}
Bool_t F::integrate_ctheta(Int_t code) {
  return (code == 1 || code==2 ||            code==4 ||          code==6);
}
Bool_t F::integrate_phi(Int_t code) {
  return (code == 1 || code==2 || code==3                               || code ==7);
}

Int_t F::getIntegralCode(Bool_t _cpsi, Bool_t _ctheta, Bool_t _phi){
  if ( _cpsi && _ctheta && _phi) return 1;
  if (!_cpsi && _ctheta && _phi) return 2;
  if ( _cpsi &&!_ctheta && _phi) return 3;
  if ( _cpsi && _ctheta &&!_phi) return 4;
  if ( _cpsi &&!_ctheta &&!_phi) return 5;
  if (!_cpsi && _ctheta &&!_phi) return 6;
  if (!_cpsi &&!_ctheta && _phi) return 7;
  return 0;
}

///////////////////////////// F1 //////////////////////////////////////////
Double_t F::f1(Angles angles) {
  Double_t cphi = TMath::Cos(angles.phi);
  return 2*angles.cpsi*angles.cpsi*(1-(1-angles.ctheta*angles.ctheta)*cphi*cphi);
};

Double_t F::integralF1(Int_t code, Angles angles, Range range)
{
  Double_t p0;
  if (integrate_cpsi(code)) { // integrate
    Double_t hi = range.cpsi.max;
    Double_t lo = range.cpsi.min;
    p0 = TMath::Power(hi,3)-TMath::Power(lo,3); p0/=3;
  } else {                    // multiply by value...
    p0 = angles.cpsi; p0*=p0;
  }
  Double_t it,ist2;
  if (integrate_ctheta(code)) { // integrate
    Double_t hi = range.ctheta.max;
    Double_t lo = range.ctheta.min;
    it = hi-lo;                                 // 1
    Double_t ict2 = TMath::Power(hi,3)-TMath::Power(lo,3); ict2/=3; // cos^2 theta
    ist2 = it-ict2;                             // sin^2 theta
  } else {                      // value
    it = 1;
    ist2 = 1-angles.ctheta*angles.ctheta;
  }
  Double_t ip,icp2;
  if (integrate_phi(code)) {
    ip = range.phi.max-range.phi.min;
    Double_t rs2p = sin(2.*range.phi.max) - sin(2.*range.phi.min);
    icp2 = ip/2+rs2p/4;
  } else {
    ip = 1;
    icp2 = cos(angles.phi); icp2*=icp2;
  }

  return 2*p0*(it*ip-ist2*icp2);

};

//////////////////////////// F2 ///////////////////////////////////////////
Double_t F::f2(Angles angles) {
  Double_t sphi = TMath::Sin(angles.phi);
  return (1-angles.cpsi*angles.cpsi)*(1-(1-angles.ctheta*angles.ctheta)*sphi*sphi);
}

Double_t F::integralF2(Int_t code, Angles angles, Range range)
{
  Double_t p1;
  if (integrate_cpsi(code)) {
       Double_t p0 = TMath::Power(range.cpsi.max,3)-TMath::Power(range.cpsi.min,3); p0/=3; // cos^2 psi
       p1 = range.cpsi.max-range.cpsi.min-p0;                          // sin^2 psi
  } else {
       Double_t p0 = angles.cpsi; p0*=p0;
       p1 = 1-p0;
  }
  Double_t it,ist2;
  if (integrate_ctheta(code)) {
    Double_t hi = range.ctheta.max;
    Double_t lo = range.ctheta.min;
    it = hi-lo;                                      // 1
    Double_t ict2 = TMath::Power(hi,3)-TMath::Power(lo,3); ict2/=3;      // cos^2 theta
    ist2 = it-ict2;                                  // sin^2 theta
  } else {
    it = 1;
    ist2 = 1-angles.ctheta*angles.ctheta;
  }
  Double_t ip,isp2;
  if (integrate_phi(code)) {
      ip = range.phi.max-range.phi.min;
      isp2 = ip/2 - (sin(2.*range.phi.max) - sin(2.*range.phi.min))/4;
  } else {
      ip = 1;
      isp2 = sin(angles.phi); isp2*=isp2;
  }

  return p1*(it*ip-ist2*isp2);
};

//////////////////////////// F3 ///////////////////////////////////////////
Double_t F::f3(Angles angles) {
  return (1-angles.cpsi*angles.cpsi)*(1-angles.ctheta*angles.ctheta);
};

Double_t F::integralF3(Int_t code, Angles angles, Range range) {
  Double_t p1;
  if (integrate_cpsi(code)) {
    Double_t hi = range.cpsi.max;
    Double_t lo = range.cpsi.min;
    Double_t p0 = TMath::Power(hi,3)-TMath::Power(lo,3); p0/=3;    // cos^2 psi
    p1 = hi-lo-p0;                             // sin^2 psi
  } else {
    Double_t p0 = angles.cpsi;
    p1 = 1-p0*p0;
  }
  Double_t ist2;
  if (integrate_ctheta(code)) {
    Double_t hi = range.ctheta.max;
    Double_t lo = range.ctheta.min;
    Double_t it = hi-lo;                               // 1
    Double_t ict2 = TMath::Power(hi,3)-TMath::Power(lo,3); ict2/=3;      // cos^2 theta
    ist2 = it-ict2;                                  // sin^2 theta
  } else {
    Double_t ct = angles.ctheta;
    ist2 = 1-ct*ct;
  }
  Double_t ip;
  if (integrate_phi(code)) {
      ip = range.phi.max-range.phi.min;
  } else {
      ip = 1;
  }

  return p1 * ist2*ip;
};

//////////////////////////// F4 ///////////////////////////////////////////
Double_t F::f4(Angles angles) {
  Double_t sphi=TMath::Sin(angles.phi);
  return - (1-angles.cpsi*angles.cpsi)*2*angles.ctheta*TMath::Sqrt(1-angles.ctheta*angles.ctheta)*sphi;//opposite (plus) sign: BaBar convention
};

Double_t F::integralF4(Int_t code, Angles angles, Range range)
{
 Double_t p1;
 if (integrate_cpsi(code)) {
   Double_t hi = range.cpsi.max;
   Double_t lo = range.cpsi.min;
   Double_t p0 = TMath::Power(hi,3)-TMath::Power(lo,3); p0/=3;  // cos^2 psi
   p1 = hi-lo-p0;                           // sin^2 psi
 } else {
   Double_t p0 = angles.cpsi; p0*=p0;
   p1 = 1-p0;
 }

 Double_t is2t;
 if (integrate_ctheta(code)) {
   Double_t hi = range.ctheta.max;
   Double_t lo = range.ctheta.min;
   Double_t x=1-hi*hi; Double_t y=1-lo*lo;
   is2t = -x*TMath::Sqrt(x)+y*TMath::Sqrt(y); is2t*=2; is2t/=3;
  } else {
    is2t = 2*angles.ctheta*TMath::Sqrt(1-angles.ctheta*angles.ctheta);//sin(2theta)
  }

 Double_t isp;
 if (integrate_phi(code)) {
   isp = -cos(range.phi.max) +cos(range.phi.min) ;
 } else {
   isp = sin(angles.phi);
 }

 return - p1*is2t*isp;//opposite (plus) sign: BaBar convention
};

//////////////////////////// F5 ///////////////////////////////////////////
Double_t F::f5(Angles angles) {
  Double_t s2phi=TMath::Sin(2*angles.phi);
  return + 2*angles.cpsi*TMath::Sqrt(1-angles.cpsi*angles.cpsi)*(1-angles.ctheta*angles.ctheta)*s2phi/TMath::Sqrt(2.);//opposite (minus) sign: BaBar
};

Double_t F::integralF5(Int_t code, Angles angles, Range range)
{
 Double_t p2;
 if (integrate_cpsi(code)) {
   Double_t hi = range.cpsi.max;
   Double_t lo = range.cpsi.min;
   Double_t x=1-hi*hi; Double_t y=1-lo*lo;
   p2 = -x*TMath::Sqrt(x)+y*TMath::Sqrt(y); p2*=2; p2/=3;         // sin (2psi)
 } else {
   Double_t p0 = angles.cpsi; p0*=p0;
   Double_t p1 = 1-p0;
   p2 = 2.*angles.cpsi*TMath::Sqrt(p1);
 }

 Double_t ist2;
 if (integrate_ctheta(code)) {
   Double_t hi = range.ctheta.max;
   Double_t lo = range.ctheta.min;
   Double_t it = hi-lo;                             // 1
   Double_t ict2 = TMath::Power(hi,3)-TMath::Power(lo,3); ict2/=3;    // cos^2 theta
   ist2 = it-ict2;                                // sin^2 theta
 } else {
   ist2 = 1-angles.ctheta*angles.ctheta;
 }

 Double_t is2p;
 if (integrate_cpsi(code)) {
   Double_t rc2p = cos(2.*range.phi.max) - cos(2.*range.phi.min);
   is2p = -rc2p/2;
 } else {
   is2p = sin(2*angles.phi);
 }

 return  + p2 * ist2*is2p /TMath::Sqrt(2.);//opposite (minus) sign: BaBar convention

};

//////////////////////////// F6 //////////////////////////////////////////
Double_t F::f6(Angles angles) {
  Double_t cphi=TMath::Cos(angles.phi);
  return  2*angles.cpsi*TMath::Sqrt(1-angles.cpsi*angles.cpsi)*2*angles.ctheta*TMath::Sqrt(1-angles.ctheta*angles.ctheta)*cphi/TMath::Sqrt(2.);
};

Double_t F::integralF6(Int_t code, Angles angles, Range range)
{
 Double_t p2;
 if (integrate_cpsi(code)) {
   Double_t hi = range.cpsi.max;
   Double_t lo = range.cpsi.min;
   Double_t x=1-hi*hi; Double_t y=1-lo*lo;
   p2 = -x*TMath::Sqrt(x)+y*TMath::Sqrt(y); p2*=2; p2/=3;         // sin (2psi)
 } else {
   Double_t p0 = angles.cpsi; p0*=p0;
   Double_t p1 = 1-p0;
   p2 = 2.*angles.cpsi*TMath::Sqrt(p1);
 }

 Double_t is2t;
 if (integrate_ctheta(code)) {
   Double_t hi = range.ctheta.max;
   Double_t lo = range.ctheta.min;
   Double_t x=1-hi*hi; Double_t y=1-lo*lo;
   is2t = -x*TMath::Sqrt(x)+y*TMath::Sqrt(y); is2t*=2; is2t/=3; // sin (2theta)
 } else {
   Double_t ist2 = 1-angles.ctheta*angles.ctheta;
   is2t = 2.*angles.ctheta*TMath::Sqrt(ist2);
 }

 Double_t icp;
 if (integrate_phi(code)) {
   icp = sin(range.phi.max) - sin(range.phi.min);
 } else {
   icp = cos(angles.phi);
 }

 return p2 * is2t*icp /TMath::Sqrt(2.);
};
