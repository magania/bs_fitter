#include <RooDataSet.h>
#include <RooAbsRealLValue.h>
#include <RooAbsRealLValue.h>
#include <RooDataSet.h>

#include "RooBkgAngle.h"

#include <TRandom3.h>
#include <RooDataSet.h>

#include <RooGlobalFunc.h>
#include <RooRealVar.h>

#include "BsFitter.h"
#include "Efficiency.h"
#include "RooBsTimeAngle.cc"
//#include "RooBkgAngle.cc"

BsFitter::BsFitter(Bool_t use_resolution, Bool_t signal_only, Bool_t sidebands, Bool_t use_efficiency, Bool_t use_phis) :
_m("_m", "m", 0, 5.1, 5.7),
_t("_t", "t", 0, -2, 12),
_et("_et", "et", 0, 0, 1),
_cpsi("_cpsi", "cos(#psi)", 0, -1, 1),
_ctheta("_ctheta", "cos(#theta)", 0, -1, 1),
_phi("_phi", "#phi", 0, -TMath::Pi(), TMath::Pi()),
_p("_p", "bs probability", 0, 0, 1) {
    gROOT->SetStyle("Plain");

    _use_resolution = use_resolution;
    _signal_only = signal_only;
    _sidebands = sidebands;
    _use_efficiency = use_efficiency;
    _use_phis = use_phis;
     const char *range = "full";
     _range = range;
    
    _resolution = 0;
    _model = 0;
    _data = 0;
    _fit_result = 0;

//    _t.setRange("np", -1,12);
//    const char *range_np = "np";
//    _range = range_np;

    _variables = new RooArgSet();
    _parameters = new RooArgSet();

    _variables->add(_m);
    _variables->add(_t);
    if (_use_resolution)
        _variables->add(_et);
    _variables->add(_cpsi);
    _variables->add(_ctheta);
    _variables->add(_phi);
    _variables->add(_p);

    if (_use_phis){
        _phis = new Phis();
    }  else {
        if (_use_efficiency)
            _efficiency = new Efficiency();
    }
    
    if ( _use_resolution ) {
        RooRealVar *S = new RooRealVar("S", "S", 0);
        _parameters->add(*S);
        _resolution = new RooGaussModel("_resolution", "gauss resolution", _t, RooFit::RooConst(0), *S, _et);
    } else {
        _resolution = new RooTruthModel("_resolution", "truth resolution", _t);
    }

 //   RooAbsPdf *signal = 0;
 //   RooAbsPdf *background = 0;
    if (!_sidebands)
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

//void BsFitter::set_variables(const char* vars) {
//    _variables->readFromFile(vars);
//    std::cout << "XRANGE: " << _cpsi.getMax() << " " << _cpsi.getMin() << std::endl;
//    _cpsi.setMax(1);
//    _cpsi.setMin(-1);
//    _ctheta.setMax(1);
//    _ctheta.setMin(-1);
//    _phi.setMax(3.1415);
//    _phi.setMin(-3.1416);
//}

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
    if (_sidebands) {
        RooDataSet *data = RooDataSet::read(data_file, *_variables);
        RooRealVar M("M", "M", 5.365081e+00, 5.28, 5.44);
        RooRealVar sigma("sigma", "sigma", 2.85725e-02, 0, 1);
        RooRealVar tau_bkg("tau_bkg", "tau_bkg", -6.11910e-01, -1.0, 0.0);
        RooRealVar xs("xs", "xs", 3.85194e-02, 0, 1);

        RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass", _m, M, sigma);
        RooExponential *bkg_mass = new RooExponential("bkg_mass", "bkg_mass", _m, tau_bkg);
        RooAddPdf *cut_model = new RooAddPdf("cut_model", "cut_model", *signal_mass, *bkg_mass, xs);
	// cut_model->fitTo(*data);

        Double_t left = M.getVal() - 3 * sigma.getVal();
        Double_t right = M.getVal() + 3 * sigma.getVal();

        TString cut = "_m < ";
        cut += left;
        cut += " || _m > ";
        cut += right;
        cout << "Cut: " << cut << endl;
        _data = new RooDataSet("_data","data", data, *_variables, cut);
        delete data;
        _m.setRange("left", _m.getMin(), left);
        _m.setRange("right", right, _m.getMax());
        const char* sideband_range = "left,right";
        _range = sideband_range;
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
    if (_use_resolution) {
        RooPolynomial p_model("p_model", "p_model",_p, RooFit::RooConst(0));
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
    if (_use_resolution) {
        _fit_result = _model->fitTo(*_data, RooFit::ConditionalObservables(RooArgSet(_et,_p)),
                RooFit::Save(kTRUE), RooFit::Hesse(hesse),
                RooFit::Minos(minos), RooFit::NumCPU(cpu),
                RooFit::Verbose(verbose)/*, RooFit::Range(_range)*/);
    } else {
        _fit_result = _model->fitTo(*_data, RooFit::ConditionalObservables(RooArgSet(_p)),
                RooFit::Save(kTRUE), RooFit::Hesse(hesse),
                RooFit::Minos(minos), RooFit::NumCPU(cpu),
                RooFit::Verbose(verbose)/*, RooFit::Range(_range)*/);
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

    if (_data) {
        if (_use_resolution) {
            if (proj_bins) {
                _et.setBins(proj_bins);
                RooDataHist projData("projData", "projData", RooArgSet(_et, _p), *_data);
                cout << "RANGE: " << _range << endl;
                _model->plotOn(x_frame, RooFit::Components(*_signal), RooFit::ProjWData(RooArgSet(_et, _p), projData), RooFit::LineColor(kGreen));
                _model->plotOn(x_frame, RooFit::Components(*_prompt), RooFit::ProjWData(RooArgSet(_et, _p), projData), RooFit::LineColor(99));
                _model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(_et, _p), projData), RooFit::LineColor(54));
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

    RooRealVar *et_mean = new RooRealVar("et_mean", "mean time error", 0);
    RooRealVar *et_sigma = new RooRealVar("et_sigma", "#sigma time error", 0);
    _parameters->add(*et_mean);
    _parameters->add(*et_sigma);

    RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass", _m, *M, *Sigma);

    RooAddPdf *signal_time_angle;
    if (_use_phis) {
        RooBsTimeAngle<Phis,TransAnglesPhis>* time_angle_bs = new RooBsTimeAngle<Phis,TransAnglesPhis>("time_angle_bs", "bs time angle pdf", kTRUE,
                _t, _cpsi, _ctheta, _phi,
                *A02, *All2, *DeltaGamma, *Tau,
                *DeltaMs, *Phi_s, *Delta_1, *Delta_2,
                *_resolution, *_phis);

        RooBsTimeAngle<Phis,TransAnglesPhis>* time_angle_bsbar = new RooBsTimeAngle<Phis,TransAnglesPhis>("time_angle_bsbar", "bs bar time angle pdf", kFALSE,
                _t, _cpsi, _ctheta, _phi,
                *A02, *All2, *DeltaGamma, *Tau,
                *DeltaMs, *Phi_s, *Delta_1, *Delta_2,
                *_resolution, *_phis);
        signal_time_angle = new RooAddPdf("signal_time_angle", "signal time-angular pdf",
                *time_angle_bs, *time_angle_bsbar, _p);
    } else {
        RooBsTimeAngle<Efficiency,TransAnglesEfficiency>* time_angle_bs = new RooBsTimeAngle<Efficiency,TransAnglesEfficiency>("time_angle_bs", "bs time angle pdf", kTRUE,
                _t, _cpsi, _ctheta, _phi,
                *A02, *All2, *DeltaGamma, *Tau,
                *DeltaMs, *Phi_s, *Delta_1, *Delta_2,
                *_resolution, *_efficiency);

        RooBsTimeAngle<Efficiency,TransAnglesEfficiency>* time_angle_bsbar = new RooBsTimeAngle<Efficiency,TransAnglesEfficiency>("time_angle_bsbar", "bs bar time angle pdf", kFALSE,
                _t, _cpsi, _ctheta, _phi,
                *A02, *All2, *DeltaGamma, *Tau,
                *DeltaMs, *Phi_s, *Delta_1, *Delta_2,
                *_resolution, *_efficiency);
        signal_time_angle = new RooAddPdf("signal_time_angle", "signal time-angular pdf",
                *time_angle_bs, *time_angle_bsbar, _p);
    }

    RooProdPdf * signal = 0;
    if (_use_resolution) {
//        RooPolynomial *p_model = new RooPolynomial("p_model", "p_model", _p, RooFit::RooConst(0));
        RooLandau *et_model = new RooLandau("et_model", "time error model", _et, *et_mean, *et_sigma);
        signal = new RooProdPdf("signal", "signal",
                RooArgSet(*signal_mass, *signal_time_angle)/*, RooFit::Conditional(*et_model, _et)/*, RooFit::Conditional(*p_model, _p)*/);
    } else {
        signal = new RooProdPdf("signal", "signal", RooArgSet(*signal_mass, *signal_time_angle));
    }

//    return 0;
    return signal;
}

RooAbsPdf* BsFitter::background_model() {
    cout << "BACKGROUND MODEL" << endl;
    RooRealVar *tm_p = new RooRealVar("tm_p", "tm_p", 0);
    RooRealVar *y1 = new RooRealVar("y1", "y1", 0);
    RooRealVar *y2 = new RooRealVar("y2", "y2", 0);
    RooRealVar *y3 = new RooRealVar("y3", "y3", 0);
    RooRealVar *y4 = new RooRealVar("y4", "y4", 0);
    RooRealVar *y5 = new RooRealVar("y5", "y5", 0);
    RooRealVar *y6 = new RooRealVar("y6", "y6", 0);
    _parameters->add(*tm_p);
    _parameters->add(*y1);
    _parameters->add(*y2);
    _parameters->add(*y3);
    _parameters->add(*y4);
    _parameters->add(*y5);
    _parameters->add(*y6);
    
    //RooExponential *exp_mass_p = new RooExponential("exp_mass_p","exp mass prompt", _m, *tm_p);
    RooPolynomial *exp_mass_p = new RooPolynomial("exp_mass_p", "pol mass prompt", _m, RooArgSet(*tm_p));
    if (_use_phis) {
      //        RooBkgAngle<Phis,TransAnglesPhis> *angle_p = new RooBkgAngle<Phis,TransAnglesPhis>("angle_p", "angle prompt", _cpsi, _ctheta, _phi,
      //                *y1, *y2, *y3, *y4, *y5, *y6, *_phis);
      //        _prompt = new RooProdPdf("_prompt", "prompt bkg", RooArgSet(*exp_mass_p, *_resolution, *angle_p));
    } else {
      RooPolynomial *cpsi_model_p = new RooPolynomial("cpsi_model_p", "cpsi_model_p", _cpsi, RooArgSet(RooFit::RooConst(0),*y1));
      RooPolynomial *ctheta_model_p = new RooPolynomial("ctheta_model_p", "ctheta_model_p", _ctheta, RooArgSet(RooFit::RooConst(0),*y2));
      RooGenericPdf *phi_model_p = new RooGenericPdf("phi_model_p", "phi_model", "1+@1*sin(@0*@2+@3)",RooArgList(_phi,*y4,*y5,*y6));
      RooProdPdf *angle_p = new RooProdPdf("angle_p", "angle_p", RooArgSet(*cpsi_model_p, *ctheta_model_p, *phi_model_p));
      _prompt = new RooProdPdf("_prompt", "prompt bkg", RooArgSet(*exp_mass_p, *_resolution, *angle_p));
    }

    RooRealVar *tm_np = new RooRealVar("tm_np", "tm_np",0);
    RooRealVar *t_m = new RooRealVar("t_m", "t_m", 0);
    RooRealVar *t_p = new RooRealVar("t_p", "t_p", 0);
    RooRealVar *t_pp = new RooRealVar("t_pp", "t_pp", 0);
    RooRealVar *fm = new RooRealVar("fm", "fm", 0);
    RooRealVar *fp = new RooRealVar("fp", "fp", 0);
    RooRealVar *z1 = new RooRealVar("z1", "z1", 0);
    RooRealVar *z2 = new RooRealVar("z2", "z2", 0);
    RooRealVar *z3 = new RooRealVar("z3", "z3", 0);
    RooRealVar *z4 = new RooRealVar("z4", "z4", 0);
    RooRealVar *z5 = new RooRealVar("z5", "z5", 0);
    RooRealVar *z6 = new RooRealVar("z6", "z6", 0);
    _parameters->add(*tm_np);
    _parameters->add(*t_m);
    _parameters->add(*t_p);
    _parameters->add(*t_pp);
    _parameters->add(*fm);
    _parameters->add(*fp);
    _parameters->add(*z1);
    _parameters->add(*z2);
    _parameters->add(*z3);
    _parameters->add(*z4);
    _parameters->add(*z5);
    _parameters->add(*z6);

    RooExponential *exp_mass_np = new RooExponential("exp_mass_np", "exp mass noprompt", _m, *tm_np);
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
      RooPolynomial *cpsi_model_np = new RooPolynomial("cpsi_model_p", "cpsi_model_p", _cpsi, RooArgSet(RooFit::RooConst(0),*z1));
      RooPolynomial *ctheta_model_np = new RooPolynomial("ctheta_model_p", "ctheta_model_p", _ctheta, RooArgSet(RooFit::RooConst(0),*z2,*z3));
      RooGenericPdf *phi_model_np = new RooGenericPdf("phi_model_p", "phi_model", "1+@1*sin(@0*@2+@3)",RooArgList(_phi,*z4,*z5,*z6));
      RooProdPdf *angle_np = new RooProdPdf("angle_p", "angle_p", RooArgSet(*cpsi_model_np, *ctheta_model_np, *phi_model_np));
      _noprompt = new RooProdPdf("_noprompt", "noprompt bkg", RooArgSet(*exp_mass_np, *time_np, *angle_np));
    }
            
    RooRealVar *xp = new RooRealVar("xp","xp",0);
    _parameters->add(*xp);
    
       RooAddPdf *background = new RooAddPdf("bkg", "background", *_prompt, *_noprompt, *xp);
    //   RooProdPdf *background = _noprompt;
    

/*    RooRealVar *et_mean_b = new RooRealVar("et_mean_b", "mean time error", 0);
    RooRealVar *et_sigma_b = new RooRealVar("et_sigma_b", "#sigma time error", 0);
    _parameters->add(*et_mean_b);
    _parameters->add(*et_sigma_b);

    RooLandau *et_model_b = new RooLandau("et_model_b", "time bkg error model", _et, *et_mean_b, *et_sigma_b);*/
    
//i    RooProdPdf *background = new RooProdPdf("background", "background x et_model_p", RooArgSet(*bkg)/*, RooFit::Conditional(*et_model_b, _et)*/);
    return _noprompt;     
    return background;
}
