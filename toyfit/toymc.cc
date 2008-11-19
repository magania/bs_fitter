#include <stdlib.h>
#include <string>
#include <sstream>

# include "src/BsFitter.hh"

//using namespace RooFit;

int main(int argc, char *argv[]) {
  int start=0;
  int end=10;
  int c;
  stringstream ss1,ss2;
     
  opterr = 0;
     
  while ((c = getopt (argc, argv, "s:e:")) != -1)
    switch (c)
      {
      case 's':
  	ss1 << optarg;
	ss1 >> start;
	cout << "start: " << start << endl;
	break;
      case 'e':
	ss2 << optarg;
	ss2 >> end;
	cout << "end: " << end << endl;
	break;
      case '?':
	if (optopt == 'l')
	  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	else if (isprint (optopt))
	  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	else
	  fprintf (stderr,
		   "Unknown option character `\\x%x'.\n",
		   optopt);
	return 1;
      default:
	abort ();
      }

  BsFitter *bs = new BsFitter();

  for( int i=start; i< end; i++){
    bs->setVariables("variables.txt");
    bs->setParameters("parameters.txt");
    bs->setData(bs->generate(3000));
    bs->setVariables("variables_fit.txt");;
    bs->fit(true, false, false, 4);
    stringstream sss;
    string result;
    sss << "result_" << i << ".out" ;
    sss >> result;
    cout << "WRITE"<< result << endl;
    bs->writeResult(result.c_str());
  }
}
