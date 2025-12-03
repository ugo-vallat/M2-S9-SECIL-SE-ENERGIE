#include "fractal.hpp"
#include <cstdio>
#include <stdexcept>
#include <cmath>
#include <cassert>
#include <thread>
#include <atomic>
#include <omp.h>
#include <iostream>

static unsigned int set_color(unsigned char R, unsigned char G, unsigned char B) {
    return (unsigned int)((R << 24) | (G << 16) | (B << 8) | 0xff);
}

Fractal::Fractal(): Fractal(200, 100) {
    
}

Fractal::Fractal(size_t size, size_t iter) : id_block(0) {
    this->height = size;
    this->width = size;
    this->iteration = iter;

    this->pixels = {};
    this->pixels.resize(height * width);
}

Fractal::~Fractal() 
{
}


void Fractal::op(size_t start_width, size_t start_height, size_t end_width, size_t end_height) {
    
    double xmin = -1, xmax = 1;
    double ymin = -1, ymax = 1;

    double a = -0.8;
    double b = 0.156;
    
    for (size_t line = start_height; line < end_height; line++) {    
        for (size_t col = start_width; col < end_width; col++) {

            size_t i = 1;
            double x = xmin+col*(xmax-xmin)/width;
            double y = ymax-line*(ymax-ymin)/height;

            while (i <= this->iteration && (((x*x) + (y*y)) <= 4)) {
                double temp_x = x;
                x = (temp_x*temp_x) - (y*y) + a;
                y = 2 * temp_x * y + b;
                i += 1;

            }
            #ifdef PARAop
            if (i > this->iteration)
                pixels[width * line + col] = 0xffffffff;
            else
                pixels[width * line + col] = set_color((4*i)%256, (2*i)%256, (6*i)%256);
            #else
            if (i > this->iteration && ((x*x) + (y*y))<=4)
                pixels[width * line + col] = 0xffffffff;
            else
                pixels[width * line + col] = set_color((4*i)%256, (2*i)%256, (6*i)%256);
            #endif
        }
    }
}

void Fractal::opmp(size_t start_width, size_t start_height, size_t end_width, size_t end_height) {
    double xmin = -1, xmax = 1;
    double ymin = -1, ymax = 1;

    double a = -0.8;
    double b = 0.156;
    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic) collaspe(2)
        for (size_t line = start_height; line < end_height; line++) {    
            for (size_t col = start_width; col < end_width; col++) {

                size_t i = 1;
                double x = xmin+col*(xmax-xmin)/width;
                double y = ymax-line*(ymax-ymin)/height;

                while (i <= this->iteration && (((x*x) + (y*y)) <= 4)) {
                    double temp_x = x;
                    x = (temp_x*temp_x) - (y*y) + a;
                    y = 2 * temp_x * y + b;
                    i += 1;

                    if (i > this->iteration && ((x*x) + (y*y))<=4)
                        pixels[width * line + col] = 0xffffffff;
                    else
                        pixels[width * line + col] = set_color((4*i)%256, (2*i)%256, (6*i)%256);
                }
            }
        }
    }
}

void Fractal::generate_img_openmp() {
    opmp(0, 0, this->width, this->height);
}

void Fractal::generate_img_seq() {
    op(0, 0, this->width, this->height);
}



void Fractal::generate_img_threads() {
    size_t nbr_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    if (nbr_threads == 0) nbr_threads = 4;

    size_t nbr_lines = this->height / nbr_threads;
    size_t rest_lines = this->height % nbr_threads;

    size_t start_pos = 0;

    for (size_t i = 0; i < nbr_threads; i++) {
        if (i == nbr_threads - 1)
            threads.emplace_back(&Fractal::op, this, 0, start_pos, this->width, start_pos + nbr_lines + rest_lines);
        else
            threads.emplace_back(&Fractal::op, this, 0, start_pos, this->width, start_pos + nbr_lines);
        start_pos += nbr_lines;
    }

    for (auto& i : threads) {
        i.join();
    }

}

/*
void Fractal::op_block(size_t id, size_t block_size) {
    size_t nbr_block_x = (width + block_size - 1) / block_size;
    

    size_t bx = id % nbr_block_x;
    size_t by = id / nbr_block_x;
    
    size_t start_pos_x = block_size * bx;
    size_t start_pos_y = block_size * by;

    size_t end_pos_x = (width > (start_pos_x + block_size))? (start_pos_x + block_size) : width ;
    size_t end_pos_y = (height > (start_pos_y + block_size)) ? (start_pos_y + block_size) : height ;

    op(start_pos_x, start_pos_y, end_pos_x, end_pos_y);
}

void Fractal::get_id(size_t block_size) {
    size_t nbr_block_x = (width + block_size - 1) / block_size;
    size_t nbr_block_y = (height + block_size - 1) / block_size;

    size_t nbr_block = nbr_block_x * nbr_block_y;

    size_t id = 0;

    while(true) {
        id = id_block.fetch_add(1);
        if (id >= nbr_block) break;

        op_block(id, block_size);
    }
}*/
void Fractal::op_block(size_t id, size_t block_size) {
    size_t nbr_block_per_line = (width + block_size - 1) / block_size;

    

    size_t bx = id % nbr_block_per_line;
    size_t by = id / nbr_block_per_line;
    
    size_t start_pos_x = block_size * bx;
    size_t start_pos_y = by;

    size_t end_pos_x = (width > (start_pos_x + block_size))? (start_pos_x + block_size) : width ;
    size_t end_pos_y = by + 1;

    op(start_pos_x, start_pos_y, end_pos_x, end_pos_y);
}

void Fractal::create_img() {
    FILE *fd = fopen("output.julia", "w");
    for(int i = 0; i < (this->height*this->height); i++) {
        fprintf(fd, "%3u|%3u|%3u\n", (this->pixels[i]>>24)&0xFF, (this->pixels[i]>>16)&0xFF, (this->pixels[i]>>8)&0xFF);
    }
    fclose(fd);
}

void Fractal::get_id(size_t block_size) {
    size_t nbr_block_per_line = (width + block_size - 1) / block_size;

    size_t nbr_block = nbr_block_per_line * height;

    size_t id = 0;

    while(true) {
        id = id_block.fetch_add(1);
        if (id >= nbr_block) break;

        op_block(id, block_size);
    }
}

void Fractal::generate_img_threadsop(size_t block_size) {
    id_block.store(0);
    size_t nbr_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    if (nbr_threads == 0) nbr_threads = 4;


    for (size_t i = 0; i < nbr_threads; i++) {
        threads.emplace_back(&Fractal::get_id, this, block_size);
    }

    for (auto& i : threads) {
        i.join();
    }
}