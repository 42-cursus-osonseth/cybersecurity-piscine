#pragma once
#include <string>
#include "type.hpp"
#include <vector>
#include <iostream>


class Parser
{
private:
    bool _reverse;
    bool _silent;
    bool _versionFlag;
    bool _helpFlag;
    std::string _reverseKey;
    std::vector<std::string> _args;
    std::string _unknonwFlag;
    

public:
    Parser();
    Parser(int argc, char ** argv);
    ~Parser();

    void parse();
    void parseFlags() const;
    Config getConfig() const;
};

