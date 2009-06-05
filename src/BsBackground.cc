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
	/* Angle Pdf parameters */
	RooRealVar *y1 = new RooRealVar(glue("y1",name), glue("y1",name), 0);
	RooRealVar *fp = new RooRealVar(glue("fp",name), glue("fp",name), 0);
	RooRealVar *y2 = new RooRealVar(glue("y2",name), glue("y2",name), 0);
	RooRealVar *y4 = new RooRealVar(glue("y4",name), glue("y4",name), 0);
	RooRealVar *y5 = new RooRealVar(glue("y5",name), glue("y5",name), 0);
	RooRealVar *y6 = new RooRealVar(glue("y6",name), glue("y6",name), 0);

	parameters.add(*m_a1);
//	parameters.add(*m_a2);
	parameters.add(*xp);
	parameters.add(*t_m);
	parameters.add(*t_p);
	parameters.add(*t_pp);
	parameters.add(*fm);
	parameters.add(*fp);
	parameters.add(*y1);
	parameters.add(*y2);
	parameters.add(*y4);
	parameters.add(*y5);
	parameters.add(*y6);

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
	RooPolynomial *cpsi_bkg = new RooPolynomial(glue("cpsi_bkg",name), glue("cpsi_bkg",name), *cpsi, RooArgSet(RooFit::RooConst(0), *y1));
	RooPolynomial *ctheta_bkg = new RooPolynomial(glue("ctheta_bkg",name), glue("ctheta_bkg",name), *ctheta, RooArgSet(RooFit::RooConst(0), *y2));
	RooGenericPdf *phi_bkg = new RooGenericPdf(glue("phi_bkg",name), glue("phi_bkg",name), "1+@1*sin(@0*@2+@3)", RooArgList(*phi, *y4, *y5, *y6));

	RooProdPdf *angle_bkg = new RooProdPdf(glue("angle_bkg",name), glue("angle_bkg",name), RooArgSet(*cpsi_bkg, *ctheta_bkg, *phi_bkg));

	RooArgSet *background_set = new RooArgSet(*mass_bkg, *angle_bkg);

	RooProdPdf *background;
	if ( et_model ){
		background_set->add(*et_model->pdf());

		background = new RooProdPdf(glue("background	",name), glue("background",name), *background_set,
				RooFit::Conditional(*time_bkg, RooArgSet(*m, *t, *cpsi, *ctheta, *phi)));
	} else {
		background_set->add(*time_bkg);
		background = new RooProdPdf(glue("background",name), glue("background",name), *background_set);
	}

	model = background;
}
