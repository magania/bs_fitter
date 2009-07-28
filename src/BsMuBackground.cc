/*
 * BsBackground.cc
 *
 *  Created on: Jun 1, 2009
 *      Author: magania
 */

#include <BsMuBackground.h>

BsMuBackground::BsMuBackground( const char* name,
		RooRealVar *m,
		RooRealVar *t,
		RooRealVar *cpsi,
		RooRealVar *ctheta,
		RooRealVar *phi,
		RooRealVar *p,
		BsResolution *resolution_a,
                BsResolution *resolution_b,
		BsEtModel *et_model_a,
                BsEtModel *et_model_b) :
		BsPdf()
{
	       std::cout << resolution_a << ' ' << resolution_b << " " << et_model_a << ' ' << et_model_b << std::endl;
	cout << glue("MU BACKGROUND MODEL",name) << endl;
	/* Mass Pdf parameters */
        RooRealVar *mass_m0 = new RooRealVar(glue("mass_m0",name), glue("mass_m0",name), 0);
	/* Time Pdf parameters */
	RooRealVar *time_np_tm = new RooRealVar(glue("time_np_tm",name), glue("time_np_tm",name), 0);
	RooRealVar *time_np_tp = new RooRealVar(glue("time_np_tp",name), glue("time_np_tp",name), 0);
	RooRealVar *time_np_tpp = new RooRealVar(glue("time_np_tpp",name), glue("time_np_tpp",name), 0);
	RooRealVar *time_np_fm = new RooRealVar(glue("time_np_fm",name), glue("time_np_fm",name), 0);
        RooRealVar *time_np_fp = new RooRealVar(glue("time_np_fp",name), glue("time_np_fp",name), 0);
	/* Angle Pdf parameters */
		/* cpsi */
		/* ctheta */
        RooRealVar *ctheta_sigma = new RooRealVar(glue("ctheta_sigma",name), glue("ctheta_sigma",name), 0);
        RooRealVar *ctheta_a1 = new RooRealVar(glue("ctheta_a1",name), glue("ctheta_a1",name), 0);
        RooRealVar *ctheta_xs = new RooRealVar(glue("ctheta_xs",name), glue("ctheta_xs",name), 0);
		/* phi */
        RooRealVar *phi_c2 = new RooRealVar(glue("phi_c2",name), glue("phi_c2",name), 0);
        RooRealVar *phi_sigma = new RooRealVar(glue("phi_sigma",name), glue("phi_sigma",name), 0);
        RooRealVar *phi_xs = new RooRealVar(glue("phi_xs",name), glue("phi_xs",name), 0);
        RooRealVar *phi_xsg = new RooRealVar(glue("phi_xsg",name), glue("phi_xsg",name), 0);
	/* Fractions */
	RooRealVar *xp = new RooRealVar(glue("xp",name), glue("xp",name), 0);

	parameters.add(*mass_m0);
	parameters.add(*time_np_tm);
	parameters.add(*time_np_tp);
	parameters.add(*time_np_tpp);
	parameters.add(*time_np_fm);
	parameters.add(*time_np_fp);
	parameters.add(*ctheta_sigma);
        parameters.add(*ctheta_a1);
        parameters.add(*ctheta_xs);
        parameters.add(*phi_sigma);
        parameters.add(*phi_xs);
        parameters.add(*phi_xsg);
	parameters.add(*xp);

	/* Mass Pdf */
	RooPolynomial *mass = new RooPolynomial(glue("mass",name), glue("mass", name), *m, RooArgList(*mass_m0));

       std::cout << "Y" << std::endl;
	/* Time Pdf */
		/* time 2a */
	RooDecay *exp_minus_2a = new RooDecay(glue("exp_minus_2a",name), glue("exp_minus_2a",name), *t, *time_np_tm, *resolution_a->pdf(), RooDecay::Flipped);
	RooDecay *exp_plus_2a = new RooDecay(glue("exp_plus_2a",name), glue("exp_plus_2a",name), *t, *time_np_tp, *resolution_a->pdf(), RooDecay::SingleSided);
	RooDecay *exp_plus_plus_2a = new RooDecay(glue("exp_plus_plus_2a",name), glue("exp_plus_plus_2a",name), *t, *time_np_tpp, *resolution_a->pdf(), RooDecay::SingleSided);

        RooAddPdf *time_2a = new RooAddPdf(glue("time_2a",name), glue("time_2a",name),
			RooArgSet(*resolution_a->pdf(), *exp_minus_2a, *exp_plus_2a, *exp_plus_plus_2a),
			RooArgSet(*xp, *time_np_fm, *time_np_fp), true);
		/* time 2b */
        RooDecay *exp_minus_2b = new RooDecay(glue("exp_minus_2b",name), glue("exp_minus_2b",name), *t, *time_np_tm, *resolution_b->pdf(), RooDecay::Flipped);
        RooDecay *exp_plus_2b = new RooDecay(glue("exp_plus_2b",name), glue("exp_plus_2b",name), *t, *time_np_tp, *resolution_b->pdf(), RooDecay::SingleSided);
        RooDecay *exp_plus_plus_2b = new RooDecay(glue("exp_plus_plus_2b",name), glue("exp_plus_plus_2b",name), *t, *time_np_tpp, *resolution_b->pdf(), RooDecay::SingleSided);

        RooAddPdf *time_2b = new RooAddPdf(glue("time_2b",name), glue("time_2b",name),
                        RooArgSet(*resolution_b->pdf(), *exp_minus_2b, *exp_plus_2b, *exp_plus_plus_2b),
                        RooArgSet(*xp, *time_np_fm, *time_np_fp), true);

	/* Angle */
               /* cpsi */
        RooPolynomial *model_cpsi = new RooPolynomial(glue("model_cpsi", name), glue("model_cpsi", name), *cpsi, RooArgList());
		/* ctheta */
	RooGaussian *gauss_ctheta = new RooGaussian(glue("gauss_ctheta",name), glue("gauss_ctheta",name), *ctheta, RooFit::RooConst(0), *ctheta_sigma);
	RooPolynomial *poly_ctheta = new RooPolynomial(glue("poly_ctheta",name), glue("poly_ctheta",name), *ctheta, RooArgList(*ctheta_a1));
	RooAddPdf *model_ctheta = new RooAddPdf(glue("model_ctheta",name), glue("model_ctheta",name), *gauss_ctheta, *poly_ctheta, *ctheta_xs);
		/* phi */
	RooGaussian *gauss_phi = new RooGaussian(glue("gauss_phi",name), glue("gauss_phi",name), *phi, RooFit::RooConst(0), *phi_sigma);
	RooGaussian *gauss_phi_m = new RooGaussian(glue("gauss_phi_m",name), glue("gauss_phi_minus",name), *phi, RooFit::RooConst(-TMath::Pi()), *phi_sigma);
	RooGaussian *gauss_phi_p = new RooGaussian(glue("gauss_phi_p",name), glue("gauss_phi_plus",name), *phi, RooFit::RooConst(TMath::Pi()), *phi_sigma);
	RooPolynomial *poly_phi = new RooPolynomial(glue("poly_phi",name), glue("poly phi",name), *phi, RooArgList(RooFit::RooConst(0), *phi_c2));
	RooAddPdf *model_phi = new RooAddPdf(glue("model_phi",name), glue("model_phi",name), 
	RooArgList(*poly_phi, *gauss_phi, *gauss_phi_m, *gauss_phi_p), RooArgList(*phi_xs, *phi_xsg, RooFit::RooConst(0.5)), true);

	RooProdPdf *angle = new RooProdPdf(glue("angle",name), glue("angle",name), RooArgList(*model_cpsi, *model_ctheta, *model_phi));

//	std::cout << "X" << std::endl;
	if ( et_model_a )
		background_a = new RooProdPdf(glue("bkg_2a",name), glue("bkg_2a",name), 
				RooArgSet(*mass, *angle, *et_model_a->pdf()),
				RooFit::Conditional(*time_2a, RooArgSet(*m, *t, *cpsi, *ctheta, *phi)));
	 else 
		background_a = new RooProdPdf(glue("bkg_2a",name), glue("bkg_2a",name),
				RooArgSet(*mass, *time_2a, *angle));
        if ( et_model_b )
                background_b = new RooProdPdf(glue("bkg_2b",name), glue("bkg_2b",name), 
                                RooArgSet(*mass, *angle, *et_model_b->pdf()),
                                RooFit::Conditional(*time_2b, RooArgSet(*m, *t, *cpsi, *ctheta, *phi)));
         else 
                background_b = new RooProdPdf(glue("bkg_2b",name), glue("bkg_2b",name),
                                RooArgSet(*mass, *time_2b, *angle));
}
