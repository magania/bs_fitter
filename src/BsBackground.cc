/*
 * BsBackground.cc
 *
 *  Created on: Jun 1, 2009
 *      Author: magania
 */

#include <BsBackground.h>

BsBackground::BsBackground( const char* name,
		RooRealVar *m,
		RooRealVar *t,
		RooRealVar *cpsi,
		RooRealVar *ctheta,
		RooRealVar *phi,
		RooRealVar *p,
		BsResolution *resolution,
		BsEtModel *et_model) :
		BsPdf()
{
	cout << glue("BACKGROUND MODEL",name) << endl;
	/* Mass Pdf parameters */
	RooRealVar *m_a1 = new RooRealVar(glue("m_a1",name), glue("m_a1",name), 0);
//	RooRealVar *m_a2 = new RooRealVar("m_a2", "m_p_a2", 0);
	/* Time Pdf parameters */
	RooRealVar *t_m = new RooRealVar(glue("t_m",name), glue("t_m",name), 0);
	RooRealVar *t_p = new RooRealVar(glue("t_p",name), glue("t_p",name), 0);
	RooRealVar *t_pp = new RooRealVar(glue("t_pp",name), glue("t_pp",name), 0);
	RooRealVar *xp = new RooRealVar(glue("xp",name), glue("xp",name), 0);
	RooRealVar *fm = new RooRealVar(glue("fm",name), glue("fm",name), 0);
        RooRealVar *fp = new RooRealVar(glue("fp",name), glue("fp",name), 0);
	/* Angle Pdf parameters */
		/* cpsi */
	RooRealVar *a1_cpsi = new RooRealVar(glue("a1_cpsi",name), glue("a1_cpsi",name), 0);
		/* ctheta */
	RooRealVar *sigma_ctheta = new RooRealVar(glue("sigma_ctheta",name), glue("sigma_ctheta",name), 0);
	RooRealVar *a1_ctheta = new RooRealVar(glue("a1_ctheta",name), glue("a1_ctheta",name), 0);
	RooRealVar *xs_ctheta = new RooRealVar(glue("xs_ctheta",name), glue("xs_ctheta",name), 0);
		/* phi */
	RooRealVar *sigma_phi = new RooRealVar(glue("sigma_phi",name), glue("sigma_phi",name), 0);
	RooRealVar *xs_phi = new RooRealVar(glue("xs_phi",name), glue("xs_phi",name), 0);
	RooRealVar *xsg_phi = new RooRealVar(glue("xsg_phi",name), glue("xsg_phi",name), 0);

	parameters.add(*m_a1);
//	parameters.add(*m_a2);
	parameters.add(*xp);
	parameters.add(*t_m);
	parameters.add(*t_p);
	parameters.add(*t_pp);
	parameters.add(*fm);
	parameters.add(*fp);
	parameters.add(*a1_cpsi);
	parameters.add(*sigma_ctheta);
	parameters.add(*a1_ctheta);
	parameters.add(*xs_ctheta);
	parameters.add(*sigma_phi);
        parameters.add(*xs_phi);
        parameters.add(*xsg_phi);

	/* Mass Pdf */
	RooPolynomial *mass_bkg = new RooPolynomial(glue("mass_bkg",name), glue("mass_bkg",name), *m, RooArgList(*m_a1));

	/* Time Pdf */
	RooDecay *exp_minus = new RooDecay(glue("exp_minus",name), glue("exp_minus",name), *t, *t_m, *resolution->pdf(), RooDecay::Flipped);
	RooDecay *exp_plus = new RooDecay(glue("exp_plus",name), glue("exp_plus",name), *t, *t_p, *resolution->pdf(), RooDecay::SingleSided);
	RooDecay *exp_plus_plus = new RooDecay(glue("exp_plus_plus",name), glue("exp_plus_plus",name), *t, *t_pp, *resolution->pdf(), RooDecay::SingleSided);

	RooAddPdf *time_bkg = new RooAddPdf(glue("time_bkg",name), glue("time_bkg",name),
			RooArgSet(*resolution->pdf(), *exp_minus, *exp_plus, *exp_plus_plus),
			RooArgSet(*xp, *fm, *fp), true);

	/* Angle */
		/* cpsi */
	RooPolynomial *model_cpsi = new RooPolynomial(glue("model_cpsi",name), glue("model_cpsi",name), *cpsi, RooArgList(*a1_cpsi));
		/* ctheta */
	RooGaussian *gauss_ctheta = new RooGaussian(glue("gauss_ctheta",name), glue("gauss_ctheta",name), *ctheta, RooFit::RooConst(0), *sigma_ctheta);
	RooPolynomial *poly_ctheta = new RooPolynomial(glue("poly_ctheta",name), glue("poly_ctheta",name), *ctheta, RooArgList(*a1_ctheta));
	RooAddPdf *model_ctheta = new RooAddPdf(glue("model_ctheta",name), glue("model_ctheta",name), *gauss_ctheta, *poly_ctheta, *xs_ctheta);
		/* phi */
	RooGaussian *gauss_phi = new RooGaussian(glue("gauss_phi",name), glue("gauss_phi",name), *phi, RooFit::RooConst(0), *sigma_phi);
	RooGaussian *gauss_phi_m = new RooGaussian(glue("gauss_phi_m",name), glue("gauss_phi_minus",name), *phi, RooFit::RooConst(-TMath::Pi()), *sigma_phi);
	RooGaussian *gauss_phi_p = new RooGaussian(glue("gauss_phi_p",name), glue("gauss_phi_plus",name), *phi, RooFit::RooConst(TMath::Pi()), *sigma_phi);
	RooPolynomial *poly_phi = new RooPolynomial(glue("poly_phi",name), glue("poly phi",name), *phi, RooArgList());
	RooAddPdf *model_phi = new RooAddPdf(glue("model_phi",name), glue("model_phi",name), RooArgList(*poly_phi, *gauss_phi, *gauss_phi_m, *gauss_phi_p), RooArgList(*xs_phi, *xsg_phi, RooFit::RooConst(0.5)), true);

	RooProdPdf *angle_bkg = new RooProdPdf(glue("angle_bkg",name), glue("angle_bkg",name), RooArgSet(*model_cpsi, *model_ctheta, *model_phi));
	RooArgSet *background_set = new RooArgSet(*mass_bkg, *angle_bkg);

	RooProdPdf *background;
	if ( et_model ){
		background_set->add(*et_model->pdf());

		background = new RooProdPdf(glue("background",name), glue("background",name), *background_set,
				RooFit::Conditional(*time_bkg, RooArgSet(*m, *t, *cpsi, *ctheta, *phi)));
	} else {
		background_set->add(*time_bkg);
		background = new RooProdPdf(glue("background",name), glue("background",name), *background_set);
	}

	model = background;
}
