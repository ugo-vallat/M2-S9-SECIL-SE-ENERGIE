#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "util_png.h"

#define NO_COMP_IMG img_rgb_hexa
#define RLE_1D_IMG img_rle_1d

unsigned char img_rgb_hexa[IMG_SIZE*3] = {0};
unsigned int img_rle_1d[IMG_SIZE*2] = {0};
unsigned int img_rle_1d_size = 0;



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



/*________________ RLE 1D ________________*/

void rle_1d_gen(void) {
    unsigned int index, ite, cur_ite, cur_pos, count;

    count = 1;
    cur_ite = compute_pixel(0);
    cur_pos = 0;
    for (index = 1; index < IMG_SIZE; index++) {
        ite = compute_pixel(index);
        if (ite == cur_ite) {
            count++;
        } else {
            RLE_1D_IMG[cur_pos + 0] = count;
            RLE_1D_IMG[cur_pos + 1] = cur_ite;
            printf("[%d,%d]\n", count, cur_ite);
            count = 1;
            cur_ite = ite;
            cur_pos+=2;
        }
    }
    RLE_1D_IMG[cur_pos + 0] = count;
    RLE_1D_IMG[cur_pos + 1] = cur_ite;
    cur_pos+=2;

    img_rle_1d_size = cur_pos / 2;
}

void rle_1d_save(char *dst) {
    FILE *fd = fopen(dst, "w");

    unsigned int dim = IMG_DIM;
    fwrite(&dim, sizeof(dim), 1, fd);
    fwrite(&img_rle_1d_size, sizeof(img_rle_1d_size), 1, fd);
    fwrite(RLE_1D_IMG, sizeof(unsigned int), img_rle_1d_size*2, fd);

    fclose(fd);
}



void rle_1d_read(char *read_file, char *output_file) {
    unsigned char (**img_png);
    unsigned int *img;
    
    FILE *fd_in;
    unsigned int dim;
    size_t img_size;

    fd_in = fopen(read_file, "r");

    if(fread(&dim, sizeof(dim), 1, fd_in) != 1) {
        fprintf(stderr, "Failed to read input\n");
        exit(1);
    }

    if(fread(&img_rle_1d_size, sizeof(img_rle_1d_size), 1, fd_in) != 1) {
        fprintf(stderr, "Failed to read input\n");
        exit(1);
    }

    printf("dim = %d | size = %d\n", dim, img_rle_1d_size);

    img_size = img_rle_1d_size*2;
    img = malloc(img_size*sizeof(unsigned int));

    if (fread(img, sizeof(unsigned int), img_size, fd_in) != img_size) {
        fprintf(stderr, "Failed to read input\n");
        exit(1);
    }

    img_png = malloc(dim * sizeof(unsigned char*));
    unsigned int cur = 0;
    for(unsigned i = 0; i < dim; i++) {
        img_png[i] = malloc(sizeof(unsigned char) * dim * 3);
        for (unsigned j = 0; j < dim; j++) {
            printf("[%d,%d]\n", img[cur], img[cur+1]);
            img_png[i][j*3+0] = (unsigned char) ((4 * img[cur+1]) & 0xFF);
            img_png[i][j*3+1] = (unsigned char) ((2 * img[cur+1]) & 0xFF);
            img_png[i][j*3+2] = (unsigned char) ((6 * img[cur+1]) & 0xFF);
            img[cur] -= 1;
            if (img[cur] == 0) {
                cur+=2;
            }
        }
    }

    fclose(fd_in);
    printf("Pour le moment, tout va bien...\n"); fflush(stdout);

    write_png(output_file, dim, img_png);

    return;
}


/*________________ BLOCK ________________*/


void block_gen(void);
void block_save(char *dst);
void block_read(char *read_file, char *output_file);