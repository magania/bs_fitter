/*
 * BsEtModel.h
 *
 *  Created on: Jun 2, 2009
 *      Author: magania
 */

#ifndef BSETMODEL_H_
#define BSETMODEL_H_

#include <BsPdf.h>

class BsEtModel : public BsPdf{
public:
	BsEtModel(const char *name, RooRealVar *et);
	void setConstant();
private:
	RooRealVar *et_xl, *et_mean, *et_sigma, *et_tau_short, *et_tau_long;
};

#endif /* BSETMODEL_H_ */
