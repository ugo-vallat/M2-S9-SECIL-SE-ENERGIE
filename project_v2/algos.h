#ifndef __ALGOS_H__
#define __ALGOS_H__



#define IMG_DIM     10000
#define IMG_SIZE    (IMG_DIM*IMG_DIM)     
#define ITE_MAX     1000     
#define XMIN        ((double)(-1.0))
#define XMAX        ((double)(1.0))
#define YMIN        ((double)(-1.0))
#define YMAX        ((double)(1.0))
#define A           ((double)(-0.8))
#define B           ((double)(0.156))

void julia_static_1d_char(void);

void julia_static_1d_u32(void);

void julia_static_3d(void);

void julia_alloc_1d_char(void);

void julia_alloc_1d_u32(void);

void julia_alloc_3d(void);

void julia_static_1d_char_opti(void);

#endif // __ALGOS_H__