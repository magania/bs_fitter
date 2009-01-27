#include <RooGlobalFunc.h>
#include <RooAddPdf.h>

#include <RooDataSet.h>
#include <RooAbsRealLValue.h>
#include <RooAbsRealLValue.h>
#include <RooDataSet.h>
#include <RooAddModel.h>

#include "RooBkgAngle.h"

#include <TRandom3.h>
#include <RooDataSet.h>

#include <RooGlobalFunc.h>
#include <RooRealVar.h>

#include "BsFitter.h"
#include "Efficiency.h"
#include "RooBsTimeAngle.cc"
//#include "RooBkgAngle.cc"

BsFitter::BsFitter(Bool_t use_resolution, Bool_t signal_only, Bool_t prompt_only, Bool_t noprompt_only, Bool_t use_efficiency, Bool_t use_phis) :
_m("_m", "m", 0),
_t("_t", "t", 0),
_et("_et", "et", 0),
_cpsi("_cpsi", "cos(#psi)", 0),
_ctheta("_ctheta", "cos(#theta)", 0),
_phi("_phi", "#phi", 0),
_p("_p", "bs probability", 0) {
    gROOT->SetStyle("Plain");

    _use_resolution = use_resolution;
    _signal_only = signal_only;
    _prompt_only = prompt_only;
    _noprompt_only = noprompt_only;
    _use_efficiency = use_efficiency;
    _use_phis = use_phis;
    const char *range = "full";
    _range = range;

    _resolution = 0;
    _model = 0;
    _data = 0;
    _fit_result = 0;
    _signal = 0;
    _background = 0;
    _prompt = 0;
    _noprompt = 0;

    //    _t.setRange("np", 1,12);
    //    const char *range_np = "np";
    //    _range = range_np;

    _variables = new RooArgSet();
    _parameters = new RooArgSet();
    _constraints = new RooArgSet();

    _variables->add(_m);
    _variables->add(_t);
    if (_use_resolution)
        _variables->add(_et);
    _variables->add(_cpsi);
    _variables->add(_ctheta);
    _variables->add(_phi);
    _variables->add(_p);

    if (_use_phis) {
        _phis = new Phis();
    } else {
        if (_use_efficiency)
            _efficiency = new Efficiency();
    }

    if (_use_resolution) {
        RooRealVar *S = new RooRealVar("S", "S", 0);
	//        RooRealVar *S2 = new RooRealVar("S2", "S2", 0);
        //RooRealVar *xr = new RooRealVar("xr", "xr", 0);
        _parameters->add(*S);
        //_parameters->add(*S2);
        //_parameters->add(*xr);
        RooGaussModel *gauss_1 = new RooGaussModel("gauss_1", "gauss 1 resolution", _t, RooFit::RooConst(0), *S, _et);
        //RooGaussModel *gauss_2 = new RooGaussModel("gauss_2", "gauss 2 resolution", _t, RooFit::RooConst(0), *S2, _et);
	//_resolution = new RooAddModel("_resolution", "gaussx2 resolution", RooArgList(*gauss_1, *gauss_2), RooArgList(*xr));
	_resolution = gauss_1;
    } else {
        _resolution = new RooTruthModel("_resolution", "truth resolution", _t);
    }

    if (!_prompt_only && !_noprompt_only)
      _signal = signal_model();
    if (!_signal_only)
      _background = background_model();

    if (_signal && _background) {
        RooRealVar *xs = new RooRealVar("xs", "x_s", 0);
        _parameters->add(*xs);
        _model = new RooAddPdf("_model", "Bs PDF", *_signal, *_background, *xs);
    } else {
        if (_signal)
            _model = _signal;
        else
            _model = _background;
    }

    if (!_model)
        cout << "EE: No Hay modelo de se~al o background" << endl;
}

void BsFitter::setVariables(const char* vars) {
    _variables->readFromFile(vars);
    _cpsi.setMax(1);
    _cpsi.setMin(-1);
    _ctheta.setMax(1);
    _ctheta.setMin(-1);
    _phi.setMax(TMath::Pi());
    _phi.setMin(-TMath::Pi());
    _p.setMin(0);
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

void BsFitter::setPhis(const char* phis) {
    _phis->setPhis(phis);
}

void BsFitter::setData(const char* data_file) {
   // if (_sidebands) {
   /*     RooDataSet *data = RooDataSet::read(data_file, *_variables);
        RooRealVar M("M", "M", 5.365081e+00, 5.28, 5.44);
        RooRealVar sigma("sigma", "sigma", 2.85725e-02, 0, 1);
        RooRealVar tau_bkg("tau_bkg", "tau_bkg", -6.11910e-01, -1.0, 0.0);
        RooRealVar xs("xs", "xs", 3.85194e-02, 0, 1);

        RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass", _m, M, sigma);
        RooExponential *bkg_mass = new RooExponential("bkg_mass", "bkg_mass", _m, tau_bkg);
        RooAddPdf *cut_model = new RooAddPdf("cut_model", "cut_model", *signal_mass, *bkg_mass, xs);
         cut_model->fitTo(*data);

        Double_t left = M.getVal() - 3 * sigma.getVal();
        Double_t right = M.getVal() + 3 * sigma.getVal();

        TString cut = "_m < ";
        cut += left;
        cut += " || _m > ";
        cut += right;
        cout << "Cut: " << cut << endl;
        _data = new RooDataSet("_data", "data", data, *_variables, cut);
        delete data;
        _m.setRange("left", _m.getMin(), left);
        _m.setRange("right", right, _m.getMax());
        const char* sideband_range = "left,right";
        _range = sideband_range;
    */
   // } else {
        _data = RooDataSet::read(data_file, *_variables);
   // }
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
    if (_use_resolution) {
        RooPolynomial p_model("p_model", "p_model", _p, RooFit::RooConst(0));
        RooDataSet *data_p = p_model.generate(RooArgSet(_p), num, kTRUE);
        RooDataSet *data_et = _model->generate(RooArgSet(_et), num, kTRUE);
        data_et->merge(data_p);
        RooDataSet *data_m = _model->generate(RooArgSet(_m, _t, _cpsi, _ctheta, _phi), *data_et);
        _data = new RooDataSet("_data", "data", data_m, *_variables);
    } else {
        _data = _model->generate(RooArgSet(_m, _t, _cpsi, _ctheta, _phi, _p), num);
    }
    _data->write(data);
}

Int_t BsFitter::fit(Bool_t hesse, Bool_t minos, Bool_t verbose, Int_t cpu) {
    if (!_data) {
        cout << "EE: No Data" << endl;
        return kFALSE;
    }

    cout << "RANGE: " << _range << endl;
    if (_signal_only) {
      _fit_result = _model->fitTo(*_data, RooFit::ConditionalObservables(RooArgSet(_et,_p)),
				  RooFit::Save(kTRUE), RooFit::Hesse(hesse),
				  RooFit::Minos(minos), RooFit::NumCPU(cpu),
				  RooFit::Verbose(verbose));
    } else {
      if (_prompt_only || _noprompt_only) {
        _fit_result = _model->fitTo(*_data, RooFit::ConditionalObservables(RooArgSet(_et)),
				    RooFit::Save(kTRUE), RooFit::Hesse(hesse),
				    RooFit::Minos(minos), RooFit::NumCPU(cpu),
				    RooFit::Verbose(verbose));
      } else {
        _fit_result = _model->fitTo(*_data, RooFit::ConditionalObservables(RooArgSet(_p)),
				    RooFit::Save(kTRUE), RooFit::Hesse(hesse),
				    RooFit::Minos(minos), RooFit::NumCPU(cpu),
				    RooFit::Verbose(verbose),
				    RooFit::ExternalConstraints(*_constraints)/*, RooFit::Range(_range)*/);
      }
    }
    return _fit_result->status();
}

void BsFitter::plotM(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
    plotVar(_m, plot_file, bins, proj_bins, log);
}

void BsFitter::plotM() {
    plotVar(_m, "mass.gif", 0, 100, kFALSE);
}

void BsFitter::plotT(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
    plotVar(_t, plot_file, bins, proj_bins, log);
}

void BsFitter::plotEt() {
    plotVar(_et, "time_error.gif", 0, 100, kTRUE);
}

void BsFitter::plotEt(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
    plotVar(_et, plot_file, bins, proj_bins, log);
}

void BsFitter::plotT() {
    plotVar(_t, "time.gif", 0, 100, kTRUE);
}

void BsFitter::plotCpsi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
    plotVar(_cpsi, plot_file, bins, proj_bins, log);
}

void BsFitter::plotCpsi() {
    plotVar(_cpsi, "cpsi.gif", 0, 100, kFALSE);
}

void BsFitter::plotCtheta(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
    plotVar(_ctheta, plot_file, bins, proj_bins, log);
}

void BsFitter::plotCtheta() {
    plotVar(_ctheta, "ctheta.gif", 0, 100, kFALSE);
}

void BsFitter::plotPhi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
    plotVar(_phi, plot_file, bins, proj_bins, log);
}

void BsFitter::plotPhi() {
    plotVar(_phi, "phi.gif", 0, 100, kFALSE);
}

void BsFitter::plotVar(RooRealVar& x, const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {

    RooPlot *x_frame = x.frame();
    if (_data)
        if (bins)
            _data->plotOn(x_frame, RooFit::Binning(bins));
        else
            _data->plotOn(x_frame);
    Double_t norm = 1;
    if (x == _t)
        norm = 2.6;

    if (_data) {
        if (_use_resolution) {
            if (proj_bins) {
                _et.setBins(proj_bins);
                RooDataHist projData("projData", "projData", RooArgSet(_et, _p), *_data);
                cout << "RANGE: " << _range << endl;

                if (_signal)
                    _model->plotOn(x_frame, RooFit::Components(*_signal), RooFit::ProjWData(RooArgSet(_et, _p), projData), RooFit::LineColor(kGreen), RooFit::Normalization(0.052705));
                //   if (_prompt)
                //       _model->plotOn(x_frame, RooFit::Components(*_prompt), RooFit::ProjWData(RooArgSet(_et, _p), projData), RooFit::LineColor(kRed));
                //   if (_noprompt)
                //      _model->plotOn(x_frame, RooFit::Components(*_noprompt), RooFit::ProjWData(RooArgSet(_et, _p), projData), RooFit::LineColor(kBlue));

                _model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(_et, _p), projData), RooFit::LineColor(13));
            } else {
                _model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(_et, _p), *_data), RooFit::Range(_range));
            }
        } else {
            _model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(_p), *_data));
        }
    } else {
        _model->plotOn(x_frame);
    }
    //    if (x == _ctheta || x == _cpsi || x == _phi)
    //        efficiency->plotOn(x_frame,RooFit::LineColor(kRed));

    TCanvas *canvas = new TCanvas("canvas", "canvas", 600, 600);
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

RooAbsPdf* BsFitter::signal_model() {
    cout << "SIGNAL MODEL" << endl;
    RooRealVar *M = new RooRealVar("M", "M", 0);
    RooRealVar *Sigma = new RooRealVar("Sigma", "#sigma", 0);

    RooRealVar *A02 = new RooRealVar("A02", "A02", 0);
    RooRealVar *All2 = new RooRealVar("All2", "A_{#parallel}(0)", 0);
    RooRealVar *DeltaGamma = new RooRealVar("DeltaGamma", "#Delta#Gamma", 0);
    RooRealVar *Delta_1 = new RooRealVar("Delta_1", "#delta_{1}", 0);
    RooRealVar *Delta_2 = new RooRealVar("Delta_2", "#delta_{2}", 0);
    RooRealVar *Phi_s = new RooRealVar("Phi_s", "#phi_{s}", 0);
    RooRealVar *Tau = new RooRealVar("Tau", "#tau", 0);
    RooRealVar *DeltaMs = new RooRealVar("DeltaMs", "#Delta M_{s}", 0);

    _parameters->add(*M);
    _parameters->add(*Sigma);
    _parameters->add(*A02);
    _parameters->add(*All2);
    _parameters->add(*DeltaGamma);
    _parameters->add(*Delta_1);
    _parameters->add(*Delta_2);
    _parameters->add(*Phi_s);
    _parameters->add(*Tau);
    _parameters->add(*DeltaMs);

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

    RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass", _m, *M, *Sigma);

    RooAbsPdf *signal_time_angle;
    if (_use_phis) {
        RooBsTimeAngle<Phis, TransAnglesPhis>* time_angle = new RooBsTimeAngle<Phis, TransAnglesPhis > ("time_angle", "signal time angle pdf",
                _t, _cpsi, _ctheta, _phi, _p,
                *A02, *All2, *DeltaGamma, *Tau,
                *DeltaMs, *Phi_s, *Delta_1, *Delta_2,
                *_resolution, *_phis);
        signal_time_angle = time_angle;
    } else {
        RooBsTimeAngle<Efficiency, TransAnglesEfficiency>* time_angle = new RooBsTimeAngle<Efficiency, TransAnglesEfficiency > ("time_angle", "signal time angle pdf",
                _t, _cpsi, _ctheta, _phi, _p,
                *A02, *All2, *DeltaGamma, *Tau,
                *DeltaMs, *Phi_s, *Delta_1, *Delta_2,
                *_resolution, *_efficiency);
        signal_time_angle = time_angle;
    }

    RooProdPdf * signal = 0;
    if (_use_resolution) {
        RooRealVar *et_sig_xl = new RooRealVar("et_sig_xl", "xl sig", 0.7, 0, 1);
        RooRealVar *et_sig_mean = new RooRealVar("et_sig_mean", "mean sig", 0.06, 0, 1);
        RooRealVar *et_sig_sigma = new RooRealVar("et_sig_sigma", "sigma sig", 0.01, 0, 0.1);
        RooRealVar *et_sig_tau_short = new RooRealVar("et_sig_tau_short", "#tau short sig", 0.05, 0, 0.5);
        RooRealVar *et_sig_tau_long = new RooRealVar("et_sig_tau_long", "#tau long sig", 0.05, 0, 0.1);
        _parameters->add(*et_sig_xl);
        _parameters->add(*et_sig_mean);
        _parameters->add(*et_sig_sigma);
        _parameters->add(*et_sig_tau_short);
        _parameters->add(*et_sig_tau_long);

        RooGaussModel *et_sig_gauss = new RooGaussModel("gauss", "gauss", _et, *et_sig_mean, *et_sig_sigma);
        RooDecay *et_sig_short = new RooDecay("et_sig_short", "short sig", _et, *et_sig_tau_short, *et_sig_gauss, RooDecay::SingleSided);
        RooDecay *et_sig_long = new RooDecay("et_sig_long", "long bgk", _et, *et_sig_tau_long, *et_sig_gauss, RooDecay::SingleSided);
        RooAddPdf *et_sig_model = new RooAddPdf("et_sig_model", "et sig model", *et_sig_long, *et_sig_short, *et_sig_xl);

	if (_signal_only)
	  signal = new RooProdPdf("signal", "signal",
				  RooArgSet(*signal_mass, *signal_time_angle));
	else
	  signal = new RooProdPdf("signal", "signal",
				  RooArgSet(*signal_mass, *et_sig_model),
				  RooFit::Conditional(*signal_time_angle, RooArgSet(_m, _t, _cpsi, _ctheta, _phi)));
    } else {
        signal = new RooProdPdf("signal", "signal", RooArgSet(*signal_mass, *signal_time_angle));
    }

    //    return 0;
    return signal;
}

RooAbsPdf* BsFitter::background_model() {
    cout << "BACKGROUND MODEL" << endl;
    if ( _prompt_only || (!_prompt_only && !_noprompt_only)) {
      RooRealVar *m_p_a1 = new RooRealVar("m_p_a1", "m_p_a1", 0);
      RooRealVar *m_p_a2 = new RooRealVar("m_p_a2", "m_p_a2", 0);
      RooRealVar *y1 = new RooRealVar("y1", "y1", 0);
      RooRealVar *y2 = new RooRealVar("y2", "y2", 0);
//      RooRealVar *y3 = new RooRealVar("y3", "y3", 0);
      RooRealVar *y4 = new RooRealVar("y4", "y4", 0);
      RooRealVar *y5 = new RooRealVar("y5", "y5", 0);
      RooRealVar *y6 = new RooRealVar("y6", "y6", 0);
      _parameters->add(*m_p_a1);
      _parameters->add(*m_p_a2);
      _parameters->add(*y1);
      _parameters->add(*y2);
//      _parameters->add(*y3);
      _parameters->add(*y4);
      _parameters->add(*y5);
      _parameters->add(*y6);
      
      RooExponential *mass_p = new RooExponential("mass_p", "cuadratic mass prompt", _m,*m_p_a1);
      if (_use_phis) {
        //        RooBkgAngle<Phis,TransAnglesPhis> *angle_p = new RooBkgAngle<Phis,TransAnglesPhis>("angle_p", "angle prompt", _cpsi, _ctheta, _phi,
        //                *y1, *y2, *y3, *y4, *y5, *y6, *_phis);
        //        _prompt = new RooProdPdf("_prompt", "prompt bkg", RooArgSet(*exp_mass_p, *_resolution, *angle_p));
      } else {
        RooPolynomial *cpsi_model_p = new RooPolynomial("cpsi_model_p", "cpsi_model_p", _cpsi, RooArgSet(RooFit::RooConst(0), *y1));
        RooPolynomial *ctheta_model_p = new RooPolynomial("ctheta_model_p", "ctheta_model_p", _ctheta, RooArgSet(RooFit::RooConst(0), *y2));
        RooGenericPdf *phi_model_p = new RooGenericPdf("phi_model_p", "phi_model", "1+@1*sin(@0*@2+@3)", RooArgList(_phi, *y4, *y5, *y6));
        RooProdPdf *angle_p = new RooProdPdf("angle_p", "angle_p", RooArgSet(*cpsi_model_p, *ctheta_model_p, *phi_model_p));
        _prompt = new RooProdPdf("_prompt", "prompt bkg", RooArgSet(*mass_p, *_resolution, *angle_p));
      }
    }
    if (_noprompt_only || (!_prompt_only && !_noprompt_only)){
      RooRealVar *m_np_a1 = new RooRealVar("m_np_a1", "m_np_a1", 0);
      RooRealVar *m_np_a2 = new RooRealVar("m_np_a2", "m_np_a2", 0);
      RooRealVar *t_m = new RooRealVar("t_m", "t_m", 0);
      RooRealVar *t_p = new RooRealVar("t_p", "t_p", 0);
      RooRealVar *t_pp = new RooRealVar("t_pp", "t_pp", 0);
      RooRealVar *fm = new RooRealVar("fm", "fm", 0);
      RooRealVar *fp = new RooRealVar("fp", "fp", 0);
      RooRealVar *z1 = new RooRealVar("z1", "z1", 0);
      RooRealVar *z2 = new RooRealVar("z2", "z2", 0);
      RooRealVar *z3 = new RooRealVar("z3", "z3", 0);
      //      RooRealVar *z4 = new RooRealVar("z4", "z4", 0);
      //      RooRealVar *z5 = new RooRealVar("z5", "z5", 0);
      //      RooRealVar *z6 = new RooRealVar("z6", "z6", 0);
      _parameters->add(*m_np_a1);
      _parameters->add(*m_np_a2);
      _parameters->add(*t_m);
      _parameters->add(*t_p);
      _parameters->add(*t_pp);
      _parameters->add(*fm);
      _parameters->add(*fp);
      _parameters->add(*z1);
      _parameters->add(*z2);
      _parameters->add(*z3);
      //      _parameters->add(*z4);
      //      _parameters->add(*z5);
      //      _parameters->add(*z6);
      
      //      RooPolynomial *mass_np = new RooPolynomial("mass_np", "cuadratic mass noprompt", _m, RooArgSet(*m_np_a1, *m_np_a2));
      RooExponential *mass_np = new RooExponential("mass_np", "exp mass noprompt", _m, *m_np_a1);
      RooDecay *exp_minus = new RooDecay("exp_minus", "exp minus", _t, *t_m, *_resolution, RooDecay::Flipped);
      RooDecay *exp_plus = new RooDecay("exp_plus", "exp plus", _t, *t_p, *_resolution, RooDecay::SingleSided);
      RooDecay *exp_plus_plus = new RooDecay("exp_plus_plus", "exp plus plus", _t, *t_pp, *_resolution, RooDecay::SingleSided);
      RooAddPdf *time_np = new RooAddPdf("time_np", "time no propmt",
					 RooArgSet(*exp_minus, *exp_plus, *exp_plus_plus), RooArgSet(*fm, *fp));
      
      if (_use_phis) {
        //        RooBkgAngle<Phis,TransAnglesPhis> *angle_np = new RooBkgAngle<Phis,TransAnglesPhis> ("angle_np", "angle noprompt", _cpsi, _ctheta, _phi,
        //                *z1, *z2, *z3, *z4, *z5, *z6, *_phis);
        //        _noprompt = new RooProdPdf("_noprompt", "noprompt bkg", RooArgSet(*exp_mass_np, *time_np, *angle_np));
      } else {
        RooPolynomial *cpsi_model_np = new RooPolynomial("cpsi_model_p", "cpsi_model_p", _cpsi, RooArgSet(*z1));
        RooPolynomial *ctheta_model_np = new RooPolynomial("ctheta_model_p", "ctheta_model_p", _ctheta, RooArgSet(*z2));
        RooPolynomial *phi_model_np = new RooPolynomial("phi_model_p", "phi_model_p", _phi, RooArgSet(*z3));
	//       RooGenericPdf *phi_model_np = new RooGenericPdf("phi_model_p", "phi_model", "1+@1*sin(@0*@2+@3)", RooArgList(_phi, *z4, *z5, *z6));
        RooProdPdf *angle_np = new RooProdPdf("angle_p", "angle_p", RooArgSet(*cpsi_model_np, *ctheta_model_np, *phi_model_np));
        _noprompt = new RooProdPdf("_noprompt", "noprompt bkg", RooArgSet(*mass_np, *time_np, *angle_np));
      }
    }

    if (!_prompt_only && !_noprompt_only){
      RooRealVar *xp = new RooRealVar("xp", "xp", 0);
      _parameters->add(*xp);
      
      RooAddPdf *bkg = new RooAddPdf("bkg", "background", *_prompt, *_noprompt, *xp);
      
      RooRealVar *et_bkg_xl = new RooRealVar("et_bkg_xl", "xl bkg", 0);
      RooRealVar *et_bkg_mean = new RooRealVar("et_bkg_mean", "mean bkg", 0);
      RooRealVar *et_bkg_sigma = new RooRealVar("et_bkg_sigma", "sigma bkg", 0);
      RooRealVar *et_bkg_tau_short = new RooRealVar("et_bkg_tau_short", "#tau short bkg", 0);
      RooRealVar *et_bkg_tau_long = new RooRealVar("et_bkg_tau_long", "#tau long bkg", 0);
      _parameters->add(*et_bkg_xl);
      _parameters->add(*et_bkg_mean);
      _parameters->add(*et_bkg_sigma);
      _parameters->add(*et_bkg_tau_short);
      _parameters->add(*et_bkg_tau_long);
      
      RooGaussModel *et_bkg_gauss = new RooGaussModel("gauss", "gauss", _et, *et_bkg_mean, *et_bkg_sigma);
      RooDecay *et_bkg_short = new RooDecay("et_bkg_short", "short bkg", _et, *et_bkg_tau_short, *et_bkg_gauss, RooDecay::SingleSided);
      RooDecay *et_bkg_long = new RooDecay("et_bkg_long", "long bgk", _et, *et_bkg_tau_long, *et_bkg_gauss, RooDecay::SingleSided);
      RooAddPdf *et_bkg_model = new RooAddPdf("et_bkg_model", "et bkg model", *et_bkg_long, *et_bkg_short, *et_bkg_xl);
      
      RooProdPdf *background = new RooProdPdf("background", "background x et_model_p",
					      *et_bkg_model, RooFit::Conditional(*bkg, RooArgSet(_m, _t, _cpsi, _ctheta, _phi)));
      return background;
    } 	
   
    if (_prompt_only && _noprompt_only){
      RooRealVar *xp = new RooRealVar("xp", "xp", 0);
      _parameters->add(*xp);
      RooAddPdf *bkg = new RooAddPdf("bkg", "background", *_prompt, *_noprompt, *xp);
      return bkg;
    } 	
    
    if (_prompt_only)
      return _prompt;
    return _noprompt;
}
