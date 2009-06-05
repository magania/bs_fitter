#include <iostream>
#include <sstream>
#include <string>

#include "RooGlobalFunc.h"
#include "TTree.h"
#include "TH1D.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TMath.h"
#include "RooDataSet.h"
#include "RooPlot.h"

//#include <root/RooDataSet.h>
#include <BsEtModel.h>
#include <BsPdf.h>
//#include <../src/BsEtModel.cc>


char* glue(const char* a, const char *b){
	char *tmp = (char*) malloc(strlen(a)+strlen(b)+2);
	strcpy(tmp, a);
	strcat(tmp, "_");
	strcat(tmp, b);
	return tmp;
}

void fit_et_sample(const char* name) {
    gROOT->SetStyle("Plain");

    stringstream ss_mass_png;
    stringstream ss_bkg_png;
    stringstream ss_sig_png;
    const char *data_file = glue("data",name);

    RooRealVar m("m", "m", 5.0, 5.8);
    RooRealVar t("t", "t", 0, -2, 12);
    RooRealVar et("et", "et", 0.01, 0, 1);
    RooRealVar cpsi("cpsi", "cos(#psi)", 0, -1, 1);
    RooRealVar ctheta("ctheta", "cos(#theta)", 0, -1, 1);
    RooRealVar phi("phi", "#phi", 0, -TMath::Pi(), TMath::Pi());
    RooRealVar p("p", "bs probability", 0, -1, 1);

    RooArgSet variables(m, t, et, cpsi, ctheta, phi, p);

    TTree tree("tree", "tree");
    tree.ReadFile(data_file, "m/D:t:et:cpsi:ctheta:phi:p");
    RooDataSet data("data", "data", &tree, variables);

    RooRealVar mean("mean", "mean", 5.37, 5.28, 5.44);
    RooRealVar sigma("sigma", "sigma", 0.038, 0.01, 0.1);
    RooRealVar mm("mm", "mm", 0, -1, 1);
    RooRealVar xs("xs", "xs", 0.2, 0.01, 1);
 
    RooGaussian gauss("gauss", "gauss", m, mean, sigma);
    RooPolynomial line("line", "line", m, mm);
    RooAddPdf model("model", "model", gauss, line, xs);
    model.fitTo(data/*, RooFit::Hesse(false), RooFit::Minos(false)*/);

    RooPlot *m_frame = m.frame();
    data.plotOn(m_frame);
    model.plotOn(m_frame, RooFit::LineColor(13));
    TCanvas *canvas_m = new TCanvas("canvas_m", "canvas mass", 600, 600);
    m_frame->Draw();
    ss_mass_png << "mass_" << name << ".png";
    canvas_m->Print(ss_mass_png.str().c_str());

	Double_t a = m.getMin();
	Double_t b = mean.getVal() - 3 * sigma.getVal();
	Double_t c = mean.getVal() + 3 * sigma.getVal();
	Double_t d = m.getMax();

	Double_t all = (d-a) + mm.getVal()*(d-a)*(d-a)/2;
	Double_t under = (c-b) + mm.getVal()*(c-b)*(c-b)/2;
	Double_t u = (1-xs.getVal())*all/under;
	Double_t x_u = xs.getVal()/(xs.getVal() + u);

	TString bkg_cut = "m < ";
	bkg_cut += b;
	bkg_cut += " || m > ";
	bkg_cut += c;
	TString signal_cut = "m > ";
	signal_cut += b;
	signal_cut += " && m < ";
	signal_cut += c;

	cout << "Bkg Cut: " << bkg_cut << endl;
	cout << "Signal Cut: " << signal_cut << endl;

	RooDataSet signal_data("signal_data", "signal_data", &tree, variables, signal_cut);
	RooDataSet bkg_data("bkg_data", "bkg_data", &tree, variables, bkg_cut);

    BsEtModel et_bkg(glue("bkg",name), &et);
    et_bkg.getParameters()->readFromFile("initial_parameters_et.txt");
    et_bkg.pdf()->fitTo(bkg_data/*,RooFit::NumCPU(4),RooFit::Minos(false),RooFit::Hesse(true)*/);

    RooPlot *et_frame = et.frame();
    bkg_data.plotOn(et_frame);
//    et_bkg_pdf().plotOn(et_frame, RooFit::Components(et_bkg_short), RooFit::LineColor(kRed));
//    et_bkg_pdf().plotOn(et_frame, RooFit::Components(et_bkg_long), RooFit::LineColor(kBlue));
    et_bkg.pdf()->plotOn(et_frame, RooFit::LineColor(13));
    TCanvas *canvassb = new TCanvas("canvassb", "canvas sidebands", 600, 600);
    gPad->SetLogy(kTRUE);
    et_frame->Draw();
    ss_bkg_png << "et_bkg_" << name << ".png";
    canvassb->Print(ss_bkg_png.str().c_str());

    TCanvas *canvasres = new TCanvas("canvares", "canvas sidebands", 600, 600);
    tree.Draw("et>>sb(100,0,1)", bkg_cut);
    tree.Draw("et>>sg(100,0,1)", signal_cut);
    TH1D *hsg = (TH1D*) gROOT->Get("sg");
    TH1D *hsb = (TH1D*) gROOT->Get("sb");
    hsg->Sumw2();
    hsg->Add(hsb, -x_u);
    hsg->Draw();
    RooDataHist rhsg("rhsg", "signal", et, hsg);

    BsEtModel et_sig(glue("sig",name), &et);
    et_sig.getParameters()->readFromFile("initial_parameters_et.txt");
    et_sig.pdf()->fitTo(rhsg,RooFit::Minos(false),RooFit::Hesse(true));

    TCanvas *canvas2 = new TCanvas("canvas2", "canvas signal", 600, 600);
    RooPlot *et_frame2 = et.frame();
    rhsg.plotOn(et_frame2);
//    et_sig_model.plotOn(et_frame2, RooFit::Components(et_sig_short), RooFit::LineColor(kRed));
//    et_sig_model.plotOn(et_frame2, RooFit::Components(et_sig_long), RooFit::LineColor(kBlue));
    et_sig.pdf()->plotOn(et_frame2, RooFit::LineColor(13));
    gPad->SetLogy(kTRUE);
    et_frame2->Draw();
    ss_sig_png << "et_sig_" << name << ".png";
    canvas2->Print(ss_sig_png.str().c_str());

    et_sig.setConstant();
    et_bkg.setConstant();
    RooArgSet parameters;
    parameters.add(*et_sig.getParameters());
    parameters.add(*et_bkg.getParameters());
    parameters.writeToFile(glue("et_parameters",name));
}


void fit_et(){
  fit_et_sample("IIa");
  fit_et_sample("IIb");
}
