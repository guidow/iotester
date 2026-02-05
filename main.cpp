#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <thread>

#include "bufferqueue.h"
#include "bufferfiller.h"

int main(int argc, char **argv) {
    BufferQueue bufferqueue;

    std::vector<std::thread> threads;
    const unsigned int num_cores = std::thread::hardware_concurrency();
    for(unsigned int i = 0 ; i < num_cores ; ++i)
        threads.push_back(std::thread(BufferFiller(bufferqueue)));

    bufferqueue.set_end_of_empty_buffers();

    for(int i = 0 ; i < 100 ; ++i)
        bufferqueue.post_empty_buffer(std::make_shared<HashedBuffer>());

    for(auto& thread : threads)
        thread.join();

    std::cout << "Done" << std::endl;

    return 0;
}
