#ifndef BS_FITTER
#define BS_FITTER

#include "TCanvas.h"
#include "TROOT.h"

#include "RooSimultaneous.h"

#include <BsSignal.h>
#include <BsBackground.h>
#include <BsResolution.h>
#include <BsEtModel.h>

#include "RooRealVar.h"

class BsFitter{
public:
	BsFitter();
    void setVariables(const char* params);
    void setParameters(const char* params);
    void writeParameters(const char* params);
    void setData(const char* data_file);

    Int_t fit(Bool_t hesse = kTRUE, Bool_t minos = kFALSE, Bool_t verbose = kFALSE, Int_t cpu = 1);

    void writeResult(const char* file_name);

    virtual void plotVar(RooRealVar* x, const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log);
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
protected:
	char* glue(const char* a, const char *b);

    RooRealVar *m, *t, *et, *cpsi, *ctheta, *phi, *p;
    RooRealVar *M, *Sigma, *A0, *A1, *DeltaGamma, *Phi_s, *Delta1, *Delta2, *Tau, *DeltaMs;
    RooRealVar *Delta1_mean, *Delta1_sigma, *Delta2_mean, *Delta2_sigma, *DeltaMs_mean, *DeltaMs_sigma;

    RooArgSet *variables;
    RooArgSet *parameters;

    RooDataSet* data;
    RooFitResult* fit_result;
    RooAbsPdf* pdf;
};

class BsSignalFitter : public BsFitter{
public:
	BsSignalFitter(const char* name);
    Int_t fit(Bool_t hesse, Bool_t minos, Bool_t verbose, Int_t cpu);

private:
    void plotVar(RooRealVar* x, const char* plot_file, Int_t bins,Int_t proj_bins, Bool_t log);
    BsResolution *resolution;
    Efficiency *efficiency;
    BsSignal *signal;
    RooAddPdf *model;
};

class BsSingleFitter : public BsFitter{
public:
	BsSingleFitter(const char* name, const char* name_et);

private:
    void plotVar(RooRealVar* x, const char* plot_file, Int_t bins,Int_t proj_bins, Bool_t log);
    BsResolution *resolution;
    BsEtModel *et_sig, *et_bkg;
    Efficiency *efficiency;

    BsSignal *signal;
    BsBackground *bkg;

    RooRealVar *xs;
    RooAddPdf *model;
};

class BsMultiFitter : public BsFitter{
public:
	BsMultiFitter();

private:
    BsResolution *resolution_IIa, *resolution_IIb;
    BsEtModel *et_sig_IIa, *et_sig_IIb, *et_bkg_IIa, *et_bkg_IIb;
    Efficiency *efficiency_v14, *efficiency_v15, *efficiency_v16;

    BsSignal *signal_v14, *signal_v15, *signal_v16;
    BsBackground *bkg_v14, *bkg_v15, *bkg_v16;

    RooRealVar *xs_v14, * xs_v15, *xs_v16;
    RooAddPdf *model_v14, *model_v15, *model_v16;
    RooCategory *category;
    RooSimultaneous *model;
};

#endif
