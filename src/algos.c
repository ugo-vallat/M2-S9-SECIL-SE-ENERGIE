#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "util_png.h"

#define NO_COMP_IMG img_rgb_hexa
#define LINEAR_IMG img_i_linear

unsigned char img_rgb_hexa[IMG_SIZE*3] = {0};
unsigned int img_i_linear[IMG_SIZE*2] = {0};



/*________________ COMMON ________________*/

unsigned char compute_pixel(unsigned int pixel) {
    double x,y, x2, y2, tmp;
    unsigned ite;

    ite = 1;
    x = XMIN + (pixel%IMG_DIM) * (XMAX - XMIN) / (double)IMG_DIM;
    y = YMAX - (pixel/IMG_DIM) * (YMAX - YMIN) / (double)IMG_DIM;
    
    x2 = x*x;
    y2 = y*y;
    while (ite <= ITE_MAX && (x2 + y2) <= 4.0) {
        tmp = x2 - y2 + A;
        y = 2.0 * x * y + B;
        x = tmp;
        ite++;
        x2=x*x;
        y2=y*y;
    }

    if (!(ite < ITE_MAX || (x2 + y2) > 4.0)) {
        ite = 0;
    }

    return ite;
}

/*________________ NO COMP ________________*/

void no_comp_gen(void) {
    unsigned index, ite;

    for (index = 0; index < IMG_SIZE; index++) {
        ite = compute_pixel(index);
        if (ite) {
            NO_COMP_IMG[index*3 + 0] = (unsigned char) ((4 * ite) & 0xFF);
            NO_COMP_IMG[index*3 + 1] = (unsigned char) ((2 * ite) & 0xFF);
            NO_COMP_IMG[index*3 + 2] = (unsigned char) ((6 * ite) & 0xFF);
        }   
        // printf("%2x %2x %2x\n", NO_COMP_IMG[index*3 + 0], NO_COMP_IMG[index*3 + 1], NO_COMP_IMG[index*3 + 2]);
    }
}


void no_comp_save(char *dst) {
    FILE *fd = fopen(dst, "w");
/*
    fprintf(fd, "%d\n", IMG_DIM);
    for(int i = 0; i < IMG_SIZE; i++) {
        fprintf(fd, "%02x%02x%02x", NO_COMP_IMG[i][0], NO_COMP_IMG[i][1], NO_COMP_IMG[i][2]);
    }
*/
    unsigned int dim = IMG_DIM;
    fwrite(&dim, sizeof(dim), 1, fd);
    fwrite(NO_COMP_IMG, sizeof(unsigned char), IMG_SIZE*3, fd);

    fclose(fd);
}


void no_comp_read(char *read_file, char *output_file) {
    unsigned char (**img_png);
    unsigned char *img;
    FILE *fd_in;
    unsigned int dim;
    size_t img_size;

    fd_in = fopen(read_file, "r");

    if(fread(&dim, sizeof(dim), 1, fd_in) != 1) {
        fprintf(stderr, "Failed to read input\n");
        exit(1);
    }

    printf("dim : %u\n", dim); fflush(stdout);
    img_size = dim*dim*3;
    img = malloc(img_size*sizeof(unsigned char));

    if (fread(img, sizeof(unsigned char), img_size, fd_in) != img_size) {
        fprintf(stderr, "Failed to read input\n");
        exit(1);
    }

    img_png = malloc(dim * sizeof(unsigned char*));
    for(unsigned i = 0; i < dim; i++) {
        img_png[i] = &(img[i*dim*3]);
    }

    fclose(fd_in);
    printf("Pour le moment, tout va bien...\n"); fflush(stdout);

    write_png(output_file, dim, img_png);

    return;
}



/*________________ LINEAR ________________*/

void linear_gen(void) {
    unsigned index, ite, cur_ite, cur_pos, count;

    count = 0;
    cur_ite = 0;
    cur_pos = 0;
    for (index = 0; index < IMG_SIZE; index++) {
        ite = compute_pixel(index);
        if (ite == cur_ite) {
            count++;
        } else {
            LINEAR_IMG[cur_pos + 0] = cur_ite;
            LINEAR_IMG[cur_pos + 1] = count;
            count = 1;
            cur_ite = ite;
            cur_pos+=2;
        }
    }
    LINEAR_IMG[cur_pos + 0] = cur_ite;
    LINEAR_IMG[cur_pos + 1] = count;
}

void linear_save(char *dst) {
    FILE *fd = fopen(dst, "w");

    unsigned int dim = IMG_DIM;
    fwrite(&dim, 1, sizeof(dim), fd);
    fwrite(NO_COMP_IMG, sizeof(unsigned char), sizeof(NO_COMP_IMG), fd);

    fclose(fd);
}



void linear_read(char *read_file, char *output_file) {
    unsigned char (**img_png);
    unsigned char *img;
    FILE *fd_in;
    unsigned int dim;
    size_t img_size;

    fd_in = fopen(read_file, "r");

    if(fread(&dim, sizeof(dim), 1, fd_in) != 1) {
        fprintf(stderr, "Failed to read input\n");
        exit(1);
    }

    printf("dim : %u\n", dim); fflush(stdout);
    img_size = dim*dim*3;
    img = malloc(img_size*sizeof(unsigned char));

    if (fread(img, sizeof(unsigned char), img_size, fd_in) != img_size) {
        fprintf(stderr, "Failed to read input\n");
        exit(1);
    }

    img_png = malloc(dim * sizeof(unsigned char*));
    for(unsigned i = 0; i < dim; i++) {
        img_png[i] = &(img[i*dim*3]);
    }

    fclose(fd_in);
    printf("Pour le moment, tout va bien...\n"); fflush(stdout);

    write_png(output_file, dim, img_png);

    return;
}