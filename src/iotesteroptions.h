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

    fs::path test_directory() {return m_test_directory;}

private:
    fs::path m_test_directory;
};
