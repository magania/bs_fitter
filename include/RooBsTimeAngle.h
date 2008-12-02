#ifndef ROO_BS_TIME_ANGLE
#define ROO_BS_TIME_ANGLE

#include "TRandom3.h"

#include "RooAbsAnaConvPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "TransAngles.h"
#include "TransAnglesEfficiency.h"
#include "TransAnglesPhis.h"
#include "Efficiency.h"
#include "Phis.h"

template<class TA, class TAI>
class RooBsTimeAngle : public RooAbsAnaConvPdf {
public:
    RooBsTimeAngle(const char *name, const char *title, Bool_t isBs,
            RooRealVar& t, RooRealVar& cpsi, RooRealVar& ctheta, RooRealVar& phi,
            RooRealVar& A02, RooRealVar& All2, RooRealVar& DG, RooRealVar& tau,
            RooRealVar& Dm, RooRealVar& phi_s, RooRealVar& delta_1, RooRealVar& delta_2,
            const RooResolutionModel& model, TA &ta);

    RooBsTimeAngle(const RooBsTimeAngle& other, const char* name = 0);

    virtual TObject* clone(const char* newname) const {
        return new RooBsTimeAngle(*this, newname);
    }
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
    TAI _angles;
    RooRealProxy _A02;
    RooRealProxy _All2;
    RooRealProxy _DG;
    RooRealProxy _tau;
    RooRealProxy _Dm;
    RooRealProxy _phi_s;
    RooRealProxy _delta_1;
    RooRealProxy _delta_2;

    TRandom3 _aleatorio;

    Bool_t _isBs;

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
