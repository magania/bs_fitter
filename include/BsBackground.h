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
#include <RooArgusBG.h>

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
		RooDataHist *hist_I, RooDataHist *hist_P, RooDataHist *hist_D,
		int angle_pdf);

private:
	RooAbsPdf* angle0( const char* name,
			RooRealVar *cpsi,
			RooRealVar *ctheta,
			RooRealVar *phi);
	RooAbsPdf* angle3( const char* name,
			RooRealVar *cpsi,
			RooRealVar *ctheta,
			RooRealVar *phi);
	RooAbsPdf* angle4( const char*, RooRealVar*,RooRealVar*,RooRealVar*);
};

#endif /* BSBACKGROUND_H_ */
