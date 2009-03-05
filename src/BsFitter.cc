#include <RooGlobalFunc.h>
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

BsFitter::BsFitter(Bool_t signal, Bool_t background,
		Bool_t resolution, Bool_t error_model, Bool_t tag_model,
		const char* efficiency_file) :
	_m("_m", "m", 0),
	_t("_t", "t", 0),
	_et("_et", "et", 0),
	_cpsi("_cpsi", "cos(#psi)", 0),
	_ctheta("_ctheta", "cos(#theta)", 0),
	_phi("_phi", "#phi", 0),
	_p("_p", "bs probability", 0) {
	gROOT->SetStyle("Plain");

	//    const char *range = "full";
	//    _range = range;

	_resolution = 0;
	_model = 0;
	_signal = 0;
	_background = 0;
	_error_signal = 0;
	_error_background = 0;

	_variables = new RooArgSet();
	_parameters = new RooArgSet();
	//    _constraints = new RooArgSet();

	_variables->add(_m);
	_variables->add(_t);
	if (resolution)
		_variables->add(_et);
	_variables->add(_cpsi);
	_variables->add(_ctheta);
	_variables->add(_phi);
	_variables->add(_p);

	_efficiency = new Efficiency();
	if (efficiency_file)
		_efficiency->setEfficiency(efficiency_file);

	if (resolution) {
		RooRealVar *S = new RooRealVar("S", "S", 0);
		_parameters->add(*S);
		RooGaussModel *resolution = new RooGaussModel("resolution",
				"resolution", _t, RooFit::RooConst(0), *S, _et);
		_resolution = resolution;
	} else {
		_resolution = new RooTruthModel("_resolution", "truth resolution", _t);
	}

	if (signal)
		_signal = signal_model(error_model, tag_model);
	if (background)
		_background = background_model(error_model, tag_model);

	if (_signal && _background) {
		RooRealVar *xs = new RooRealVar("xs", "x_s", 0);
		_parameters->add(*xs);
		_model = new RooAddPdf("model", "model", *_signal, *_background, *xs);
	} else {
		if (_signal)
			_model = _signal;
		else
			_model = _background;
	}

	if (!_model) {
		cout << "EE: No Hay modelo de se~al o background" << endl;
		exit(1);
	}
}

void BsFitter::setVariables(const char* vars) {
	_variables->readFromFile(vars);
	_cpsi.setMax(1);
	_cpsi.setMin(-1);
	_ctheta.setMax(1);
	_ctheta.setMin(-1);
	_phi.setMax(TMath::Pi());
	_phi.setMin(-TMath::Pi());
	_p.setMin(-1);
	_p.setMax(1);
}

void BsFitter::setParameters(const char* params) {
	_parameters->readFromFile(params);
}

void BsFitter::writeParameters(const char* params) {
	_parameters->writeToFile(params);
}

void BsFitter::setEfficiency(const char* eff) {
	_efficiency->setEfficiency(eff);
}


void BsFitter::setData(const char* data_file) {
	if (!_signal && _background) {
		RooRealVar mean("mean", "mean", 5.37, 5.28, 5.44);
		RooRealVar sigma("sigma", "sigma", 0.038, 0.01, 0.1);
		RooRealVar m("m", "m", 0, -1/6, 1);
		RooRealVar xs("xs", "xs", 0.2, 0.01, 1);

		RooDataSet *data = RooDataSet::read(data_file, *_variables);

		RooGaussian gauss("gauss", "gauss", _m, mean, sigma);
		RooPolynomial line("line", "line", _m, m);
		RooAddPdf model("model", "model", gauss, line, xs);
		model.fitTo(*data,
				RooFit::Hesse(false),
				RooFit::Minos(false));

		Double_t left = mean.getVal() - 3 * sigma.getVal();
		Double_t right = mean.getVal() + 3 * sigma.getVal();

		TString cut = "_m < ";
		cut += left;
		cut += " || _m > ";
		cut += right;
		cout << "Bkg Cut: " << cut << endl;
		_data = new RooDataSet("_data", "data", data, *_variables, cut);
		delete data;
	} else {
		_data = RooDataSet::read(data_file, *_variables);
	}
}

void BsFitter::setData(RooDataSet* data_set) {
	_data = data_set;
}

//void BsFitter::setDataFromCin() {
//    data = new RooDataSet("data", "data", *_variables);
//
//    Double_t m, t, cpsi, ctheta, phi, d;
//    while (!cin.eof()) {
//        cin >> m >> t >> cpsi >> ctheta >> phi >> d;
//        cout << m << ' ' << t << ' ' << cpsi << ' ' << ctheta << ' ' << phi << ' ' << d << endl;
//        _m = m;
//        _t = t;
//        _cpsi = cpsi;
//        _ctheta = ctheta;
//        _phi = phi;
//        _d = d;
//        data->add(*_variables);
//    }
//
//}

void BsFitter::generate(Int_t num, const char* data) {
	_variables->Print();
	if (_resolution) {
		//  RooPolynomial p_model("p_model", "p_model", _p, RooFit::RooConst(0));
		//  RooDataSet *data_p = p_model.generate(RooArgSet(_p), num, kTRUE);
		_p = 1;
		RooDataSet *data_et = _model->generate(RooArgSet(_et), num, kTRUE);
		//  data_et->merge(data_p);
		RooDataSet *data_m =
			_model->generate(RooArgSet(_m, _t, _et, _cpsi,_ctheta, _phi), *data_et);
		_data = new RooDataSet("_data", "data", data_m, *_variables);
	} else {
		_data =
			_model->generate(RooArgSet(_m, _t, _cpsi, _ctheta, _phi, _p),num);
	}
	_data->write(data);
}

Int_t BsFitter::fit(Bool_t hesse, Bool_t minos, Bool_t verbose, Int_t cpu) {
	if (!_data) {
		cout << "EE: No Data" << endl;
		return kFALSE;
	}

	if (_resolution) {
		if (_error_signal || _error_background){
			_fit_result = _model->fitTo(*_data,
					RooFit::ConditionalObservables(RooArgSet(_p)),
					RooFit::Save(kTRUE),
					RooFit::Hesse(hesse),
					RooFit::Minos(minos),
					RooFit::NumCPU(cpu),
					RooFit::Verbose(verbose)/*, RooFit::Strategy(2)*/);
		} else {
			_fit_result = _model->fitTo(*_data,
					RooFit::ConditionalObservables(RooArgSet(_et, _p)),
					RooFit::Save(kTRUE),
					RooFit::Hesse(hesse),
					RooFit::Minos(minos),
					RooFit::NumCPU(cpu),
					RooFit::Verbose(verbose)/*, RooFit::Strategy(2)*/);
		}
	} else {
		_fit_result = _model->fitTo(*_data,
				RooFit::ConditionalObservables(RooArgSet(_p)),
				RooFit::Save(kTRUE),
				RooFit::Hesse(hesse),
				RooFit::Minos(minos),
				RooFit::NumCPU(cpu),
				RooFit::Verbose(verbose)/*, RooFit::Strategy(2)*/);
	}
	return _fit_result->status();
}

void BsFitter::plotM(const char* plot_file, Int_t bins, Int_t proj_bins,
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
		if (_resolution) {
			if (proj_bins) {
				_et.setBins(proj_bins);
				RooDataHist projData("projData", "projData",
						RooArgSet(_et, _p), *_data);
/*
				if (_signal)
					_model->plotOn(x_frame,
							RooFit::Components(*_signal),
//							RooFit::ProjWData(RooArgSet(_et), projData),
							RooFit::LineColor(kGreen));
							//RooFit::Normalization(__xs));
				if (_background)
					_model->plotOn(x_frame,
							RooFit::Components(*_background),
//							RooFit::ProjWData(RooArgSet(_et), projData),
							RooFit::LineColor(kRed));
							//RooFit::Normalization((1 - __xs) * __xp));
*/
				_model->plotOn(x_frame,
//						RooFit::ProjWData(RooArgSet(_et),projData),
						RooFit::LineColor(13));
			} else {
				_model->plotOn(x_frame,
						RooFit::ProjWData(RooArgSet(_et, _p),*_data));
						//RooFit::Range(_range));
			}
		} else {
			_model->plotOn(x_frame,
					RooFit::ProjWData(RooArgSet(_p), *_data));
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
}

void BsFitter::writeResult(const char* file_name) {
	ofstream out(file_name);
	_fit_result->printMultiline(out, 0, kTRUE);
	out << "minNll\t" << _fit_result->minNll() << endl;
	out.close();
}

RooAbsPdf* BsFitter::signal_model(Bool_t error_model, Bool_t tag_model) {
	cout << "SIGNAL MODEL" << endl;
	RooRealVar *M = new RooRealVar("M", "M", 0);
	RooRealVar *Sigma = new RooRealVar("Sigma", "#sigma", 0);

	RooRealVar *A0 = new RooRealVar("A0", "A0", 0);
	RooRealVar *A1 = new RooRealVar("A1", "A1", 0);
	RooRealVar *DeltaGamma = new RooRealVar("DeltaGamma", "#Delta#Gamma", 0);
	RooRealVar *Delta_1 = new RooRealVar("Delta_1", "#delta_{1}", 0);
	RooRealVar *Delta_2 = new RooRealVar("Delta_2", "#delta_{2}", 0);
	RooRealVar *Phi_s = new RooRealVar("Phi_s", "#phi_{s}", 0);
	RooRealVar *Tau = new RooRealVar("Tau", "#tau", 0);
	RooRealVar *DeltaMs = new RooRealVar("DeltaMs", "#Delta M_{s}", 0);

	RooFormulaVar *All2 = new RooFormulaVar("All2", "(1-@0)*@1", RooArgList(
			*A0, *A1));
	RooFormulaVar *Ap2 = new RooFormulaVar("Ap2", "1-@0-@1", RooArgList(*A0,
			*All2));

	_parameters->add(*M);
	_parameters->add(*Sigma);
	_parameters->add(*A0);
	_parameters->add(*A1);
	_parameters->add(*DeltaGamma);
	_parameters->add(*Delta_1);
	_parameters->add(*Delta_2);
	_parameters->add(*Phi_s);
	_parameters->add(*Tau);
	_parameters->add(*DeltaMs);

	/*
	 RooRealVar *delta_1_mean = new RooRealVar("delta_1_mean", "#delta_1 mean", 0);
	 RooRealVar *delta_1_sigma = new RooRealVar("delta_1_sigma", "#delta_1 #sigma", 0);
	 RooRealVar *delta_2_mean = new RooRealVar("delta_2_mean", "#delta_2 mean", 0);
	 RooRealVar *delta_2_sigma = new RooRealVar("delta_2_sigma", "#delta_2 #sigma", 0);
	 _parameters->add(*delta_1_mean);
	 _parameters->add(*delta_1_sigma);
	 _parameters->add(*delta_2_mean);
	 _parameters->add(*delta_2_sigma);

	 RooGaussian *delta_1_constraint = new RooGaussian("delta_1_constraint", "#delta_1 Gaussian Constraint", *Delta_1, *delta_1_mean, *delta_1_sigma);
	 RooGaussian *delta_2_constraint = new RooGaussian("delta_2_constraint", "#delta_2 Gaussian Constraint", *Delta_2, *delta_2_mean, *delta_2_sigma);
	 _constraints->add(*delta_1_constraint);
	 _constraints->add(*delta_2_constraint);
	 */

	RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass",
			_m, *M, *Sigma);

	RooAbsPdf* signal_time_angle = 0;
	if (tag_model){
		RooDataSet *d_data_bs = new RooDataSet("d_data_bs", "d_data_bs",_p);
		RooDataSet *d_data_bsbar = new RooDataSet("d_data_bsbar", "d_data_bsbar",_p);
		d_data_bs->read("d_data_bs.txt",_p);
		d_data_bsbar->read("d_data_bsbar.txt",_p);

		RooDataHist *d_hist_bs = new RooDataHist("d_hist_bs","d_hist_bs", _p, *d_data_bs);
		RooDataHist *d_hist_bsbar = new RooDataHist("d_hist_bsbar","d_hist_bsbar", _p, *d_data_bsbar);

		RooHistPdf *d_pdf_bs = new RooHistPdf("d_pdf_bs", "d_pdf_bs", _p, *d_hist_bs);
		RooHistPdf *d_pdf_bsbar = new RooHistPdf("d_pdf_bsbar", "d_pdf_bsbar", _p, *d_hist_bs);

		RooRealVar *var_plus_one = new RooRealVar("var_plus_one", "var plus one", 1);
		RooRealVar *var_minus_one = new RooRealVar("var_minus_one", "var minus one", -1);

		RooBsTimeAngle* time_angle_bs = new RooBsTimeAngle("time_angle_bs",
				"time angle bs", _t, _cpsi, _ctheta, _phi, *var_plus_one, *A0, *All2,
				*Ap2, *DeltaGamma, *Tau, *DeltaMs, *Phi_s, *Delta_1, *Delta_2,
				*_resolution, *_efficiency);
		RooBsTimeAngle* time_angle_bsbar = new RooBsTimeAngle("time_angle_bsbar",
				"time angle bsbar", _t, _cpsi, _ctheta, _phi, *var_minus_one, *A0, *All2,
				*Ap2, *DeltaGamma, *Tau, *DeltaMs, *Phi_s, *Delta_1, *Delta_2,
				*_resolution, *_efficiency);
		RooProdPdf *d_time_angle_bs = new RooProdPdf("d_time_angle_bs", "d_time_angle_bs",
				RooArgSet(*time_angle_bs, *d_pdf_bs));
		RooProdPdf *d_time_angle_bsbar = new RooProdPdf("d_time_angle_bsbar", "d_time_angle_bsbar",
				RooArgSet(*time_angle_bsbar, *d_pdf_bsbar));

		signal_time_angle = new RooAddPdf("signal_time_angle", "signal time angle",
				*d_time_angle_bs, *d_time_angle_bsbar, RooFit::RooConst(0.5));
	} else {
		signal_time_angle = new RooBsTimeAngle("signal_time_angle",
				"signal time angle pdf", _t, _cpsi, _ctheta, _phi, _p, *A0, *All2,
				*Ap2, *DeltaGamma, *Tau, *DeltaMs, *Phi_s, *Delta_1, *Delta_2,
				*_resolution, *_efficiency);
	}

	if (_resolution && error_model) {
		RooRealVar *et_sig_xl =
				new RooRealVar("et_sig_xl", "xl sig", 0.7, 0, 1);
		RooRealVar *et_sig_mean = new RooRealVar("et_sig_mean", "mean sig",
				0.06, 0, 1);
		RooRealVar *et_sig_sigma = new RooRealVar("et_sig_sigma", "sigma sig",
				0.01, 0, 0.1);
		RooRealVar *et_sig_tau_short = new RooRealVar("et_sig_tau_short",
				"#tau short sig", 0.05, 0, 0.5);
		RooRealVar *et_sig_tau_long = new RooRealVar("et_sig_tau_long",
				"#tau long sig", 0.05, 0, 0.1);
		_parameters->add(*et_sig_xl);
		_parameters->add(*et_sig_mean);
		_parameters->add(*et_sig_sigma);
		_parameters->add(*et_sig_tau_short);
		_parameters->add(*et_sig_tau_long);

		RooGaussModel *et_sig_gauss = new RooGaussModel("gauss", "gauss", _et,
				*et_sig_mean, *et_sig_sigma);
		RooDecay *et_sig_short = new RooDecay("et_sig_short", "short sig", _et,
				*et_sig_tau_short, *et_sig_gauss, RooDecay::SingleSided);
		RooDecay *et_sig_long = new RooDecay("et_sig_long", "long bgk", _et,
				*et_sig_tau_long, *et_sig_gauss, RooDecay::SingleSided);
		RooAddPdf *et_sig_model = new RooAddPdf("et_sig_model", "et sig model",
				*et_sig_long, *et_sig_short, *et_sig_xl);

		_error_signal = et_sig_model;

		RooProdPdf *signal = new RooProdPdf("signal", "signal",
				RooArgSet(*signal_mass, *et_sig_model),
				RooFit::Conditional(*signal_time_angle, RooArgSet(_m, _t, _cpsi, _ctheta, _phi)));
		return signal;
	} else {
		RooProdPdf *signal = new RooProdPdf("signal", "signal", RooArgSet(*signal_mass,
				*signal_time_angle));
		return signal;
	}
}

RooAbsPdf* BsFitter::background_model(Bool_t error_model, Bool_t tag_model) {
	cout << "BACKGROUND MODEL" << endl;
	RooRealVar *m_a1 = new RooRealVar("m_a1", "m_a1", 0);
//	RooRealVar *m_a2 = new RooRealVar("m_a2", "m_p_a2", 0);

	RooRealVar *t_m = new RooRealVar("t_m", "t_m", 0);
	RooRealVar *t_p = new RooRealVar("t_p", "t_p", 0);
	RooRealVar *t_pp = new RooRealVar("t_pp", "t_pp", 0);
	RooRealVar *xp = new RooRealVar("xp", "xp", 0);
	RooRealVar *fm = new RooRealVar("fm", "fm", 0);
	RooRealVar *fp = new RooRealVar("fp", "fp", 0);

	RooRealVar *y1 = new RooRealVar("y1", "y1", 0);
	RooRealVar *y2 = new RooRealVar("y2", "y2", 0);
	RooRealVar *y4 = new RooRealVar("y4", "y4", 0);
	RooRealVar *y5 = new RooRealVar("y5", "y5", 0);
	RooRealVar *y6 = new RooRealVar("y6", "y6", 0);

	_parameters->add(*m_a1);
//	_parameters->add(*m_a2);

	_parameters->add(*xp);
	_parameters->add(*t_m);
	_parameters->add(*t_p);
	_parameters->add(*t_pp);
	_parameters->add(*fm);
	_parameters->add(*fp);

	_parameters->add(*y1);
	_parameters->add(*y2);
	_parameters->add(*y4);
	_parameters->add(*y5);
	_parameters->add(*y6);


	RooPolynomial *mass_bkg = new RooPolynomial("mass_bkg", "mass_bkg", _m, RooArgList(*m_a1));

	RooDecay *exp_minus = new RooDecay("exp_minus", "exp minus", _t, *t_m,
			*_resolution, RooDecay::Flipped);

	RooDecay *exp_plus = new RooDecay("exp_plus", "exp plus", _t, *t_p,
			*_resolution, RooDecay::SingleSided);

	RooDecay *exp_plus_plus = new RooDecay("exp_plus_plus", "exp plus plus",
			_t, *t_pp, *_resolution, RooDecay::SingleSided);

	RooAddPdf *time_bkg = new RooAddPdf("time_bkg", "time bkg",
			RooArgSet(*_resolution, *exp_minus, *exp_plus, *exp_plus_plus),
			RooArgSet(*xp, *fm, *fp), true);


	RooPolynomial *cpsi_bkg = new RooPolynomial("cpsi_bkg",
				"cpsi_bkg", _cpsi, RooArgSet(RooFit::RooConst(0), *y1));

	RooPolynomial *ctheta_bkg = new RooPolynomial("ctheta_bkg",
				"ctheta_bkg", _ctheta, RooArgSet(RooFit::RooConst(0), *y2));

	RooGenericPdf *phi_bkg = new RooGenericPdf("phi_bkg", "phi_bkg",
			"1+@1*sin(@0*@2+@3)", RooArgList(_phi, *y4, *y5, *y6));

	RooProdPdf *angle_bkg = new RooProdPdf("angle_bkg", "angle_bkg",
			RooArgSet(*cpsi_bkg, *ctheta_bkg, *phi_bkg));

	RooArgSet *background_set = new RooArgSet(*mass_bkg, *angle_bkg);
	if (tag_model) {
		const char *data_file = "fit.dat";

		RooRealVar mean("mean", "mean", 5.37, 5.28, 5.44);
		RooRealVar sigma("sigma", "sigma", 0.038, 0.01, 0.1);
		RooRealVar m("m", "m", 0, -1/6, 1);
		RooRealVar xs("xs", "xs", 0.2, 0.01, 1);

		RooDataSet *data = RooDataSet::read(data_file, *_variables);

		RooGaussian gauss("gauss", "gauss", _m, mean, sigma);
		RooPolynomial line("line", "line", _m, m);
		RooAddPdf model("model", "model", gauss, line, xs);
		model.fitTo(*data,
				RooFit::Hesse(false),
				RooFit::Minos(false));

		Double_t left = mean.getVal() - 3 * sigma.getVal();
		Double_t right = mean.getVal() + 3 * sigma.getVal();

		TString cut = "_m < ";
		cut += left;
		cut += " || _m > ";
		cut += right;
		cout << "Bkg Cut: " << cut << endl;
		RooDataSet *d_data_bkg = new RooDataSet("d_data_bkg", "d_data_bkg", data , _p, cut);
		delete data;

		RooDataHist *d_hist_bkg = new RooDataHist("d_hist_bkg","d_hist_bkg", _p, *d_data_bkg);
		RooHistPdf *d_pdf_bkg = new RooHistPdf("d_pdf_bkg", "d_pdf_bkg", _p, *d_hist_bkg);

		background_set->add(*d_pdf_bkg);
	}

	if (_resolution && error_model){
		RooRealVar *et_bkg_xl = new RooRealVar("et_bkg_xl", "xl bkg", 0);
		RooRealVar *et_bkg_mean = new RooRealVar("et_bkg_mean", "mean bkg", 0);
		RooRealVar *et_bkg_sigma = new RooRealVar("et_bkg_sigma", "sigma bkg", 0);
		RooRealVar *et_bkg_tau_short =
			new RooRealVar("et_bkg_tau_short", "#tau short bkg", 0);
		RooRealVar *et_bkg_tau_long =
			new RooRealVar("et_bkg_tau_long","#tau long bkg", 0);
		_parameters->add(*et_bkg_xl);
		_parameters->add(*et_bkg_mean);
		_parameters->add(*et_bkg_sigma);
		_parameters->add(*et_bkg_tau_short);
		_parameters->add(*et_bkg_tau_long);

		RooGaussModel *et_bkg_gauss =
			new RooGaussModel("gauss", "gauss",
					_et, *et_bkg_mean, *et_bkg_sigma);
		RooDecay *et_bkg_short =
			new RooDecay("et_bkg_short", "short bkg",
					_et, *et_bkg_tau_short, *et_bkg_gauss, RooDecay::SingleSided);
		RooDecay *et_bkg_long =
			new RooDecay("et_bkg_long", "long bgk",
					_et, *et_bkg_tau_long, *et_bkg_gauss, RooDecay::SingleSided);
		RooAddPdf *et_bkg_model =
			new RooAddPdf("et_bkg_model", "et bkg model",
				*et_bkg_long, *et_bkg_short, *et_bkg_xl);

		_error_background = et_bkg_model;
		background_set->add(*et_bkg_model);

		RooProdPdf *background = new RooProdPdf("background", "background",
				*background_set,
				RooFit::Conditional(*time_bkg, RooArgSet(_m, _t, _cpsi, _ctheta, _phi)));
		return background;
	} else {
		background_set->add(*time_bkg);
		RooProdPdf *background = new RooProdPdf("background", "background",
				*background_set);
		return background;;
	}
}
