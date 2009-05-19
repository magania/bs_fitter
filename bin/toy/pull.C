Double_t PI = 3.141592654;
Double_t PI2 = 1.570796327;

void correctAngle(Double_t* angle){
	if ( (*angle) > -PI  && (*angle) < PI )
		return;
	if ( (*angle) < -PI )
		(*angle) = 2*PI+(*angle);
	else
		(*angle) = -2*PI+(*angle);
	correctAngle(angle);
}

bool transformDG(Double_t *DG, Double_t *Phi_s, Double_t *Delta_1, Double_t *Delta_2){
	if ( (*DG)*cos(*Phi_s) < 0 )
		return false;

	if ((*DG)<0 ) {
		(*DG) = -(*DG);
		(*Phi_s) = PI-(*Phi_s);
		(*Delta_1) = PI-(*Delta_1);
		(*Delta_1) = PI-(*Delta_2);
	}

	correctAngle(Phi_s);
	correctAngle(Delta_1);
	correctAngle(Delta_2);

	return true;
}


void plotVar(const char* var, RooRealVar &RR, const char* cuts ,TTree &fits){
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

void pull(){
 gROOT->SetStyle("Plain");
 gStyle->SetOptStat("000000000");
 gStyle->SetOptFit(1011);
 gStyle->SetStatW(0.3);

 RooRealVar rA0("A0", "A0", 0);
 RooRealVar rA1("A1", "A1", 0);
 RooRealVar rDeltaGamma("DeltaGamma", "#Delta#Gamma", 0);
 RooRealVar rDelta_1("Delta_1", "#delta_{1}", 0);
 RooRealVar rDelta_2("Delta_2", "#delta_{2}", 0);
 RooRealVar rPhi_s("Phi_s", "#phi_{s}", 0);
 RooRealVar rTau("Tau", "#tau", 0);
 //RooRealVar DeltaMs("DeltaMs", "#Delta M_{s}", 0);

 RooArgSet parameters(A0,A1,DeltaGamma,Delta_1, Delta_2, Phi_s, Tau);
 parameters.readFromFile("../parameters.txt");

 Double_t A0,eA0,A1,eA1,DG,eDG,Delta_1,eDelta_1,Delta_2,eDelta_2,Phi_s,ePhi_s,Tau,eTau;
 TTree fits("fits","fits");
 fits.Branch("A0", &A0, "A0");
 fits.Branch("eA0", &eA0, "eA0");
 fits.Branch("A1", &A1, "A1");
 fits.Branch("eA1", e&A1, "eA1");
 fits.Branch("DeltaGamma", &DG, "DeltaGamma");
 fits.Branch("eDeltaGamma", &eDG, "eDeltaGamma");
 fits.Branch("Delta_1", &Delta_1, "Delta_1");
 fits.Branch("eDelta_1", &eDelta_1, "eDelta_1");
 fits.Branch("Delta_2", &Delta_2, "Delta_2");
 fits.Branch("eDelta_2", &eDelta_2, "eDelta_2");
 fits.Branch("Phi_s", &Phi_s, "Phi_s");
 fits.Branch("ePhi_s", &ePhi_s, "ePhi_s");
 fits.Branch("Tau", &Tau, "Tau");
 fits.Branch("eTau", &eTau, "eTau");


 ifstream fitdat("fits.dat");
 int total = 0;
 int corrected = 0;
 while (!fitdat.eof()){
	 fitdat >> A0 >> eA0 >> A1 >> eA1 >> DG >> eDG >> Delta_1 >> eDelta_1 >> Delta_2 >> eDelta_2 >> Phi_s >> ePhi_s >> Tau >> eTau;
	 total++;
	 if (transformDG(*DG, *Phi_s, *Delta_1, *Delta_2)){
		 tree.Fill();
		 corrected++;
	 }
 }


 const char * cuts = "";
 plotVar("A0", rA0, cuts, fits);
 plotVar("A1", rA1, cuts, fits);
 plotVar("DeltaGamma", rDeltaGamma, cuts, fits);
 plotVar("Delta_1", rDelta_1, cuts, fits);
 plotVar("Delta_2", rDelta_2, cuts, fits);
 plotVar("Phi_s", rPhi_s, cuts, fits);
 plotVar("Tau", rTau, cuts, fits);

 cout << endl;
 cout << "Total fits: " << total << " Corrected fits: " << corrected << endl;
 cout << "^ **Parameter** ^ **Generated Value** ^ **Parameter Mean** ^ **Parameter Sigma** ^ **Pull Mean** ^ **Pull Sigma** ^" << endl;
 cout << wiki.str() << endl;
}

