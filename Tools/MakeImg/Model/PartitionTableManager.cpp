//
// Created by Stoorx on 27.06.2019.
//

#include "PartitionTableManager.hpp"

void MbrPartitionTableManager::CreatePartitionTable(
    Context& context,
    const CreatePartitionTableParameters& parameters
) {
    auto params = dynamic_cast<const MbrCreatePartitionTableParameters*>(&parameters);
    if (params != nullptr) {
        char mbr[512];
        if (params->UseDefaultBootloader) {
            memset(mbr, 0, 510);
            mbr[510] = 0x55;
            mbr[511] = 0xAA;
        }
        else {
            auto mbrFile = std::ifstream(params->BootsectorFileName);
            if (mbrFile.is_open()) { // if file exists
                mbrFile.read(mbr, 512);
            }
            else {
                throw FileNotFoundException(params->BootsectorFileName);
            }
        }
        context.DiskImage->writeBuffer(0, mbr, 512);
    }
    else throw IllegalStateException("Bad cast");
}

void MbrPartitionTableManager::RegisterPartition(
    Context& context,
    uint32_t number,
    const RegisterPartitionParameters& parameters
) {
    auto params = dynamic_cast<const MbrPartitionParameters*>(&parameters);
    if (params != nullptr) {
        char mbr[512];
        context.DiskImage->readBuffer(0, mbr, 512);
        auto partitionEntry = (MbrPartitionEntry*)(&mbr[MbrPartitionEntry::FirstEntryOffset]) + number;
        
        partitionEntry->Active =
            (params->Active) ? MbrPartitionEntry::ActivePartitionFlag : MbrPartitionEntry::PassivePartitionFlag;
        partitionEntry->StartHead = params->StartHead;
        partitionEntry->StartSectorCylinderHigh = params->StartSectorCylinderHigh;
        partitionEntry->StartCylinderLow = params->StartCylinderLow;
        partitionEntry->PartitionType = params->PartitionType;
        partitionEntry->EndHead = params->EndHead;
        partitionEntry->EndSectorCylinderHigh = params->EndSectorCylinderHigh;
        partitionEntry->EndCylinderLow = params->EndCylinderLow;
        partitionEntry->StartLBA = params->StartLBA;
        partitionEntry->EndLBA = params->EndLBA;
        
        context.DiskImage->writeBuffer(0, mbr, 512);
    }
    else throw IllegalStateException("Bad cast");
}

bool MbrPartitionTableManager::DetectPartitionTable(const Context& context) {
    return false; // TODO: logic
}
