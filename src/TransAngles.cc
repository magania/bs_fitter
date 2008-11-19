#include "TransAngles.h"
#include "RooRealVar.h"
//#include "iostream.h"
//ClassImp(TransAngles) 
//;

TransAngles::TransAngles(RooAbsReal* parent, RooRealVar& cpsi, RooRealVar& ctheta, RooRealVar& phi) :
  _cpsi("_cpsi", "cos(#psi)", parent, cpsi),
  _ctheta("_ctheta", "cos(#theta)", parent, ctheta),
  _phi("_phi", "#phi", parent, phi)
{
}

TransAngles::TransAngles(RooAbsReal* parent, const TransAngles& other, const char* name) :
  _cpsi("_cpsi", parent, other._cpsi),
  _ctheta("_ctheta", parent, other._ctheta),
  _phi("_phi", parent, other._phi)
{
}


bool TransAngles::integrate_cpsi(Int_t code) const {
  return (code == 1 ||            code==3 || code==4 || code==5);
}
bool TransAngles::integrate_ctheta(Int_t code) const {
  return (code == 1 || code==2 ||            code==4 ||          code==6);
}
bool TransAngles::integrate_phi(Int_t code) const {
  return (code == 1 || code==2 || code==3                               || code ==7);
}

Int_t TransAngles::getIntegralCode(Bool_t _cpsi, Bool_t _ctheta, Bool_t _phi) const{
  if ( _cpsi && _ctheta && _phi) return 1;
  if (!_cpsi && _ctheta && _phi) return 2;
  if ( _cpsi &&!_ctheta && _phi) return 3;
  if ( _cpsi && _ctheta &&!_phi) return 4;
  if ( _cpsi &&!_ctheta &&!_phi) return 5;
  if (!_cpsi && _ctheta &&!_phi) return 6;
  if (!_cpsi &&!_ctheta && _phi) return 7;
  return 0;
}

Double_t TransAngles::f(Int_t i) const
{
  switch (i) {
  case 1: return f1();
  case 2: return f2();
  case 3: return f3();
  case 4: return f4();
  case 5: return f5();
  case 6: return f6();
  default: return 0;
  }
}

Double_t TransAngles::integralF(Int_t i, Int_t code, const char* range) const
{
  switch (i) {
  case 1: return integralF1(code, range);
  case 2: return integralF2(code, range);
  case 3: return integralF3(code, range);
  case 4: return integralF4(code, range);
  case 5: return integralF5(code, range);
  case 6: return integralF6(code, range);
  default: return 0;
  }
}

Double_t TransAngles::f1() const {
  double cpsi = _cpsi;
  double ctheta = _ctheta;
  double cphi = cos(_phi);

  double x = 2*cpsi*cpsi*(1-(1-ctheta*ctheta)*cphi*cphi);
  return x;
};

Double_t TransAngles::integralF1(Int_t code,const char* range) const
{
  double p0;
  if (integrate_cpsi(code)) { // integrate
    double hi = _cpsi.max(range);
    double lo = _cpsi.min(range);
    p0 = pow(hi,3)-pow(lo,3); p0/=3;
  } else {                    // multiply by value...
    p0 = _cpsi; p0*=p0;
  }
  double it,ist2;
  if (integrate_ctheta(code)) { // integrate
    double hi = _ctheta.max(range);
    double lo = _ctheta.min(range);
    it = hi-lo;                                 // 1
    double ict2 = pow(hi,3)-pow(lo,3); ict2/=3; // cos^2 theta
    ist2 = it-ict2;                             // sin^2 theta
  } else {                      // value
    it = 1;
    ist2 = 1-_ctheta*_ctheta;
  }
  double ip,icp2;
  if (integrate_phi(code)) {
    ip = _phi.max(range)-_phi.min(range);
    double rs2p = sin(2.*_phi.max(range)) - sin(2.*_phi.min(range));
    icp2 = ip/2+rs2p/4;
  } else {
    ip = 1;
    icp2 = cos(_phi); icp2*=icp2;
  }

  return 2*p0*(it*ip-ist2*icp2);

};

///////////////////////////////////////////////////////////////////////////

Double_t TransAngles::f2() const {
  double cpsi = _cpsi;
  double ctheta = _ctheta;
  double sphi = sin(_phi);
  return (1-cpsi*cpsi)*(1-(1-ctheta*ctheta)*sphi*sphi);
}

Double_t TransAngles::integralF2(Int_t code,const char* range) const
{
  double p1;
  if (integrate_cpsi(code)) {
       double p0 = pow(_cpsi.max(range),3)-pow(_cpsi.min(range),3); p0/=3; // cos^2 psi
       p1 = _cpsi.max(range)-_cpsi.min(range)-p0;                          // sin^2 psi
  } else {
       double p0 = _cpsi; p0*=p0;
       p1 = 1-p0;
  }
  double it,ist2;
  if (integrate_ctheta(code)) {
    double hi = _ctheta.max(range);
    double lo = _ctheta.min(range);
    it = hi-lo;                                      // 1
    double ict2 = pow(hi,3)-pow(lo,3); ict2/=3;      // cos^2 theta
    ist2 = it-ict2;                                  // sin^2 theta
  } else {
    it = 1;
    ist2 = 1-_ctheta*_ctheta;
  }
  double ip,isp2;
  if (integrate_phi(code)) {
      ip = _phi.max(range)-_phi.min(range);
      isp2 = ip/2 - (sin(2.*_phi.max(range)) - sin(2.*_phi.min(range)))/4;
  } else {
      ip = 1;
      isp2 = sin(_phi); isp2*=isp2;
  }

  return p1*(it*ip-ist2*isp2);
};

///////////////////////////////////////////////////////////////////////////

Double_t TransAngles::f3() const {
  double cpsi = _cpsi;
  double ctheta = _ctheta;
  return (1-cpsi*cpsi)*(1-ctheta*ctheta);
};

Double_t TransAngles::integralF3(Int_t code,const char* range) const {
  double p1;
  if (integrate_cpsi(code)) {
    double hi = _cpsi.max(range);
    double lo = _cpsi.min(range);
    double p0 = pow(hi,3)-pow(lo,3); p0/=3;    // cos^2 psi
    p1 = hi-lo-p0;                             // sin^2 psi
  } else {
    double p0 = _cpsi;
    p1 = 1-p0*p0;
  }
  double ist2;
  if (integrate_ctheta(code)) {
    double hi = _ctheta.max(range);
    double lo = _ctheta.min(range);
    double it = hi-lo;                               // 1
    double ict2 = pow(hi,3)-pow(lo,3); ict2/=3;      // cos^2 theta
    ist2 = it-ict2;                                  // sin^2 theta
  } else {
    double ct = _ctheta;
    ist2 = 1-ct*ct;
  }
  double ip;
  if (integrate_phi(code)) {
      ip = _phi.max(range)-_phi.min(range);
  } else {
      ip = 1;
  }

  return p1 * ist2*ip;
};

///////////////////////////////////////////////////////////////////////////

Double_t TransAngles::f4() const {
  double cpsi = _cpsi;
  double ctheta = _ctheta;
  double sphi=sin(_phi);
  return - (1-cpsi*cpsi)*2*ctheta*sqrt(1-ctheta*ctheta)*sphi;//opposite (plus) sign: BaBar convention
};

Double_t TransAngles::integralF4(Int_t code,const char* range) const
{
 double p1;
 if (integrate_cpsi(code)) {
   double hi = _cpsi.max(range);
   double lo = _cpsi.min(range);
   double p0 = pow(hi,3)-pow(lo,3); p0/=3;  // cos^2 psi
   p1 = hi-lo-p0;                           // sin^2 psi
 } else {
   double p0 = _cpsi; p0*=p0;
   p1 = 1-p0;
 }

 double is2t;
 if (integrate_ctheta(code)) {
   double hi = _ctheta.max(range);
   double lo = _ctheta.min(range);
   double x=1-hi*hi; double y=1-lo*lo;
   is2t = -x*sqrt(x)+y*sqrt(y); is2t*=2; is2t/=3;
  } else {
    is2t = 2*_ctheta*sqrt(1-_ctheta*_ctheta);//sin(2theta)
  }

 double isp;
 if (integrate_phi(code)) {
   isp = -cos(_phi.max(range)) +cos(_phi.min(range)) ;
 } else {
   isp = sin(_phi);
 }

 return - p1*is2t*isp;//opposite (plus) sign: BaBar convention
};

///////////////////////////////////////////////////////////////////////////

Double_t TransAngles::f5() const{
  double cpsi = _cpsi;
  double ctheta = _ctheta;
  double s2phi=sin(2*_phi);
  return + 2*cpsi*sqrt(1-cpsi*cpsi)*(1-ctheta*ctheta)*s2phi/sqrt(2.);//opposite (minus) sign: BaBar
};

Double_t TransAngles::integralF5(Int_t code,const char* range) const
{
 double p2;
 if (integrate_cpsi(code)) {
   double hi = _cpsi.max(range);
   double lo = _cpsi.min(range);
   double x=1-hi*hi; double y=1-lo*lo;
   p2 = -x*sqrt(x)+y*sqrt(y); p2*=2; p2/=3;         // sin (2psi)
 } else {
   double p0 = _cpsi; p0*=p0;
   double p1 = 1-p0;
   p2 = 2.*_cpsi*sqrt(p1);
 }

 double ist2;
 if (integrate_ctheta(code)) {
   double hi = _ctheta.max(range);
   double lo = _ctheta.min(range);
   double it = hi-lo;                             // 1
   double ict2 = pow(hi,3)-pow(lo,3); ict2/=3;    // cos^2 theta
   ist2 = it-ict2;                                // sin^2 theta
 } else {
   ist2 = 1-_ctheta*_ctheta;
 }

 double is2p;
 if (integrate_cpsi(code)) {
   double rc2p = cos(2.*_phi.max(range)) - cos(2.*_phi.min());
   is2p = -rc2p/2;
 } else {
   is2p = sin(2*_phi);
 }

 return  + p2 * ist2*is2p /sqrt(2.);//opposite (minus) sign: BaBar convention

};

//////////////////////////////////////////////////////

Double_t TransAngles::f6() const{
  double cpsi = _cpsi;
  double ctheta = _ctheta;
  double cphi=cos(_phi);
  return  2*cpsi  *sqrt(1-cpsi  *cpsi  )
    *2*ctheta*sqrt(1-ctheta*ctheta)*cphi/sqrt(2.);
};

Double_t TransAngles::integralF6(Int_t code,const char* range) const
{
 double p2;
 if (integrate_cpsi(code)) {
   double hi = _cpsi.max(range);
   double lo = _cpsi.min(range);
   double x=1-hi*hi; double y=1-lo*lo;
   p2 = -x*sqrt(x)+y*sqrt(y); p2*=2; p2/=3;         // sin (2psi)
 } else {
   double p0 = _cpsi; p0*=p0;
   double p1 = 1-p0;
   p2 = 2.*_cpsi*sqrt(p1);
 }

 double is2t;
 if (integrate_ctheta(code)) {
   double hi = _ctheta.max(range);
   double lo = _ctheta.min(range);
   double x=1-hi*hi; double y=1-lo*lo;
   is2t = -x*sqrt(x)+y*sqrt(y); is2t*=2; is2t/=3; // sin (2theta)
 } else {
   double ist2 = 1-_ctheta*_ctheta;
   is2t = 2.*_ctheta*sqrt(ist2);
 }

 double icp;
 if (integrate_phi(code)) {
   icp = sin(_phi.max(range)) - sin(_phi.min(range));
 } else {
   icp = cos(_phi);
 }

//  if (code !=1 )
//  cout << "F6 " << code << ' ' << _cpsi.min(range) << ' ' << _cpsi.max(range) << ' '
//       << _ctheta.min(range) << ' ' << _ctheta.max(range) << ' '
//       << _phi.min(range) << ' ' << _phi.max(range) << ' '
//       << p2 * is2t*icp /sqrt(2.) << endl; 
 return p2 * is2t*icp /sqrt(2.);
};
