/*
 * BsMuBackground.h
 *
 *  Created on: Jul 2,, 2009
 *      Author: magania
 */

#ifndef BSMUBACKGROUND_H_
#define BSMUBACKGROUND_H_

#include <BsResolution.h>
#include <BsEtModel.h>
#include <TMath.h>

#include <BsPdf.h>

class BsMuBackground : public BsPdf {
public:
	BsMuBackground( const char* name,
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

#endif /* BSMUBACKGROUND_H_ */
