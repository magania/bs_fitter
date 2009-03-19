/*
 * File:   bs.cc
 * Author: magania
 *
 * Created on November 27, 2008, 11:51 AM
 */

#include "BsFitter.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <iostream>

/*
 *
 */

using namespace std;

void usage(void) {
    printf("\n");
    printf("\033[1mUsage: bs operation [options].\033[m \n");
    printf("operation:\n");
    printf("\t \033[1mgenerate\033[m  \tGenerate toy montecarlo events.\n");
    printf("\t \033[1mfit\033[m       \tFit data\n");
    printf("\t \033[1mplot\033[m      \tDraw projections.\n");
    printf("\t \033[1mefficiency\033[m\tFit efficiency coeficients.\n");
    printf("options:\n");
    printf("\t \033[1m-d\033[m data          \t Set data file.\n");
    printf("\t \033[1m-v\033[m variables     \t Set variables file.\n");
    printf("\t \033[1m-p\033[m parameters    \t Set parameters file.\n");
    printf("\t \033[1m-e\033[m efficiency    \t Set efficiency file. (Phi's or coefficients)\n");
    printf("\t \033[1m-o\033[m out           \t Set file result file.\n\n");
    printf("\t \033[1m--resolution\033[m     \t Use resolution.\n");
    printf("\t \033[1m--signal\033[m         \t Include signal.\n");
    printf("\t \033[1m--bkg\033[m            \t Include background.\n");
    printf("\t \033[1m--efficiency\033[m     \t Use efficiency.\n");
    printf("\t \033[1m--error\033[m          \t Use error model.\n");
    printf("\t \033[1m--verbose\033[m        \t Verbose fit.\n\n");
    printf("Report bugs to <magania@fnal.gov>.\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    static int resolution = false;
    static int signal = false;
    static int background = false;
    static int uefficiency = false;
    static int verbose = false;
    static int error_model = false;
    static int tag_model = false;

    int jobs =1;
    const char *data = "fit.dat";
    const char *parameters = "parameters.txt";
    const char *variables = "variables.txt";
    const char *efficiency = "efficiency.txt";
    const char *out = "fit.txt";

    int c;
    while (1) {
        static struct option long_options[] ={
            /* These options set a flag. */
            {"resolution"  , no_argument, &resolution, 1},
            {"signal"    , no_argument, &signal      , 1},
            {"bkg"    , no_argument, &background     , 1},
            {"efficiency"  , no_argument, &uefficiency, 1},
            {"error"  , no_argument, &error_model, 1},
            {"tag"  , no_argument, &tag_model, 1},
            {"verbose"        , no_argument, &verbose       , 1},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "d:p:e:o:v:j:",
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
            case 'e':
                efficiency = optarg;
                break;
            case 'o':
                out = optarg;
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

    bool generate = false;
    bool fit = false;
    bool plot = false;
    bool fit_eff = false;
    if (argc == (optind + 1)) {
        if (!strcmp(argv[optind], "generate"))
            generate = true;
        if (!strcmp(argv[optind], "fit"))
            fit = true;
        if (!strcmp(argv[optind], "plot"))
            plot = true;
        if (!strcmp(argv[optind], "efficiency"))
            fit_eff = true;
        if (!(generate || fit || plot || fit_eff)) {
            printf("Unknown operation %s.\n", argv[optind]);
            usage();
        }
    } else {
        if ((optind + 1) < argc) {
            printf("Too many operations ");
            while (optind < argc)
                printf("%s ", argv[optind++]);
            printf(".\n");
            usage();
        } else {
            printf("Operation missing.\n");
            usage();
        }
    }

    /* Exit on unposible flags */
    //    if (prompt_only && noprompt_only) {
    //   printf("Flags prompt-only and noprompt-only conflict.\n");
    //   exit(EXIT_FAILURE);
    // }
    if (fit_eff) {
        if (background) {
            printf("Fit efficiency must be with signal only.\n");
            exit(EXIT_FAILURE);
        }
    }
    if(!(signal || background)) {
        printf("Signal or background is needed .\n");
        exit(EXIT_FAILURE);
    }

    /* Do the real stuff */
    if (!uefficiency)
        efficiency = 0;
    BsFitter bs(signal, background,
    		resolution, error_model, tag_model, efficiency);
    bs.setVariables(variables);
    bs.setParameters(parameters);

    if (fit || plot || fit_eff)
        bs.setData(data);

    if (generate) {
        cout << "Generating .." << endl;
        bs.generate(3000, data);
    }

    if (fit || fit_eff) {
        bs.writeParameters("parameters_out.txt");
        bs.fit(true, false, verbose, jobs);
        bs.writeResult(out);
        bs.writeParameters("parameters_out.txt");
    }

    if (plot) {
        bs.plotM();
        bs.plotT();
        //bs.plotEt();
        bs.plotCpsi();
        bs.plotCtheta();
        bs.plotPhi();
    }

    if ( fit_eff ) {

    }

    return (EXIT_SUCCESS);
}
