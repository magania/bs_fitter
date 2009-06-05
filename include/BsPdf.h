/*
 * BsPdf.h
 *
 *  Created on: Jun 2, 2009
 *      Author: magania
 */

#ifndef BSPDF_H_
#define BSPDF_H_

#include <stdlib.h>
#include <stdio.h>

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooAbsPdf.h"
#include "RooResolutionModel.h"
#include "RooFitResult.h"
#include "RooDataSet.h"
#include "RooGlobalFunc.h"
#include "RooProdPdf.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooGaussModel.h"
#include "RooTruthModel.h"
#include "RooLandau.h"
#include "RooProdPdf.h"
#include "RooChebychev.h"
#include "RooExponential.h"
#include "RooDecay.h"
#include "RooPolynomial.h"
#include "RooGenericPdf.h"
#include "RooHistPdf.h"

class BsPdf{
public:
	BsPdf();
	RooAbsPdf *pdf();
	char * glue(const char* a, const char *b);
	RooArgSet* getParameters();

protected:
	RooArgSet parameters;
	RooAbsPdf *model;
};

#endif /* BSPDF_H_ */
