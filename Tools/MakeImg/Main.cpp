//
// Created by Stoorx on 19.06.2019.
//

#include <string>
#include <vector>
#include <MakeImg.hpp>
#include <ExitProcessException.hpp>

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
    return 0;
}