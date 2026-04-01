/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#pragma once

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class IotesterOptions
{
public:
    IotesterOptions(int argc, char **argv);
    ~IotesterOptions() {}

    void print_usage();

    fs::path test_directory() const {return m_test_directory;}
    bool write_files() const {return m_write_files;}
    bool check_files() const {return m_check_files;}

private:
    fs::path m_test_directory;
    bool m_write_files = true;
    bool m_check_files = true;
};
