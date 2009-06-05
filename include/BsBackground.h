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
		BsResolution *resolution,
		BsEtModel *et_model);
};

#endif /* BSBACKGROUND_H_ */
