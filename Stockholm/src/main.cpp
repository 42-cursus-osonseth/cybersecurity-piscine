#include "FileManager.hpp"
#include "Parser.hpp"
#include "type.hpp"
#include "CryptoManager.hpp"


#pragma GCC diagnostic pop


int main(int argc, char ** argv) {

    try{
        Parser parser(argc, argv);
        parser.parse();
        Config conf = parser.getConfig();
   
        FileManager fileManager;
        std::vector<fs::path> files = fileManager.getFilesToEncrypt();
        cryptoManager cryptoManager(conf, files);
        cryptoManager.init();
    }
    catch (const std::exception &e){
        std::cerr << e.what() <<  std::endl;
        return 1;
    }

    return 0;

}
