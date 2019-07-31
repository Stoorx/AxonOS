//
// Created by Stoorx on 19.06.2019.
//

#include <string>
#include <vector>
#include <MakeImg.hpp>
#include <Exceptions/ExitProcessException.hpp>

int main(int argc, char** argv){
    std::vector<std::string> arguments;
    for(int i = 1; i < argc; i++){
        arguments.emplace_back(argv[i]);
    }
    try {
        MakeImg::Main(arguments);
    }
    catch (ExitProcessException& e){
        return e.ExitCode;
    }
    catch (std::exception& e){
        std::cerr << std::string("Uncatched exception: ") + e.what();
        return -1;
    }
    return 0;
}