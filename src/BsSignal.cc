/*
 * BsSignal.cc
 *
 *  Created on: May 29, 2009
 *      Author: magania
 */

#include <BsSignal.h>

BsSignal::BsSignal(const char* name,
		RooRealVar *m,
		RooRealVar *t,
		RooRealVar *cpsi,
		RooRealVar *ctheta,
		RooRealVar *phi,
		RooRealVar *p,
		RooRealVar *bdtI,
		RooRealVar *bdtP,
		RooRealVar *M,
		RooRealVar *Sigma,
		RooRealVar *A0,
		RooRealVar *A1,
		RooRealVar *DeltaGamma,
		RooRealVar *Phi_s,
		RooRealVar *Delta1,
		RooRealVar *Delta2,
		RooRealVar *Tau,
		RooRealVar *DeltaMs,
		RooRealVar *Delta1_mean, RooRealVar *Delta1_sigma,
		RooRealVar *Delta2_mean, RooRealVar *Delta2_sigma,
		RooRealVar *DeltaMs_mean, RooRealVar *DeltaMs_sigma,
		BsResolution *resolution, BsEtModel *et_model, Efficiency *efficiency,
		RooDataHist *hist_I, RooDataHist *hist_P, RooDataHist *hist_D) :
			BsPdf()
{
	cout << glue("SIGNAL MODEL", name) << endl;

	/* Constraints */
	RooGaussian *Delta1_constraint = new RooGaussian("Delta1_constraint", "#delta_{1} Gaussian Constraint", *Delta1, *Delta1_mean, *Delta1_sigma);
	RooGaussian *Delta2_constraint = new RooGaussian("Delta2_constraint", "#delta_{2} Gaussian Constraint", *Delta2, *Delta2_mean, *Delta2_sigma);
	RooGaussian *DeltaMs_constraint = new RooGaussian("DeltaMs_constraint", "#Delta m_{s} Gaussian Constraint", *DeltaMs, *DeltaMs_mean, *DeltaMs_sigma);

	/* Mass Pdf */
	RooGaussian *signal_mass = new RooGaussian("signal_mass", "signal_mass", *m, *M, *Sigma);

	/* Time-Angle Pdf */
	RooFormulaVar *All2 = new RooFormulaVar("All2", "(1-@0)*@1", RooArgList(*A0, *A1));
	RooFormulaVar *Ap2 = new RooFormulaVar("Ap2", "1-@0-@1", RooArgList(*A0,*All2));

	RooFormulaVar *SinPhi = new RooFormulaVar("SinPhi", "sin(@0)", *Phi_s);
	RooFormulaVar *CosPhi = new RooFormulaVar("CosPhi", "cos(@0)", *Phi_s);
	RooFormulaVar *SinDelta1 = new RooFormulaVar("SinDelta1", "sin(@0)", *Delta1);
	RooFormulaVar *CosDelta1 = new RooFormulaVar("CosDelta1", "cos(@0)", *Delta1);
	RooFormulaVar *SinDelta2 = new RooFormulaVar("SinDelta2", "sin(@0)", *Delta2);
	RooFormulaVar *CosDelta2 = new RooFormulaVar("CosDelta2", "cos(@0)", *Delta2);

	RooBsTimeAngle *signal_time_angle = new RooBsTimeAngle("signal_time_angle", "signal time angle",
			*t, *cpsi, *ctheta, *phi, *p,
			*A0, *All2, *Ap2, *DeltaGamma, *Tau, *DeltaMs,
			*SinPhi, *CosPhi, *SinDelta1, *CosDelta1, *SinDelta2, *CosDelta2,
			*resolution->pdf(), *efficiency);

	RooHistPdf *bdtinclusive, *bdtprompt, *dilution_sig;
	if (hist_I && hist_P){
		bdtinclusive = new RooHistPdf(glue("bdtinclusive_sig",name), glue("bdtinclusive_sig",name), RooArgSet(*bdtI), *hist_I);
		bdtprompt = new RooHistPdf(glue("bdtprompt_sig",name), glue("bdtprompt_sig",name), RooArgSet(*bdtP), *hist_P);
	}

	if (hist_D)
		dilution_sig = new RooHistPdf(glue("dilution_sig",name), glue("dilution_sig",name), RooArgSet(*p), *hist_D);

	/* Signal Pdf */
	RooArgSet *signal_set = new RooArgSet(*signal_mass, /* *Delta1_constraint, *Delta2_constraint,*/ *DeltaMs_constraint);
	if ( hist_I && hist_P){
//		signal_set->add(*bdtinclusive);
//		signal_set->add(*bdtprompt);
	}
//	if (hist_D)
//		signal_set->add(*dilution_sig);

	RooProdPdf *signal;
	if (et_model){
		signal_set->add(*et_model->pdf());
		signal = new RooProdPdf("signal", "signal",
				*signal_set,
				RooFit::Conditional(*signal_time_angle, RooArgSet(*m, *t, *cpsi, *ctheta, *phi, *p)));
	} else {
		signal_set->add(*signal_time_ange);
		signal = new RooProdPdf("signal", "signal", *signal_set);
	}

	model = signal;
}
