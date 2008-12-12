
//#include <root/RooDataSet.h>
//#include <root/RooDecay.h>

fit_et() {
    RooRealVar _m("_m", "m", 5.1, 5.7);
    RooRealVar _t("_t", "t", 0, -2, 12);
    RooRealVar _et("_et", "et", 0, 0, 1);
    RooRealVar _cpsi("_cpsi", "cos(#psi)", 0, -1, 1);
    RooRealVar _ctheta("_ctheta", "cos(#theta)", 0, -1, 1);
    RooRealVar _phi("_phi", "#phi", 0, -TMath::Pi(), TMath::Pi());
    RooRealVar _p("_p", "bs probability", 0, 0, 1);

    RooArgSet *variables = new RooArgSet(_m, _t, _et, _cpsi, _ctheta, _phi, _p);

    TTree tree("tree", "tree");
    tree.ReadFile("fit.dat", "_m/D:_t:_et:_cpsi:_ctheta:_phi:_p");
    RooDataSet data("data", "data", &tree, *variables);

    RooRealVar M("M", "M", 5.36508e+00, 5.28, 5.44);
    RooRealVar sigma("sigma", "sigma", 2.85741e-02, 0, 1);
    RooRealVar tau_bkg("tau_bkg", "tau_bkg",  -6.11868e-01, -1, 0);
    RooRealVar xs("xs", "xs",  3.85161e-02, 0, 1);

    RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass", _m, M, sigma);
    RooExponential *bkg_mass = new RooExponential("bkg_mass", "bkg_mass", _m, tau_bkg);
    RooAddPdf *cut_model = new RooAddPdf("cut_model", "cut_model", *signal_mass, *bkg_mass, xs);
    cut_model->fitTo(data);

    Double_t left = M.getVal() - 3 * sigma.getVal();
    Double_t right = M.getVal() + 3 * sigma.getVal();
    Double_t tau_b = tau_bkg.getVal();
    Double_t bkg_under_signal = (1 / tau_b) * (exp(tau_b * right) - exp(tau_b * left));
    Double_t m_right = (1 / tau_b) * log(exp(tau_b * right) + bkg_under_signal * tau_b);
    Double_t m_left = (1 / tau_b) * log(exp(tau_b * left) - bkg_under_signal * tau_b);

    cout << bkg_under_signal << endl;
    cout << m_left << ' ' << left << ' ' << right << ' ' << m_right << endl;

    _m.setMin(m_left);
    _m.setMax(m_right);
    _m.setRange("left", m_left, left);
    _m.setRange("right", right, m_right);

    TString scut = "_m>";
    scut+=left;
    scut+=" && _m<";
    scut+=right;
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
    
    //    delete data;
    RooDataSet dataeq("dataeq", "dataeq", &tree, *variables);
    RooDataSet sidebands("sidebands", "sidebands", &tree, *variables, cut);
    RooDataSet signal("signal", "signal", &tree, *variables, scut);
    cut_model->fitTo(dataeq);

    RooPlot *m_frame = _m.frame();
    dataeq.plotOn(m_frame);
    cut_model->plotOn(m_frame, RooFit::LineColor(13));
    TCanvas *canvas_m = new TCanvas("canvas_m", "canvas mass", 600, 600);
    m_frame->Draw();

    RooRealVar et_bkg_xl("et_bkg_xl", "xl bkg",0.7, 0, 1);
    RooRealVar et_bkg_mean("et_bkg_mean", "mean bkg",0.06, 0, 1);
    RooRealVar et_bkg_sigma("et_bkg_sigma", "sigma bkg",0.01, 0, 0.1);
    RooRealVar et_bkg_tau_short("et_bkg_tau_short", "#tau short bkg", 0.05, 0, 0.5);
    RooRealVar et_bkg_tau_long("et_bkg_tau_long", "#tau long bkg", 0.05 ,0 , 0.1);
RooArgSet et_parameters(et_bkg_xl, et_bkg_mean, et_bkg_sigma, et_bkg_tau_short, et_bkg_tau_long);

    RooGaussModel et_bkg_gauss("gauss", "gauss", _et, et_bkg_mean, et_bkg_sigma);
    RooDecay et_bkg_short("et_bkg_short", "short bkg", _et, et_bkg_tau_short,
			  et_bkg_gauss, RooDecay::SingleSided);
    RooDecay et_bkg_long("et_bkg_long", "long bgk", _et, et_bkg_tau_long,
			 et_bkg_gauss, RooDecay::SingleSided);
    RooAddPdf et_bkg_model("et_bkg_model", "et bkg model", et_bkg_long, et_bkg_short, et_bkg_xl);

    et_bkg_model.fitTo(sidebands,RooFit::NumCPU(4));

    RooPlot *et_frame = _et.frame();
    sidebands.plotOn(et_frame);
    et_bkg_model.plotOn(et_frame, RooFit::Components(et_bkg_short) ,RooFit::LineColor(kRed));
    et_bkg_model.plotOn(et_frame, RooFit::Components(et_bkg_long) ,RooFit::LineColor(kBlue));
    et_bkg_model.plotOn(et_frame, RooFit::LineColor(13));
    TCanvas *canvassb = new TCanvas("canvassb", "canvas sidebands", 600, 600);
    et_frame->Draw();

    et_bkg_xl.setConstant(true);
    et_bkg_mean.setConstant(true);
    et_bkg_sigma.setConstant(true);
    et_bkg_tau_short.setConstant(true);
    et_bkg_tau_long.setConstant(true);
    xs.setConstant(true);

    RooRealVar et_sig_xl("et_sig_xl", "xl sig",0.7, 0, 1);
    RooRealVar et_sig_mean("et_sig_mean", "mean sig",0.06, 0, 1);
    RooRealVar et_sig_sigma("et_sig_sigma", "sigma sig",0.01, 0, 0.1);
    RooRealVar et_sig_tau_short("et_sig_tau_short", "#tau short sig", 0.05, 0, 0.5);
    RooRealVar et_sig_tau_long("et_sig_tau_long", "#tau long sig", 0.05 ,0 , 0.1);
    et_parameters.add(et_sig_xl);
    et_parameters.add(et_sig_mean);
    et_parameters.add(et_sig_sigma);
    et_parameters.add(et_sig_tau_short);
    et_parameters.add(et_sig_tau_long);

    RooGaussModel et_sig_gauss("gauss", "gauss", _et, et_sig_mean, et_sig_sigma);
    RooDecay et_sig_short("et_sig_short", "short sig", _et, et_sig_tau_short,
			  et_sig_gauss, RooDecay::SingleSided);
    RooDecay et_sig_long("et_sig_long", "long bgk", _et, et_sig_tau_long,
			 et_sig_gauss, RooDecay::SingleSided);
    RooAddPdf et_sig_model("et_sig_model", "et sig model", et_sig_long, et_sig_short, et_sig_xl);

    RooAddPdf et_model("et_model", "et model", et_sig_model, et_bkg_model, xs);

    xs = xs.getVal()/(bkg_under_signal+xs.getVal());
    et_model.fitTo(signal,RooFit::Minos(false),RooFit::Hesse(false));
    //    et_parameters.writeToFile("et_parameters.txt");

    TCanvas *canvasres = new TCanvas("canvares", "canvas sidebands", 600, 600);
    tree.Draw("_et>>sb", cut);
    tree.Draw("_et>>sg", scut);
    TH1D *hsg = (TH1D*)gROOT->Get("sg");
    TH1D *hsb = (TH1D*)gROOT->Get("sb");
    hsg->Sumw2();
    hsg->Add(hsb, -(1-xs.getVal()));
    hsg->Draw();
    RooDataHist rhsg("rhsg", "signal", _et, hsg);

    //    et_sig_model.fitTo(rhsg,RooFit::Minos(false),RooFit::Hesse(false));

    et_parameters.writeToFile("et_parameters.txt");
    RooPlot *et_frame2 = _et.frame();
    rhsg.plotOn(et_frame2);
    //    data->plotOn(et_frame2);
    //    et_model->plotOn(et_frame2, RooFit::Components(et_bkg_model) ,RooFit::LineColor(kRed));
    et_sig_model.plotOn(et_frame2, RooFit::LineColor(13));
    TCanvas *canvas2 = new TCanvas("canvas2", "canvas signal", 600, 600);
    et_frame2->Draw();
}
