/*
 * BsResolution.h
 *
 *  Created on: Jun 2, 2009
 *      Author: magania
 */

#ifndef BSRESOLUTION_H_
#define BSRESOLUTION_H_

#include <BsPdf.h>

class BsResolution : public BsPdf{
public:
	BsResolution(const char *name, RooRealVar *t,RooRealVar *et);
	RooResolutionModel *pdf();
private:
	RooResolutionModel *model;
};

#endif /* BSRESOLUTION_H_ */
