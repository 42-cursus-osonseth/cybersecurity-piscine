#include "FileManager.hpp"
#include "Parser.hpp"
#include "type.hpp"
#include "CryptoManager.hpp"
#include "Logs.hpp"

#pragma GCC diagnostic pop

int main(int argc, char **argv)
{
    Logs logs;
    Config conf;
 
    try{
        Parser parser(argc, argv);
        parser.parse();
        conf = parser.getConfig();
        logs.setSilent(conf.silent);
    }
    catch (const std::exception &e){
        std::cerr << e.what() << std::endl;
        return 1;
    }
    try{

        FileManager fileManager;
        std::vector<fs::path> files = fileManager.getFilesToEncrypt();
        cryptoManager crypto(conf, files);
        crypto.init();
    }
    catch (const std::exception &e){
        logs.printError(e.what());
        return 1;
    }
    return 0;
}
