#ifndef BS_FITTER
#define BS_FITTER

#include "TCanvas.h"
#include "TROOT.h"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooResolutionModel.h"
#include "RooFitResult.h"
#include "RooDataSet.h"
#include "RooGlobalFunc.h"
#include "RooProdPdf.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooGaussModel.h"
#include "RooTruthModel.h"
#include "RooLandau.h"
#include "RooProdPdf.h"
#include "RooChebychev.h"
#include "RooExponential.h"
#include "RooDecay.h"
#include "RooPolynomial.h"
#include "RooGenericPdf.h"
#include "RooHistPdf.h"

#include <RooBsTimeAngle.h>
//#include <RooBkgAngle.h>

class BsFitter {
 public:
  BsFitter(Bool_t signal, Bool_t background,
		  Bool_t resolution, Bool_t error_model, Bool_t tag_model,
		  const char* efficiency_file = 0);

    void setVariables(const char* params);
    void setParameters(const char* params);
    void writeParameters(const char* params);
    void setEfficiency(const char* efficiency);
    void setData(const char* data_file);
    void setData(RooDataSet* data_set);
//    void setDataFromCin();

    void generate(Int_t num, const char* data);
    Int_t fit(Bool_t hesse = kTRUE, Bool_t minos = kFALSE, Bool_t verbose = kFALSE, Int_t cpu = 1);

    void plotM(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
    void plotM();
    void plotT(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
    void plotT();
    void plotEt(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
    void plotEt();
    void plotCpsi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
    void plotCpsi();
    void plotCtheta(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
    void plotCtheta();
    void plotPhi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
    void plotPhi();

    void writeResult(const char* file_name);

private:
    RooAbsPdf* signal_model(Bool_t error_model, Bool_t tag_model);
    RooAbsPdf* background_model(Bool_t error_model, Bool_t tag_model);
    void plotVar(RooRealVar& x, const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);

    RooRealVar _m;
    RooRealVar _t;
    RooRealVar _et;
    RooRealVar _cpsi;
    RooRealVar _ctheta;
    RooRealVar _phi;
    RooRealVar _p;

    RooArgSet* _parameters;
    RooArgSet* _variables;
    RooResolutionModel *_resolution;
    RooAbsPdf* _model;
    Efficiency* _efficiency;
    //RooProdPdf *_prompt;
    //RooProdPdf *_noprompt;
    RooAbsPdf *_signal;
    RooAbsPdf *_background;
    RooAbsPdf *_error_signal;
    RooAbsPdf *_error_background;
    //RooArgSet* _constraints;

    RooDataSet* _data;
    RooFitResult* _fit_result;

//    const char* _range;
};

#endif
