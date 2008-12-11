
#include <root/RooDataSet.h>
#include <root/RooDecay.h>

fit_et() {
    RooRealVar _m("_m", "m", 5.1, 5.7);
    RooRealVar _t("_t", "t", 0, -2, 12);
    RooRealVar _et("_et", "et", 0, 0, 1);
    RooRealVar _cpsi("_cpsi", "cos(#psi)", 0, -1, 1);
    RooRealVar _ctheta("_ctheta", "cos(#theta)", 0, -1, 1);
    RooRealVar _phi("_phi", "#phi", 0, -TMath::Pi(), TMath::Pi());
    RooRealVar _p("_p", "bs probability", 0, 0, 1);

    RooArgSet *variables = new RooArgSet(_m, _t, _et, _cpsi, _ctheta, _phi, _p);
    RooDataSet *data = RooDataSet::read("bin/fit.dat", *variables);

    RooRealVar M("M", "M", 5.36508e+00, 5.28, 5.44);
    RooRealVar sigma("sigma", "sigma", 2.85741e-02, 0, 1);
    RooRealVar tau_bkg("tau_bkg", "tau_bkg",  -6.11868e-01, -1, 0);
    RooRealVar xs("xs", "xs",  3.85161e-02, 0, 1);

    RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass", _m, M, sigma);
    RooExponential *bkg_mass = new RooExponential("bkg_mass", "bkg_mass", _m, tau_bkg);
    RooAddPdf *cut_model = new RooAddPdf("cut_model", "cut_model", *signal_mass, *bkg_mass, xs);
   // cut_model->fitTo(*data);

    Double_t left = M.getVal() - 3 * sigma.getVal();
    Double_t right = M.getVal() + 3 * sigma.getVal();
    Double_t tau_b = tau_bkg.getVal();
    Double_t bkg_under_signal = (1 / tau_b) * (exp(tau_b * right) - exp(tau_b * left));
    Double_t m_right = (1 / tau_b) * log(exp(tau_b * right) + bkg_under_signal * tau_b);
    Double_t m_left = (1 / tau_b) * log(exp(tau_b * left) - bkg_under_signal * tau_b);

    cout << m_left << ' ' << left << ' ' << right << ' ' << m_right << endl;

    _m.setMin(m_left);
    _m.setMax(m_right);
    _m.setRange("left", m_left, left);
    _m.setRange("right", right, m_right);

    //   TString cut = "m>";
    //   cut+=m_left;
    //   cut+=" && m<";
    //   cut+=m_right;
    TString cut = "((_m>";
    cut += m_left;
    cut += " && _m<";
    cut += left;
    cut += ") || (_m>";
    cut += right;
    cut += " && _m<";
    cut += m_right;
    cut += "))";
    //  cut+="&& (t/et >5)";

    cout << cut << endl;
    
    data = new RooDataSet("data", "data", data, *variables, cut);
    RooPlot *m_frame = _m.frame();
    data->plotOn(m_frame);
    cut_model->plotOn(m_frame, RooFit::LineColor(13));
    TCanvas *canvas = new TCanvas("canvas", "canvas", 600, 600);
    m_frame->Draw();

    RooRealVar *xl = new RooRealVar("xl", "xl", 0, 1);
    RooRealVar *mean = new RooRealVar("mean", "mean", 0, 1);
    RooRealVar *e_sigma = new RooRealVar("sigma", "sigma", 0, 1);
    RooRealVar *tau_1 = new RooRealVar("tau_1", "tau_1", 0, 10);
    RooRealVar *tau_2 = new RooRealVar("tau_2", "tau_2", 0, 10);

    RooGaussModel *gauss = new RooGaussModel("gauss", "gauss", _et, RooFit::RooConst(0), *e_sigma);
    RooDecay *e_short = new RooDecay("e_short", "e_short", _et, *tau_1, *gauss, RooDecay::SingleSided);
    RooDecay *e_long = new RooDecay("e_short", "e_short", _et, *tau_2, *gauss, RooDecay::SingleSided);
    RooAddPdf *et_model = new RooAddPdf("et_model", "et model", *e_short, *e_long, *xl);

//    RooFFTConvPdf *et_model = new RooFFTConvPdf("et_model","landau (X) gauss",_et,*landau_1,*gauss_2) ;
//    RooAbsPdf *et_model = landau_1;

    et_model->fitTo(*data);

    RooPlot *et_frame = _et.frame();
    data->plotOn(et_frame);
    et_model->plotOn(et_frame, RooFit::LineColor(13));
    TCanvas *canvas = new TCanvas("canvas", "canvas", 600, 600);
    et_frame->Draw();
}
