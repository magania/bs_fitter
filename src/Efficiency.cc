#include "Efficiency.h"

Efficiency::Efficiency() :
a1("a1", "a1", 0),
a2("a2", "a2", 0),
a3("a3", "a3", 0),
a4("a4", "a4", 0),
a5("a5", "a5", 0),
a6("a6", "a6", 0),
a7("a7", "a7", 0),
a8("a8", "a8", 0),
a9("a9", "a9", 0),
b1("b1", "b1", 0),
b2("b2", "b2", 0),
b3("b3", "b3", 0),
b4("b4", "b4", 0),
b5("b5", "b5", 0),
b6("b6", "b6", 0),
b7("b7", "b7", 0),
b8("b8", "b8", 0),
b9("b9", "b9", 0),
c1("c1", "c1", 0),
c2("c2", "c2", 0),
c3("c3", "c3", 0),
c4("c4", "c4", 0),
c5("c5", "c5", 0),
c6("c6", "c6", 0),
c7("c7", "c7", 0),
c8("c8", "c8", 0),
c9("c9", "c9", 0),
As(a1, a2, a3, a4, a5, a6, a7, a8, a9),
Bs(b1, b2, b3, b4, b5, b6, b7, b8, b9),
Cs(c1, c2, c3, c4, c5, c6, c7, c8, c9)
{
}

void Efficiency::setEfficiency(const char* efficiency) {
    As.readFromFile(efficiency);
    Bs.readFromFile(efficiency);
    Cs.readFromFile(efficiency);
}

void Efficiency::writeEfficiency(const char* file) {
    std::ofstream out(file);
    As.writeToStream(out, kFALSE);
    Bs.writeToStream(out, kFALSE);
    Cs.writeToStream(out, kFALSE);
    out.close();
}

RooArgSet* Efficiency::getAs(){
    return &As;
}

RooArgSet* Efficiency::getBs(){
    return &Bs;
}

RooArgSet* Efficiency::getCs(){
    return &Cs;
}
