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
		RooRealVar *bdtI,
		RooRealVar *bdtP,
		BsResolution *resolution,
		BsEtModel *et_model,
		RooDataHist *hist_I, RooDataHist *hist_P, RooDataHist* hist_D,
		int angle_pdf) :
		BsPdf()
{
	cout << glue("BACKGROUND MODEL",name) << endl;
	/* Mass Pdf parameters */
//	RooRealVar *m_a1 = new RooRealVar(glue("m_a1",name), glue("m_a1",name), 0);
      RooRealVar *m_m0 = new RooRealVar(glue("m_m0",name), glue("m_m0",name), 0);
      RooRealVar *m_c = new RooRealVar(glue("m_c",name), glue("m_c",name), 0);
      RooRealVar *m_p = new RooRealVar(glue("m_p",name), glue("m_p",name), 0);
	RooRealVar *m_t = new RooRealVar(glue("m_t",name), glue("m_t",name), 0);
	/* Time Pdf parameters */
	RooRealVar *t_m = new RooRealVar(glue("t_m",name), glue("t_m",name), 0);
	RooRealVar *t_p = new RooRealVar(glue("t_p",name), glue("t_p",name), 0);
	RooRealVar *t_pp = new RooRealVar(glue("t_pp",name), glue("t_pp",name), 0);
	RooRealVar *xp = new RooRealVar(glue("xp",name), glue("xp",name), 0);
	RooRealVar *fm = new RooRealVar(glue("fm",name), glue("fm",name), 0);
    RooRealVar *fp = new RooRealVar(glue("fp",name), glue("fp",name), 0);

	parameters.add(*m_m0);
        parameters.add(*m_c);
        parameters.add(*m_p);
	parameters.add(*m_t);
	parameters.add(*xp);
	parameters.add(*t_m);
	parameters.add(*t_p);
	parameters.add(*t_pp);
	parameters.add(*fm);
	parameters.add(*fp);

	/* Mass Pdf */
//	RooPolynomial *mass_p_bkg = new RooPolynomial(glue("mass_p_bkg",name), glue("mass_p_bkg",name), *m, RooArgList(*m_a1));
	RooArgusBG *mass_p_bkg = new RooArgusBG(glue("mass_p_bkg",name), glue("mass_p_bkg",name), *m, *m_m0, *m_c, *m_p);
	RooExponential *mass_np_bkg = new RooExponential(glue("mass_np_bkg",name), glue("mass_np_bkg",name), *m, *m_t);

	/* Time Pdf */
	RooDecay *exp_minus = new RooDecay(glue("exp_minus",name), glue("exp_minus",name), *t, *t_m, *resolution->pdf(), RooDecay::Flipped);
	RooDecay *exp_plus = new RooDecay(glue("exp_plus",name), glue("exp_plus",name), *t, *t_p, *resolution->pdf(), RooDecay::SingleSided);
	RooDecay *exp_plus_plus = new RooDecay(glue("exp_plus_plus",name), glue("exp_plus_plus",name), *t, *t_pp, *resolution->pdf(), RooDecay::SingleSided);

	RooAddPdf *time_np = new RooAddPdf(glue("time_np",name), glue("time_np",name),
			RooArgSet(*exp_minus, *exp_plus, *exp_plus_plus),
			RooArgSet(*fm, *fp), true);

	RooProdPdf *mass_time_p = new RooProdPdf(glue("mass_time_p",name), glue("mass_time_p",name), RooArgSet(*mass_p_bkg,*resolution->pdf() ));
	RooProdPdf *mass_time_np = new RooProdPdf(glue("mass_time_np",name), glue("mass_time_np",name), RooArgSet(*mass_np_bkg, *time_np));

	RooAddPdf *mass_time = new RooAddPdf(glue("mass_time",name), glue("mass_time",name), *mass_time_p, *mass_time_np, *xp);


	RooHistPdf *bdtinclusive = new RooHistPdf(glue("bdtinclusive_bkg",name), glue("bdtinclusive_bkg",name), RooArgSet(*bdtI), *hist_I);
	RooHistPdf *bdtprompt = new RooHistPdf(glue("bdtprompt_bkg",name), glue("bdtprompt_bkg",name), RooArgSet(*bdtP), *hist_P);
	RooHistPdf *dilution_bkg = new RooHistPdf(glue("dilution_bkg",name), glue("dilution_bkg",name), RooArgSet(*p), *hist_D);

	RooAbsPdf *angle = 0;
	if (angle_pdf == 0 )
		angle = angle0(name, cpsi, ctheta, phi);
	if (angle_pdf == 3)
		angle = angle3(name, cpsi, ctheta, phi);
	if (angle_pdf == 4)
		angle = angle4(name, cpsi, ctheta, phi);

	RooArgSet *background_set = new RooArgSet(/*bdtinclusive, *bdtprompt, *dilution_bkg*/);
	if (angle)
		background_set->add(*angle);
	RooProdPdf *background;
	if ( et_model ){
		background_set->add(*et_model->pdf());

		background = new RooProdPdf(glue("background",name), glue("background",name), *background_set,
				RooFit::Conditional(*mass_time, RooArgSet(*m, *t, *cpsi, *ctheta, *phi)));
	} else {
		background_set->add(*mass_time);
		background = new RooProdPdf(glue("background",name), glue("background",name), *background_set);
	}

	model = background;
}

RooAbsPdf* BsBackground::angle0( const char* name,
		RooRealVar *cpsi,
		RooRealVar *ctheta,
		RooRealVar *phi) {
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

	parameters.add(*a1_cpsi);
	parameters.add(*sigma_ctheta);
	parameters.add(*a1_ctheta);
	parameters.add(*xs_ctheta);
	parameters.add(*sigma_phi);
    parameters.add(*xs_phi);
    parameters.add(*xsg_phi);

	/* Angle */
		/* cpsi */
	RooPolynomial *model_cpsi = new RooPolynomial(glue("model_cpsi",name), glue("model_cpsi",name), *cpsi, RooArgList(RooFit::RooConst(0),*a1_cpsi));
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

	return angle_bkg;
}

RooAbsPdf* BsBackground::angle3( const char* name,
		RooRealVar *cpsi,
		RooRealVar *ctheta,
		RooRealVar *phi) {
	/* Angle Pdf parameters */
	/* cpsi */
	/* ctheta */
	RooRealVar *ctheta_a1 = new RooRealVar(glue("ctheta_a1",name), glue("ctheta_a1",name), 0);
	RooRealVar *ctheta_a2 = new RooRealVar(glue("ctheta_a2",name), glue("ctheta_a2",name), 0);
	/* phi */
	RooRealVar *phi_a1 = new RooRealVar(glue("phi_a1",name), glue("phi_a1",name), 0);
	RooRealVar *phi_a2 = new RooRealVar(glue("phi_a2",name), glue("phi_a2",name), 0);
	RooRealVar *phi_a3 = new RooRealVar(glue("phi_a3",name), glue("phi_a3",name), 0);
	RooRealVar *phi_a4 = new RooRealVar(glue("phi_a4",name), glue("phi_a4",name), 0);
	RooRealVar *phi_a5 = new RooRealVar(glue("phi_a5",name), glue("phi_a5",name), 0);
	RooRealVar *phi_a6 = new RooRealVar(glue("phi_a6",name), glue("phi_a6",name), 0);
	RooRealVar *phi_a7 = new RooRealVar(glue("phi_a7",name), glue("phi_a7",name), 0);

	parameters.add(*ctheta_a1);
	parameters.add(*ctheta_a2);
	parameters.add(*phi_a1);
	parameters.add(*phi_a2);
	parameters.add(*phi_a3);
	parameters.add(*phi_a4);
	parameters.add(*phi_a5);
	parameters.add(*phi_a6);
	parameters.add(*phi_a7);


	 /* Angle */
	/* cpsi */
	RooPolynomial *model_cpsi = new RooPolynomial(glue("model_cpsi", name), glue("model_cpsi", name), *cpsi, RooArgList());
	/* ctheta */
	RooPolynomial *model_ctheta = new RooPolynomial(glue("model_ctheta",name), glue("model_ctheta",name), *ctheta,
			RooArgList(RooFit::RooConst(0), *ctheta_a1, RooFit::RooConst(0), *ctheta_a2));
	/* phi */
	RooPolynomial *model_phi = new RooPolynomial(glue("model_phi",name), glue("model_phi",name), *phi,
			RooArgList(*phi_a1, *phi_a2, *phi_a3, *phi_a4, *phi_a5, *phi_a6, *phi_a7));

	RooProdPdf *angle = new RooProdPdf(glue("angle",name), glue("angle",name), RooArgList(*model_cpsi, *model_ctheta, *model_phi));

	return angle;
}

RooAbsPdf* BsBackground::angle4( const char* name,
		RooRealVar *cpsi,
		RooRealVar *ctheta,
		RooRealVar *phi) {
	/* Angle Pdf parameters */
	/* cpsi */
	RooRealVar *cpsi_a2 = new RooRealVar(glue("cpsi_a2",name), glue("cpsi_a2",name), 0);
	/* ctheta */
	RooRealVar *ctheta_a1 = new RooRealVar(glue("ctheta_a1",name), glue("ctheta_a1",name), 0);
	RooRealVar *ctheta_a2 = new RooRealVar(glue("ctheta_a2",name), glue("ctheta_a2",name), 0);
	/* phi */
	RooRealVar *phi_a1 = new RooRealVar(glue("phi_a1",name), glue("phi_a1",name), 0);
	RooRealVar *phi_a2 = new RooRealVar(glue("phi_a2",name), glue("phi_a2",name), 0);
	RooRealVar *phi_a3 = new RooRealVar(glue("phi_a3",name), glue("phi_a3",name), 0);
	RooRealVar *phi_a4 = new RooRealVar(glue("phi_a4",name), glue("phi_a4",name), 0);
	RooRealVar *phi_a5 = new RooRealVar(glue("phi_a5",name), glue("phi_a5",name), 0);
	RooRealVar *phi_a6 = new RooRealVar(glue("phi_a6",name), glue("phi_a6",name), 0);
	RooRealVar *phi_a7 = new RooRealVar(glue("phi_a7",name), glue("phi_a7",name), 0);

	parameters.add(*cpsi_a2);
	parameters.add(*ctheta_a1);
	parameters.add(*ctheta_a2);
	parameters.add(*phi_a1);
	parameters.add(*phi_a2);
	parameters.add(*phi_a3);
	parameters.add(*phi_a4);
	parameters.add(*phi_a5);
	parameters.add(*phi_a6);
	parameters.add(*phi_a7);

	RooFormulaVar *aphi = new RooFormulaVar(glue("aphi",name),glue("aphi",name), "TMath::Abs(@0)", RooArgSet(*phi));

	 /* Angle */
	/* cpsi */
	RooPolynomial *model_cpsi = new RooPolynomial(glue("model_cpsi", name), glue("model_cpsi", name), *cpsi, RooArgList(RooFit::RooConst(0),*cpsi_a2));
	/* ctheta */
	RooPolynomial *model_ctheta = new RooPolynomial(glue("model_ctheta",name), glue("model_ctheta",name), *ctheta,
			RooArgList(RooFit::RooConst(0), *ctheta_a1, RooFit::RooConst(0), *ctheta_a2));
	/* phi */
	RooPolynomial *model_phi = new RooPolynomial(glue("model_phi",name), glue("model_phi",name), *aphi,
			RooArgList(*phi_a1, *phi_a2, *phi_a3, *phi_a4, *phi_a5, *phi_a6, *phi_a7));

	RooProdPdf *angle = new RooProdPdf(glue("angle",name), glue("angle",name), RooArgList(*model_cpsi, *model_ctheta, *model_phi));

	return angle;
}


