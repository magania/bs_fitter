/*
 * BsDMuBackground.cc
 *
 *  Created on: Jul 27, 2009
 *      Author: magania
 */

#include <BsDMuBackground.h>

BsDMuBackground::BsDMuBackground( const char* name,
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
	cout << glue("DMU BACKGROUND MODEL",name) << endl;
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
        RooRealVar *ctheta_a1 = new RooRealVar(glue("ctheta_a1",name), glue("ctheta_a1",name), 0);
        RooRealVar *ctheta_a2 = new RooRealVar(glue("ctheta_a2",name), glue("ctheta_a2",name), 0);
                /* phi */
        RooRealVar *phi_a1 = new RooRealVar(glue("phi_a1",name), glue("phi_a1",name), 0);
        RooRealVar *phi_a2 = new RooRealVar(glue("phi_a2",name), glue("phi_a2",name), 0);
        RooRealVar *phi_a3 = new RooRealVar(glue("phi_a3",name), glue("phi_a3",name), 0);
	/* Fractions */
	RooRealVar *xp = new RooRealVar(glue("xp",name), glue("xp",name), 0);

	parameters.add(*mass_m0);
	parameters.add(*time_np_tm);
	parameters.add(*time_np_tp);
	parameters.add(*time_np_tpp);
	parameters.add(*time_np_fm);
	parameters.add(*time_np_fp);
        parameters.add(*ctheta_a1);
        parameters.add(*ctheta_a2);
        parameters.add(*phi_a1);
        parameters.add(*phi_a2);
	parameters.add(*phi_a3);
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
        RooPolynomial *model_ctheta = new RooPolynomial(glue("model_ctheta",name), glue("model_ctheta",name), *ctheta,
                    RooArgList(RooFit::RooConst(0), *ctheta_a1, RooFit::RooConst(0), *ctheta_a2));
                /* phi */
        RooPolynomial *model_phi = new RooPolynomial(glue("model_phi",name), glue("model_phi",name), *phi,  
                    RooArgList(RooFit::RooConst(0), *phi_a1, RooFit::RooConst(0), *phi_a2, RooFit::RooConst(0), *phi_a3));

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
