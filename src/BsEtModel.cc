/*
 * BsEtModel.cc
 *
 *  Created on: Jun 1, 2009
 *      Author: magania
 */

#include <BsEtModel.h>

BsEtModel::BsEtModel(const char* name, RooRealVar *et, RooDataHist *hist, int the_model):
	BsPdf()
{
        cout << glue("ET MODEL",name)  << ' ' << the_model<< endl;
	
	et_mean = new RooRealVar(glue("et_mean",name), glue("et_mean",name), 0.06, 0, 1);
	et_sigma = new RooRealVar(glue("et_sigma",name), glue("et_sigma",name), 0.02, 0,0.1);
	parameters.add(*et_mean);
	parameters.add(*et_sigma);

	RooAbsPdf *et_model=0;
	if (the_model == 1) {
	  et_xl = new RooRealVar(glue("et_xl",name), glue("et_xl",name), 0.05, 0, 1);
	  et_tau_short = new RooRealVar(glue("et_tau_short",name), glue("et_tau_short",name), 0.001, 0.0001, 1);
	  et_tau_long = new RooRealVar(glue("et_tau_long",name), glue("et_tau_long",name), 0.19, 0.001, 2);

	  parameters.add(*et_xl);
	  parameters.add(*et_tau_short);
	  parameters.add(*et_tau_long);

	  RooGaussModel *et_gauss = new RooGaussModel(glue("gauss",name), glue("gauss",name), *et, *et_mean, *et_sigma);
	  RooDecay *et_short = new RooDecay(glue("et_short",name), glue("et_short",name), *et, *et_tau_short, *et_gauss, RooDecay::SingleSided);
	  RooDecay *et_long = new RooDecay(glue("et_long",name), glue("et_long",name), *et, *et_tau_long, *et_gauss, RooDecay::SingleSided);
	  et_model = new RooAddPdf(glue("et_model",name), glue("et_model",name), *et_long, *et_short, *et_xl);
	}

	if(the_model == 2){
	  et_model = new RooLandau(glue("et_modell",name), glue("et_modell",name), *et, *et_mean, *et_sigma);
	}

	if(!et_model){
	  cout << "Et Model failed" << endl;
	  exit(EXIT_FAILURE);
	}

	et_model->fitTo(*hist);
	setConstant(the_model);

	model = et_model;
	
	//	model = new RooHistPdf(glue("et_model",name), glue("et_model",name), *et ,*hist,0);

}

void BsEtModel::setConstant(int the_model){
	et_mean->setConstant(true);
	et_sigma->setConstant(true);
	if(the_model == 1){
	  et_xl->setConstant(true);
	  et_tau_short->setConstant(true);
	  et_tau_long->setConstant(true);
	}
}
