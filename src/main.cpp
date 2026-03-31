/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <filesystem>
#include <chrono>

#include "bufferqueue.h"
#include "bufferfiller.h"
#include "filewriter.h"
#include "iotesteroptions.h"

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    try {
    IotesterOptions options{argc, argv};

    BufferQueue bufferqueue;
    auto begin_write = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    const unsigned int num_cores = std::thread::hardware_concurrency();
    for(unsigned int i = 0 ; i < num_cores ; ++i)
    {
        threads.push_back(std::thread(BufferFiller(bufferqueue)));
        bufferqueue.post_empty_buffer(std::make_shared<HashedBuffer>());
        bufferqueue.post_empty_buffer(std::make_shared<HashedBuffer>());
    }

    std::thread writer_thread(FileWriter(
        bufferqueue,
        options.test_directory()));

    for(auto& thread : threads)
        thread.join();
    writer_thread.join();

    auto end_write = std::chrono::high_resolution_clock::now();
    std::cout << "Done writing test files after " << end_write - begin_write << std::endl;

    fs::path working_dir = options.test_directory() / "iotest";
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

    auto end_read = std::chrono::high_resolution_clock::now();
    std::cout << "Done reading files after " << end_read - end_write << std::endl;
    std::cout << checksum_failures << " checksum failures after reading data back" << std::endl;

    return checksum_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch(std::runtime_error &e) {
        std::cerr << "iotester: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
