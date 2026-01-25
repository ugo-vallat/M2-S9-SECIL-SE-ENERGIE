#include "algos.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



/*___________________________________ STATIC ALLOCATION ___________________________________*/

unsigned char g_img_static_char_1d[IMG_DIM*IMG_DIM*3];
uint32_t g_img_static_u32_1d[IMG_DIM*IMG_DIM];
unsigned char g_img_static_char_3d[IMG_DIM][IMG_DIM][3];

void julia_static_1d_char(void) {

    for (int line = 0; line < IMG_DIM; line++) {
        for (int col = 0; col < IMG_DIM; col++) {

            int i = 1;
            double x = XMIN + col * (XMAX - XMIN) / (double)IMG_DIM;
            double y = YMAX - line * (YMAX - YMIN) / (double)IMG_DIM;

            while (i <= ITE_MAX && (x * x + y * y) <= 4.0) {
                /* En Python : x, y = x**2 - y**2 + a, 2*x*y + b
                   La RHS utilise l'ancien x et y, donc on utilise des temporaires. */
                double newx = x * x - y * y + A;
                double newy = 2.0 * x * y + B;
                x = newx;
                y = newy;

                i++;

                if (i > ITE_MAX && (x * x + y * y) <= 4.0) {
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 0] = 0;
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 1] = 0;
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 2] = 0;
                } else {
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 0] = (unsigned char)((4 * i) % 256);
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 1] = (unsigned char)((2 * i) % 256);
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 2] = (unsigned char)((6 * i) % 256);
                }
            }
        }
    }
}

void julia_static_1d_u32(void) {

    for (int line = 0; line < IMG_DIM; line++) {
        for (int col = 0; col < IMG_DIM; col++) {

            int i = 1;
            double x = XMIN + col * (XMAX - XMIN) / (double)IMG_DIM;
            double y = YMAX - line * (YMAX - YMIN) / (double)IMG_DIM;

            while (i <= ITE_MAX && (x * x + y * y) <= 4.0) {
                /* En Python : x, y = x**2 - y**2 + a, 2*x*y + b
                   La RHS utilise l'ancien x et y, donc on utilise des temporaires. */
                double newx = x * x - y * y + A;
                double newy = 2.0 * x * y + B;
                x = newx;
                y = newy;

                i++;

                if (i > ITE_MAX && (x * x + y * y) <= 4.0) {
                    g_img_static_u32_1d[line*IMG_DIM + col] = 0;

                } else {
                    g_img_static_u32_1d[line*IMG_DIM + col] = ((unsigned char)((4 * i) % 256)) << 16 | 
                        ((unsigned char)((2 * i) % 256)) << 8 |((unsigned char)((6 * i) % 256));

                }
            }
        }
    }
}

void julia_static_3d(void) {

    for (int line = 0; line < IMG_DIM; line++) {
        for (int col = 0; col < IMG_DIM; col++) {

            int i = 1;
            double x = XMIN + col * (XMAX - XMIN) / (double)IMG_DIM;
            double y = YMAX - line * (YMAX - YMIN) / (double)IMG_DIM;

            while (i <= ITE_MAX && (x * x + y * y) <= 4.0) {
                /* En Python : x, y = x**2 - y**2 + a, 2*x*y + b
                   La RHS utilise l'ancien x et y, donc on utilise des temporaires. */
                double newx = x * x - y * y + A;
                double newy = 2.0 * x * y + B;
                x = newx;
                y = newy;

                i++;

                if (i > ITE_MAX && (x * x + y * y) <= 4.0) {
                    g_img_static_char_3d[line][col][0] = 0;
                    g_img_static_char_3d[line][col][1] = 0;
                    g_img_static_char_3d[line][col][2] = 0;
                } else {
                    g_img_static_char_3d[line][col][0] = (unsigned char)((4 * i) % 256);
                    g_img_static_char_3d[line][col][1] = (unsigned char)((2 * i) % 256);
                    g_img_static_char_3d[line][col][2] = (unsigned char)((6 * i) % 256);
                }
            }
        }
    }
}

void julia_static_1d_char_opti(void) {
    double x, y, newx, newy;
    int line, col, i;
    
    for (line = 0; line < IMG_DIM; line++) {
        for (col = 0; col < IMG_DIM; col++) {

            i = 1;
            x = XMIN + col * (XMAX - XMIN) / (double)IMG_DIM;
            y = YMAX - line * (YMAX - YMIN) / (double)IMG_DIM;

            while (i <= ITE_MAX && (x * x + y * y) <= 4.0) {
                /* En Python : x, y = x**2 - y**2 + a, 2*x*y + b
                   La RHS utilise l'ancien x et y, donc on utilise des temporaires. */
                newx = x * x - y * y + A;
                newy = 2.0 * x * y + B;
                x = newx;
                y = newy;

                i++;

                if (i > ITE_MAX && (x * x + y * y) <= 4.0) {
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 0] = 0;
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 1] = 0;
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 2] = 0;
                } else {
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 0] = (unsigned char)((4 * i) % 256);
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 1] = (unsigned char)((2 * i) % 256);
                    g_img_static_char_1d[(line*IMG_DIM + col)*3 + 2] = (unsigned char)((6 * i) % 256);
                }
            }
        }
    }
}


/*___________________________________ DYNAMIC ALLOCATION ___________________________________*/

unsigned char *g_img_alloc_char_1d;
uint32_t *g_img_alloc_u32_1d;
unsigned char ***g_img_alloc_char_3d;

void julia_alloc_1d_char(void) {
    g_img_alloc_char_1d = malloc(IMG_DIM*IMG_DIM*3*sizeof(unsigned char));

    for (int line = 0; line < IMG_DIM; line++) {
        for (int col = 0; col < IMG_DIM; col++) {

            int i = 1;
            double x = XMIN + col * (XMAX - XMIN) / (double)IMG_DIM;
            double y = YMAX - line * (YMAX - YMIN) / (double)IMG_DIM;

            while (i <= ITE_MAX && (x * x + y * y) <= 4.0) {
                /* En Python : x, y = x**2 - y**2 + a, 2*x*y + b
                   La RHS utilise l'ancien x et y, donc on utilise des temporaires. */
                double newx = x * x - y * y + A;
                double newy = 2.0 * x * y + B;
                x = newx;
                y = newy;

                i++;

                if (i > ITE_MAX && (x * x + y * y) <= 4.0) {
                    g_img_alloc_char_1d[(line*IMG_DIM + col)*3 + 0] = 0;
                    g_img_alloc_char_1d[(line*IMG_DIM + col)*3 + 1] = 0;
                    g_img_alloc_char_1d[(line*IMG_DIM + col)*3 + 2] = 0;
                } else {
                    g_img_alloc_char_1d[(line*IMG_DIM + col)*3 + 0] = (unsigned char)((4 * i) % 256);
                    g_img_alloc_char_1d[(line*IMG_DIM + col)*3 + 1] = (unsigned char)((2 * i) % 256);
                    g_img_alloc_char_1d[(line*IMG_DIM + col)*3 + 2] = (unsigned char)((6 * i) % 256);
                }
            }
        }
    }
}

void julia_alloc_1d_u32(void) {
    g_img_alloc_u32_1d = malloc(sizeof(uint32_t)*IMG_DIM*IMG_DIM);

    for (int line = 0; line < IMG_DIM; line++) {
        for (int col = 0; col < IMG_DIM; col++) {

            int i = 1;
            double x = XMIN + col * (XMAX - XMIN) / (double)IMG_DIM;
            double y = YMAX - line * (YMAX - YMIN) / (double)IMG_DIM;

            while (i <= ITE_MAX && (x * x + y * y) <= 4.0) {
                /* En Python : x, y = x**2 - y**2 + a, 2*x*y + b
                   La RHS utilise l'ancien x et y, donc on utilise des temporaires. */
                double newx = x * x - y * y + A;
                double newy = 2.0 * x * y + B;
                x = newx;
                y = newy;

                i++;

                if (i > ITE_MAX && (x * x + y * y) <= 4.0) {
                    g_img_alloc_u32_1d[line*IMG_DIM + col] = 0;

                } else {
                    g_img_alloc_u32_1d[line*IMG_DIM + col] = ((unsigned char)((4 * i) % 256)) << 16 | 
                        ((unsigned char)((2 * i) % 256)) << 8 |((unsigned char)((6 * i) % 256));

                }
            }
        }
    }
}

void julia_alloc_3d(void) {
    g_img_alloc_char_3d = malloc(sizeof(unsigned char **)*IMG_DIM);

    for (int line = 0; line < IMG_DIM; line++) {
        g_img_alloc_char_3d[line] = malloc(sizeof(unsigned char *)*IMG_DIM);
        for (int col = 0; col < IMG_DIM; col++) {
            g_img_alloc_char_3d[line][col] = malloc(sizeof(unsigned char) * 3);

            int i = 1;
            double x = XMIN + col * (XMAX - XMIN) / (double)IMG_DIM;
            double y = YMAX - line * (YMAX - YMIN) / (double)IMG_DIM;

            while (i <= ITE_MAX && (x * x + y * y) <= 4.0) {
                /* En Python : x, y = x**2 - y**2 + a, 2*x*y + b
                   La RHS utilise l'ancien x et y, donc on utilise des temporaires. */
                double newx = x * x - y * y + A;
                double newy = 2.0 * x * y + B;
                x = newx;
                y = newy;

                i++;
                if (i > ITE_MAX && (x * x + y * y) <= 4.0) {
                    g_img_alloc_char_3d[line][col][0] = 0;
                    g_img_alloc_char_3d[line][col][1] = 0;
                    g_img_alloc_char_3d[line][col][2] = 0;
                } else {
                    g_img_alloc_char_3d[line][col][0] = (unsigned char)((4 * i) % 256);
                    g_img_alloc_char_3d[line][col][1] = (unsigned char)((2 * i) % 256);
                    g_img_alloc_char_3d[line][col][2] = (unsigned char)((6 * i) % 256);
                }
            }
        }
    }
}
