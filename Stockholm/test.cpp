#include "FileManager.hpp"



int main() {

    try{
        FileManager fm;
    }
    catch (const std::exception &e){
        std::cerr << e.what() <<  std::endl;
    }
   
}
