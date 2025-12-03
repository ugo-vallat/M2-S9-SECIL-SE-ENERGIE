#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#include "util_png.h"

int write_png(const char *filename, int dim, unsigned char **img) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        return -1;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        return -1;
    }

    if (setjmp(png_jmpbuf(png))) {  // gestion erreurs libpng
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return -1;
    }

    png_init_io(png, fp);

    // En-tête PNG
    png_set_IHDR(
        png,
        info,
        dim,
        dim,
        8,                      // 8 bits par canal
        PNG_COLOR_TYPE_RGB,     // RGB
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png, info);

    // Libpng attend un tableau de pointeurs vers chaque ligne
    png_bytep *row_pointers = img;

    printf("OK...\n"); fflush(stdout);
    png_write_image(png, img);
    png_write_end(png, NULL);

    png_destroy_write_struct(&png, &info);
    fclose(fp);

    return 0;
}