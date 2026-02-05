#pragma once

#include <memory>
#include <stdexcept>
#include <queue>
#include <thread>
#include <condition_variable>

#include "hashedbuffer.h"

class BufferQueue {
public:
    BufferQueue() {};
    ~BufferQueue() {};

    class Eof : public std::exception {};

    std::shared_ptr<HashedBuffer> get_buffer_to_fill();
    void post_filled_buffer(std::shared_ptr<HashedBuffer> buffer);

    void post_empty_buffer(std::shared_ptr<HashedBuffer> buffer);
    void set_end_of_empty_buffers();

private:
    bool m_end_of_empty_buffers = false; // True if we do not expect any more empty buffers to be posted
    std::queue<std::shared_ptr<HashedBuffer>> m_filled_buffers;
    std::mutex m_filled_buffers_mutex;

    std::queue<std::shared_ptr<HashedBuffer>> m_empty_buffers;
    std::mutex m_empty_buffers_mutex;
    std::condition_variable m_empty_buffers_condition;
};
