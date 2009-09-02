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

BsFitter::BsFitter(const char* root_file, const char * cut){
	gROOT->SetStyle("Plain");

	/* magic line from Rene - for future reference! */
	gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo",
			"*",
			"RIO",
			"TStreamerInfo",
			"TStreamerInfo()");

	/* variables */
	variables = new RooArgSet();
	m = new RooRealVar("m", "m", 0, 5, 5.8);
	t = new RooRealVar("t", "t", 0,-3, 15);
	et = new RooRealVar("et", "et", 0.01, 0, 1);
	cpsi = new RooRealVar("cpsi", "cos(#psi)", -1, 1);
	ctheta = new RooRealVar("ctheta", "cos(#theta)", -1 , 1);
	phi = new RooRealVar("phi", "#phi", -TMath::Pi(), TMath::Pi());
	p = new RooRealVar("p", "D", -1, 1);
	bdtI = new RooRealVar("bdtI", "inclusive BDT", -0.5, 1);
	bdtP = new RooRealVar("bdtP", "prompt BDT", -0.5, 1);
	category = new RooCategory("category","category");
	category->defineType("nseg0",0);
	category->defineType("nseg1",1);
	category->defineType("nseg2",2);
	category->defineType("nseg3",3);

	variables->add(*m);
	variables->add(*t);
	variables->add(*et);
	variables->add(*cpsi);
	variables->add(*ctheta);
	variables->add(*phi);
	variables->add(*p);
	variables->add(*bdtI);
	variables->add(*bdtP);
	variables->add(*category);

	/* Parameters */
	parameters = new RooArgSet();
	M = new RooRealVar ("M", "M", 5.26, 5.46);
	Sigma = new RooRealVar ("Sigma", "#sigma", 0.023, 0.043);
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

//	parameters->add(*M);
//	parameters->add(*Sigma);
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

	cout << "\033[1m" << " Reading data ..." << "\033[m" << endl;
	TFile tree_file(root_file);
	TTree *Tree = (TTree*) tree_file.Get("tree");
	if (!Tree){
		cout << "\033[1m" << " Tree not found." << "\033[m" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "\033[1m" << " Found tree in: " << root_file
	     << " Entries:" << Tree->GetEntries() << "\033[m" << endl;

	cout << "\033[1m" << " Extra cut: " << cut << "\033[m" << endl;
	Tree->Draw(">>entry_list", cut, "entrylist");
	TEntryList *entry_list = (TEntryList*)gDirectory->Get("entry_list");

	Long64_t nevent = entry_list->GetN();
	cout << "\033[1m" <<  " Selected: " << nevent << " events."<< "\033[m" << endl;
	if (!nevent )
		exit(EXIT_FAILURE);

    Double_t t_m, t_pdl, t_epdl, t_cpsi, t_ctheta, t_phi, t_d;
	Int_t t_defined, t_category, t_muplus_nseg, t_muminus_nseg;;
	Float_t t_inclusive, t_prompt;

	Double_t t_t, t_et, t_D;

	Tree->SetBranchAddress("bs_mass",&t_m);
	Tree->SetBranchAddress("bs_pdl",&t_pdl);
	Tree->SetBranchAddress("bs_epdl",&t_epdl);
	Tree->SetBranchAddress("bs_angle_cpsi",&t_cpsi);
	Tree->SetBranchAddress("bs_angle_ctheta",&t_ctheta);
	Tree->SetBranchAddress("bs_angle_phi",&t_phi);
	Tree->SetBranchAddress("dilution",&t_d);
	Tree->SetBranchAddress("dilution_defined", &t_defined);
	Tree->SetBranchAddress("mu_plus_nseg", &t_muplus_nseg);
	Tree->SetBranchAddress("mu_minus_nseg",&t_muminus_nseg);
	Tree->SetBranchAddress("inclusiveBDT", &t_inclusive);
	Tree->SetBranchAddress("promptBDT", &t_prompt);

	data = new RooDataSet("data","data",*variables);
	for (Long64_t i=0;i< nevent;i++) {
		Tree->GetEntry(entry_list->GetEntry(i));

		Double_t t_t= t_pdl/0.0299792458;
		Double_t t_et=t_epdl/0.0299792458;
		Double_t t_D=0;

		if ( t_defined )
			t_D = (fabs(t_d)>0.55)?  (t_d>0?0.5957:-0.5957) : 0.7895*t_d + 0.3390*t_d*fabs(t_d);

		*m = t_m;
		*t = t_t;
		*et = t_et;
		*cpsi = t_cpsi;
		*ctheta = t_ctheta;
		*phi = t_phi;
		*p = t_D;
		*bdtI = t_inclusive;
		*bdtP = t_prompt;
		//std::cout << t_muplus_nseg << ' ' << t_muminus_nseg << std::endl;
		int nseg = t_muplus_nseg>t_muminus_nseg?t_muminus_nseg:t_muplus_nseg;
		category->setIndex(nseg);
        data->add(*variables);

		if (!(i%10000))
			cout << "Processed " << i << " ..." << endl;
	 }
	data->Print();
}

void BsFitter::histogram(const char *cut, RooDataHist *signal_hist_et, RooDataHist *bkg_hist_et,
		RooDataHist *signal_hist_I, RooDataHist *bkg_hist_I,
		RooDataHist *signal_hist_P, RooDataHist *bkg_hist_P,
		RooDataHist *signal_hist_D, RooDataHist *bkg_hist_D){
	cout << "\033[1m" << " Preliminary mass fit: gaus+line ..." << "\033[m" << endl;
    RooRealVar *mm0 = new RooRealVar("mm0", "mm0", 0, -1/5.8, 1);
    RooRealVar *xs0 = new RooRealVar("xs0", "xs0", 0.1, 0.01, 1);
	RooGaussian gauss("gauss", "gauss", *m, *M, *Sigma);
    RooPolynomial line("line", "line", *m, *mm0);

    RooAddPdf model("model", "model", gauss, line, *xs0);
    RooDataSet data_histo("data_histo", "data_histo", *variables, RooFit::Import(*data), RooFit::Cut(cut));
	model.fitTo(data_histo/*, RooFit::Hesse(false), RooFit::Minos(false)*/, RooFit::NumCPU(2));

	/*
	RooPlot *m_frame = m.frame();
	data.plotOn(m_frame);
	    model.plotOn(m_frame, RooFit::LineColor(13));
	    TCanvas *canvas_m = new TCanvas("canvas_m", "canvas mass", 600, 600);
	    m_frame->Draw();
	    ss_mass_png << "mass_" << name << ".png";
	    canvas_m->Print(ss_mass_png.str().c_str());
	*/

	Double_t a = m->getMin();
	Double_t b = M->getVal() - 3 * Sigma->getVal();
	Double_t c = M->getVal() + 3 * Sigma->getVal();
	Double_t d = m->getMax();

	Double_t all = (d-a) + mm0->getVal()*(d-a)*(d-a)/2;
	Double_t under = (c-b) + mm0->getVal()*(c-b)*(c-b)/2;
	//Double_t u = (1-xs0->getVal())*all/under;
	//Double_t x_u = xs0->getVal()/(xs0->getVal() + u);
	Double_t x_u = under/(all - under);

	bkg_cut = "m < ";
	bkg_cut += b;
	bkg_cut += " || m > ";
	bkg_cut += c;
	signal_cut = "m > ";
	signal_cut += b;
	signal_cut += " && m < ";
	signal_cut += c;

	cout << "Bkg Cut: " << bkg_cut << endl;
	cout << "Signal Cut: " << signal_cut << endl;
	cout << "\033[1m" << " Events\tSignal\tBkg\tS/sqrt(N)\txs\txu" << "\033[m" << endl;
	unsigned int N = data_histo.numEntries();
	cout << "\033[1m " << N << '\t'
		 << N*xs0->getVal() << '\t'
		 << N*(1-xs0->getVal()) << '\t'
		 << N*xs0->getVal()/sqrt(N) << "\t\t"
		 << xs0->getVal() << '\t'
		 << x_u << '\t'
		 << "\033[m" << endl;

	signal_hist_et->add(data_histo, signal_cut, 1.0);
	signal_hist_et->add(data_histo, bkg_cut, -x_u);
	bkg_hist_et->add(data_histo, bkg_cut, x_u);
	signal_hist_et->Print();
	bkg_hist_et->Print();

	signal_hist_I->add(data_histo, signal_cut, 1.0);
	signal_hist_I->add(data_histo, bkg_cut, -x_u);
	bkg_hist_I->add(data_histo, bkg_cut, x_u);
	signal_hist_I->Print();
	bkg_hist_I->Print();

	signal_hist_P->add(data_histo, signal_cut, 1.0);
	signal_hist_P->add(data_histo, bkg_cut, -x_u);
	bkg_hist_P->add(data_histo, bkg_cut, x_u);
	signal_hist_P->Print();
	bkg_hist_P->Print();

	signal_hist_D->add(data_histo, signal_cut, 1.0);
	signal_hist_D->add(data_histo, bkg_cut, -x_u);
	bkg_hist_D->add(data_histo, bkg_cut, x_u);
	signal_hist_D->Print();
	bkg_hist_D->Print();

	delete mm0;
	delete xs0;
}

void BsFitter::plotVar(RooRealVar* x, const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
	std::cout << "BsFitter::plotVar" << std::endl;
}

void BsFitter::plotM(const char* plot_file, Int_t bins, Int_t proj_bins,Bool_t log) {
plotVar(m, plot_file, bins, proj_bins, log);
}

void BsFitter::plotM() {
plotVar(m, "mass.png", 0, 100, kFALSE);
}

void BsFitter::plotT(const char* plot_file, Int_t bins, Int_t proj_bins,Bool_t log) {
plotVar(t, plot_file, bins, proj_bins, log);
}

void BsFitter::plotEt() {
plotVar(et, "time_error.png", 0, 100, kTRUE);
}

void BsFitter::plotEt(const char* plot_file, Int_t bins, Int_t proj_bins,Bool_t log) {
plotVar(et, plot_file, bins, proj_bins, log);
}

void BsFitter::plotT() {
plotVar(t, "time.png", 0, 100, kTRUE);
}

void BsFitter::plotCpsi(const char* plot_file, Int_t bins, Int_t proj_bins,Bool_t log) {
plotVar(cpsi, plot_file, bins, proj_bins, log);
}

void BsFitter::plotCpsi() {
plotVar(cpsi, "cpsi.png", 0, 100, kFALSE);
}

void BsFitter::plotCtheta(const char* plot_file, Int_t bins, Int_t proj_bins,
Bool_t log) {
plotVar(ctheta, plot_file, bins, proj_bins, log);
}

void BsFitter::plotCtheta() {
plotVar(ctheta, "ctheta.png", 0, 100, kFALSE);
}

void BsFitter::plotPhi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log) {
plotVar(phi, plot_file, bins, proj_bins, log);
}

void BsFitter::plotPhi() {
plotVar(phi, "phi.png", 0, 100, kFALSE);
}

void BsFitter::plotD() {
plotVar(p, "D.png", 0, 100, kFALSE);
}

void BsFitter::plotI() {
plotVar(bdtI, "inclusive.png", 0, 100, kFALSE);
}

void BsFitter::plotP() {
plotVar(bdtP, "prompt.png", 0, 100, kFALSE);
}

BsSignalFitter::BsSignalFitter(const char* name, const char* root_file, const char* cut, const char* parameters_file) :
	BsFitter(root_file, cut)
{
	/* PDF's */
	resolution = new BsResolution(name, t, et);
	parameters->add(*resolution->getParameters());

	efficiency = new Efficiency(glue("efficiency",name));

	signal = new BsSignal (name, m, t, cpsi, ctheta, phi, p, bdtI, bdtP,
			M, Sigma, A0, A1, DeltaGamma, Phi_s, Delta1, Delta2, Tau, DeltaMs,
			Delta1_mean, Delta1_sigma, Delta2_mean, Delta2_sigma, DeltaMs_mean, DeltaMs_sigma,
			resolution, 0, efficiency);

	pdf = signal->pdf();
	parameters->readFromFile(parameters_file);
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
			projData = new RooDataHist("projData", "projData",RooArgSet(*et), *data);
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

BsSingleFitter::BsSingleFitter(const char* name, const char* name_et, const char* root_file, const char *cut, const char* parameters_file, int angle_pdf) :
	BsFitter(root_file, cut)
{
	RooDataHist *signal_hist_et = new RooDataHist("signal_hist_et", "signal_hist_et", RooArgSet(*et));
	RooDataHist *bkg_hist_et = new RooDataHist("bkg_hist_et", "bkg_hist_et", RooArgSet(*et));

	RooDataHist *signal_hist_I = new RooDataHist("signal_hist_I", "signal_hist_I", RooArgSet(*bdtI));
	RooDataHist *bkg_hist_I = new RooDataHist("bkg_hist_I", "bkg_hist_I", RooArgSet(*bdtI));

	RooDataHist *signal_hist_P = new RooDataHist("signal_hist_P", "signal_hist_P", RooArgSet(*bdtP));
	RooDataHist *bkg_hist_P = new RooDataHist("bkg_hist_P", "bkg_hist_P", RooArgSet(*bdtP));

	RooDataHist *signal_hist_D = new RooDataHist("signal_hist_D", "signal_hist_D", RooArgSet(*p));
	RooDataHist *bkg_hist_D = new RooDataHist("bkg_hist_D", "bkg_hist_D", RooArgSet(*p));

	histogram("", signal_hist_et, bkg_hist_et,
			signal_hist_I, bkg_hist_I,
			signal_hist_P, bkg_hist_P,
			signal_hist_D, bkg_hist_D);

	/* PDF's */
	resolution = new BsResolution(name, t, et);
	parameters->add(*resolution->getParameters());

	et_sig = new BsEtModel(glue("sig",name_et), et, signal_hist_et, 2);
	et_bkg = new BsEtModel(glue("bkg",name_et), et, bkg_hist_et, 1);
	parameters->add(*et_sig->getParameters());
	parameters->add(*et_bkg->getParameters());


	{
	RooPlot *et_frame = et->frame();
	signal_hist_et->plotOn(et_frame);
	et_sig->pdf()->plotOn(et_frame, RooFit::LineColor(13));
	TCanvas *canvas_et = new TCanvas("canvas_et", "canvas et", 600, 600);
	et_frame->Draw();
	gPad->SetLogy(1);
	canvas_et->Print("et_sig.png");
	}

	{
	RooPlot *et_frame = et->frame();
	bkg_hist_et->plotOn(et_frame);
	et_bkg->pdf()->plotOn(et_frame, RooFit::LineColor(13));
	TCanvas *canvas_et = new TCanvas("canvas_et", "canvas et", 600, 600);
	et_frame->Draw();
	gPad->SetLogy(1);
	canvas_et->Print("et_bkg.png");
	}

	efficiency = new Efficiency(glue("efficiency",name));

	signal = new BsSignal (name, m, t, cpsi, ctheta, phi, p, bdtI, bdtP,
			M, Sigma, A0, A1, DeltaGamma, Phi_s, Delta1, Delta2, Tau, DeltaMs,
			Delta1_mean, Delta1_sigma, Delta2_mean, Delta2_sigma, DeltaMs_mean, DeltaMs_sigma,
			resolution, et_sig, efficiency, signal_hist_I, signal_hist_P, signal_hist_D);

	bkg = new BsBackground(name, m, t, cpsi, ctheta, phi, p, bdtI, bdtP, resolution, et_bkg,
			bkg_hist_I, bkg_hist_P, bkg_hist_D, angle_pdf);
	parameters->add(*bkg->getParameters());

	xs = new RooRealVar (glue("xs",name), glue("x_s",name), 0);
	parameters->add(*xs);

	model = new RooAddPdf (glue("model",name), glue("model",name), *signal->pdf(), *bkg->pdf(), *xs);
	pdf = model;
	parameters->readFromFile(parameters_file);
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
			projData = new RooDataHist("projData", "projData",RooArgSet(*et), *data);
		} else {
			projData = data;
		}

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

BsMultiFitter::BsMultiFitter(const char* root_file, const char* cut, const char* parameters_file) :
	BsFitter(root_file, cut)
{
	RooDataHist *signal_hist_et1 = new RooDataHist("signal_hist_et1", "signal_hist_et1", RooArgSet(*et));
	RooDataHist *bkg_hist_et1 = new RooDataHist("bkg_hist_et1", "bkg_hist_et1", RooArgSet(*et));
	RooDataHist *signal_hist_I1 = new RooDataHist("signal_hist_I1", "signal_hist_I1", RooArgSet(*bdtI));
	RooDataHist *bkg_hist_I1 = new RooDataHist("bkg_hist_I1", "bkg_hist_I1", RooArgSet(*bdtI));
	RooDataHist *signal_hist_P1 = new RooDataHist("signal_hist_P1", "signal_hist_P1", RooArgSet(*bdtP));
	RooDataHist *bkg_hist_P1 = new RooDataHist("bkg_hist_P1", "bkg_hist_P1", RooArgSet(*bdtP));
	RooDataHist *signal_hist_D1 = new RooDataHist("signal_hist_D1", "signal_hist_D1", RooArgSet(*p));
	RooDataHist *bkg_hist_D1 = new RooDataHist("bkg_hist_D1", "bkg_hist_D1", RooArgSet(*p));
	histogram("category == 1", signal_hist_et1, bkg_hist_et1, signal_hist_I1, bkg_hist_I1, signal_hist_P1, bkg_hist_P1, signal_hist_D1, bkg_hist_D1);

	RooDataHist *signal_hist_et3 = new RooDataHist("signal_hist_et3", "signal_hist_et3", RooArgSet(*et));
	RooDataHist *bkg_hist_et3 = new RooDataHist("bkg_hist_et3", "bkg_hist_et3", RooArgSet(*et));
	RooDataHist *signal_hist_I3 = new RooDataHist("signal_hist_I3", "signal_hist_I3", RooArgSet(*bdtI));
	RooDataHist *bkg_hist_I3 = new RooDataHist("bkg_hist_I3", "bkg_hist_I3", RooArgSet(*bdtI));
	RooDataHist *signal_hist_P3 = new RooDataHist("signal_hist_P3", "signal_hist_P3", RooArgSet(*bdtP));
	RooDataHist *bkg_hist_P3 = new RooDataHist("bkg_hist_P3", "bkg_hist_P3", RooArgSet(*bdtP));
	RooDataHist *signal_hist_D3 = new RooDataHist("signal_hist_D3", "signal_hist_D3", RooArgSet(*p));
	RooDataHist *bkg_hist_D3 = new RooDataHist("bkg_hist_D3", "bkg_hist_D3", RooArgSet(*p));
	histogram("category == 3", signal_hist_et3, bkg_hist_et3, signal_hist_I3, bkg_hist_I3, signal_hist_P3, bkg_hist_P3, signal_hist_D3, bkg_hist_D3);

	/* PDF's */
	resolution = new BsResolution("", t, et);
	parameters->add(*resolution->getParameters());

	et_sig_1 = new BsEtModel ("sig_1", et, signal_hist_et1, 1);
	et_sig_3 = new BsEtModel ("sig_3", et, signal_hist_et3, 1);
	et_bkg_1 = new BsEtModel ("bkg_1", et, bkg_hist_et1, 1);
	et_bkg_3 = new BsEtModel ("bkg_3", et, bkg_hist_et3, 1);

//	parameters->add(*et_sig_IIa->getParameters());
//	parameters->add(*et_sig_IIb->getParameters());
//	parameters->add(*et_bkg_IIa->getParameters());
//	parameters->add(*et_bkg_IIb->getParameters());

	efficiency_1 = new Efficiency("efficiency_1");
	efficiency_3 = new Efficiency("efficiency_3");

	signal_1 = new BsSignal ("1", m, t, cpsi, ctheta, phi, p, bdtI, bdtP,
			M, Sigma, A0, A1, DeltaGamma, Phi_s, Delta1, Delta2, Tau, DeltaMs,
			Delta1_mean, Delta1_sigma, Delta2_mean, Delta2_sigma, DeltaMs_mean, DeltaMs_sigma,
			resolution, et_sig_1, efficiency_1);
	signal_3 = new BsSignal ("3", m, t, cpsi, ctheta, phi, p, bdtI, bdtP,
			M, Sigma, A0, A1, DeltaGamma, Phi_s, Delta1, Delta2, Tau, DeltaMs,
			Delta1_mean, Delta1_sigma, Delta2_mean, Delta2_sigma, DeltaMs_mean, DeltaMs_sigma,
			resolution, et_sig_3, efficiency_3);

	bkg_1 = new BsBackground("1", m, t, cpsi, ctheta, phi, p, bdtI, bdtP, resolution, et_bkg_1, bkg_hist_I1, bkg_hist_P1, bkg_hist_D1, 0);
	bkg_3 = new BsBackground("3", m, t, cpsi, ctheta, phi, p, bdtI, bdtP, resolution, et_bkg_3, bkg_hist_I3, bkg_hist_P3, bkg_hist_D1, 3);

	parameters->add(*bkg_1->getParameters());
	parameters->add(*bkg_3->getParameters());

	xs_1 = new RooRealVar ("xs_1", "x_s 1", 0);
	xs_3 = new RooRealVar ("xs_3", "x_s 3", 0);
	parameters->add(*xs_1);
	parameters->add(*xs_3);

	model_1 = new RooAddPdf ("model_1", "model_1", *signal_1->pdf(), *bkg_1->pdf(), *xs_1);
	model_3 = new RooAddPdf ("model_3", "model_3", *signal_3->pdf(), *bkg_3->pdf(), *xs_3);

	model = new RooSimultaneous("model", "model", *category);
	model->addPdf(*model_1,"nseg1");
	model->addPdf(*model_3,"nseg3");

	pdf=model;
	parameters->readFromFile(parameters_file);
}


char *BsFitter::glue(const char* a, const char *b){
	char *tmp = (char*) malloc(strlen(a)+strlen(b)+2);
	strcpy(tmp, a);
	strcat(tmp, "_");
	strcat(tmp, b);
	return tmp;
}

void BsFitter::writeParameters(const char* file) {
	//ofstream out(file);
	parameters->writeToFile(file);
//	out.close();
}


Int_t BsFitter::fit(Bool_t hesse, Bool_t minos, Bool_t verbose, Int_t cpu) {
	if (!data) {
		cout << "EE: No Data" << endl;
		return kFALSE;
	}

	fit_result = pdf->fitTo(*data,
					RooFit::ConditionalObservables(RooArgSet(*p, *et)),
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
