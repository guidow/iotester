/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#pragma once

#include <filesystem>

#include "bufferqueue.h"

namespace fs = std::filesystem;

class FileWriter {
public:
    FileWriter(BufferQueue& bufferqueue, const fs::path& path)
        : m_queue(bufferqueue)
        , m_path(path)
        {}
    ~FileWriter() {}

    void operator()();
private:
    BufferQueue& m_queue;
    fs::path m_path;
};
