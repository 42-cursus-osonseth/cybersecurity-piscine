#include "Parser.hpp"
#include "const.hpp"

Parser::Parser()
{
}

Parser::Parser(int argc, char **argv) 
        : _reverse(false), _silent(false), _versionFlag(false), _helpFlag(false),
          _reverseKey(""), _args({}), _unknonwFlag(""){
    for (int i = 1; i < argc; i++)
        _args.push_back(argv[i]);
}

void Parser::parse()
{
    for (long unsigned int i = 0; i < _args.size(); i++)
    {
        if (_args[i] == "-v" || _args[i] == "--version")
            _versionFlag = true;
        else if (_args[i] == "-h" || _args[i] == "--help")
            _helpFlag = true;
        else if (_args[i] == "-s" || _args[i] == "--silent")
            _silent = true;
        else if (_args[i] == "-r" || _args[i] == "--reverse")
        {
            _reverse = true;
            if (i + 1 < _args.size()){
                _reverseKey = _args[i + 1];
                i++;
            }
            else
                throw std::runtime_error("Key required with the -r option");
        }
        else
            _unknonwFlag = _args[i];
    }
    parseFlags();
}

void Parser::parseFlags() const
{
    if (_helpFlag)
    {
        std::cout << HELP << std::endl;
        std::exit(0);
    }
    else if (_versionFlag)
    {
        std::cout << VERSION << std::endl;
        std::exit(0);
    }
    else if (!_unknonwFlag.empty())
    {
        std::cout << "Error: unknown option" << _unknonwFlag << std::endl;
        std::exit(1);
    }
}

Config Parser::getConfig() const
{
    return Config{_silent, _reverse, _reverseKey};
}

Parser::~Parser()
{
}