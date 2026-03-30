/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#include "hashedbuffer.h"

#include <cryptopp/sha.h>

std::string HashedBuffer::digest_str() const
{
    std::ostringstream digest_stream;
    for(const auto& c : m_digest)
            digest_stream << std::hex << std::setw(2) << std::setfill('0') << ((uint16_t)c & 0xff);
    return digest_stream.str();
}

void HashedBuffer::calculate_hash()
{
    CryptoPP::SHA256 hash;
    hash.Update((const CryptoPP::byte*) m_bytes.data(), size());
    hash.Final((CryptoPP::byte*) &m_digest[0]);
}
