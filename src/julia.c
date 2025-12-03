#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>



#define NUM_THREADS 36      // Nombre de threads
#define BLOCK_SIZE  (1<<10)      // Taille d'un bloc
#define IMG_DIM     10000
#define IMG_SIZE    (IMG_DIM*IMG_DIM)     
#define ITE_MAX     10000     
#define XMIN        ((double)(-1.0))
#define XMAX        ((double)(1.0))
#define YMIN        ((double)(-1.0))
#define YMAX        ((double)(1.0))
#define A           ((double)(-0.8))
#define B           ((double)(0.156))


unsigned int img[IMG_SIZE] __attribute__((aligned(16))) = {0};




int next_block = 0;

// Mutex pour protéger next_block
// pthread_mutex_t block_mutex = PTHREAD_MUTEX_INITIALIZER;


long long get_time_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}

void* worker(void *arg) {
    int id = *((int*)arg);
    unsigned start_index = id*BLOCK_SIZE;
    unsigned end_index = start_index + BLOCK_SIZE;

    unsigned index, ite;
    double x,y, x2, y2, tmp;

    while (1) {
        
        if (start_index >= IMG_SIZE) {
            break;
        }
        if(end_index >= IMG_SIZE) {
            end_index = IMG_SIZE;
        }



        for (index = start_index; index < end_index; index++) {
            ite = 1;
            x = XMIN + (index%IMG_DIM) * (XMAX - XMIN) / (double)IMG_DIM;
            y = YMAX - (index/IMG_DIM) * (YMAX - YMIN) / (double)IMG_DIM;
            
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

            if (ite < ITE_MAX || (x2 + y2) > 4.0) {
                img[index] = (unsigned int) ((4 * ite) & 0xFF) << 16 | ((2 * ite) & 0xFF) << 8 | ((6 * ite) & 0xFF);
            }


            
        }
        start_index += BLOCK_SIZE*NUM_THREADS;
        end_index = start_index + BLOCK_SIZE;
        
    }
    return NULL;
}


int main(void) {
    // long long start = get_time_microseconds();

    // printf("[START] main\n");
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

    // Libération des ressources
    // pthread_mutex_destroy(&block_mutex);

    // long long end = get_time_microseconds();
    // printf("[END] t = %lld ys\n", end-start);

    // FILE *fd = fopen("output.julia", "w");
    // for(int i = 0; i < IMG_SIZE; i++) {
    //     fprintf(fd, "%3u|%3u|%3u\n", img[3*i], img[3*i+1], img[3*i+2]);
    // }
    // fclose(fd);

    return EXIT_SUCCESS;
}
