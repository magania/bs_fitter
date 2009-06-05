/*
 * bs-fit.cc
 *
 *  Created on: Jun 1, 2009
 *      Author: magania
 */

#include "BsFitter.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <iostream>

using namespace std;

void usage(void) {
    printf("\n");
    printf("\033[1mUsage: bs-fit [options].\033[m \n");
    printf("options:\n");
    printf("\t \033[1m-d\033[m data          \t Set data file.\n");
    printf("\t \033[1m-v\033[m variables     \t Set variables file.\n");
    printf("\t \033[1m-p\033[m parameters    \t Set parameters file.\n");
    printf("\t \033[1m-o\033[m out           \t Set file result file.\n\n");
    printf("\t \033[1m-n\033[m name          \t Set name of the parameters.\n\n");
    printf("\t \033[1m-e\033[m name          \t Set name of the et parameters.\n\n");
    printf("\t \033[1m--verbose\033[m        \t Verbose fit.\n\n");
    printf("Report bugs to <magania@fnal.gov>.\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
 /* magic line from Rene - for future reference! */
// gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo",
// "*",
// "TStreamerInfo",
// "RIO",
// "TStreamerInfo()");

    static int verbose = false;
    int jobs = 1;

    const char *data = "fit.dat";
    const char *variables = "variables.txt";
    const char *parameters = "parameters.txt";
    const char *out = "fit.txt";
    const char *name = "";
    const char *name_et = "";

    int c;
    while (1) {
        static struct option long_options[] ={
            /* These options set a flag. */
            {"verbose"        , no_argument, &verbose       , 1},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "d:p:o:v:j:n:e:",
                long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
            case 'd':
                data = optarg;
                break;
            case 'p':
                parameters = optarg;
                break;
            case 'o':
                out = optarg;
                break;
            case 'n':
                name = optarg;
                break;
            case 'e':
                name_et = optarg;
                break;
            case 'v':
                variables = optarg;
                break;
            case 'j':
                jobs = atoi(optarg);
                break;
            case '?':
                usage();
            default:
                usage();
        }
    }

    /* Do the real stuff */
    BsSingleFitter bs(name, name_et);
    bs.setVariables(variables);
    bs.setParameters(parameters);
    bs.setData(data);
    bs.writeParameters("parameters_read.txt");
    bs.fit(true, false, verbose, jobs);
    bs.writeResult(out);
    bs.writeParameters("parameters_out.txt");

    return (EXIT_SUCCESS);
}
