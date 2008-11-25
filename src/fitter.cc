
#include "BsFitter.h"

# include "BsFitter.h"

int main(int argc, char *argv[]) {
    const char *data = "fit.dat";    
    const char *variables = "variables.txt";
    const char *parameters = "parameters.txt";
    const char *result = "result.txt";
    
    bool plot = true;
    bool use_stdin = false;
    
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "nd:v:p:r:")) != -1)
        switch (c) {
            case 'd':
                if (*optarg == '-')
                    use_stdin = true;
                else
                    data = optarg;
                break;
            case 'v':
                variables = optarg;
                break;
            case 'p':
                parameters = optarg;
                break;
            case 'r':
                result = optarg;
                break;
            case 'n':
                plot = false;
                break;
            case '?':
                if (optopt == 'd' || optopt == 'v' || optopt == 'p' || optopt == 'r')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
                return 1;
            default:
                abort();
        }

    BsFitter *bs = new BsFitter();

    bs->setVariables(variables);
    bs->setParameters(parameters);
    bs->setEfficiency("eff_poly.txt");
//    if (use_stdin)
  //      bs->setDataFromCin();
//    else
//        bs->setData(data); 
    bs->generate(10000);
    
   bs->fit();   bs->writeResult(result);
    if ( plot ) {
        bs->plotM();
        bs->plotT();
        bs->plotCpsi();
        bs->plotCtheta();
        bs->plotPhi();
    }
}
