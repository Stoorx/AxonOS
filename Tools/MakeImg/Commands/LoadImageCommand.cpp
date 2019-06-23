//
// Created by Stoorx on 23.06.2019.
//

#include "LoadImageCommand.hpp"
#include <Exceptions/IllegalStateException.hpp>

void LoadImageCommand::Execute(Context& context) {
    if (context.DiskImage != nullptr) {
        throw IllegalStateException("Attempt of creating new image while active image exists");
    }
    if(std::filesystem::exists(FileName)){
        context.DiskImage = new DiskImage(FileName);
    }else{
        throw FileNotFoundException(FileName);
    }
}

LoadImageCommand::LoadImageCommand(const std::string& fileName) : Command(), FileName(fileName){}
