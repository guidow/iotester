/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#include "filewriter.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

void FileWriter::operator()()
{
    if(!fs::exists(m_path))
        throw std::runtime_error("Path " + m_path.string() + " does not exist");

    fs::path working_dir = m_path / "iotest";
    std::error_code error;
    if(!fs::create_directory(working_dir, error) && error)
        throw std::runtime_error("Could not create working directory " + working_dir.string() + ": " + error.message());

    std::cout << "Writing to path " << working_dir << std::endl;

    std::size_t buffers_submitted = 32 *2;

    try
    {
        while(true)
        {
            auto buffer = m_queue.get_buffer_to_write();
            fs::path outpath = working_dir / buffer->digest_str();
            std::ofstream outfile(outpath, std::ios::out | std::ios::binary);
            if(!outfile.good())
                throw std::runtime_error("Could not open " + outpath.string());
            outfile.write(buffer->data(), buffer->size());
            if(buffers_submitted < 10000)
            {
                m_queue.post_empty_buffer(buffer);
                buffers_submitted++;
            }
            else
            {
                m_queue.set_end_of_empty_buffers();
            }
        }
    }
    catch (BufferQueue::Eof& eof)
    {}
    catch(std::runtime_error &exception)
    {
        std::cerr << "Caught exception in FileWriter: " << exception.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Caught unknown exception in FileWriter" << std::endl;
    }
}
