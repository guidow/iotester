/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#include "iotesteroptions.h"

#include <vector>

#include "version.h"

IotesterOptions::IotesterOptions(int argc, char **argv)
{
    if(argc < 2) {
        print_usage();
        throw std::runtime_error("No test directory specified");
    }
    if(argc > 100) {
        throw std::runtime_error("Implausibly large number of command line arguments");
    }

    for(int i = 1 ; i < argc ; ++i) {
        std::string argument = argv[i];
        std::vector<char> short_args;

        if(argument.length() > 0 && argument[0] != '-') {
            m_test_directory = argument; }
        else {
            if(argument.length() > 1 && argument[0] == '-' && argument[1] != '-') { // If current arg is short args
                for(auto i = argument.cbegin() + 1 ; i != argument.end() ; ++i) {
                    short_args.push_back(*i);
                }
            }
            else {
                short_args.push_back('-');
            }
            for(const auto short_arg : short_args) {
                if(short_arg == 'V' || argument == "--version") {
                    std::cout << "Iotester version " << IOTESTER_VERSION << std::endl;
                    exit(EXIT_SUCCESS);
                }
                else if(argument == "--no-write") {
                    m_write_files = false;
                }
                else if(argument == "--no-check") {
                    m_check_files = false;
                }
                else {
                    if(short_arg == '-') {
                        throw std::runtime_error("Unrecognized argument: " + argument);
                    }
                    else {
                        throw std::runtime_error(std::string("Unrecognized short argument: ") + short_arg);
                    }
                }
            }
        }
    }
}

void IotesterOptions::print_usage()
{
    std::cerr << "Usage:\n";
    std::cerr << "iotester [options] <directory>\n\n";
    std::cerr << "Options:\n";
    std::cerr << "--version|-v:\t\tPrint version and exit\n";
    std::cerr << "--no-write:\t\tDo not write any test files\n";
    std::cerr << "--no-check:\t\tDo not check or delete any test files" << std::endl;
}
