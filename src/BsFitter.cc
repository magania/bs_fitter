#include "BsFitter.h"
BsFitter::BsFitter() :
  m("m","m", 0),
  t("t","t", 0),
#ifndef RES_TRUE
  et("et","et", 0),
#endif
  cpsi("cpsi","cos(#psi)", 0),
  ctheta("ctheta","cos(#theta)", 0),
  phi("phi","#phi", 0),
  d("d", "dilution", 0)
{
  resolution = 0;
  model = 0;
  data = 0;
  fit_result = 0;

  cout << "HERE" << endl;
  variables = new RooArgSet();
  parameters = new RooArgSet();

  variables->add(m);
  variables->add(t);
#ifndef RES_TRUE
  variables->add(et);
#endif
  variables->add(cpsi);
  variables->add(ctheta);
  variables->add(phi);
  variables->add(d);

#ifdef RES_GAUSS
  RooRealVar *S = new RooRealVar("S", "S", 0);
  parameters->add(*S);
  resolution = new RooGaussModel("resolution", "gauss resolution", t, RooFit::RooConst(0), *S, et);  
#endif

#ifdef RES_TRUE
  resolution = new RooTruthModel("resolution", "truth resolution", t);
#endif

  RooAbsPdf* signal = signal_model();
  RooAbsPdf* background = background_model();

  if ( signal && background ){
    RooRealVar *xs = new RooRealVar("xs", "x_s",0);
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

void BsFitter::setVariables(const char* vars) {variables->readFromFile(vars); }

void BsFitter::setParameters(const char* params){ parameters->readFromFile(params); }
void BsFitter::setData(const char* data_file){ data = RooDataSet::read(data_file, *variables); }
void BsFitter::setData(RooDataSet* data_set) {data = data_set; }

RooDataSet* BsFitter::generate(Int_t num){
#ifdef RES_TRUE
  return model->generate(RooArgSet(m, t, cpsi, ctheta, phi, d), num);
#endif
//     RooDataSet *data_p = p_model.generate(RooArgSet(p), num_events, kTRUE);
//     RooDataSet *data_p_et = et_model.generate(RooArgSet(et), num_events, kTRUE);
//     data_p_et->merge(data_p);
//     RooDataSet *data = signal_model.generate(RooArgSet(phi,ctheta, cpsi, t), ProtoData(*data_p_et));
}

Int_t BsFitter::fit(Bool_t hesse, Bool_t minos, Bool_t verbose, Int_t cpu)
{
  if ( !data ){
    cout << "EE: No Data" << endl;
    return kFALSE;
  }
  
#ifdef RES_TRUE
  fit_result = model->fitTo(*data, RooFit::ConditionalObservables(RooArgSet(d)),
			    RooFit::Save(kTRUE), RooFit::Hesse(hesse),
			    RooFit::Minos(minos), RooFit::NumCPU(cpu),
			    RooFit::Verbose(verbose));
#endif
#ifndef RES_TRUE
  fit_result = model->fitTo(*data, RooFit::ConditionalObservables(RooArgSet(et,d)),
			    RooFit::Save(kTRUE), RooFit::Hesse(hesse),
			    RooFit::Minos(minos), RooFit::NumCPU(cpu),
			    RooFit::Verbose(verbose));
#endif
  return fit_result->status();  
}

void BsFitter::plotM(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log){
  plotVar(m, plot_file, bins, proj_bins, log);
}
void BsFitter::plotM(){
  plotVar(m, "mass.gif", 0, 100, kFALSE);
}
void BsFitter::plotT(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log){
  plotVar(t, plot_file, bins, proj_bins, log);
}
void BsFitter::plotT(){
  plotVar(t, "time.gif", 0, 100, kTRUE);
}
void BsFitter::plotCpsi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log){
  plotVar(cpsi, plot_file, bins, proj_bins, log);
}
void BsFitter::plotCpsi(){
  plotVar(cpsi, "cpsi.gif", 0, 100, kFALSE);
}
void BsFitter::plotCtheta(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log){
  plotVar(ctheta, plot_file, bins, proj_bins, log);
}
void BsFitter::plotCtheta(){
  plotVar(ctheta, "ctheta.gif", 0, 100, kFALSE);
}
void BsFitter::plotPhi(const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log){
  plotVar(phi, plot_file, bins, proj_bins, log);
}
void BsFitter::plotPhi(){
  plotVar(phi, "phi.gif", 0, 100, kFALSE);
}

void BsFitter::plotVar(RooRealVar& x, const char* plot_file, Int_t bins, Int_t proj_bins, Bool_t log)
{
  RooPlot *x_frame = x.frame();
  if ( data )
    if (bins)
      data->plotOn(x_frame, RooFit::Binning(bins));
    else
      data->plotOn(x_frame);

  if ( data ) {
#ifdef RES_TRUE
    model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(d), *data));
#endif
#ifndef RES_TRUE
    if (proj_bins){
      et.setBins(proj_bins);
      RooDataHist projData("projData","projData",RooArgSet(et,d), *data);
      model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(et,d), projData));
    } else {
      model->plotOn(x_frame, RooFit::ProjWData(RooArgSet(et,d), *data));
    }
#endif
  } else {
    model->plotOn(x_frame);
  }

  TCanvas *canvas = new TCanvas("canvas", "canvas", 600, 600);
  x_frame->Draw();
  if (log)
    gPad->SetLogy(1);
  canvas->Print(plot_file);
}

void BsFitter::writeResult(const char* file_name)
{
  ofstream out(file_name);
  fit_result->printMultiline(out,0, kTRUE);
  out.close();
}

RooAbsPdf* BsFitter::signal_model()
{
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

  RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass", m, *M, *sigma);

#ifdef EFFICIENCY
  return 0;
#endif

#ifndef EFFICIENCY
  RooBsTimeAngle *time_angle_bs= new RooBsTimeAngle ("time_angle_bs", "bs time angle pdf", kTRUE,
						     t, cpsi, ctheta, phi,
						     *A02, *All2, *DeltaGamma, *tau,
						     *DeltaMs, *phi_s, *delta_1, *delta_2,
						     *resolution);
  
  RooBsTimeAngle *time_angle_bsbar= new RooBsTimeAngle("time_angle_bsbar", "bs bar time angle pdf", kFALSE,
						       t, cpsi, ctheta, phi,
						       *A02, *All2, *DeltaGamma, *tau,
						       *DeltaMs, *phi_s, *delta_1, *delta_2,
						       *resolution);
  RooAddPdf *signal_time_angle = new RooAddPdf("signal_time_angle", "signal time-angular pdf",
					      *time_angle_bs, *time_angle_bsbar, d);
#endif

  RooProdPdf *signal =
    new RooProdPdf("signal", "signal", RooArgSet(*signal_mass, *signal_time_angle));
  
  return signal;
}

RooAbsPdf* BsFitter::background_model()
{
  return 0;
}
