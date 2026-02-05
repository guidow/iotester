#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <filesystem>

#include "bufferqueue.h"
#include "bufferfiller.h"
#include "filewriter.h"

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    BufferQueue bufferqueue;

    std::vector<std::thread> threads;
    const unsigned int num_cores = std::thread::hardware_concurrency();
    for(unsigned int i = 0 ; i < num_cores ; ++i)
        threads.push_back(std::thread(BufferFiller(bufferqueue)));

    std::thread writer_thread(FileWriter(
        bufferqueue,
        fs::path(".")));

    for(auto& thread : threads)
        thread.join();
    writer_thread.join();

    std::cout << "Done" << std::endl;

    return 0;
}
