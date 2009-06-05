/*
 * BsPdf.cc
 *
 *  Created on: Jun 2, 2009
 *      Author: magania
 */

#include <BsPdf.h>

BsPdf::BsPdf() :
	parameters(){
}

RooAbsPdf *BsPdf::pdf(){
	return model;
}

char *BsPdf::glue(const char* a, const char *b){
	char *tmp = (char*) malloc(strlen(a)+strlen(b)+2);
	strcpy(tmp, a);
	strcat(tmp, "_");
	strcat(tmp, b);
	return tmp;
}

RooArgSet* BsPdf::getParameters() {
	return &parameters;
}
