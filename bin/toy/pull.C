plotVar(const char* var, RooRealVar &RR, const char* cuts ,TTree &fits){
 stringstream ss, ss_canvas, ss_var, ss_hvar, ss_pull, ss_evar, ss_eevar;
 ss_var << var << " >> hvar_" << var << "(50," << RR.getVal()-5*RR.getError() << "," << RR.getVal()+5*RR.getError() <<")";
 ss_evar  << 'e' << var << " >> hvar_e" << var << "(50,0," << 3*RR.getError() << ")";
 ss << "(" << var <<"-" << RR.getVal() << ")/e" << var << " >> pull_" << var << "(50,-6,6)";
 ss_canvas << "canvas_" << var;
 ss_hvar << "hvar_" << var;
 ss_pull << "pull_" << var;
 ss_eevar << "hvar_e" << var;

 TCanvas *canvas = new TCanvas(ss_canvas.str().c_str(),ss_canvas.str().c_str(),1280,600);
 canvas->Divide(3);
 canvas->cd(1);
 cout << ss_var.str() << endl;
 fits.Draw(ss_var.str().c_str(), cuts);
 ((TH1*) gROOT->Get(ss_hvar.str().c_str()))->Fit("gaus"); 
 ((TH1*) gROOT->Get(ss_hvar.str().c_str()))->SetTitle(var);
 TArrow *flecha = new TArrow(RR.getVal(), 30, RR.getVal(),0, 0.01,">");
 flecha->Draw();
 TF1 *fit = ((TH1*) gROOT->Get(ss_hvar.str().c_str()))->GetFunction("gaus");
 Double_t mean = fit->GetParameter(1);
 Double_t mean_error = fit->GetParError(1);
 Double_t sigma = fit->GetParameter(2);
 Double_t sigma_error = fit->GetParError(2);
 wiki << "|**" << RR.GetName() << "** | " 
      << RR.getVal() << " +/- " << RR.getError() << " | " 
      << mean << " +/- " << mean_error << " | " 
      << sigma << " +/- " << sigma_error << " | ";

 canvas->cd(2);
 fits.Draw(ss_evar.str().c_str(), cuts);
 ((TH1*) gROOT->Get(ss_eevar.str().c_str()))->SetTitle("Error");
 flecha = new TArrow(RR.getError(), 40, RR.getError(),0, 0.01,">");
 flecha->Draw();


 canvas->cd(3);
 cout << fits.Draw(ss.str().c_str(), cuts) << endl;
 ((TH1*) gROOT->Get(ss_pull.str().c_str()))->Fit("gaus");
 ((TH1*) gROOT->Get(ss_pull.str().c_str()))->SetTitle("Pull");
 fit = ((TH1*) gROOT->Get(ss_pull.str().c_str()))->GetFunction("gaus");
 Double_t mean = fit->GetParameter(1);
 Double_t mean_error = fit->GetParError(1);
 Double_t sigma = fit->GetParameter(2);
 Double_t sigma_error = fit->GetParError(2);
 wiki << mean << " +/- " << mean_error << " | "
      << sigma << " +/- " << sigma_error << " | "<< endl;


}


stringstream wiki;

pull(){
 gROOT->SetStyle("Plain");
 gStyle->SetOptStat("000000000");
 gStyle->SetOptFit(1011);
 gStyle->SetStatW(0.3);

 RooRealVar A0("A0", "A0", 0);
 RooRealVar A1("A1", "A1", 0);
 RooRealVar DeltaGamma("DeltaGamma", "#Delta#Gamma", 0);
 RooRealVar Delta_1("Delta_1", "#delta_{1}", 0);
 RooRealVar Delta_2("Delta_2", "#delta_{2}", 0);
 RooRealVar Phi_s("Phi_s", "#phi_{s}", 0);
 RooRealVar Tau("Tau", "#tau", 0);
 RooRealVar DeltaMs("DeltaMs", "#Delta M_{s}", 0);

 RooArgSet parameters(A0,A1,DeltaGamma,Delta_1, Delta_2, Phi_s, Tau);
 parameters.readFromFile("../parameters.txt"); 

 TTree fits("fits","fits");
 cout << "Read: " 
      << fits.ReadFile("fits.dat","A0/D:eA0:A1:eA1:DeltaGamma:eDeltaGamma:Delta_1:eDelta_1:Delta_2:eDelta_2:Phi_s:ePhi_s:Tau:eTau") 
      << endl;

 const char * cuts = "Phi_s>-3.141592654 && Phi_s<3.141592654 && Delta_1>-2*3.141592654 && Delta_1<2*3.141592654 && Delta_2>-3.141592654 && Delta_2<3*3.141592654";
 plotVar("A0", A0, cuts, fits);
 plotVar("A1", A1, cuts, fits);
 plotVar("DeltaGamma", DeltaGamma, cuts, fits);
 plotVar("Delta_1", Delta_1, cuts, fits);
 plotVar("Delta_2", Delta_2, cuts, fits);
 plotVar("Phi_s", Phi_s, cuts, fits);
 plotVar("Tau", Tau, cuts, fits);

 cout << endl;
 cout << "^ **Parameter** ^ **Generated Value** ^ **Parameter Mean** ^ **Parameter Sigma** ^ **Pull Mean** ^ **Pull Sigma** ^" << endl;
 cout << wiki.str() << endl;
}

