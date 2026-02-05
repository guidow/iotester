#include "bufferqueue.h"

std::shared_ptr<HashedBuffer> BufferQueue::get_buffer_to_fill()
{
    if(m_counter > 1000)
        throw Eof();
    m_counter++;
    return std::make_shared<HashedBuffer>();
}

void BufferQueue::post_filled_buffer(std::shared_ptr<HashedBuffer> buffer)
{
    std::lock_guard<std::mutex> lock(m_filled_buffers_mutex);
    m_filled_buffers.push(buffer);
}
