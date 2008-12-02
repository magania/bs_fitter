#include "Phis.h"
#include "BsFitter.h"

Phis::Phis() :
Phi1("Phi1", "Phi1",0),
Phi2("Phi2", "Phi2", 0),
Phi3("Phi3", "Phi3", 0),
Phi4("Phi4", "Phi4", 0),
Phi5("Phi5", "Phi5", 0),
Phi6("Phi6", "Phi6", 0),
phis(Phi1, Phi2, Phi3, Phi4, Phi5, Phi6)
{
}

void Phis::setPhis(const char* file){
    phis.readFromFile(file);
}

void Phis::writePhis(const char* file){
    phis.writeToFile(file);
}

RooArgSet* Phis::getPhis(){
    return &phis;
}

Phis::~Phis() {
    cout << "Phis destructor" << endl;
}
