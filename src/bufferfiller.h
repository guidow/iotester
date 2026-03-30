/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#pragma once
#include <random>
#include <memory>

#include "hashedbuffer.h"
#include "bufferqueue.h"

class BufferFiller {
public:
    BufferFiller(BufferQueue& bufferqueue);
    BufferFiller(const BufferFiller&& other);
    ~BufferFiller() {};

    void operator()();

    void fill_buffer(std::shared_ptr<HashedBuffer> buffer);

private:
    std::random_device m_random_source;
    std::mt19937 m_prng;
    BufferQueue& m_queue;
};

