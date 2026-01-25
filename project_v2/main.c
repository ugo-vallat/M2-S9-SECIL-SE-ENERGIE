
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "algos.h"
#include <sys/time.h>



typedef enum {
    ALGO_STATIC_1D_CHAR=0,
    ALGO_STATIC_1D_U32=1,
    ALGO_STATIC_3D_CHAR=2,
    ALGO_ALLOC_1D_CHAR=3,
    ALGO_ALLOC_1D_U32=4,
    ALGO_ALLOC_3D_CHAR=5,
    ALGO_STATIC_1D_CHAR_OPTI=6,
    MAX_ALOG=7
} Algo_t;

Algo_t algo;
// Act act;
char *file;
char file_out[512];



void usage(char *exec) {
    printf("usage : %s <act> <algo> <file_path>\n", exec);
    printf("\taglo in STATIC_1D_CHAR:0, STATIC_1D_U32:1, STATIC_3D_CHAR:2, ALLOC_1D_CHAR:3, ALLOC_1D_U32:4, ALLOC_3D_CHAR:5, ALGO_STATIC_1D_CHAR_OPTI=6\n");
    // printf("\tact in [gen,read]\n");
    exit(1);
}

void get_args(int argc, char *argv[]) {

    if (argc != 2) {
        usage(argv[0]);
    }

    // if (strcmp(argv[1], "gen") == 0) {
    //     act = ACT_GEN;
    // } else if (strcmp(argv[1], "read") == 0) {
    //     act = ACT_READ;
    // } else {
    //     fprintf(stderr, "Invalid action\n");
    //     usage(argv[0]);
    // }

    unsigned tmp;
    sscanf(argv[1], "%u", &tmp);
    if(tmp >= MAX_ALOG) {
        fprintf(stderr, "Invalid algo\n");
        usage(argv[0]);
    }
    algo = tmp;

    // file = argv[3];
    // sprintf(file_out, "%s.png", file);
}


void gen_image() {
    switch (algo) {
        case ALGO_STATIC_1D_CHAR:
            julia_static_1d_char();
            break;
        case ALGO_STATIC_1D_U32:
            julia_static_1d_u32();
            break;
        case ALGO_STATIC_3D_CHAR:
            julia_static_3d();
            break;
        case ALGO_ALLOC_1D_CHAR:
            julia_alloc_1d_char();
            break;
        case ALGO_ALLOC_1D_U32:
            julia_alloc_1d_u32();
            break;
        case ALGO_ALLOC_3D_CHAR:
            julia_alloc_3d();
            break;
        case ALGO_STATIC_1D_CHAR_OPTI:
            julia_static_1d_char_opti();
            break;
        default:
            fprintf(stderr, "Invalid algorithm\n");
    }
}

long long get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}


int main(int argc, char *argv[]) {
    get_args(argc, argv);

    // printf("[GEN] start...");fflush(stdout);
    // long long start = get_time_microseconds();
    gen_image();
    // long long end = get_time_microseconds();
    // printf("OK\n");
    // printf("Duration : %lldms\n", (end - start) / 1000 );

    return 0;
}







































