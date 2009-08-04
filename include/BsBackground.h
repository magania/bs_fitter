/*
 * BsBackground.h
 *
 *  Created on: Jun 2, 2009
 *      Author: magania
 */

#ifndef BSBACKGROUND_H_
#define BSBACKGROUND_H_

#include <BsResolution.h>
#include <BsEtModel.h>
#include <TMath.h>

#include <BsPdf.h>

class BsBackground : public BsPdf {
public:
	BsBackground( const char* name,
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
		RooDataHist *bkg_hist_I, RooDataHist *bkg_hist_P);

private:
	RooAbsPdf* angle0( const char* name,
			RooRealVar *cpsi,
			RooRealVar *ctheta,
			RooRealVar *phi);
};

#endif /* BSBACKGROUND_H_ */
