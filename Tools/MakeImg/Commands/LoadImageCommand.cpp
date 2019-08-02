#include <memory>

//
// Created by Stoorx on 23.06.2019.
//

#include "LoadImageCommand.hpp"
#include <Exceptions/IllegalStateException.hpp>
#include <fstream>

void LoadImageCommand::Execute(Context& context) {
    if (context.CurrentDiskImage != nullptr) {
        throw IllegalStateException("Attempt of creating new image while active image exists");
    }
    if(std::ifstream(FileName)){
        context.CurrentDiskImage = std::make_shared<DiskImage>(FileName);
    }else{
        throw FileNotFoundException(FileName);
    }
}

LoadImageCommand::LoadImageCommand(const std::string& fileName) : Command(), FileName(fileName){}
