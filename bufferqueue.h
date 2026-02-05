#pragma once
#include <memory>
#include <stdexcept>
#include <queue>
#include <thread>

#include "hashedbuffer.h"

class BufferQueue {
public:
    BufferQueue() {};
    ~BufferQueue() {};

    class Eof : public std::exception {};

    std::shared_ptr<HashedBuffer> get_buffer_to_fill();
    void post_filled_buffer(std::shared_ptr<HashedBuffer> buffer);

private:
    std::size_t m_counter = 0;
    std::queue<std::shared_ptr<HashedBuffer>> m_filled_buffers;
    std::mutex m_filled_buffers_mutex;
};
