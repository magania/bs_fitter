/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOBKGANGLE
#define ROOBKGANGLE

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "RooRandom.h"
#include "TRandom3.h"

#include "Efficiency.h"
#include "TransAngles.h"
#include "Phis.h"
#include "TransAnglesEfficiency.h"
#include "TransAnglesPhis.h"

template<class TA, class TAI>
class RooBkgAngle : public RooAbsPdf {
public:

//    RooBkgAngle() {
//    };
    RooBkgAngle(const char *name, const char *title,
            RooRealVar &cpsi, RooRealVar &ctheta, RooRealVar &phi,
            RooRealVar& B1, RooRealVar& _B2, RooRealVar& _B3,
            RooRealVar& B4, RooRealVar& _B5, RooRealVar& _B6,
            TA &ta);
/*    RooBkgAngle(const char *name, const char *title,
            RooRealVar &cpsi, RooRealVar &ctheta, RooRealVar &phi,
            RooRealVar& B1, RooRealVar& _B2, RooRealVar& _B3,
            RooRealVar& B4, RooRealVar& _B5, RooRealVar& _B6,
            Phis phis);*/
    RooBkgAngle(const RooBkgAngle& other, const char* name = 0);

    virtual TObject* clone(const char* newname) const {
        return new RooBkgAngle(*this, newname);
    }

    inline virtual ~RooBkgAngle() {
    }

    Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName = 0) const;
    Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

    Int_t getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK = kTRUE) const;

    void initGenerator(Int_t code) {
    }; // optional pre-generation initialization
    void generateEvent(Int_t code);

protected:

    TAI _angles;
    RooRealProxy _B1;
    RooRealProxy _B2;
    RooRealProxy _B3;
    RooRealProxy _B4;
    RooRealProxy _B5;
    RooRealProxy _B6;

    TRandom3 _aleatorio;
    
    Double_t evaluate() const;

private:
   ClassDef(RooBkgAngle, 1) // Your description goes here...
};

#endif