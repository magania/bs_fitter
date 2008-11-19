#ifndef BS_FITTER
#define BS_FITTER

#include "TCanvas.h"


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

#include "RooBsTimeAngle.h"

class BsFitter {
public:
  BsFitter();
 
  void setVariables(const char* vars);
  void setParameters(const char* params);
  void setData(const char* data_file);
  void setData(RooDataSet* data_set);

  RooDataSet* generate(Int_t num);
  Int_t fit(Bool_t hesse=kTRUE, Bool_t minos=kFALSE, Bool_t verbose=kFALSE, Int_t cpu=1);

  void plotM(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
  void plotM();
  void plotT(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
  void plotT();
  void plotCpsi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
  void plotCpsi();
  void plotCtheta(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
  void plotCtheta();
  void plotPhi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
  void plotPhi();

  void writeResult(const char* file_name);

private:
  RooAbsPdf* signal_model();
  RooAbsPdf* background_model();
  void plotVar(RooRealVar& x, const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);

  RooRealVar m;
  RooRealVar t;
#ifndef RES_TRUE
  RooRealVar et;
#endif
  RooRealVar cpsi;
  RooRealVar ctheta;
  RooRealVar phi;
  RooRealVar d;

  RooArgSet* parameters;
  RooArgSet* variables;
  RooResolutionModel *resolution;
  RooAbsPdf* model;
  RooDataSet* data;
  RooFitResult* fit_result;
};

#endif
