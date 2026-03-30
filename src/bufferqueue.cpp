#include "bufferqueue.h"

#include <algorithm>

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
    {
        std::lock_guard<std::mutex> lock(m_filled_buffers_mutex);
        m_filled_buffers.push(buffer);
    }
    m_filled_buffers_condition.notify_one();
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

std::shared_ptr<HashedBuffer> BufferQueue::get_buffer_to_write()
{
    std::shared_ptr<HashedBuffer> out;
    while(!out)
    {
        std::unique_lock<std::mutex> lock(m_filled_buffers_mutex);
        if(m_filled_buffers.size() > 0)
        {
            out = m_filled_buffers.front();
            m_filled_buffers.pop();
        }
        else if(m_num_bufferfillers == 0 && m_max_bufferfillers > 0)
        {
            throw Eof();
        }
        else
        {
            m_filled_buffers_condition.wait(lock);
        }
    }
    return out;
}

void BufferQueue::register_buffer_filler()
{
    std::lock_guard<std::mutex> lock(m_filled_buffers_mutex);
    m_num_bufferfillers++;
    m_max_bufferfillers = std::max(m_max_bufferfillers, m_num_bufferfillers);
}

void BufferQueue::unregister_buffer_filler()
{
    std::lock_guard<std::mutex> lock(m_filled_buffers_mutex);
    m_num_bufferfillers--;
}
