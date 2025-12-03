#include <iostream>
//#include <SDL3/SDL.h>
#include "fractal.hpp"
#include <cstring>

void usage() {
    std::cout << "./fractal {size} {iteration} (mode SEQ, PARA, OPENMP)" << std::endl;
    std::cout << "./fractal {size} {iteration} {block_size} (mode PARAop)" << std::endl;
    std::cout << "size is an unsigned integer to set the image width and height" << std::endl;
}



int main(int ac, char**av) {
    if (!(ac == 3 || ac == 4)) {
        usage();
        return 1;
    }
/*
    if (!SDL_Init(0)) {
        std::cout << "failed to init SDL" << std::endl;
        return 2;
    }
*/
    char* endParam = nullptr;
    long size = strtol(av[1], &endParam, 10);

    if (*endParam != '\0' && size == 0) {
        usage();
        return 1;
    }

    long iteration = strtol(av[2], &endParam, 10);
    if (*endParam != '\0' && iteration == 0) {
        usage();
        return 1;
    }

    #ifdef PARAop
        if (ac != 4) {
            usage();
            return 1;
        }

        size_t block_size = strtol(av[3], &endParam, 10);
        if (*endParam != '\0' && block_size == 0) {
            usage();
            return 1;
        }
    #endif

    
    Fractal fract = {(size_t)size, (size_t)iteration};


    #ifdef SEQ
        fract.generate_img_seq();
    #endif

    #ifdef PARA
        fract.generate_img_threads();
    #endif

    #ifdef PARAop
        fract.generate_img_threadsop(block_size);
    #endif

    #ifdef OPENMP
        fract.generate_img_openmp();
    #endif

    // fract.create_img();

    //SDL_Quit();

    return 0;
}