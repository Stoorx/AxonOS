//
// Created by Stoorx on 23.06.2019.
//

#include "NewTableMbrCommand.hpp"

NewTableMbrCommand::NewTableMbrCommand(const std::string& mbrFileName) :
    UseDefaultBootloader(false), MbrFileName(mbrFileName) {
}

void NewTableMbrCommand::Execute(Context& context) {
    auto mbrFile = std::ifstream(MbrFileName);
    if(mbrFile.is_open()){ // if file exists
        char mbr[512];
        mbrFile.read(mbr, 512);
        context.DiskImage->writeBuffer(0, mbr, 512);
    }else{
        throw FileNotFoundException(MbrFileName);
    }
}
