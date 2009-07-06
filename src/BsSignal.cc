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
		BsResolution *resolution, BsEtModel *et_model, Efficiency *efficiency) :
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

	/* Signal Pdf */
	RooProdPdf *signal;
	if (et_model){
		signal = new RooProdPdf("signal", "signal",
				RooArgSet(*signal_mass, *et_model->pdf(), /* *Delta1_constraint, *Delta2_constraint,*/ *DeltaMs_constraint),
				RooFit::Conditional(*signal_time_angle, RooArgSet(*m, *t, *cpsi, *ctheta, *phi, *p)));
	} else {
		signal = new RooProdPdf("signal", "signal",
				RooArgSet(*signal_mass, *signal_time_angle, /* *Delta1_constraint, *Delta2_constraint,*/ *DeltaMs_constraint));
	}

	model = signal;
}
