//
// Created by Stoorx on 23.06.2019.
//

#include "NewTableMbrCommand.hpp"
#include <memory.h>

NewTableMbrCommand::NewTableMbrCommand(const std::string& mbrFileName) :
    UseDefaultBootloader(false), MbrFileName(mbrFileName) {
}

void NewTableMbrCommand::Execute(Context& context) {
    char mbr[512];
    if (UseDefaultBootloader) {
        memset(mbr, 0, 512);
    }
    else {
        auto mbrFile = std::ifstream(MbrFileName);
        if (mbrFile.is_open()) { // if file exists
            mbrFile.read(mbr, 512);
        }
        else {
            throw FileNotFoundException(MbrFileName);
        }
    }
    context.DiskImage->writeBuffer(0, mbr, 512);
    context.PartitionType = PartitionTableType::MBR;
}
