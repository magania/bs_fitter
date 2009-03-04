
//#include <root/RooDataSet.h>
//#include <root/RooDecay.h>

void fit_et() {
	const char *data_file = "fit.dat";

    RooRealVar _m("_m", "m", 5.1, 5.7);
    RooRealVar _t("_t", "t", 0, -2, 12);
    RooRealVar _et("_et", "et", 0.01, 0, 1);
    RooRealVar _cpsi("_cpsi", "cos(#psi)", 0, -1, 1);
    RooRealVar _ctheta("_ctheta", "cos(#theta)", 0, -1, 1);
    RooRealVar _phi("_phi", "#phi", 0, -TMath::Pi(), TMath::Pi());
    RooRealVar _p("_p", "bs probability", 0, -1, 1);

    RooArgSet variables(_m, _t, _et, _cpsi, _ctheta, _phi, _p);

    TTree tree("tree", "tree");
    tree.ReadFile(data_file, "_m/D:_t:_et:_cpsi:_ctheta:_phi:_p");
    RooDataSet data("data", "data", &tree, variables);

	RooRealVar mean("mean", "mean", 5.37, 5.28, 5.44);
	RooRealVar sigma("sigma", "sigma", 0.038, 0.01, 0.1);
	RooRealVar m("m", "m", 0, -1, 1);
	RooRealVar xs("xs", "xs", 0.2, 0.01, 1);

	RooGaussian gauss("gauss", "gauss", _m, mean, sigma);
	RooPolynomial line("line", "line", _m, m);
	RooAddPdf model("model", "model", gauss, line, xs);
	model.fitTo(data,
			RooFit::Hesse(false),
			RooFit::Minos(false));

    RooPlot *m_frame = _m.frame();
    data.plotOn(m_frame);
    model->plotOn(m_frame, RooFit::LineColor(13));
    TCanvas *canvas_m = new TCanvas("canvas_m", "canvas mass", 600, 600);
    m_frame->Draw();
    canvas_m->Print("mass_cut.gif");

	Double_t a = _m.getMin();
	Double_t b = mean.getVal() - 3 * sigma.getVal();
	Double_t c = mean.getVal() + 3 * sigma.getVal();
	Double_t d = _m.getMax();

	Double_t all = (d-a) + m.getVal()*(d-a)*(d-a)/2;
	Double_t under = (c-b) + m.getVal()*(c-b)*(c-b)/2;
	Double_t u = (1-xs.getVal())*all/under;
	Double_t x_u = xs.getVal()/(xs.getVal() + u);

	TString bkg_cut = "_m < ";
	bkg_cut += b;
	bkg_cut += " || _m > ";
	bkg_cut += c;
	TString signal_cut = "_m > ";
	signal_cut += b;
	signal_cut += " && _m < ";
	signal_cut += c;

	cout << "Bkg Cut: " << bkg_cut << endl;
	cout << "Signal Cut: " << signal_cut << endl;

	RooDataSet signal_data("signal_data", "signal_data", &tree, variables, signal_cut);
	RooDataSet bkg_data("bkg_data", "bkg_data", &tree, variables, bkg_cut);

    RooRealVar et_bkg_xl("et_bkg_xl", "xl bkg", 0.1, 0, 1);
    RooRealVar et_bkg_mean("et_bkg_mean", "mean bkg", 0.06, 0, 1);
    RooRealVar et_bkg_sigma("et_bkg_sigma", "sigma bkg", 0.01, 0, 0.1);
    RooRealVar et_bkg_tau_short("et_bkg_tau_short", "#tau short bkg", 0.05, 0.001, 0.5);
    RooRealVar et_bkg_tau_long("et_bkg_tau_long", "#tau long bkg", 0.05, 0.001, 0.5);
    RooArgSet et_parameters(et_bkg_xl, et_bkg_mean, et_bkg_sigma, et_bkg_tau_short, et_bkg_tau_long);

    RooGaussModel et_bkg_gauss("gauss", "gauss", _et, et_bkg_mean, et_bkg_sigma);
    RooDecay et_bkg_short("et_bkg_short", "short bkg", _et, et_bkg_tau_short,
            et_bkg_gauss, RooDecay::SingleSided);
    RooDecay et_bkg_long("et_bkg_long", "long bgk", _et, et_bkg_tau_long,
            et_bkg_gauss, RooDecay::SingleSided);
    RooAddPdf et_bkg_model("et_bkg_model", "et bkg model", et_bkg_long, et_bkg_short, et_bkg_xl);

    et_bkg_model.fitTo(bkg_data,
    		RooFit::NumCPU(4),
    		RooFit::Minos(false),
    		RooFit::Hesse(false));

    RooPlot *et_frame = _et.frame();
    bkg_data.plotOn(et_frame);
    et_bkg_model.plotOn(et_frame, RooFit::Components(et_bkg_short), RooFit::LineColor(kRed));
    et_bkg_model.plotOn(et_frame, RooFit::Components(et_bkg_long), RooFit::LineColor(kBlue));
    et_bkg_model.plotOn(et_frame, RooFit::LineColor(13));
    TCanvas *canvassb = new TCanvas("canvassb", "canvas sidebands", 600, 600);
    gPad->SetLogy(kTRUE);
    et_frame->Draw();
    canvassb->Print("et_bkg.gif");

    et_bkg_xl.setConstant(true);
    et_bkg_mean.setConstant(true);
    et_bkg_sigma.setConstant(true);
    et_bkg_tau_short.setConstant(true);
    et_bkg_tau_long.setConstant(true);
    xs.setConstant(true);

    RooRealVar et_sig_xl("et_sig_xl", "xl sig", 0.3, 0, 1);
    RooRealVar et_sig_mean("et_sig_mean", "mean sig", 0.06, 0, 1);
    RooRealVar et_sig_sigma("et_sig_sigma", "sigma sig", 0.01, 0, 0.1);
    RooRealVar et_sig_tau_short("et_sig_tau_short", "#tau short sig", 0.01, 0.001, 1);
    RooRealVar et_sig_tau_long("et_sig_tau_long", "#tau long sig", 0.05, 0.001, 1);
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

  //  xs = xs.getVal() / (bkg_under_signal + xs.getVal());
    et_model.fitTo(signal_data,
    		RooFit::NumCPU(4),
    		RooFit::Minos(false),
    		RooFit::Hesse(false));

    TCanvas *canvasres = new TCanvas("canvares", "canvas sidebands", 600, 600);
    tree.Draw("_et>>sb", bkg_cut);
    tree.Draw("_et>>sg", signal_cut);
    TH1D *hsg = (TH1D*) gROOT->Get("sg");
    TH1D *hsb = (TH1D*) gROOT->Get("sb");
    hsg->Sumw2();
    hsg->Add(hsb, -x_u);
    hsg->Draw();
    RooDataHist rhsg("rhsg", "signal", _et, hsg);

    //    et_sig_model.fitTo(rhsg,RooFit::Minos(false),RooFit::Hesse(false));
    et_sig_xl.setConstant(true);
    et_sig_mean.setConstant(true);
    et_sig_sigma.setConstant(true);
    et_sig_tau_short.setConstant(true);
    et_sig_tau_long.setConstant(true);
    et_parameters.writeToFile("et_parameters.txt");

    TCanvas *canvas2 = new TCanvas("canvas2", "canvas signal", 600, 600);
    RooPlot *et_frame2 = _et.frame();
    rhsg.plotOn(et_frame2);
    //    data->plotOn(et_frame2);
    //    et_model->plotOn(et_frame2, RooFit::Components(et_bkg_model) ,RooFit::LineColor(kRed));
    et_sig_model.plotOn(et_frame2, RooFit::Components(et_sig_short), RooFit::LineColor(kRed));
    et_sig_model.plotOn(et_frame2, RooFit::Components(et_sig_long), RooFit::LineColor(kBlue));
    et_sig_model.plotOn(et_frame2, RooFit::LineColor(13));
    gPad->SetLogy(kTRUE);
    et_frame2->Draw();
    canvas2->Print("et_sig.gif");

}

