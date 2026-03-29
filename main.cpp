#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
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

    fs::path working_dir = fs::path(".") / "iotest";
    std::size_t checksum_failures = 0;
    for(auto const& dentry : fs::directory_iterator{working_dir})
    {
        HashedBuffer filecontent;
        std::ifstream file(dentry.path());
        if(!file.good())
        {
            std::cerr << "Could not open " << dentry.path() << " for reading" << std::endl;
            return EXIT_FAILURE;
        }
        file.read(filecontent.data(), filecontent.size());
        filecontent.calculate_hash();
        if(dentry.path().filename() != filecontent.digest_str())
        {
            std::cerr << "Wrong SHA-256 sum for " << dentry.path() << ": " << filecontent.digest_str() << std::endl;
            checksum_failures++;
        }
        else
        {
            fs::remove(dentry.path());
        }
    }

    return checksum_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
