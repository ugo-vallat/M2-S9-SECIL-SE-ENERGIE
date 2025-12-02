#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>



#define NUM_THREADS 8      // Nombre de threads
#define BLOCK_SIZE  256      // Taille d'un bloc
#define IMG_DIM     10000
#define IMG_SIZE    (IMG_DIM*IMG_DIM)     
#define ITE_MAX     10000     
#define XMIN        ((double)(-1.0))
#define XMAX        ((double)(1.0))
#define YMIN        ((double)(-1.0))
#define YMAX        ((double)(1.0))
#define A           ((double)(-0.8))
#define B           ((double)(0.156))


char img[IMG_SIZE*3] __attribute__((aligned(64)));




int next_block = 0;

// Mutex pour protéger next_block
pthread_mutex_t block_mutex = PTHREAD_MUTEX_INITIALIZER;

// Fonction exécutée par chaque thread
void* worker(void *arg) {
    printf("[START] thread\n");
    unsigned start_index, end_index;
    unsigned index, ite;
    double x,y, x2, y2;
    
    while (1) {
        

        // Récupération du prochain bloc à traiter
        pthread_mutex_lock(&block_mutex);
        start_index = next_block * BLOCK_SIZE;
        next_block++;
        pthread_mutex_unlock(&block_mutex);

        if (start_index >= IMG_SIZE) {
            return NULL;
        }
        end_index = start_index + BLOCK_SIZE;
        
        if(end_index >= IMG_SIZE) {
            end_index = IMG_SIZE - 1;
        }

        // Si on est en dehors du tableau, on arrête
        if (start_index >= IMG_SIZE) {
            break;
        }

        for (index = start_index; index < end_index; index++) {
            ite = 1;
            x = XMIN + (index%IMG_DIM) * (XMAX - XMIN) / (double)IMG_DIM;
            y = YMAX - (index/IMG_DIM) * (YMAX - YMIN) / (double)IMG_DIM;

            // x2 = x*x;
            // y2 = y*y;
            while (ite <= ITE_MAX && (x*x + y*y) <= 4.0) {
                x = x*x - y*y + A;
                y = 2.0 * x * y + B;
                ite++;
            }

            if (ite > ITE_MAX && (x*x + y*y) <= 4.0) {
                img[3*index] = 0;
                img[3*index+1] = 0;
                img[3*index+2] = 0;
            } else {
                img[3*index] = (unsigned char)((4 * ite) % 256);
                img[3*index+1] = (unsigned char)((2 * ite) % 256);
                img[3*index+2] = (unsigned char)((6 * ite) % 256);
            }
            }
        
    }

    printf("[END] thread\n");

    return NULL;
}

long long get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}

int main(void) {
    long long start = get_time_microseconds();

    printf("[START] main\n");
    int i;

    // Préparation des threads
    pthread_t threads[NUM_THREADS];

    // Création des threads
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, worker, NULL) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    // Attente de la fin de tous les threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Affichage du résultat
    // printf("Tableau après incrémentation :\n");
    // for (i = 0; i < IMG_SIZE; i++) {
    //     if (i%IMG_DIM == 0) {
    //         printf("\n");
    //     }
    //     printf("%d,%d,%d ", img[3*i], img[3*i+1], img[3*i+2]);
    // }
    // printf("\n");

    // Libération des ressources
    pthread_mutex_destroy(&block_mutex);

    long long end = get_time_microseconds();
    printf("[END] t = %lld ys\n", end-start);

    return EXIT_SUCCESS;
}
