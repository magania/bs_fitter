void plotOfVar(RooRealVar &var, RooRealVar &evar, RooDataSet &data){
  TString name = var.GetName();
  name+= "_frame";
  TCanvas *canvas = new TCanvas(name, name,1200,600);
  canvas->Divide(2);

  canvas->cd(1);
  RooRealVar mean_var("mean_var","mean", var.getVal(), var.getVal()-1, var.getVal()+1);
  RooRealVar sigma_var("sigma_var","sigma",0,10);
  RooGaussian gauss_var("gauss_var", "gauss", var, mean_var, sigma_var);
  gauss_var.fitTo(data);

  RooPlot *frame = var.frame();
  data.plotOn(frame);
  gauss_var.plotOn(frame);  
  gauss_var.paramOn(frame, Layout(0.45,0.99,0.99));
  frame->Draw();

 
  canvas->cd(2);
  RooRealVar pull("pull", "pull", 0, -3, 3);
  RooDataSet pull_data("pull_data", "pull data", pull);
  for (int i= 0; i < data.numEntries(); i++){
    RooArgSet* data_set = data.get(i);
    pull = (var.getVal() - data_set->getRealValue(var.GetName()))/data_set->getRealValue(evar.GetName());
    pull_data.add(pull); 
  }

  RooRealVar mean("mean","mean", 0, -1, +1);
  RooRealVar sigma("sigma","sigma",0,2);
  RooGaussian gauss("gauss", "gauss", pull, mean, sigma);
  gauss.fitTo(pull_data);


  RooPlot *frame = pull.frame();
  pull_data.plotOn(frame);
  gauss.plotOn(frame); 
  gauss.paramOn(frame, Layout(0.45,0.99,0.99)); 
  frame->Draw();

  //  canvas->cd(3);
  //  RooPlot *frame = evar.frame();
  //  data.plotOn(frame);
  //  frame->Draw();
  name += ".gif";
  canvas->Print(name);
}

fit_toy_signal(){
	gROOT->SetStyle("Plain");

  using namespace RooFit;

  RooRealVar A02("A02", "A02", 0);
  RooRealVar All2("All2", "A_{#parallel}(0)", 0);
  RooRealVar DeltaGamma("DeltaGamma", "#Delta#Gamma", 0);
  RooRealVar delta_1("delta_1", "#delta_{1}", 0);
  RooRealVar delta_2("delta_2", "#delta_{2}", 0);
  RooRealVar phi_s("phi_s", "#phi_{s}", 0);
  RooRealVar tau("tau", "#tau", 0);

  RooArgSet *parameters = new RooArgSet();
  parameters->add(A02);
  parameters->add(All2);
  parameters->add(DeltaGamma);
  parameters->add(delta_1);
  parameters->add(delta_2);
  parameters->add(phi_s);
  parameters->add(tau);
  //  parameters->add(DeltaMs);

  parameters->readFromFile("parameters.txt");

  RooRealVar DeltaMs("DeltaMs", "#Delta M_{s}", 0);
  RooRealVar p("p", "bs prob.", 0);

  RooRealVar eA02("eA02", "e A02", -1, 1);
  RooRealVar eAll2("eAll2", "e A_{#parallel}(0)", -1, 1);
  RooRealVar eDeltaGamma("eDeltaGamma", "e #Delta#Gamma", -1, 1);
  RooRealVar edelta_1("edelta_1", "e #delta_{1}", -1,1);
  RooRealVar edelta_2("edelta_2", "e #delta_{2}", -1,1);
  RooRealVar ephi_s("ephi_s", "e #phi_{s}", -5, 5);
  RooRealVar etau("etau", "e #tau", -1, 1);

  RooArgSet signal_params(tau, DeltaGamma, A02, All2, delta_1, delta_2, phi_s, DeltaMs, p);
  //  signal_params.readFromFile("signal_params.txt");

  TTree tree("tree","tree");
  tree.ReadFile("data_set.txt","A02/D:eA02:All2:eAll2:DeltaGamma:eDeltaGamma:delta_1:edelta_1:delta_2:edelta_2:phi_s:ephi_s:tau:etau");
  //  tree.Draw("ephi_s");
  RooArgSet set(A02,All2,DeltaGamma,delta_1,delta_2,phi_s,tau);
  set.add(eA02);
  set.add(eAll2);
  set.add(eDeltaGamma);
  set.add(edelta_1);
  set.add(edelta_2);
  set.add(ephi_s);
  set.add(etau);

   set.Print();

  RooDataSet data("data", "data", &tree ,set);
  data.Print();

  plotOfVar(A02,eA02,data);
  plotOfVar(All2,eAll2,data);
  plotOfVar(DeltaGamma,eDeltaGamma,data);
  plotOfVar(delta_1,edelta_1,data);
  plotOfVar(delta_2,edelta_2,data);
  plotOfVar(phi_s,ephi_s,data);
  plotOfVar(tau,etau,data);
  return ;
}
