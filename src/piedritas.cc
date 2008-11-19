#include "src/TimeAngle.hh"

using namespace std;

int main(){
  Angles a;
  a.cpsi = 0.2;
  a.ctheta = 0.1;
  a.phi = 1;
  
  Parameters p;
  p.A02 = 0.50000;
  p.All2 = 0.33000;
  p.DG = 0.19;
  p.tau = 1.521;
  p.Dm = 17.770;
  p.phi_s = -0.350000;
  p.delta_1 = -0.460000;
  p.delta_2 = 2.9200;

  TimeAngle ta;
  cout << "eval: " <<  ta.eval(0,a,p, true) << endl;
  
}
