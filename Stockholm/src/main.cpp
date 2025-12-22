#include "FileManager.hpp"
#include "Parser.hpp"
#include "type.hpp"

#pragma GCC diagnostic pop


int main(int argc, char ** argv) {

    try{
        Parser ps(argc, argv);
        ps.parse();
        Config cnf = ps.getConfig();
        FileManager fm;
        std::vector<fs::path> files = fm.getFilesToEncrypt();
        for (const auto& f : files){
            std::cout << f.string() << std::endl;
        }
        
    }
    catch (const std::exception &e){
        std::cerr << e.what() <<  std::endl;
        return 1;
    }

    return 0;

   
}
