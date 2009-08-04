#ifndef BS_FITTER
#define BS_FITTER

#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "TEntryList.h"

#include "RooSimultaneous.h"

#include <BsSignal.h>
#include <BsBackground.h>
#include <BsResolution.h>
#include <BsEtModel.h>

#include "RooRealVar.h"

class BsFitter{
public:
	BsFitter(const char* root_file, const char * cut);
//    void setParameters(const char* params);
    void writeParameters(const char* params);

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
	void histogram(const char *cut, RooDataHist *signal_hist_et, RooDataHist *bkg_hist_et,
			RooDataHist *signal_hist_I, RooDataHist *bkg_hist_I,
			RooDataHist *signal_hist_P, RooDataHist *bkg_hist_P);

//	double x_u;
    RooRealVar *m, *t, *et, *cpsi, *ctheta, *phi, *p, *bdtI, *bdtP;
//    RooRealVar *mm0, *xs0;
    RooCategory *category;
    RooRealVar *M, *Sigma, *A0, *A1, *DeltaGamma, *Phi_s, *Delta1, *Delta2, *Tau, *DeltaMs;
    RooRealVar *Delta1_mean, *Delta1_sigma, *Delta2_mean, *Delta2_sigma, *DeltaMs_mean, *DeltaMs_sigma;

    RooArgSet *variables;
    RooArgSet *parameters;

    RooDataSet* data;
    RooFitResult* fit_result;
    RooAbsPdf* pdf;

    TString signal_cut, bkg_cut;
};

class BsSignalFitter : public BsFitter{
public:
	BsSignalFitter(const char* name, const char* root_file, const char* cut, const char* parameters_file);
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
	BsSingleFitter(const char* name, const char* name_et, const char* root_file, const char* cut, const char* parameters_file);

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
	BsMultiFitter(const char* root_file, const char* cut, const char* parameters_file);

private:
    BsResolution *resolution;
    BsEtModel *et_sig_0, *et_sig_1, *et_sig_3, *et_bkg_0, *et_bkg_1, *et_bkg_3;
    Efficiency *efficiency_0, *efficiency_1, *efficiency_3;

    BsSignal *signal_0, *signal_1, *signal_3;
    BsBackground *bkg_0, *bkg_1, *bkg_3;

    RooRealVar *xs_0, * xs_1, *xs_3;
    RooAddPdf *model_0, *model_1, *model_3;
    RooSimultaneous *model;
};

#endif
