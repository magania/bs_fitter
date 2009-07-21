#include <RooAddPdf.h>

#include <RooDataSet.h>
#include <RooAbsRealLValue.h>
#include <RooAbsRealLValue.h>
#include <RooDataSet.h>
#include <RooAddModel.h>

#include <TRandom3.h>
#include <RooDataSet.h>

#include <RooGlobalFunc.h>
#include <RooRealVar.h>

#include "BsFitter.h"
#include "Efficiency.h"

#include "TROOT.h"
#include "TPluginManager.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TDirectory.h"

BsFitter::BsFitter(){
	gROOT->SetStyle("Plain");

	/* magic line from Rene - for future reference! */
	gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo",
			"*",
			"TStreamerInfo",
			"RIO",
			"TStreamerInfo()");

	/* variables */
	variables = new RooArgSet();
	m = new RooRealVar("m", "m", 0, 5, 5.8);
	t = new RooRealVar("t", "t", 0,-2, 12);
	et = new RooRealVar("et", "et", 0, 0, 2);
	cpsi = new RooRealVar("cpsi", "cos(#psi)", 0);
	ctheta = new RooRealVar("ctheta", "cos(#theta)", 0);
	phi = new RooRealVar("phi", "#phi", 0);
	p = new RooRealVar("p", "bs probability", 0);

	variables->add(*m);
	variables->add(*t);
	variables->add(*et);
	variables->add(*cpsi);
	variables->add(*ctheta);
	variables->add(*phi);
	variables->add(*p);

	/* Parameters */
	parameters = new RooArgSet();
	M = new RooRealVar ("M", "M", 0);
	Sigma = new RooRealVar ("Sigma", "#sigma", 0);
	A0 = new RooRealVar ("A0", "A0", 0);
	A1 = new RooRealVar ("A1", "A1", 0);
	DeltaGamma = new RooRealVar ("DeltaGamma", "#Delta#Gamma", 0);
	Phi_s = new RooRealVar ("Phi_s", "#Phi_{s}", 0);
	//CosPhi = new RooRealVar ("CosPhi", "cos(#Phi_{s})", 0);
	Delta1 = new RooRealVar ("Delta1", "#delta_{1}", 0);
	Delta2 = new RooRealVar ("Delta2", "#delta_{2}", 0);
	Tau = new RooRealVar ("Tau", "#tau", 0);
	DeltaMs = new RooRealVar("DeltaMs", "#Delta M_{s}", 0);
	/* Constraint parameters */
	Delta1_mean = new RooRealVar("Delta1_mean", "#delta_1 mean", 0);
	Delta1_sigma = new RooRealVar("Delta1_sigma", "#delta_1 sigma", 0);
	Delta2_mean = new RooRealVar("Delta2_mean", "#delta_2 mean", 0);
	Delta2_sigma = new RooRealVar("Delta2_sigma", "#delta_2 sigma", 0);
	DeltaMs_mean = new RooRealVar("DeltaMs_mean", "#delta_2 mean", 0);
	DeltaMs_sigma = new RooRealVar("DeltaMs_sigma", "#delta_2 sigma", 0);

	parameters->add(*M);
	parameters->add(*Sigma);
	parameters->add(*A0);
	parameters->add(*A1);
	parameters->add(*DeltaGamma);
	parameters->add(*Phi_s);
	parameters->add(*Delta1);
	parameters->add(*Delta2);
	parameters->add(*Tau);
	parameters->add(*DeltaMs);

	parameters->add(*Delta1_mean);
	parameters->add(*Delta1_sigma);
	parameters->add(*Delta2_mean);
	parameters->add(*Delta2_sigma);
	parameters->add(*DeltaMs_mean);
	parameters->add(*DeltaMs_sigma);
}

void BsFitter::plotVar(RooRealVar* x, const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
	std::cout << "BsFitter::plotVar" << std::endl;
}

void BsFitter::plotM(const char* plot_file, Int_t bins, Int_t proj_bins,Bool_t log) {
plotVar(m, plot_file, bins, proj_bins, log);
}

void BsFitter::plotM() {
plotVar(m, "mass.gif", 0, 100, kFALSE);
}

void BsFitter::plotT(const char* plot_file, Int_t bins, Int_t proj_bins,Bool_t log) {
plotVar(t, plot_file, bins, proj_bins, log);
}

void BsFitter::plotEt() {
plotVar(et, "time_error.gif", 0, 100, kTRUE);
}

void BsFitter::plotEt(const char* plot_file, Int_t bins, Int_t proj_bins,Bool_t log) {
plotVar(et, plot_file, bins, proj_bins, log);
}

void BsFitter::plotT() {
plotVar(t, "time.gif", 0, 100, kTRUE);
}

void BsFitter::plotCpsi(const char* plot_file, Int_t bins, Int_t proj_bins,Bool_t log) {
plotVar(cpsi, plot_file, bins, proj_bins, log);
}

void BsFitter::plotCpsi() {
plotVar(cpsi, "cpsi.gif", 0, 100, kFALSE);
}

void BsFitter::plotCtheta(const char* plot_file, Int_t bins, Int_t proj_bins,
Bool_t log) {
plotVar(ctheta, plot_file, bins, proj_bins, log);
}

void BsFitter::plotCtheta() {
plotVar(ctheta, "ctheta.gif", 0, 100, kFALSE);
}

void BsFitter::plotPhi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
plotVar(phi, plot_file, bins, proj_bins, log);
}

void BsFitter::plotPhi() {
plotVar(phi, "phi.gif", 0, 100, kFALSE);
}

BsSignalFitter::BsSignalFitter(const char* name){
	/* PDF's */
	resolution = new BsResolution(name, t, et);
	parameters->add(*resolution->getParameters());

	efficiency = new Efficiency(glue("efficiency",name));

	signal = new BsSignal (name, m, t, cpsi, ctheta, phi, p,
			M, Sigma, A0, A1, DeltaGamma, Phi_s, Delta1, Delta2, Tau, DeltaMs,
			Delta1_mean, Delta1_sigma, Delta2_mean, Delta2_sigma, DeltaMs_mean, DeltaMs_sigma,
			resolution, 0, efficiency);

	pdf = signal->pdf();
}

Int_t BsSignalFitter::fit(Bool_t hesse, Bool_t minos, Bool_t verbose, Int_t cpu) {
        if (!data) {
                cout << "EE: No Data" << endl;
                return kFALSE;
        }

        fit_result = pdf->fitTo(*data,
                                        RooFit::ConditionalObservables(RooArgSet(*p,*et)),
                                        RooFit::Save(kTRUE),
                                        RooFit::Hesse(hesse),
                                        RooFit::Minos(minos),
                                        RooFit::NumCPU(cpu),
                                        RooFit::Verbose(verbose)/*, RooFit::Strategy(2)*/);
        return fit_result->status();
}

void BsSignalFitter::plotVar(RooRealVar* x, const char* plot_file, Int_t bins,Int_t proj_bins, Bool_t log) {
	RooPlot *x_frame = x->frame();
	if (data)
		if (bins)
			data->plotOn(x_frame, RooFit::Binning(bins));
		else
			data->plotOn(x_frame);

	if (data) {
		RooAbsData *projData;
		if (proj_bins) {
			et->setBins(proj_bins);
			projData = new RooDataHist("projData", "projData",RooArgSet(*et, *p), *data);
		} else {
			projData = data;
		}

//		model->plotOn(x_frame,
	//	RooFit::LineColor(13));
		pdf->Print();
		pdf->plotOn(x_frame,
				RooFit::ProjWData(RooArgSet(*et), *projData),
				RooFit::LineColor(kGreen));
	} else {
		model->plotOn(x_frame);
	}

	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 800);
	x_frame->Draw();
	if (log)
		gPad->SetLogy(1);
	canvas->Print(plot_file);
}

BsSingleFitter::BsSingleFitter(const char* name, const char* name_et){
	/* PDF's */
	resolution = new BsResolution(name, t, et);
	parameters->add(*resolution->getParameters());

	et_sig = new BsEtModel(glue("sig",name_et), et);
	et_bkg = new BsEtModel(glue("bkg",name_et), et);
	parameters->add(*et_sig->getParameters());
	parameters->add(*et_bkg->getParameters());

	efficiency = new Efficiency(glue("efficiency",name));

	signal = new BsSignal (name, m, t, cpsi, ctheta, phi, p,
			M, Sigma, A0, A1, DeltaGamma, Phi_s, Delta1, Delta2, Tau, DeltaMs,
			Delta1_mean, Delta1_sigma, Delta2_mean, Delta2_sigma, DeltaMs_mean, DeltaMs_sigma,
			resolution, et_sig, efficiency);

	bkg = new BsBackground(name, m, t, cpsi, ctheta, phi, p, resolution, et_bkg);
	parameters->add(*bkg->getParameters());

	xs = new RooRealVar (glue("xs",name), glue("x_s",name), 0);
	parameters->add(*xs);

	model = new RooAddPdf (glue("model",name), glue("model",name), *signal->pdf(), *bkg->pdf(), *xs);
	pdf = model;
}

void BsSingleFitter::plotVar(RooRealVar* x, const char* plot_file, Int_t bins,Int_t proj_bins, Bool_t log) {
	RooPlot *x_frame = x->frame();
	if (data)
		if (bins)
			data->plotOn(x_frame, RooFit::Binning(bins));
		else
			data->plotOn(x_frame);

	if (data) {
		RooAbsData *projData;
		if (proj_bins) {
			et->setBins(proj_bins);
			projData = new RooDataHist("projData", "projData",RooArgSet(*et, *p), *data);
		} else {
			projData = data;
		}

//		model->plotOn(x_frame,
	//	RooFit::LineColor(13));
		pdf->Print();
		pdf->plotOn(x_frame, RooFit::Components(*signal->pdf()),
				RooFit::ProjWData(RooArgSet(*et), *projData), RooFit::Normalization(xs->getVal()),
				RooFit::LineColor(kGreen));
                bkg->pdf()->plotOn(x_frame,
                                RooFit::ProjWData(RooArgSet(*et), *projData),  RooFit::Normalization((1-xs->getVal())),
                                RooFit::LineColor(kRed));
                pdf->plotOn(x_frame,
                                RooFit::ProjWData(RooArgSet(*et), *projData),
                                RooFit::LineColor(13));
	} else {
		pdf->plotOn(x_frame);
	}

	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 800);
	x_frame->Draw();
	if (log)
		gPad->SetLogy(1);
	canvas->Print(plot_file);
}

BsMultiFitter::BsMultiFitter(){
    /* Variables */
	category = new RooCategory("category","category");
	category->defineType("v14");
	category->defineType("v15");
	category->defineType("v16");

	variables->add(*category);

	/* PDF's */
	resolution_IIa = new BsResolution("IIa", t, et);
	resolution_IIb = new BsResolution("IIb", t, et);
	parameters->add(*resolution_IIa->getParameters());
	parameters->add(*resolution_IIb->getParameters());

	et_sig_IIa = new BsEtModel ("sig_IIa", et);
	et_sig_IIb = new BsEtModel ("sig_IIb", et);
	et_bkg_IIa = new BsEtModel ("bkg_IIa", et);
	et_bkg_IIb = new BsEtModel ("bkg_IIb", et);
	parameters->add(*et_sig_IIa->getParameters());
	parameters->add(*et_sig_IIb->getParameters());
	parameters->add(*et_bkg_IIa->getParameters());
	parameters->add(*et_bkg_IIb->getParameters());

	efficiency_v14 = new Efficiency("efficiency_v14");
	efficiency_v15 = new Efficiency("efficiency_v15");
	efficiency_v16 = new Efficiency("efficiency_v16");

	signal_v14 = new BsSignal ("v14", m, t, cpsi, ctheta, phi, p,
			M, Sigma, A0, A1, DeltaGamma, Phi_s, Delta1, Delta2, Tau, DeltaMs,
			Delta1_mean, Delta1_sigma, Delta2_mean, Delta2_sigma, DeltaMs_mean, DeltaMs_sigma,
			resolution_IIa, et_sig_IIa, efficiency_v14);
	signal_v15 = new BsSignal ("v15", m, t, cpsi, ctheta, phi, p,
			M, Sigma, A0, A1, DeltaGamma, Phi_s, Delta1, Delta2, Tau, DeltaMs,
			Delta1_mean, Delta1_sigma, Delta2_mean, Delta2_sigma, DeltaMs_mean, DeltaMs_sigma,
			resolution_IIb, et_sig_IIb, efficiency_v15);
	signal_v16 = new BsSignal ("v16", m, t, cpsi, ctheta, phi, p,
			M, Sigma, A0, A1, DeltaGamma, Phi_s, Delta1, Delta2, Tau, DeltaMs,
			Delta1_mean, Delta1_sigma, Delta2_mean, Delta2_sigma, DeltaMs_mean, DeltaMs_sigma,
			resolution_IIb, et_sig_IIb, efficiency_v16);

	bkg_v14 = new BsBackground("v14", m, t, cpsi, ctheta, phi, p, resolution_IIa, et_bkg_IIa);
	bkg_v15 = new BsBackground("v15", m, t, cpsi, ctheta, phi, p, resolution_IIb, et_bkg_IIb);
	bkg_v16 = new BsBackground("v16", m, t, cpsi, ctheta, phi, p, resolution_IIb, et_bkg_IIb);

	parameters->add(*bkg_v14->getParameters());
	parameters->add(*bkg_v15->getParameters());
	parameters->add(*bkg_v16->getParameters());

	xs_v14 = new RooRealVar ("xs_v14", "x_s v14", 0);
	xs_v15 = new RooRealVar ("xs_v15", "x_s v15", 0);
	xs_v16 = new RooRealVar ("xs_v16", "x_s v16", 0);
	parameters->add(*xs_v14);
	parameters->add(*xs_v15);
	parameters->add(*xs_v16);

	model_v14 = new RooAddPdf ("model_v14", "model_v14", *signal_v14->pdf(), *bkg_v14->pdf(), *xs_v14);
	model_v15 = new RooAddPdf ("model_v15", "model_v15", *signal_v15->pdf(), *bkg_v15->pdf(), *xs_v15);
	model_v16 = new RooAddPdf ("model_v16", "model_v16", *signal_v16->pdf(), *bkg_v16->pdf(), *xs_v16);

	model = new RooSimultaneous("model", "model", *category);
	model->addPdf(*model_v14,"v14");
	model->addPdf(*model_v15,"v15");
	model->addPdf(*model_v16,"v16");

	pdf=model;
}


char *BsFitter::glue(const char* a, const char *b){
	char *tmp = (char*) malloc(strlen(a)+strlen(b)+2);
	strcpy(tmp, a);
	strcat(tmp, "_");
	strcat(tmp, b);
	return tmp;
}

void BsFitter::setVariables(const char* vars) {
	variables->readFromFile(vars);
	cpsi->setMax(1);
	cpsi->setMin(-1);
	ctheta->setMax(1);
	ctheta->setMin(-1);
	phi->setMax(TMath::Pi());
	phi->setMin(-TMath::Pi());
	p->setMin(-1);
	p->setMax(1);
}

void BsFitter::setParameters(const char* params) {
	parameters->readFromFile(params);
}

void BsFitter::writeParameters(const char* file) {
	//ofstream out(file);
	parameters->writeToFile(file);
//	out.close();
}

void BsFitter::setData(const char* data_file) {
	data = RooDataSet::read(data_file, *variables);
}

Int_t BsFitter::fit(Bool_t hesse, Bool_t minos, Bool_t verbose, Int_t cpu) {
	if (!data) {
		cout << "EE: No Data" << endl;
		return kFALSE;
	}

	fit_result = pdf->fitTo(*data,
					RooFit::ConditionalObservables(*p),
					RooFit::Save(kTRUE),
					RooFit::Hesse(hesse),
					RooFit::Minos(minos),
					RooFit::NumCPU(cpu),
					RooFit::Verbose(verbose)/*, RooFit::Strategy(2)*/);
	return fit_result->status();
}

void BsFitter::writeResult(const char* file_name) {
	ofstream out(file_name);
	out.precision(15);
	fit_result->printMultiline(out, 0, kTRUE);
	out << "minNll\t" << fit_result->minNll() << endl;
	out.close();
}



/*void BsFitter::plotM(const char* plot_file, Int_t bins, Int_t proj_bins,
		Bool_t log) {
	plotVar(_m, plot_file, bins, proj_bins, log);
}

void BsFitter::plotM() {
	plotVar(_m, "mass.gif", 0, 100, kFALSE);
}

void BsFitter::plotT(const char* plot_file, Int_t bins, Int_t proj_bins,
		Bool_t log) {
	plotVar(_t, plot_file, bins, proj_bins, log);
}

void BsFitter::plotEt() {
	plotVar(_et, "time_error.gif", 0, 100, kTRUE);
}

void BsFitter::plotEt(const char* plot_file, Int_t bins, Int_t proj_bins,
		Bool_t log) {
	plotVar(_et, plot_file, bins, proj_bins, log);
}

void BsFitter::plotT() {
	plotVar(_t, "time.gif", 0, 100, kTRUE);
}

void BsFitter::plotCpsi(const char* plot_file, Int_t bins, Int_t proj_bins,
		Bool_t log) {
	plotVar(_cpsi, plot_file, bins, proj_bins, log);
}

void BsFitter::plotCpsi() {
	plotVar(_cpsi, "cpsi.gif", 0, 100, kFALSE);
}

void BsFitter::plotCtheta(const char* plot_file, Int_t bins, Int_t proj_bins,
		Bool_t log) {
	plotVar(_ctheta, plot_file, bins, proj_bins, log);
}

void BsFitter::plotCtheta() {
	plotVar(_ctheta, "ctheta.gif", 0, 100, kFALSE);
}

void BsFitter::plotPhi(const char* plot_file, Int_t bins, Int_t proj_bins,
		Bool_t log) {
	plotVar(_phi, plot_file, bins, proj_bins, log);
}

void BsFitter::plotPhi() {
	plotVar(_phi, "phi.gif", 0, 100, kFALSE);
}

void BsFitter::plotVar(RooRealVar& x, const char* plot_file, Int_t bins,
		Int_t proj_bins, Bool_t log) {

	RooPlot *x_frame = x.frame();
	if (_data)
		if (bins)
			_data->plotOn(x_frame, RooFit::Binning(bins));
		else
			_data->plotOn(x_frame);

	//  Double_t norm = 1;
	//  if (x == _t)
	//      norm = 2.6;

	//double __xs = 4.7754e-01;//1.28021;
	//double __xp = 6.5374e-01;

	if (_data) {
		RooAbsData *projData;
		if (proj_bins) {
			_et.setBins(proj_bins);
			projData = new RooDataHist("projData", "projData",RooArgSet(_et, _p), *_data);
		} else {
			projData = _data;
		}
		if (!_resolution){
			_model->plotOn(x_frame,
					RooFit::LineColor(13));
		}
		if (_resolution /*&& !_error_signal && !_using_tag/){
			if (_signal)
				_model->plotOn(x_frame,
						RooFit::Components(*_signal),
						RooFit::ProjWData(RooArgSet(_et), *projData),
						RooFit::LineColor(kGreen));
			//RooFit::Normalization(__xs));
			if (_background)
				_model->plotOn(x_frame,
						RooFit::Components(*_background),
						RooFit::ProjWData(RooArgSet(_et), *projData),
						RooFit::LineColor(kRed));
						//RooFit::Normalization((1 - __xs) * __xp));
			_model->plotOn(x_frame,
					RooFit::ProjWData(RooArgSet(_et), *projData),
					RooFit::LineColor(13));
		}

	} else {
		_model->plotOn(x_frame);
	}
	//    if (x == _ctheta || x == _cpsi || x == _phi)
	//        efficiency->plotOn(x_frame,RooFit::LineColor(kRed));

	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 800);
	x_frame->Draw();
	if (log)
		gPad->SetLogy(1);
	canvas->Print(plot_file);
} */
