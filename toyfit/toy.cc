#include <stdlib.h>
#include <string>

# include "BsFitter.h"

//using namespace RooFit;

int main(int argc, char *argv[]) {
  char *list = NULL;
  int c;
     
  opterr = 0;
     
  while ((c = getopt (argc, argv, "l:")) != -1)
    switch (c)
      {
      case 'l':
	list = optarg;
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

  string data_set;
  ifstream data_sets(list);
  while (getline(data_sets, data_set)) {
    bs->setVariables("variables.txt");
    bs->setParameters("parameters.txt");
    bs->setData(data_set.c_str());
    bs->setVariables("variables_fit.txt");;
    Int_t stat = bs->fit();
	cout << "STAT:" << stat << endl;;
    string result = data_set + ".out";
    bs->writeResult(result.c_str());
  }
}
