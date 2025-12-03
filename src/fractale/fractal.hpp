#ifndef __FRACTAL_HPP__
#define __FRACTAL_HPP__

#include <memory>
#include <vector>
#include <atomic>

class Fractal {
    public:
        Fractal();
        Fractal(size_t size = 200, size_t iter = 100);
        ~Fractal();
        void op(size_t start_width, size_t start_height, size_t end_width, size_t end_height);
        void opmp(size_t start_width, size_t start_height, size_t end_width, size_t end_height);
        void generate_img_seq();
        void generate_img_threads();
        void generate_img_threadsop(size_t block_size);
        void op_block(size_t id, size_t block_size);
        void generate_img_openmp();

    private:
        void get_id(size_t block_size);


    private:
        std::vector<unsigned int> pixels;
        size_t width;
        size_t height;
        size_t iteration;
        std::atomic<size_t> id_block;
};

#endif