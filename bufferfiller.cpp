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

void BufferFiller::operator()()
{
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
};

void BufferFiller::fill_buffer(std::shared_ptr<HashedBuffer> buffer)
{
    std::uniform_int_distribution<std::uint32_t> distrib;
    for (auto& dword : buffer->buffer())
        dword = distrib(m_prng);

    buffer->calculate_hash();
}
