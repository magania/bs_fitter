/*
 * BsDMuBackground.h
 *
 *  Created on: Jul 27, 2009
 *      Author: magania
 */

#ifndef BSDMUBACKGROUND_H_
#define BSDMUBACKGROUND_H_

#include <BsResolution.h>
#include <BsEtModel.h>
#include <TMath.h>

#include <BsPdf.h>

class BsDMuBackground : public BsPdf {
public:
	BsDMuBackground( const char* name,
		RooRealVar *m,
		RooRealVar *t,
		RooRealVar *cpsi,
		RooRealVar *ctheta,
		RooRealVar *phi,
		RooRealVar *p,
		BsResolution *resolution_a,
                BsResolution *resolution_b,
		BsEtModel *et_model_a,
                BsEtModel *et_model_b);

	RooAbsPdf* pdf_2a() {return background_a;};
	RooAbsPdf* pdf_2b() {return background_b;};

private:
	RooAbsPdf *background_a, *background_b;
};

#endif /* BSDMUBACKGROUND_H_ */
