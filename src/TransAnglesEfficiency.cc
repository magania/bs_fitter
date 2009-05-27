#include "TransAnglesEfficiency.h"
#include <iostream>

TransAnglesEfficiency::TransAnglesEfficiency(RooAbsReal* parent, RooRealVar& cpsi, RooRealVar& ctheta, RooRealVar& phi, Efficiency &e, Bool_t s_ok) :
TransAngles(parent, cpsi, ctheta, phi),
_a1("_a1", "a1", parent, e.a1),
_a2("_a2", "a2", parent, e.a2),
_a3("_a3", "a3", parent, e.a3),
_a4("_a4", "a4", parent, e.a4),
_a5("_a5", "a5", parent, e.a5),
_a6("_a6", "a6", parent, e.a6),
_a7("_a7", "a7", parent, e.a7),
_a8("_a8", "a8", parent, e.a8),
_a9("_a9", "a9", parent, e.a9),
_b1("_b1", "b1", parent, e.b1),
_b2("_b2", "b2", parent, e.b2),
_b3("_b3", "b3", parent, e.b3),
_b4("_b4", "b4", parent, e.b4),
_b5("_b5", "b5", parent, e.b5),
_b6("_b6", "b6", parent, e.b6),
_b7("_b7", "b7", parent, e.b7),
_b8("_b8", "b8", parent, e.b8),
_b9("_b9", "b9", parent, e.b9),
_c1("_c1", "c1", parent, e.c1),
_c2("_c2", "c2", parent, e.c2),
_c3("_c3", "c3", parent, e.c3),
_c4("_c4", "c4", parent, e.c4),
_c5("_c5", "c5", parent, e.c5),
_c6("_c6", "c6", parent, e.c6),
_c7("_c7", "c7", parent, e.c7),
_c8("_c8", "c8", parent, e.c8),
_c9("_c9", "c9", parent, e.c9),
static_ok(s_ok)
{
    int_1 = -1.3290852052359397e-8 * (1155. + 231. * _a2 - 429. * _a4 - 121. * _a6 - 61. * _a8)* (231. * _b2 * (2570.2338484575303 * _c2 + 39.47841760435743 * (177.1403090745182 * _c4 + 12275.389781681304 * _c6 - 754439.5001675352 * _c8) + 315. * (2. + 50. * _c4 - 1325. * _c6 + 69890. * _c8)) + 1155. * (-3200.2338484575303 * _c2 - 315. * (2. + 18. * _c4 - 443. * _c6 + 23298. * _c8) - 39.47841760435743 * (1437.1403090745182 * _c4 + 41529.599053916856 * _c6 + 842185.6569003665 * _c8)) + 55. * _b6 * (3578.2338484575307 * _c2 - 63. * (-10. + 6. * _c4 - 431. * _c6 + 23286. * _c8) + 39.47841760435743 * (2193.1403090745184 * _c4 + 59082.12461725819 * _c6 + 1.8001607511411083e6 * _c8)) + 165. * _b4 * (3704.2338484575307 * _c2 - 63. * (-10. + 38. * _c4 - 1313. * _c6 + 69878. * _c8) + 39.47841760435743 * (2445.1403090745184 * _c4 + 64932.966471705295 * _c6 + 2.119485782554689e6 * _c8)) + _b8 * (102886.78160526838 * _c2 + 315. * (58. + 10. * _c4 + 1265. * _c6 - 69830. * _c8) + 39.47841760435743 * (61837.06896316103 * _c4 + 1.6724257209193576e6 * _c6 + 4.996938656319708e7 * _c8)));
    int_2 = 1.3290852052359397e-8 * (693. * _a2 - 11. * (105. + 9. * _a4 + _a6) - 3. * _a8)* (165. * _b4 * (3326.2338484575307 * _c2 + 63. * (10. + 58. * _c4 - 1333. * _c6 + 69898. * _c8) + 39.47841760435743 * (1689.1403090745182 * _c4 + 47380.44090836397 * _c6 + 1.1615106883139475e6 * _c8)) + 55. * _b6 * (3452.2338484575307 * _c2 + 63. * (10. + 26. * _c4 - 451. * _c6 + 23306. * _c8) + 39.47841760435743 * (1941.1403090745182 * _c4 + 53231.282762811075 * _c6 + 1.4808357197275278e6 * _c8)) + 1155. * (-3830.2338484575303 * _c2 + 315. * (-2. + 14. * _c4 - 439. * _c6 + 23294. * _c8) - 39.47841760435743 * (2697.140309074518 * _c4 + 70783.8083261524 * _c6 + 2.438810813968269e6 * _c8)) + 231. * _b2 * (4460.23384845753 * _c2 - 315. * (-2. + 46. * _c4 - 1321. * _c6 + 69886. * _c8) + 39.47841760435743 * (3957.140309074518 * _c4 + 100038.01759838795 * _c6 + 4.0354359710361715e6 * _c8)) + _b8 * (100996.78160526838 * _c2 + 315. * (58. + 106. * _c4 - 1381. * _c6 + 69946. * _c8) + 39.47841760435743 * (58057.06896316103 * _c4 + 1.584663093102651e6 * _c6 + 4.517951109199337e7 * _c8)));
    int_3 = 2.6581704104718793e-8 * (693. * _a2 - 11. * (105. + 9. * _a4 + _a6) - 3. * _a8)*(693. * _b2 - 11. * (105. + 9. * _b4 + _b6) - 3. * _b8)* (1757.6169242287651 * _c2 + 315. * (1. + _c4 - 1. * _c6 + _c8) + 19.739208802178716 * (2067.1403090745184 * _c4 + 56156.70369003463 * _c6 + 1.640498235434318e6 * _c8));
    int_4 = 0.005696741357050134 * (693. * _a2 - 11. * (105. + 9. * _a4 + _a6) - 3. * _a8)*(_b1 - 1. * _b3)* (_c1 + 12.478417604357432 * _c3 + 247.87996017365685 * _c5 + 5690.997801077727 * _c7 + 143464.25816772878 * _c9);
    int_5 = 0.0020141022221180026 * (_a1 - 1. * _a3)*(693. * _b2 - 11. * (105. + 9. * _b4 + _b6) - 3. * _b8)*(_c1 + 30.478417604357432 * _c3 + 726.5850164351028 * _c5 + 18430.728116892853 * _c7 + 492529.4992742345 * _c9);
    int_6 = -10.962374249993156 * (_a1 - 1. * _a3)*(_b1 - 1. * _b3)* (_c2 + 26.956835208714864 * _c4 + 784.0751333340436 * _c6 + 23809.424715423258 * _c8);

    for (int i=0; i<10; i++) {
        _aa[i] = eval_coef(i, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
        _bb[i] = eval_coef(i, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _a8, _b9);
        _cc[i] = eval_coef(i, _c1, _c2, _c3, _c4, _c5, _c6, _c7, _c8, _c9);
    }
}

TransAnglesEfficiency::TransAnglesEfficiency(RooAbsReal* parent, const TransAnglesEfficiency& other, const char* name) :
TransAngles(parent, other, name),
_a1("_a1", parent, other._a1),
_a2("_a2", parent, other._a2),
_a3("_a3", parent, other._a3),
_a4("_a4", parent, other._a4),
_a5("_a5", parent, other._a5),
_a6("_a6", parent, other._a6),
_a7("_a7", parent, other._a7),
_a8("_a8", parent, other._a8),
_a9("_a9", parent, other._a9),
_b1("_b1", parent, other._b1),
_b2("_b2", parent, other._b2),
_b3("_b3", parent, other._b3),
_b4("_b4", parent, other._b4),
_b5("_b5", parent, other._b5),
_b6("_b6", parent, other._b6),
_b7("_b7", parent, other._b7),
_b8("_b8", parent, other._b8),
_b9("_b9", parent, other._b9),
_c1("_c1", parent, other._c1),
_c2("_c2", parent, other._c2),
_c3("_c3", parent, other._c3),
_c4("_c4", parent, other._c4),
_c5("_c5", parent, other._c5),
_c6("_c6", parent, other._c6),
_c7("_c7", parent, other._c7),
_c8("_c8", parent, other._c8),
_c9("_c9", parent, other._c9),
int_1(other.int_1),
int_2(other.int_2),
int_3(other.int_3),
int_4(other.int_4),
int_5(other.int_5),
int_6(other.int_6),
static_ok(other.static_ok) {
    for (int i = 0; i < 10; i++) {
        _aa[i] = eval_coef(i, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
        _bb[i] = eval_coef(i, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _a8, _b9);
        _cc[i] = eval_coef(i, _c1, _c2, _c3, _c4, _c5, _c6, _c7, _c8, _c9);
    }
}

Double_t TransAnglesEfficiency::eval_coef(Int_t i, Double_t a1, Double_t a2, Double_t a3, Double_t a4, Double_t a5,
        Double_t a6, Double_t a7, Double_t a8, Double_t a9) {
    switch (i) {
        case 0:
            return a4 - a2 - a6 + a8 + 1;
        case 1:
            return a1 - 3 * a3 + 5 * a5 - 7 * a7 + 9 * a9;
        case 2:
            return 2 * a2 - 8 * a4 + 18 * a6 - 32 * a8;
        case 3:
            return 4 * a3 - 20 * a5 + 56 * a7 - 120 * a9;
        case 4:
            return 8 * a4 - 48 * a6 + 160 * a8;
        case 5:
            return 16 * a5 - 112 * a7 + 432 * a9;
        case 6:
            return 32 * a6 - 256 * a8;
        case 7:
            return 64 * a7 - 576 * a9;
        case 8:
            return 128 * a8;
        case 9:
            return 256 * a9;
    }
}

Double_t TransAnglesEfficiency::ChebyshevT(Int_t i, Double_t x) const {
    switch (i) {
        case 0:
            return 1;
        case 1:
            return x;
        case 2:
            return 2*x * x - 1;
        case 3:
            return 4*x * x * x - 3*x;
        case 4:
            return 8 * TMath::Power(x, 4) - 8*x * x + 1;
        case 5:
            return 16 * TMath::Power(x, 5) - 20*x * x * x + 5*x;
        case 6:
            return 32 * TMath::Power(x, 6) - 48 * TMath::Power(x, 4) + 18*x * x - 1;
        case 7:
            return 64 * TMath::Power(x, 7) - 112 * TMath::Power(x, 5) + 56*x * x * x - 7*x;
        case 8:
            return 128 * TMath::Power(x, 8) - 256 * TMath::Power(x, 6) + 160 * TMath::Power(x, 4) - 32*x * x + 1;
        case 9:
            return 256 * TMath::Power(x, 9) - 576 * TMath::Power(x, 7) + 432 * TMath::Power(x, 5) - 120*x * x * x + 9*x;
        default:
            std::cout << "EE: incorrect index chebyschev" << std::endl;
            return -1;
    }
}
/*Double_t f(Int_t i) const {
    switch (i) {
        case 1:
            return 2 * _cpsi * _cpsi * (1 - (1 - _ctheta * _ctheta) * _cphi * _cphi);
        case 2:
            Double_t sphi = TMath::Sin(_phi);
            return (1 - _cpsi * _cpsi)*(1 - (1 - _ctheta * _ctheta) * sphi * sphi);
        case 3:
            return (1 - _cpsi * _cpsi)*(1 - _ctheta * _ctheta);
        case 4:
            Double_t sphi = TMath::Sin(_phi);
            return -(1 - _cpsi * _cpsi)*2 * _ctheta * TMath::Sqrt(1 - _ctheta * _ctheta) * _sphi; //opposite (plus) sign: BaBar convention
        case 5:
            Double_t s2phi = TMath::Sin(2 * _phi);
            return +2 * _cpsi * TMath::Sqrt(1 - _cpsi * _cpsi)*(1 - _ctheta * _ctheta) * s2phi / 1.41421356237309515; //opposite (minus) sign: BaBar
        case 6:
            Double_t cphi = cos(_phi);
            return 2 * _cpsi * TMath::Sqrt(1 - _cpsi * _cpsi)*2 * _ctheta * TMath::Sqrt(1 - _ctheta * _ctheta) * cphi / 1.41421356237309515;
        default:
            cout << "EE: incorrect index" << endl;
            return -1;
    }
}*/

Double_t TransAnglesEfficiency::efficiency(const Double_t *coeff, Double_t *pow) const {
    return coeff[0] + coeff[1] * pow[1] + coeff[2] * pow[2] + coeff[3] * pow[3]
            + coeff[4] * pow[4] + coeff[5] * pow[5] + coeff[6] * pow[6]
            + coeff[7] * pow[7] + coeff[8] * pow[8] + coeff[9] * pow[9];
}

void TransAnglesEfficiency::power(Double_t x, Double_t *pow) const {
//    pow[0] = 1;
    pow[1] = x;
    pow[2] = pow[1]*x;
    pow[3] = pow[2]*x;
    pow[4] = pow[3]*x;
    pow[5] = pow[4]*x;
    pow[6] = pow[5]*x;
    pow[7] = pow[6]*x;
    pow[8] = pow[7]*x;
    pow[9] = pow[8]*x;
}

Double_t TransAnglesEfficiency::ec_cpsi() const {
    static Double_t cpsiE[10];
    power(_cpsi, cpsiE);
    return efficiency(_aa, cpsiE);
}

Double_t TransAnglesEfficiency::ecpsi() const {
    return (1 + _a1 * ChebyshevT(1, _cpsi) + _a2 * ChebyshevT(2, _cpsi) +
            _a3 * ChebyshevT(3, _cpsi) + _a4 * ChebyshevT(4, _cpsi) +
            _a5 * ChebyshevT(5, _cpsi) + _a6 * ChebyshevT(6, _cpsi) +
            _a7 * ChebyshevT(7, _cpsi) + _a8 * ChebyshevT(8, _cpsi) +
            _a9 * ChebyshevT(9, _cpsi));
}

Double_t TransAnglesEfficiency::ec_ctheta() const {
    static Double_t cthetaE[10];
    power(_ctheta, cthetaE);
    return efficiency(_bb, cthetaE);
}

Double_t TransAnglesEfficiency::ectheta() const {
    return (1 + _b1 * ChebyshevT(1, _ctheta) + _b2 * ChebyshevT(2, _ctheta) +
            _b3 * ChebyshevT(3, _ctheta) + _b4 * ChebyshevT(4, _ctheta) +
            _b5 * ChebyshevT(5, _ctheta) + _b6 * ChebyshevT(6, _ctheta) +
            _b7 * ChebyshevT(7, _ctheta) + _b8 * ChebyshevT(8, _ctheta) +
            _b9 * ChebyshevT(9, _ctheta));
}

 Double_t TransAnglesEfficiency::ec_phi() const {
    static Double_t phiE[10];
    power(_phi, phiE);
    return efficiency(_cc, phiE);
}

Double_t TransAnglesEfficiency::ephi() const {
    return (1 + _c1 * ChebyshevT(1, _phi) + _c2 * ChebyshevT(2, _phi) +
            _c3 * ChebyshevT(3, _phi) + _c4 * ChebyshevT(4, _phi) +
            _c5 * ChebyshevT(5, _phi) + _c6 * ChebyshevT(6, _phi) +
            _c7 * ChebyshevT(7, _phi) + _c8 * ChebyshevT(8, _phi) +
            _c9 * ChebyshevT(9, _phi));
}

Double_t TransAnglesEfficiency::fe(Int_t i) const {
    if (static_ok)
        return f(i) * ec_cpsi() * ec_ctheta() * ec_phi();
    else
        return f(i) * ecpsi() * ectheta() * ephi();
}

Double_t TransAnglesEfficiency::int_fe(Int_t i, Int_t code, const char* range) const {
    //std::cout << "TAE: " << i << ' ' << code << ' ' << range << std::endl;
	//std::cout << "TAE: " << i << ' ' << code << ' ' << static_ok << ' ' << int_1 << ' ' << int_2 << ' ' << int_3 << ' ' << int_4 << ' ' << int_5 << ' ' << int_6  << std::endl;
//    std::cout << _a1 << ' ' << _a2 << ' ' << _a3 << ' ' << _a4 << ' ' << _a5 << std::endl;
//    std::cout << _b1 << ' ' << _b2 << ' ' << _b3 << ' ' << _b4 << ' ' << _b5 << std::endl;
//    std::cout << _c1 << ' ' << _c2 << ' ' << _c3 << ' ' << _c4 << ' ' << _c5 << std::endl;
    switch (code) {
        case 1:
            switch (i) {
                case 1:
                    if (static_ok) return int_1;
                    else return -1.3290852052359397e-8 * (1155. + 231. * _a2 - 429. * _a4 - 121. * _a6 - 61. * _a8)* (231. * _b2 * (2570.2338484575303 * _c2 + 39.47841760435743 * (177.1403090745182 * _c4 + 12275.389781681304 * _c6 - 754439.5001675352 * _c8) + 315. * (2. + 50. * _c4 - 1325. * _c6 + 69890. * _c8)) + 1155. * (-3200.2338484575303 * _c2 - 315. * (2. + 18. * _c4 - 443. * _c6 + 23298. * _c8) - 39.47841760435743 * (1437.1403090745182 * _c4 + 41529.599053916856 * _c6 + 842185.6569003665 * _c8)) + 55. * _b6 * (3578.2338484575307 * _c2 - 63. * (-10. + 6. * _c4 - 431. * _c6 + 23286. * _c8) + 39.47841760435743 * (2193.1403090745184 * _c4 + 59082.12461725819 * _c6 + 1.8001607511411083e6 * _c8)) + 165. * _b4 * (3704.2338484575307 * _c2 - 63. * (-10. + 38. * _c4 - 1313. * _c6 + 69878. * _c8) + 39.47841760435743 * (2445.1403090745184 * _c4 + 64932.966471705295 * _c6 + 2.119485782554689e6 * _c8)) + _b8 * (102886.78160526838 * _c2 + 315. * (58. + 10. * _c4 + 1265. * _c6 - 69830. * _c8) + 39.47841760435743 * (61837.06896316103 * _c4 + 1.6724257209193576e6 * _c6 + 4.996938656319708e7 * _c8)));
                case 2:
                    if (static_ok) return int_2;
                    return 1.3290852052359397e-8 * (693. * _a2 - 11. * (105. + 9. * _a4 + _a6) - 3. * _a8)* (165. * _b4 * (3326.2338484575307 * _c2 + 63. * (10. + 58. * _c4 - 1333. * _c6 + 69898. * _c8) + 39.47841760435743 * (1689.1403090745182 * _c4 + 47380.44090836397 * _c6 + 1.1615106883139475e6 * _c8)) + 55. * _b6 * (3452.2338484575307 * _c2 + 63. * (10. + 26. * _c4 - 451. * _c6 + 23306. * _c8) + 39.47841760435743 * (1941.1403090745182 * _c4 + 53231.282762811075 * _c6 + 1.4808357197275278e6 * _c8)) + 1155. * (-3830.2338484575303 * _c2 + 315. * (-2. + 14. * _c4 - 439. * _c6 + 23294. * _c8) - 39.47841760435743 * (2697.140309074518 * _c4 + 70783.8083261524 * _c6 + 2.438810813968269e6 * _c8)) + 231. * _b2 * (4460.23384845753 * _c2 - 315. * (-2. + 46. * _c4 - 1321. * _c6 + 69886. * _c8) + 39.47841760435743 * (3957.140309074518 * _c4 + 100038.01759838795 * _c6 + 4.0354359710361715e6 * _c8)) + _b8 * (100996.78160526838 * _c2 + 315. * (58. + 106. * _c4 - 1381. * _c6 + 69946. * _c8) + 39.47841760435743 * (58057.06896316103 * _c4 + 1.584663093102651e6 * _c6 + 4.517951109199337e7 * _c8)));
                case 3:
                    if (static_ok) return int_3;
                    else return 2.6581704104718793e-8 * (693. * _a2 - 11. * (105. + 9. * _a4 + _a6) - 3. * _a8)*(693. * _b2 - 11. * (105. + 9. * _b4 + _b6) - 3. * _b8)* (1757.6169242287651 * _c2 + 315. * (1. + _c4 - 1. * _c6 + _c8) + 19.739208802178716 * (2067.1403090745184 * _c4 + 56156.70369003463 * _c6 + 1.640498235434318e6 * _c8));
                case 4:
                    if (static_ok) return int_4;
                    else return 0.005696741357050134 * (693. * _a2 - 11. * (105. + 9. * _a4 + _a6) - 3. * _a8)*(_b1 - 1. * _b3)* (_c1 + 12.478417604357432 * _c3 + 247.87996017365685 * _c5 + 5690.997801077727 * _c7 + 143464.25816772878 * _c9);
                case 5:
                    if (static_ok) return int_5;
                    else return 0.0020141022221180026 * (_a1 - 1. * _a3)*(693. * _b2 - 11. * (105. + 9. * _b4 + _b6) - 3. * _b8)*(_c1 + 30.478417604357432 * _c3 + 726.5850164351028 * _c5 + 18430.728116892853 * _c7 + 492529.4992742345 * _c9);
                case 6:
                    if (static_ok) return int_6;
                    else return -10.962374249993156 * (_a1 - 1. * _a3)*(_b1 - 1. * _b3)* (_c2 + 26.956835208714864 * _c4 + 784.0751333340436 * _c6 + 23809.424715423258 * _c8);
                default:
                    std::cout << "EE: incorrect index" << std::endl;
                    return -1;
            }
        case 2:
            switch (i) {
                case 1:
                    return ecpsi()*(-0.000023026401180712653 * _cpsi * _cpsi * (231. * _b2 * (2570.2338484575303 * _c2 + 39.47841760435743 * (177.1403090745182 * _c4 + 12275.389781681304 * _c6 - 754439.5001675352 * _c8) + 315. * (2. + 50. * _c4 - 1325. * _c6 + 69890. * _c8)) + 1155. * (-3200.2338484575303 * _c2 - 315. * (2. + 18. * _c4 - 443. * _c6 + 23298. * _c8) - 39.47841760435743 * (1437.1403090745182 * _c4 + 41529.599053916856 * _c6 + 842185.6569003665 * _c8)) + 55. * _b6 * (3578.2338484575307 * _c2 - 63. * (-10. + 6. * _c4 - 431. * _c6 + 23286. * _c8) + 39.47841760435743 * (2193.1403090745184 * _c4 + 59082.12461725819 * _c6 + 1.8001607511411083e6 * _c8)) + 165. * _b4 * (3704.2338484575307 * _c2 - 63. * (-10. + 38. * _c4 - 1313. * _c6 + 69878. * _c8) + 39.47841760435743 * (2445.1403090745184 * _c4 + 64932.966471705295 * _c6 + 2.119485782554689e6 * _c8)) + _b8 * (102886.78160526838 * _c2 + 315. * (58. + 10. * _c4 + 1265. * _c6 - 69830. * _c8) + 39.47841760435743 * (61837.06896316103 * _c4 + 1.6724257209193576e6 * _c6 + 4.996938656319708e7 * _c8))));
                case 2:
                    return ecpsi()*(0.000011513200590356326 * (-1. + _cpsi * _cpsi)* (165. * _b4 * (3326.2338484575307 * _c2 + 63. * (10. + 58. * _c4 - 1333. * _c6 + 69898. * _c8) + 39.47841760435743 * (1689.1403090745182 * _c4 + 47380.44090836397 * _c6 + 1.1615106883139475e6 * _c8)) + 55. * _b6 * (3452.2338484575307 * _c2 + 63. * (10. + 26. * _c4 - 451. * _c6 + 23306. * _c8) + 39.47841760435743 * (1941.1403090745182 * _c4 + 53231.282762811075 * _c6 + 1.4808357197275278e6 * _c8)) + 1155. * (-3830.2338484575303 * _c2 + 315. * (-2. + 14. * _c4 - 439. * _c6 + 23294. * _c8) - 39.47841760435743 * (2697.140309074518 * _c4 + 70783.8083261524 * _c6 + 2.438810813968269e6 * _c8)) + 231. * _b2 * (4460.23384845753 * _c2 - 315. * (-2. + 46. * _c4 - 1321. * _c6 + 69886. * _c8) + 39.47841760435743 * (3957.140309074518 * _c4 + 100038.01759838795 * _c6 + 4.0354359710361715e6 * _c8)) + _b8 * (100996.78160526838 * _c2 + 315. * (58. + 106. * _c4 - 1381. * _c6 + 69946. * _c8) + 39.47841760435743 * (58057.06896316103 * _c4 + 1.584663093102651e6 * _c6 + 4.517951109199337e7 * _c8))));
                case 3:
                    return ecpsi()*(0.0011544011544011544 * (1. - 1. * _cpsi * _cpsi)*(-693. * _b2 + 11. * (105. + 9. * _b4 + _b6) + 3. * _b8)*(41.341702240399755 * (_c2 - 4. * _c4 + 9. * _c6 - 16. * _c8) + 27614.10951110209 * (_c6 - 8. * _c8) + 847903.2699291363 * _c8 + 6.283185307179586 * (1. - 1. * _c2 + _c4 - 1. * _c6 + _c8) + 979.2629913129006 * (_c4 - 6. * _c6 + 20. * _c8)));
                case 4:
                    return ecpsi()*(4.934802200544679 * (-1. + _cpsi * _cpsi)*(_b1 - 1. * _b3)* (_c1 - 27. * _c3 + 2045. * _c5 - 336343. * _c7 + 39.47841760435743 * (_c3 - 85. * _c5 + 14014. * _c7 - 3.99387e6 * _c9) + 61528.90838881947 * (_c7 - 297. * _c9) + 9.828195294011407e7 * _c9 + 1558.5454565440386 * (_c5 - 175. * _c7 + 49923. * _c9)));
                case 5:
                    return ecpsi()*(0.003626658185962243 * _cpsi * TMath::Sqrt(2. - 2. * _cpsi * _cpsi)* (693. * _b2 - 11. * (105. + 9. * _b4 + _b6) - 3. * _b8)*(_c1 + 30.478417604357432 * _c3 + 726.5850164351028 * _c5 + 18430.728116892853 * _c7 + 492529.4992742345 * _c9));
                case 6:
                    return ecpsi()*(-19.739208802178716 * _cpsi * TMath::Sqrt(2. - 2. * _cpsi * _cpsi)*(_b1 - 1. * _b3)*(_c2 + 26.956835208714864 * _c4 + 784.0751333340436 * _c6 + 23809.424715423258 * _c8));
                default:
                    std::cout << "EE: incorrect index" << std::endl;
                    return -1;

            }
        case 3:
            switch (i) {
                case 1:
                    return ectheta()*(0.0011544011544011544 * (1155. + 231. * _a2 - 429. * _a4 - 121. * _a6 - 61. * _a8)*(423951.63496456813 * (1. + _ctheta * _ctheta) * _c8 + 3.141592653589793 * (1. - 2. * _c2 + 17. * _c4 - 442. * _c6 + _ctheta * _ctheta * (1. - 15. * _c4 + 440. * _c6 - 23295. * _c8) + 23297. * _c8) + 13807.054755551046 * ((1. + _ctheta * _ctheta) * _c6 + 16. * (-4. + 3. * _ctheta * _ctheta) * _c8) + 20.670851120199877 * ((1. + _ctheta * _ctheta) * _c2 + 8. * (-2. + _ctheta * _ctheta) * _c4 + 9. * (49. - 47. * _ctheta * _ctheta) * _c6 + 32. * (-728. + 727. * _ctheta * _ctheta) * _c8) + 489.6314956564503 * ((1. + _ctheta * _ctheta) * _c4 + 4. * ((-9. + 6. * _ctheta * _ctheta) * _c6 + 5. * (97. - 95. * _ctheta * _ctheta) * _c8))));
                case 2:
                    return ectheta()*(0.0011544011544011544 * (-693. * _a2 + 11. * (105. + 9. * _a4 + _a6) + 3. * _a8)*(423951.63496456813 * (1. + _ctheta * _ctheta) * _c8 + 20.670851120199877 * ((1. + _ctheta * _ctheta) * _c2 + 8. * (1. - 2. * _ctheta * _ctheta) * _c4 + 9. * (-47. + 49. * _ctheta * _ctheta) * _c6 + 32. * (727. - 728. * _ctheta * _ctheta) * _c8) + 13807.054755551046 * ((1. + _ctheta * _ctheta) * _c6 + 16. * (3. - 4. * _ctheta * _ctheta) * _c8) + 3.141592653589793 * (1. - 15. * _c4 + 440. * _c6 - 23295. * _c8 + _ctheta * _ctheta * (1. - 2. * _c2 + 17. * _c4 - 442. * _c6 + 23297. * _c8)) + 489.6314956564503 * ((1. + _ctheta * _ctheta) * _c4 - 4. * ((-6. + 9. * _ctheta * _ctheta) * _c6 + 5. * (95. - 97. * _ctheta * _ctheta) * _c8))));
                case 3:
                    return ectheta()*(0.0011544011544011544 * (1. - 1. * _ctheta * _ctheta)*(-693. * _a2 + 11. * (105. + 9. * _a4 + _a6) + 3. * _a8)*(41.341702240399755 * (_c2 - 4. * _c4 + 9. * _c6 - 16. * _c8) + 27614.10951110209 * (_c6 - 8. * _c8) + 847903.2699291363 * _c8 + 6.283185307179586 * (1. - 1. * _c2 + _c4 - 1. * _c6 + _c8) + 979.2629913129006 * (_c4 - 6. * _c6 + 20. * _c8)));
                case 4:
                    return ectheta()*(0.014506632743848972 * _ctheta * TMath::Sqrt(1. - 1. * _ctheta * _ctheta)*(693. * _a2 - 11. * (105. + 9. * _a4 + _a6) - 3. * _a8)* (_c1 + 12.478417604357432 * _c3 + 247.87996017365685 * _c5 + 5690.997801077727 * _c7 + 143464.25816772878 * _c9));
                case 5:
                    return ectheta()*(1.7447160499097196 * (-1. + _ctheta * _ctheta)*(_a1 - 1. * _a3)*(_c1 + 30.478417604357432 * _c3 + 726.5850164351028 * _c5 + 18430.728116892853 * _c7 + 492529.4992742345 * _c9));
                case 6:
                    return ectheta()*(0.7853981633974483 * _ctheta * TMath::Sqrt(2. - 2. * _ctheta * _ctheta)*(_a1 - 1. * _a3)* (-25.132741228718345 * (_c2 + 26.956835208714864 * _c4 + 784.0751333340436 * _c6) - 598396.1101773836 * _c8));
                default:
                    std::cout << "EE: incorrect index" << std::endl;
                    return -1;
            }
        case 4:
            switch (i) {
                case 1:
                    return ephi()*(6.663210126413589e-7 * (1155. + 231. * _a2 - 429. * _a4 - 121. * _a6 - 61. * _a8)*(-462. * _b2 - 110. * (-21. + 3. * _b4 + _b6) - 58. * _b8 + (693. * _b2 - 11. * (105. + 9. * _b4 + _b6) - 3. * _b8) * TMath::Cos(2. * _phi)));
                case 2:
                    return ephi()*(6.663210126413589e-7 * (693. * _a2 - 11. * (105. + 9. * _a4 + _a6) - 3. * _a8)* (462. * _b2 + 110. * (-21. + 3. * _b4 + _b6) + 58. * _b8 + (693. * _b2 - 11. * (105. + 9. * _b4 + _b6) - 3. * _b8) * TMath::Cos(2. * _phi)));
                case 3:
                    return ephi()*(1.3326420252827178e-6 * (-693. * _a2 + 11. * (105. + 9. * _a4 + _a6) + 3. * _a8)*(-693. * _b2 + 11. * (105. + 9. * _b4 + _b6) + 3. * _b8));
                case 4:
                    return ephi()*(0.0009066645464905608 * (693. * _a2 - 11. * (105. + 9. * _a4 + _a6) - 3. * _a8)*(_b1 - 1. * _b3) * TMath::Sin(_phi));
                case 5:
                    return ephi()*(-0.0012822172981698026 * (_a1 - 1. * _a3)*(693. * _b2 - 11. * (105. + 9. * _b4 + _b6) - 3. * _b8) * TMath::Cos(_phi) * TMath::Sin(_phi));
                case 6:
                    return ephi()*(0.4361790124774299 * (_a1 - 1. * _a3)*(_b1 - 1. * _b3) * TMath::Cos(_phi));
                default:
                    std::cout << "EE: incorrect index" << std::endl;
                    return -1;
            }
    }
}

