#pragma once

#include <string>

inline const std::string VERSION = "Version: 1.0";
inline const std::string HELP = R"(
Usage: stockholm [OPTIONS]

Options:
  -h, --help            Show this help message and exit
  -v, --version         Show program version and exit
  -s, --silent          Suppress all output
  -r, --reverse KEY     Reverse the infection using the given KEY (must be at least 16 characters)
)";

