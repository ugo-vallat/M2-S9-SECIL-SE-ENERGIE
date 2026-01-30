#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>



#define NUM_THREADS 32      // Nombre de threads
#define IMG_DIM     10000

#define BLOCK_SIZE  (IMG_DIM*10)      // Taille d'un bloc
#define IMG_SIZE    (IMG_DIM*IMG_DIM)     
#define ITE_MAX     10000     
#define XMIN        ((double)(-1.0))
#define XMAX        ((double)(1.0))
#define YMIN        ((double)(-1.0))
#define YMAX        ((double)(1.0))
#define A           ((double)(-0.8))
#define B           ((double)(0.156))


unsigned int img[IMG_SIZE];




// int next_block = 0;
unsigned int ids[NUM_THREADS];

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

    unsigned index, ite, l, c;
    double x,y, x2, y2, tmp;

    while (1) {
        
        if (start_index >= IMG_SIZE) {
            break;
        }
        if(end_index >= IMG_SIZE) {
            end_index = IMG_SIZE;
        }

        for (index = start_index; index < end_index; index++) {
            l = index/IMG_DIM;
            c = index%IMG_DIM;

            if(c == 0 || l == 0) {
                ite = 1;
                x = XMIN + l * (XMAX - XMIN) / (double)IMG_DIM;
                y = YMAX - c * (YMAX - YMIN) / (double)IMG_DIM;
                
                x2 = x*x;
                y2 = y*y;
                while (ite <= ITE_MAX && (x2 + y2) <= 4.0) {
                    tmp = x2 - y2 + A;
                    y =  2 * x * y + B;
                    x = tmp;
                    x2=x*x;
                    y2=y*y;
                    ite++;
                }
                
                if (ite < ITE_MAX || (x2 + y2) > 4.0) {
                    img[index] = (unsigned int) ((4 * ite) & 0xFF) << 16 | ((2 * ite) & 0xFF) << 8 | ((6 * ite) & 0xFF);
                }
                continue;
            }
            
            if(c > l) {
                continue;
            }

            ite = 1;
            x = XMIN + l * (XMAX - XMIN) / (double)IMG_DIM;
            y = YMAX - c * (YMAX - YMIN) / (double)IMG_DIM;
            
            x2 = x*x;
            y2 = y*y;
            while (ite <= ITE_MAX && (x2 + y2) <= 4.0) {
                tmp = x2 - y2 + A;
                y =  2 * x * y + B;
                x = tmp;
                x2=x*x;
                y2=y*y;
                ite++;
            }
            
            if (ite < ITE_MAX || (x2 + y2) > 4.0) {
                img[index] = (unsigned int) ((4 * ite) & 0xFF) << 16 | ((2 * ite) & 0xFF) << 8 | ((6 * ite) & 0xFF);
                // img[(IMG_DIM-l) * IMG_DIM + (IMG_DIM-c)] = img[index];
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
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, worker, &(ids[i])) != 0) {
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

    // FILE *fd = fopen("output_ref.julia", "w");
    // for(int i = 0; i < IMG_SIZE; i++) {
    //     // for(int j = 0; j < IMG_DIM; j++) {
    //     //     fprintf(fd, "%3u ", img[i*IMG_DIM+j]);
    //     // }
    //     // fprintf(fd, "\n");
    //     fprintf(fd, "%3u|%3u|%3u\n", (img[i] >> 16) & 0xFF, (img[i] >> 8) & 0xFF, img[i] & 0xFF);
    // }
    // fclose(fd);

    return EXIT_SUCCESS;
}
