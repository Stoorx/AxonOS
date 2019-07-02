//
// Created by Stoorx on 23.06.2019.
//

#include "NewTableMbrCommand.hpp"
#include <Model/PartitionTableManager.hpp>
#include <memory.h>

NewTableMbrCommand::NewTableMbrCommand(const std::string& mbrFileName) :
    UseDefaultBootloader(false), MbrFileName(mbrFileName) {
}

void NewTableMbrCommand::Execute(Context& context) {
    char mbr[512];
    if (UseDefaultBootloader) {
        memset(mbr, 0, 510);
        mbr[510] = 0x55;
        mbr[511] = 0xAA;
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
    context.PartitionManager = std::shared_ptr<PartitionTableManager>(new MbrPartitionTableManager());
}
