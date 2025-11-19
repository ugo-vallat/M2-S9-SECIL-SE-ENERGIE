#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
    
    int taille = 400;
    double xmin = -1.0, xmax = 1.0;
    double ymin = -1.0, ymax = 1.0;
    int iterationmax = 100;

    double a = -0.8;
    double b = 0.156;

    /* pixels[line][col][canal] : 3 canaux (R, G, B) */
    unsigned char pixels[200][200][3];

    for (int line = 0; line < taille; line++) {
        for (int col = 0; col < taille; col++) {

            int i = 1;
            double x = xmin + col * (xmax - xmin) / (double)taille;
            double y = ymax - line * (ymax - ymin) / (double)taille;

            while (i <= iterationmax && (x * x + y * y) <= 4.0) {
                /* En Python : x, y = x**2 - y**2 + a, 2*x*y + b
                   La RHS utilise l'ancien x et y, donc on utilise des temporaires. */
                double newx = x * x - y * y + a;
                double newy = 2.0 * x * y + b;
                x = newx;
                y = newy;

                i++;

                if (i > iterationmax && (x * x + y * y) <= 4.0) {
                    pixels[line][col][0] = 0;
                    pixels[line][col][1] = 0;
                    pixels[line][col][2] = 0;
                } else {
                    pixels[line][col][0] = (unsigned char)((4 * i) % 256);
                    pixels[line][col][1] = (unsigned char)((2 * i) % 256);
                    pixels[line][col][2] = (unsigned char)((6 * i) % 256);
                }
            }
        }
    }

    /* Exemple : écrire l'image en PPM pour pouvoir la visualiser */
    FILE *f = fopen("fractal.ppm", "wb");
    if (!f) {
        perror("Erreur d'ouverture de fichier");
        return 1;
    }
    fprintf(f, "P6\n%d %d\n255\n", taille, taille);
    for (int line = 0; line < taille; line++) {
        for (int col = 0; col < taille; col++) {
            fwrite(pixels[line][col], 1, 3, f);
        }
    }
    fclose(f);

    return 0;
}
