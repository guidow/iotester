#include "bufferqueue.h"

std::shared_ptr<HashedBuffer> BufferQueue::get_buffer_to_fill()
{
    std::shared_ptr<HashedBuffer> out;
    while(!out)
    {
        std::unique_lock<std::mutex> lock(m_empty_buffers_mutex);
        if(m_empty_buffers.size() > 0)
        {
            out = m_empty_buffers.front();
            m_empty_buffers.pop();
        }
        else if(m_end_of_empty_buffers)
        {
            throw Eof();
        }
        else
        {
            m_empty_buffers_condition.wait(lock);
        }
    }
    return out;
}

void BufferQueue::post_filled_buffer(std::shared_ptr<HashedBuffer> buffer)
{
    std::lock_guard<std::mutex> lock(m_filled_buffers_mutex);
    m_filled_buffers.push(buffer);
}

void BufferQueue::post_empty_buffer(std::shared_ptr<HashedBuffer> buffer)
{
    {
        std::lock_guard<std::mutex> lock(m_empty_buffers_mutex);
        m_empty_buffers.push(buffer);
    }
    m_empty_buffers_condition.notify_one();
}

void BufferQueue::set_end_of_empty_buffers()
{
    m_end_of_empty_buffers = true;
    m_empty_buffers_condition.notify_all();
}
