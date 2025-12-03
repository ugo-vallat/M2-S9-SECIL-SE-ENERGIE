
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "defines.h"


Algo_t algo;
Act act;
char *file;
char file_out[512];



void usage(char *exec) {
    printf("usage : %s <act> <algo> <file_path>\n", exec);
    printf("\taglo in NO_COMP:0, LINEAR:1, BLOCK:2, SIMETRY:3, DICO:4, NO_PIXEL:5, MEAN:6, FOURIER =7\n");
    printf("\tact in [gen,read]\n");
    exit(1);
}

void get_args(int argc, char *argv[]) {

    if (argc != 4) {
        usage(argv[0]);
    }

    if (strcmp(argv[1], "gen") == 0) {
        act = ACT_GEN;
    } else if (strcmp(argv[1], "read") == 0) {
        act = ACT_READ;
    } else {
        fprintf(stderr, "Invalid action\n");
        usage(argv[0]);
    }

    unsigned tmp;
    sscanf(argv[2], "%u", &tmp);
    if(tmp >= MAX_ALOG) {
        fprintf(stderr, "Invalid algo\n");
        usage(argv[0]);
    }
    algo = tmp;

    file = argv[3];
    sprintf(file_out, "%s.png", file);
}

void gen_image() {
    switch (algo) {
        case ALGO_NO_COMP:
            no_comp_gen();
            printf("[MAIN] gen OK\n");
            no_comp_save(file);
            printf("[MAIN] save OK\n");
            break;
        case ALGO_LINEAR:
            fprintf(stderr, "[LINEAR] algo not implemented yet\n");
            break;
        case ALGO_BLOCK:
            fprintf(stderr, "[BLOCK] algo not implemented yet\n");
            break;
        case ALGO_SIMETRY:
            fprintf(stderr, "[SIMETRY] algo not implemented yet\n");
            break;
        case ALGO_DICO:
            fprintf(stderr, "[DICO] algo not implemented yet\n");
            break;
        case ALGO_NO_PIXEL:
            fprintf(stderr, "[NO_PIXEL] algo not implemented yet\n");
            break;
        case ALGO_MEAN:
            fprintf(stderr, "[MEAN] algo not implemented yet\n");
            break;
        case ALGO_FOURIER:
            fprintf(stderr, "[FOURIER] algo not implemented yet\n");
            break;
        default:
            fprintf(stderr, "Invalid algorithm\n");
    }
}

void read_image() {
    switch (algo) {
        case ALGO_NO_COMP:
            no_comp_read(file, file_out);
            break;
        case ALGO_LINEAR:
            fprintf(stderr, "[LINEAR] algo not implemented yet\n");
            break;
        case ALGO_BLOCK:
            fprintf(stderr, "[BLOCK] algo not implemented yet\n");
            break;
        case ALGO_SIMETRY:
            fprintf(stderr, "[SIMETRY] algo not implemented yet\n");
            break;
        case ALGO_DICO:
            fprintf(stderr, "[DICO] algo not implemented yet\n");
            break;
        case ALGO_NO_PIXEL:
            fprintf(stderr, "[NO_PIXEL] algo not implemented yet\n");
            break;
        case ALGO_MEAN:
            fprintf(stderr, "[MEAN] algo not implemented yet\n");
            break;
        case ALGO_FOURIER:
            fprintf(stderr, "[FOURIER] algo not implemented yet\n");
            break;
        default:
            fprintf(stderr, "Invalid algorithm\n");
    }
}


int main(int argc, char *argv[]) {
    get_args(argc, argv);

    switch (act) {
        case ACT_GEN:
            gen_image();
            break;
        case ACT_READ:
            read_image();
            break;
        default:
            fprintf(stderr, "Invalid action\n");
    }

    return 0;
}







































