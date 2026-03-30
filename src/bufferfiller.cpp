/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#include "bufferfiller.h"

#include <iostream>

BufferFiller::BufferFiller(BufferQueue& bufferqueue)
    : m_prng(m_random_source())
    , m_queue(bufferqueue)
    {}

BufferFiller::BufferFiller(const BufferFiller&& other)
    : m_prng(other.m_prng)
    , m_queue(other.m_queue)
    {}

class RegistrationGuard {
public:
    RegistrationGuard(BufferQueue& bq)
    : m_bq(bq)
    {m_bq.register_buffer_filler();}
    ~RegistrationGuard() {m_bq.unregister_buffer_filler();}
private:
    BufferQueue& m_bq;
};

void BufferFiller::operator()()
{
    RegistrationGuard registration(m_queue);
    try {
        while(true)
        {
            auto buffer = m_queue.get_buffer_to_fill();
            fill_buffer(buffer);
            m_queue.post_filled_buffer(buffer);
        }
    }
    catch(BufferQueue::Eof& eof)
    {}
    catch(std::runtime_error &exception)
    {
        std::cerr << "Caught exception in BufferFiller: " << exception.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Caught unknown exception in BufferFiller" << std::endl;
    }
};

void BufferFiller::fill_buffer(std::shared_ptr<HashedBuffer> buffer)
{
    std::uniform_int_distribution<std::uint32_t> distrib;
    for (auto& dword : buffer->buffer())
        dword = distrib(m_prng);

    buffer->calculate_hash();
}
