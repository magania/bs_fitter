#ifndef ROO_BS_TIME_ANGLE
#define ROO_BS_TIME_ANGLE

#include "TRandom3.h"

#include "RooAbsAnaConvPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "TransAngles.h"
#include "TransAnglesEfficiency.h"
#include "Efficiency.h"

class RooBsTimeAngle : public RooAbsAnaConvPdf {
public:
    RooBsTimeAngle(const char *name, const char *title,
            RooRealVar& t,
            RooRealVar& cpsi,
            RooRealVar& ctheta,
            RooRealVar& phi,
            RooAbsReal& p,
            RooAbsReal& A02,
            RooAbsReal& All2,
            RooAbsReal& Ap2,
            RooRealVar& DG,
            RooRealVar& tau,
            RooRealVar& Dm,
            RooAbsReal& sinphi,
            RooAbsReal& cosphi,
            RooAbsReal& sindelta1,
            RooAbsReal& cosdelta1,
            RooAbsReal& sindelta2,
            RooAbsReal& cosdelta2,
            const RooResolutionModel& model,
            Efficiency &efficiency);

    RooBsTimeAngle(const RooBsTimeAngle& other, const char* name = 0);

    virtual TObject* clone(const char* newname) const ;
    virtual ~RooBsTimeAngle();

    virtual Double_t coefficient(Int_t basisIndex) const;

    virtual Int_t getCoefAnalyticalIntegral(Int_t coef, RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
    virtual Double_t coefAnalyticalIntegral(Int_t coef, Int_t code, const char* rangeName = 0) const;

    Int_t getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK = kTRUE) const;
    void initGenerator(Int_t code);
    void generateEvent(Int_t code);

private:
    inline Double_t coeficiente(Int_t basisIndex, Int_t fIndex) const;

protected:
    RooRealProxy _t;
    TransAnglesEfficiency _angles;
    RooRealProxy _p;
    RooRealProxy _A02;
    RooRealProxy _All2;
    RooRealProxy _Ap2;
    RooRealProxy _DG;
    RooRealProxy _tau;
    RooRealProxy _Dm;
    RooRealProxy _sinphi;
    RooRealProxy _cosphi;
    RooRealProxy _sindelta1;
    RooRealProxy _cosdelta1;
    RooRealProxy _sindelta2;
    RooRealProxy _cosdelta2;

    TRandom3 _aleatorio;

    Int_t _basisExpCosh;
    Int_t _basisExpSinh;
    Int_t _basisExpCos;
    Int_t _basisExpSin;

    Double_t _C_Cosh;
    Double_t _C_Sinh;
    Double_t _C_Cos;
    Double_t _C_Sin;

//    Double_t tmp;

    ClassDef(RooBsTimeAngle, 1) // B0s Time and Angular decay PDF
};

#endif
