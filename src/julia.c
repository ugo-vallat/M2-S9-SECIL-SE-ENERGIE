#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>



#define NUM_THREADS 8      // Nombre de threads
#define BLOCK_SIZE  1024      // Taille d'un bloc
#define IMG_DIM     10000
#define IMG_SIZE    (IMG_DIM*IMG_DIM)     
#define ITE_MAX     1000     
#define XMIN        ((double)(-1.0))
#define XMAX        ((double)(1.0))
#define YMIN        ((double)(-1.0))
#define YMAX        ((double)(1.0))
#define A           ((double)(-0.8))
#define B           ((double)(0.156))


char img[IMG_SIZE*3] __attribute__((aligned(64)));




int next_block = 0;

// Mutex pour protéger next_block
// pthread_mutex_t block_mutex = PTHREAD_MUTEX_INITIALIZER;


long long get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}


// Fonction exécutée par chaque thread
void* worker(void *arg) {
    int id = *((int*)arg);
    // printf("[START] thread\n");
    unsigned start_index = id*BLOCK_SIZE;
    unsigned end_index = start_index + BLOCK_SIZE;

    unsigned index, ite;
    double x,y, x2, y2, tmp;
    long long t_init = 0, t_for = 0, t_mem = 0, t_start;
    while (1) {
        
        // t_start = get_time_microseconds();
        // Récupération du prochain bloc à traiter
        // pthread_mutex_lock(&block_mutex);
        // start_index = next_block * BLOCK_SIZE;
        // next_block++;
        // pthread_mutex_unlock(&block_mutex);

        
        if (start_index >= IMG_SIZE) {
            break;
        }
        if(end_index >= IMG_SIZE) {
            end_index = IMG_SIZE - 1;
        }

        // t_init += get_time_microseconds() - t_start;

        for (index = start_index; index < end_index; index++) {
            ite = 1;
            x = XMIN + (index%IMG_DIM) * (XMAX - XMIN) / (double)IMG_DIM;
            y = YMAX - (index/IMG_DIM) * (YMAX - YMIN) / (double)IMG_DIM;
            
            x2 = x*x;
            y2 = y*y;
            // t_start = get_time_microseconds();
            while (ite <= ITE_MAX && (x2 + y2) <= 4.0) {
                tmp = x2 - y2 + A;
                y = 2.0 * x * y + B;
                x = tmp;
                ite++;
                x2=x*x;
                y2=y*y;
            }

            // t_for += get_time_microseconds() - t_start;
            // t_start = get_time_microseconds();

            if (ite < ITE_MAX || (x*x + y*y) > 4.0) {
                img[3*index] = (unsigned char)((4 * ite) % 256);
                img[3*index+1] = (unsigned char)((2 * ite) % 256);
                img[3*index+2] = (unsigned char)((6 * ite) % 256);
            }

            // t_mem += get_time_microseconds() - t_start;

            
        }
        start_index += BLOCK_SIZE*NUM_THREADS;
        end_index = start_index + BLOCK_SIZE;
        
    }

    // printf("[END] thread(%d) : t_init = %lld, t_for = %lld, t_mem = %lld\n", id, t_init, t_for, t_mem);

    return NULL;
}

int main(void) {
    long long start = get_time_microseconds();

    printf("[START] main\n");
    int i;

    // Préparation des threads
    pthread_t threads[NUM_THREADS];

    // Création des threads
    for (i = 0; i < NUM_THREADS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        if (pthread_create(&threads[i], NULL, worker, id) != 0) {
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
    // pthread_mutex_destroy(&block_mutex);

    long long end = get_time_microseconds();
    printf("[END] t = %lld ys\n", end-start);

    return EXIT_SUCCESS;
}
