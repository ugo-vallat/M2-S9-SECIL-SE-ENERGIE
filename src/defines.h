#ifndef __DEFINES_H__
#define __DEFINES_H__


#define XMIN        ((double)(-1.0))
#define XMAX        ((double)(1.0))
#define YMIN        ((double)(-1.0))
#define YMAX        ((double)(1.0))
#define ITE_MAX     1000
#define A           ((double)(-0.8))
#define B           ((double)(0.156))


#define IMG_DIM     10000
#define IMG_SIZE    (IMG_DIM*IMG_DIM)    



/* ALGO_NO_COMP */

void no_comp_gen(void);
void no_comp_save(char *dst);
void no_comp_read(char *read_file, char *output_file);

/* ALGO_LINEAR */

void linear_gen(void);
void linear_save(char *dst);
void linear_read(char *read_file, char *output_file);

/* ALGO_BLOCK */

/* ALGO_SIMETRY */

/* ALGO_DICO */

/* ALGO_MEAN */

/* ALGO_FOURIER */





typedef enum {
    ALGO_NO_COMP =0,
    ALGO_LINEAR  =1,
    ALGO_BLOCK   =2,
    ALGO_SIMETRY =3,
    ALGO_DICO    =4,
    ALGO_NO_PIXEL=5,
    ALGO_MEAN    =6,
    ALGO_FOURIER =7,
    MAX_ALOG=8
} Algo_t;

typedef enum {
    ACT_GEN=0,
    ACT_READ=1,
    MAX_ACT
} Act;






#endif // __DEFINES_H__