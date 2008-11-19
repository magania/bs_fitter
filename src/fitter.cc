# include "BsFitter.h"

int main(int argc, char *argv[]) {
    const char *data = "fit.dat";    
    const char *variables = "variables.txt";
    const char *parameters = "parameters.txt";
    const char *result = "result.txt";
    
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "d:v:p:r:")) != -1)
        switch (c) {
            case 'd':
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
    bs->setData(data);
    bs->fit();
    bs->writeResult(result);
    bs->plotM();
    bs->plotT();
    bs->plotCpsi();
    bs->plotCtheta();
    bs->plotPhi();
}
