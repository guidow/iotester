/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#pragma once

#include <cstdint>
#include <array>
#include <sstream>
#include <iomanip>

class HashedBuffer {
public:
    HashedBuffer() {};
    ~HashedBuffer() {};

    static const std::size_t sha256_digest_size = 32;
    typedef std::array<uint8_t, sha256_digest_size> digestbuffertype;
    typedef std::array<uint32_t, 256 * 1024> buffertype;

    std::size_t size() const {return m_bytes.size() * sizeof(uint32_t);}
    char* data() {return (char *)m_bytes.data();}

    void calculate_hash();
    buffertype& buffer() {return m_bytes;}
    digestbuffertype& digest_buffer() {return m_digest;}
    std::string digest_str() const;

private:
    buffertype m_bytes;
    digestbuffertype m_digest;
};
