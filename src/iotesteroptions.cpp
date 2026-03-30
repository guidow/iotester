/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#include "iotesteroptions.h"

IotesterOptions::IotesterOptions(int argc, char **argv)
{
    if(argc < 2) {
        print_usage();
        throw std::runtime_error("No test directory specified");
    }
    if(argc > 100) {
        throw std::runtime_error("Implausibly large number of command line arguments");
    }

    m_test_directory = argv[1];
}

void IotesterOptions::print_usage()
{
    std::cerr << "Usage:\n";
    std::cerr << "iotester <directory>" << std::endl;
}
