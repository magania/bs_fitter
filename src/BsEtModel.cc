/*
 * BsEtModel.cc
 *
 *  Created on: Jun 1, 2009
 *      Author: magania
 */

#include <BsEtModel.h>

BsEtModel::BsEtModel(const char* name, RooRealVar *et):
	BsPdf()
{
	cout << glue("ET MODEL",name) << endl;

	et_xl = new RooRealVar(glue("et_xl",name), glue("et_xl",name), 0);
	et_mean = new RooRealVar(glue("et_mean",name), glue("et_mean",name), 0);
	et_sigma = new RooRealVar(glue("et_sigma",name), glue("et_sigma",name), 0);
	et_tau_short = new RooRealVar(glue("et_tau_short",name), glue("et_tau_short",name), 0);
	et_tau_long = new RooRealVar(glue("et_tau_long",name), glue("et_tau_long",name), 0);
	parameters.add(*et_xl);
	parameters.add(*et_mean);
	parameters.add(*et_sigma);
	parameters.add(*et_tau_short);
	parameters.add(*et_tau_long);

	RooGaussModel *et_gauss = new RooGaussModel(glue("gauss",name), glue("gauss",name), *et, *et_mean, *et_sigma);
	RooDecay *et_short = new RooDecay(glue("et_short",name), glue("et_short",name), *et, *et_tau_short, *et_gauss, RooDecay::SingleSided);
	RooDecay *et_long = new RooDecay(glue("et_long",name), glue("et_long",name), *et, *et_tau_long, *et_gauss, RooDecay::SingleSided);
	RooAddPdf *et_model = new RooAddPdf(glue("et_model",name), glue("et_model",name), *et_long, *et_short, *et_xl);

	model = et_model;
}

void BsEtModel::setConstant(){
	et_xl->setConstant(true);
	et_mean->setConstant(true);
	et_sigma->setConstant(true);
	et_tau_short->setConstant(true);
	et_tau_long->setConstant(true);
}
