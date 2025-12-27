#pragma once

#include <iostream>

class Logs
{
private:
    bool _silent;
    
public:
    Logs();
    Logs(bool silent);
    ~Logs();

    void setSilent(bool s);

    void printError(std::string err) const;
};


