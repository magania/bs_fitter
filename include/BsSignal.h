/*
 * BsSignal.h
 *
 *  Created on: May 29, 2009
 *      Author: magania
 */

#ifndef BSSIGNAL_H_
#define BSSIGNAL_H_

#include <RooBsTimeAngle.h>


#include <BsResolution.h>
#include <BsEtModel.h>
#include <Efficiency.h>

#include <BsPdf.h>

class BsSignal : public BsPdf{
public:
	BsSignal(const char* name,
			RooRealVar *m,
			RooRealVar *t,
			RooRealVar *cpsi,
			RooRealVar *ctheta,
			RooRealVar *phi,
			RooRealVar *p,
			RooRealVar *M,
			RooRealVar *Sigma,
			RooRealVar *A0,
			RooRealVar *A1,
			RooRealVar *DeltaGamma,
			RooRealVar *Phi_s,
			RooRealVar *Delta1,
			RooRealVar *Delta2,
			RooRealVar *Tau,
			RooRealVar *DeltaMs,
			RooRealVar *Delta1_mean, RooRealVar *Delta1_sigma,
			RooRealVar *Delta2_mean, RooRealVar *Delta2_sigma,
			RooRealVar *DeltaMs_mean, RooRealVar *DeltaMs_sigma,
			BsResolution *resolution, BsEtModel *et_model, Efficiency *efficiency);
};

#endif /* BSSIGNAL_H_ */
