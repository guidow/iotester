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
#include "formatting.h"

namespace fs = std::filesystem;

void write_test_files(const IotesterOptions &options) {
    auto begin_write = std::chrono::high_resolution_clock::now();
    BufferQueue bufferqueue;
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
    std::cout << "Done writing test files after ";
    format_time(std::cout, end_write - begin_write) << std::endl;
}

struct CheckFilesResult
{
    std::size_t files_checked = 0;
    std::size_t read_errors = 0;
    std::size_t checksum_errors = 0;
};

CheckFilesResult check_files(const IotesterOptions &options) {
    auto start_read = std::chrono::high_resolution_clock::now();
    fs::path working_dir = options.test_directory() / "iotest";
    CheckFilesResult out;
    for(auto const& dentry : fs::directory_iterator{working_dir})
    {
        HashedBuffer filecontent;
        std::ifstream file(dentry.path());
        if(!file.good())
        {
            std::cerr << "Could not open " << dentry.path() << " for reading" << std::endl;
            out.read_errors++;
            continue;
        }
        file.read(filecontent.data(), filecontent.size());
        if(!file.good())
        {
            out.read_errors++;
            continue;
        }
        filecontent.calculate_hash();
        if(dentry.path().filename() != filecontent.digest_str())
        {
            std::cerr << "Wrong SHA-256 sum for " << dentry.path() << ": " << filecontent.digest_str() << std::endl;
            out.checksum_errors++;
        }
        else
        {
            fs::remove(dentry.path());
        }
    }

    auto end_read = std::chrono::high_resolution_clock::now();
    std::cout << "Done reading files after ";
    format_time(std::cout, end_read - start_read) << std::endl;

    return out;
}

int main(int argc, char **argv) {
    try {
    IotesterOptions options{argc, argv};

    if(options.write_files())
    {
        write_test_files(options);
    }

    if(options.check_files())
    {
        CheckFilesResult check_files_result = check_files(options);
        std::cout << check_files_result.checksum_errors << " checksum failures and " << check_files_result.read_errors << " read errors after reading data back" << std::endl;

        if(check_files_result.checksum_errors == 0 && check_files_result.read_errors == 0) {
            return EXIT_SUCCESS;
        }
        else {
            return EXIT_FAILURE;
        }
    }

    }
    catch(std::runtime_error &e)
    {
        std::cerr << "iotester: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
