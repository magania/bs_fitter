/*
 * BsResolution.cc
 *
 *  Created on: Jun 2, 2009
 *      Author: magania
 */

#include <BsResolution.h>

BsResolution::BsResolution(const char *name, RooRealVar *t,RooRealVar *et) :
	BsPdf()
{
	std::cout << glue("RESOLUTION MODEL",name) << std::endl;

	RooRealVar *S = new RooRealVar(glue("S",name), glue("S",name), 0);
	parameters.add(*S);
	RooGaussModel *resolution = new RooGaussModel(glue("resolution",name), glue("resolution",name), *t, RooFit::RooConst(0), *S, *et);
	model = resolution;
}

RooResolutionModel *BsResolution::pdf(){
	return model;
}
