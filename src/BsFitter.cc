
#include "RooBkgAngle.h"


#include <root/TRandom3.h>


#include <root/RooDataSet.h>

#include <RooGlobalFunc.h>
#include <RooRealVar.h>

#include "BsFitter.h"
#include "Efficiency.h"

BsFitter::BsFitter() :
_m("_m", "m", 0),
_t("_t", "t", 0),
#ifndef RES_TRUE
_et("_et", "et", 0),
#endif
_cpsi("_cpsi", "cos(#psi)", 0),
_ctheta("_ctheta", "cos(#theta)", 0),
_phi("_phi", "#phi", 0),
_d("_d", "dilution", 0) {
    gROOT->SetStyle("Plain");

    resolution = 0;
    model = 0;
    data = 0;
    fit_result = 0;

    cout << "HERE" << endl;
    variables = new RooArgSet();
    parameters = new RooArgSet();

    variables->add(_m);
    variables->add(_t);
#ifndef RES_TRUE
    variables->add(_et);
#endif
    variables->add(_cpsi);
    variables->add(_ctheta);
    variables->add(_phi);
    variables->add(_d);

#ifdef RES_GAUSS
    RooRealVar *S = new RooRealVar("S", "S", 0);
    parameters->add(*S);
    resolution = new RooGaussModel("resolution", "gauss resolution", _t, RooFit::RooConst(0), *S, _et);
#endif

#ifdef RES_TRUE
    resolution = new RooTruthModel("resolution", "truth resolution", _t);
#endif

    RooAbsPdf* signal = signal_model();
    RooAbsPdf* background = background_model();

    if (signal && background) {
        RooRealVar *xs = new RooRealVar("xs", "x_s", 0);
        parameters->add(*xs);
        model = new RooAddPdf("model", "Bs PDF", *signal, *background, *xs);
    } else {
        if (signal)
            model = signal;
        else
            model = background;
    }

    if (!model)
        cout << "EE: No Hay modelo de se~al o background" << endl;

}

void BsFitter::setVariables(const char* vars) {
    variables->readFromFile(vars);
    std::cout << "XRANGE: " << _cpsi.getMax() << " " << _cpsi.getMin() << std::endl;
    _cpsi.setMax(1);
    _cpsi.setMin(-1);
    _ctheta.setMax(1);
    _ctheta.setMin(-1);
    _phi.setMax(3.1415);
    _phi.setMin(-3.1416);
}

void BsFitter::setParameters(const char* params) {
    parameters->readFromFile(params);
}

void BsFitter::setEfficiency(const char* eff) {
    efficiency->setEfficiency(eff);
}

void BsFitter::setData(const char* data_file) {
    data = RooDataSet::read(data_file, *variables);
}

void BsFitter::setData(RooDataSet* data_set) {
    data = data_set;
}

void BsFitter::setDataFromCin() {
    data = new RooDataSet("data", "data", *variables);

    Double_t m, t, cpsi, ctheta, phi, d;
    while (!cin.eof()) {
        cin >> m >> t >> cpsi >> ctheta >> phi >> d;
        cout << m << ' ' << t << ' ' << cpsi << ' ' << ctheta << ' ' << phi << ' ' << d << endl;
        _m = m;
        _t = t;
        _cpsi = cpsi;
        _ctheta = ctheta;
        _phi = phi;
        _d = d;
        data->add(*variables);
    }

}

void BsFitter::generate(Int_t num) {
#ifdef RES_TRUE
    return model->generate(RooArgSet(_m, _t, _cpsi, _ctheta, _phi, _d), num);
#endif

    RooDataSet *data_et = model->generate(RooArgSet(_et), num, kTRUE);
    data = model->generate(RooArgSet(_m, _t, _cpsi, _ctheta, _phi), *data_et);

    /*
#ifdef EFFICIENCY
    Double_t min = 0.65;
    RooDataSet *data_et = model->generate(RooArgSet(_et), num/min, kTRUE);
    RooDataSet *data_noeff = model->generate(RooArgSet(_m, _t, _cpsi, _ctheta, _phi), *data_et);

    data = new RooDataSet("data","data",*variables);
    TRandom3 aleatorio(0);
    for (int i=0; i<data_noeff->numEntries(); i++){
        *variables = *data_noeff->get(i);
        Double_t eff = efficiency->getVal();
        Double_t y = aleatorio.Rndm();
        if ((eff-min) <0 || (eff-min)>1){
            cout << "EE: eff=" << eff << " min=" << min << endl;
        }
        if (y < eff-min)
            data->add(*variables);
    }
#endif
     */
    cout << "Total Generated Entries: " << data->numEntries() << endl;
}

Int_t BsFitter::fit(Bool_t hesse, Bool_t minos, Bool_t verbose, Int_t cpu) {
    if (!data) {
        cout << "EE: No Data" << endl;
        return kFALSE;
    }

#ifdef RES_TRUE
    fit_result = model->fitTo(*data, RooFit::ConditionalObservables(RooArgSet(_d)),
            RooFit::Save(kTRUE), RooFit::Hesse(hesse),
            RooFit::Minos(minos), RooFit::NumCPU(cpu),
            RooFit::Verbose(verbose));
#endif
#ifndef RES_TRUE
    fit_result = model->fitTo(*data, RooFit::ConditionalObservables(RooArgSet(_et, _d)),
            RooFit::Save(kTRUE), RooFit::Hesse(hesse),
            RooFit::Minos(minos), RooFit::NumCPU(cpu),
            RooFit::Verbose(verbose));
#endif
    return fit_result->status();
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
    if (data)
        if (bins)
            data->plotOn(x_frame, RooFit::Binning(bins));
        else
            data->plotOn(x_frame);

    if (data) {
#ifdef RES_TRUE
        model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(_d), *data));
#endif
#ifndef RES_TRUE
        if (proj_bins) {
            _et.setBins(proj_bins);
            RooDataHist projData("projData", "projData", RooArgSet(_et, _d), *data);
            if (x == _m || x == _t)
            model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(_et, _d), projData));
        } else {
            model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(_et, _d), *data));
        }
#endif
    } else {
        model->plotOn(x_frame);
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
    fit_result->printMultiline(out, 0, kTRUE);
    out.close();
}

RooAbsPdf* BsFitter::signal_model() {
    cout << "SIGNAL MODEL" << endl;
    RooRealVar *M = new RooRealVar("M", "M", 0);
    RooRealVar *sigma = new RooRealVar("sigma", "#sigma", 0);

    RooRealVar *A02 = new RooRealVar("A02", "A02", 0);
    RooRealVar *All2 = new RooRealVar("All2", "A_{#parallel}(0)", 0);
    RooRealVar *DeltaGamma = new RooRealVar("DeltaGamma", "#Delta#Gamma", 0);
    RooRealVar *delta_1 = new RooRealVar("delta_1", "#delta_{1}", 0);
    RooRealVar *delta_2 = new RooRealVar("delta_2", "#delta_{2}", 0);
    RooRealVar *phi_s = new RooRealVar("phi_s", "#phi_{s}", 0);
    RooRealVar *tau = new RooRealVar("tau", "#tau", 0);
    RooRealVar *DeltaMs = new RooRealVar("DeltaMs", "#Delta M_{s}", 0);

    parameters->add(*M);
    parameters->add(*sigma);
    parameters->add(*A02);
    parameters->add(*All2);
    parameters->add(*DeltaGamma);
    parameters->add(*delta_1);
    parameters->add(*delta_2);
    parameters->add(*phi_s);
    parameters->add(*tau);
    parameters->add(*DeltaMs);

    RooRealVar *et_mean = new RooRealVar("et_mean", "mean time error", 0);
    RooRealVar *et_sigma = new RooRealVar("et_sigma", "#sigma time error", 0);
    parameters->add(*et_mean);
    parameters->add(*et_sigma);

    RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass", _m, *M, *sigma);

#ifdef EFFICIENCY
    efficiency = new Efficiency();
    RooBsTimeAngle *time_angle_bs = new RooBsTimeAngle("time_angle_bs", "bs time angle pdf", kTRUE,
            _t, _cpsi, _ctheta, _phi,
            *A02, *All2, *DeltaGamma, *tau,
            *DeltaMs, *phi_s, *delta_1, *delta_2,
            *resolution,efficiency, kTRUE);

    RooBsTimeAngle *time_angle_bsbar = new RooBsTimeAngle("time_angle_bsbar", "bs bar time angle pdf", kFALSE,
            _t, _cpsi, _ctheta, _phi,
            *A02, *All2, *DeltaGamma, *tau,
            *DeltaMs, *phi_s, *delta_1, *delta_2,
            *resolution, efficiency, kTRUE);
    RooAddPdf *signal_time_angle = new RooAddPdf("signal_time_angle", "signal time-angular pdf",
            *time_angle_bs, *time_angle_bsbar, _d);
#endif

#ifndef EFFICIENCY
    RooBsTimeAngle *time_angle_bs = new RooBsTimeAngle("time_angle_bs", "bs time angle pdf", kTRUE,
            _t, _cpsi, _ctheta, _phi,
            *A02, *All2, *DeltaGamma, *tau,
            *DeltaMs, *phi_s, *delta_1, *delta_2,
            *resolution);

    RooBsTimeAngle *time_angle_bsbar = new RooBsTimeAngle("time_angle_bsbar", "bs bar time angle pdf", kFALSE,
            _t, _cpsi, _ctheta, _phi,
            *A02, *All2, *DeltaGamma, *tau,
            *DeltaMs, *phi_s, *delta_1, *delta_2,
            *resolution);
    RooAddPdf *signal_time_angle = new RooAddPdf("signal_time_angle", "signal time-angular pdf",
            *time_angle_bs, *time_angle_bsbar, _d);
#endif

    RooLandau *et_model = new RooLandau("et_model", "time error model", _et, *et_mean, *et_sigma);

    RooProdPdf *signal = new RooProdPdf("signal", "signal",
            RooArgSet(*signal_mass, *signal_time_angle/**time_angle_bs*/), RooFit::Conditional(*et_model, _et));

//    return 0;
    return signal;
}

RooAbsPdf* BsFitter::background_model() {
//    return 0;
    cout << "Bkg mode" << endl;
    RooRealVar *tm_p = new RooRealVar("tm_p", "tm_p", 0);
    RooRealVar *B1_p = new RooRealVar("B1_p", "B1_p", 0);
    RooRealVar *B2_p = new RooRealVar("B2_p", "B2_p", 0);
    RooRealVar *B3_p = new RooRealVar("B3_p", "B3_p", 0);
    RooRealVar *B4_p = new RooRealVar("B4_p", "B4_p", 0);
    RooRealVar *B5_p = new RooRealVar("B5_p", "B5_p", 0);
    RooRealVar *B6_p = new RooRealVar("B6_p", "B6_p", 0);
    parameters->add(*tm_p);
    parameters->add(*B1_p);
    parameters->add(*B2_p);
    parameters->add(*B3_p);
    parameters->add(*B4_p);
    parameters->add(*B5_p);
    parameters->add(*B6_p);
    
    RooExponential *exp_mass_p = new RooExponential("exp_mass_p","exp mass prompt", _m, *tm_p);
    RooBkgAngle *angle_p= new RooBkgAngle("angle_p","angle prompt",_cpsi, _ctheta, _phi,
            *B1_p, *B2_p, *B3_p, *B4_p, *B5_p, *B6_p, efficiency);
    RooProdPdf *prompt = new RooProdPdf("prompt","prompt bkg", RooArgSet(*exp_mass_p, *resolution, *angle_p));

    RooRealVar *tm_np = new RooRealVar("tm_np", "tm_np",0);
    RooRealVar *t_m = new RooRealVar("t_m", "t_m", 0);
    RooRealVar *t_p = new RooRealVar("t_p", "t_p", 0);
    RooRealVar *t_pp = new RooRealVar("t_pp", "t_pp", 0);
    RooRealVar *fm = new RooRealVar("fm", "fm", 0);
    RooRealVar *fp = new RooRealVar("fp", "fp", 0);
    RooRealVar *B1_np = new RooRealVar("B1_np", "B1_np", 0);
    RooRealVar *B2_np = new RooRealVar("B2_np", "B2_np", 0);
    RooRealVar *B3_np = new RooRealVar("B3_np", "B3_np", 0);
    RooRealVar *B4_np = new RooRealVar("B4_np", "B4_np", 0);
    RooRealVar *B5_np = new RooRealVar("B5_np", "B5_np", 0);
    RooRealVar *B6_np = new RooRealVar("B6_np", "B6_np", 0);
    parameters->add(*tm_np);
    parameters->add(*t_m);
    parameters->add(*t_p);
    parameters->add(*t_pp);
    parameters->add(*fm);
    parameters->add(*fp);
    parameters->add(*B1_np);
    parameters->add(*B2_np);
    parameters->add(*B3_np);
    parameters->add(*B4_np);
    parameters->add(*B5_np);
    parameters->add(*B6_np);

    RooExponential *exp_mass_np = new RooExponential("exp_mass_np", "exp mass noprompt", _m, *tm_np);
    RooDecay *exp_minus = new RooDecay("exp_minus", "exp minus", _t, *t_m, *resolution, RooDecay::Flipped);
    RooDecay *exp_plus = new RooDecay("exp_plus", "exp plus", _t, *t_p, *resolution, RooDecay::SingleSided);
    RooDecay *exp_plus_plus = new RooDecay("exp_plus_plus", "exp plus plus", _t, *t_pp, *resolution, RooDecay::SingleSided);
    RooAddPdf *time_np = new RooAddPdf("time_np", "time no propmt",
            RooArgSet(*exp_minus, *exp_plus, *exp_plus_plus), RooArgSet(*fm, *fp));
    RooBkgAngle *angle_np = new RooBkgAngle("angle_np", "angle noprompt", _cpsi, _ctheta, _phi,
            *B1_np, *B2_np, *B3_np, *B4_np, *B5_np, *B6_np, efficiency);
    RooProdPdf *noprompt = new RooProdPdf("noprompt", "noprompt bkg", RooArgSet(*exp_mass_np, *time_np, *angle_np));
    
    RooRealVar *xp = new RooRealVar("xp","xp",0);
    parameters->add(*xp);
    RooAddPdf *background = new RooAddPdf("background", "background", *prompt, *noprompt, *xp);
            
    return background;
}
