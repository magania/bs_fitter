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
    printf("\t \033[1m--no-resolution\033[m  \t Set true resolution.\n");
    printf("\t \033[1m--signal-only\033[m    \t Fit signal model only.\n");
    printf("\t \033[1m--prompt-only\033[m    \t Fit prompt model only.\n");
    printf("\t \033[1m--noprompt-only\033[m  \t Fit no prompt model only.\n");
    printf("\t \033[1m--no-efficiency\033[m  \t Use efficiency = 1.\n");
    printf("\t \033[1m--use-phis\033[m       \t Use Phi constants no normalize.\n\n");
    printf("\t \033[1m--verbose\033[m        \t Verbose fit.\n\n");
    printf("Report bugs to <magania@fnal.gov>.\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    static int use_resolution = true;
    static int signal_only = false;
    static int prompt_only = false;
    static int noprompt_only = false;
    static int use_efficiency = true;
    static int use_phis = false;
    static int no_efficiency = false;
    static int verbose = false;

    int jobs =4;
    const char *data = "fit.dat";
    const char *parameters = "parameters.txt";
    const char *variables = "variables.txt";
    const char *efficiency = "efficiency.txt";
    const char *out = "fit.txt";

    int c;
    while (1) {
        static struct option long_options[] ={
            /* These options set a flag. */
            {"no-resolution"  , no_argument, &use_resolution, 0},
            {"signal-only"    , no_argument, &signal_only   , 1},
            {"prompt-only"    , no_argument, &prompt_only   , 1},
            {"noprompt-only"  , no_argument, &noprompt_only , 1},
            {"no-efficiency"  , no_argument, &no_efficiency , 1},
            {"use-phis"       , no_argument, &use_phis      , 1},
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
    
    if (use_phis)
        use_efficiency = false;

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
    if (generate && use_phis) {
        printf("Can't generate with Phi's.\n");
        exit(EXIT_FAILURE);
    }
    if (plot && use_phis) {
        printf("Can't plot with Phi's.\n");
        exit(EXIT_FAILURE);
    }
    if (fit_eff) {
        if (!signal_only) {
            printf("Fit efficiency must be with signal-only.\n");
            exit(EXIT_FAILURE);
        }
        if (!use_efficiency) {
            printf("Fit efficiency requieres efficiency.\n");
            exit(EXIT_FAILURE);
        }
        if (use_phis) {
            printf("Fit efficiency can't be done with Phi's.\n");
            exit(EXIT_FAILURE);
        }
    }
    if(signal_only && !prompt_only && !noprompt_only) {
        printf("signal_only and sidebands options together is stupid.\n");
        exit(EXIT_FAILURE);
    }
    
    /* Do the real stuff */
    BsFitter bs(use_resolution, signal_only, prompt_only, noprompt_only, use_efficiency, use_phis);
    bs.setVariables(variables);
    bs.setParameters(parameters);
    if (use_efficiency && !no_efficiency)
        bs.setEfficiency(efficiency);
    if (use_phis)
        bs.setPhis(efficiency);

    if (fit || plot || fit_eff)
        bs.setData(data);
    
    if (generate) {
        cout << "Generating .." << endl;
        bs.generate(3000, data);
    }
    
    if (fit || fit_eff) {
        bs.writeParameters("parameters_out.txt");
        bs.fit(true, true, verbose, 4);
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
