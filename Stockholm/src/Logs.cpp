#include "Logs.hpp"

Logs::Logs()
{
}
Logs::Logs(bool silent): _silent(silent){

}

Logs::~Logs()
{
}

void Logs::setSilent(bool s){
    _silent = s;
}

void Logs::printError(std::string err) const{
    if (_silent)
        return;
    std::cerr << err << std::endl;
}