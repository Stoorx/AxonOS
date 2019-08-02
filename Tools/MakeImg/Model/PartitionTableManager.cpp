//
// Created by Stoorx on 27.06.2019.
//

#include "PartitionTableManager.hpp"
#include <Exceptions/IncorrectParameterException.hpp>

std::map<PartitionType, uint8_t> MbrPartitionTableManager::PartitionTypeMapping = {
    {PartitionType::Unallocated, 0x00},
    {PartitionType::Invalid, 0xFF},
    {PartitionType::Fat32Lba, 0x0C}
};

void MbrPartitionTableManager::CreatePartitionTable(
    Context& context,
    const CreatePartitionTableParameters& parameters
) {
    auto params = dynamic_cast<const MbrCreatePartitionTableParameters*>(&parameters);
    if (params != nullptr) {
        uint8_t mbr[512];
        if (params->UseDefaultBootloader) {
            memset(mbr, 0, 510);
            mbr[510] = 0x55;
            mbr[511] = 0xAA;
        }
        else {
            auto mbrFile = std::ifstream(params->BootsectorFileName);
            if (mbrFile.is_open()) { // if file exists
                mbrFile.read((char*)mbr, 512);
            }
            else {
                throw FileNotFoundException(params->BootsectorFileName);
            }
        }
        context.CurrentDiskImage->writeBuffer(0, mbr, 512);
    }
    else throw IllegalStateException("Bad cast");
}

void MbrPartitionTableManager::RegisterPartition(
    Context& context,
    uint32_t number,
    const RegisterPartitionParameters& parameters
) {
    auto params = dynamic_cast<const MbrRegisterPartitionParameters*>(&parameters);
    if (params != nullptr) {
        uint8_t mbr[512];
        context.CurrentDiskImage->readBuffer(0, mbr, 512);
        auto partitionEntry = (MbrPartitionEntry*)(&mbr[MbrPartitionEntry::FirstEntryOffset]) + number;
        
        partitionEntry->Active =
            (params->Active) ? MbrPartitionEntry::ActivePartitionFlag : MbrPartitionEntry::PassivePartitionFlag;
        partitionEntry->StartHead               = params->StartHead;
        partitionEntry->StartSectorCylinderHigh = params->StartSectorCylinderHigh;
        partitionEntry->StartCylinderLow        = params->StartCylinderLow;
        partitionEntry->PartitionType           = params->PartitionType;
        partitionEntry->EndHead                 = params->EndHead;
        partitionEntry->EndSectorCylinderHigh   = params->EndSectorCylinderHigh;
        partitionEntry->EndCylinderLow          = params->EndCylinderLow;
        partitionEntry->StartLBA                = params->StartLBA;
        partitionEntry->SizeLBA                 = params->EndLBA - params->StartLBA + 1;
    
        context.CurrentDiskImage->writeBuffer(0, mbr, 512);
    }
    else throw IllegalStateException("Bad cast");
}

bool MbrPartitionTableManager::DetectPartitionTable(const Context& context) {
    uint8_t mbr[512];
    context.CurrentDiskImage->readBuffer(0, mbr, 512);
    return mbr[510] == 0x55 && mbr[511] == 0xAA;
}

PartitionTableType MbrPartitionTableManager::GetPartitionTableType() const {
    return PartitionTableType::MBR;
}

uint64_t MbrPartitionTableManager::GetPartitionOffset(const Context& context, uint32_t partitionNumber) const {
    uint8_t mbr[512];
    context.CurrentDiskImage->readBuffer(0, mbr, 512);
    auto partitionEntry = (MbrPartitionEntry*)(&mbr[MbrPartitionEntry::FirstEntryOffset]) + partitionNumber;
    return partitionEntry->StartLBA;
}

uint64_t MbrPartitionTableManager::GetPartitionSize(const Context& context, uint32_t partitionNumber) const {
    uint8_t mbr[512];
    context.CurrentDiskImage->readBuffer(0, mbr, 512);
    auto partitionEntry = (MbrPartitionEntry*)(&mbr[MbrPartitionEntry::FirstEntryOffset]) + partitionNumber;
    return partitionEntry->SizeLBA;
}

void MbrPartitionTableManager::setPartitionType(
    Context& context,
    uint32_t partitionNumber,
    PartitionType partitionType
) {
    uint8_t mbr[512];
    context.CurrentDiskImage->readBuffer(0, mbr, 512);
    auto partitionEntry = (MbrPartitionEntry*)(&mbr[MbrPartitionEntry::FirstEntryOffset]) + partitionNumber;
    
    auto mappingIt = PartitionTypeMapping.find(partitionType);
    if (mappingIt != PartitionTypeMapping.end()) {
        partitionEntry->PartitionType = mappingIt->second;
        context.CurrentDiskImage->writeBuffer(0, mbr, 512);
    }
    else {
        throw IncorrectParameterException(
            "Partition type",
            std::to_string((uint32_t)partitionType),
            "Unsupported partition type"
        );
    }
}
//TODO: Refactor: make a cache for partitions entries to avoid reading from the disk
//TODO: Refactor: think about adding the Context in the manager as a field

void LoopPartitionManager::RegisterPartition(
    Context& context,
    uint32_t number,
    const RegisterPartitionParameters& parameters
) {
    // do nothing
}

void LoopPartitionManager::CreatePartitionTable(Context& context, const CreatePartitionTableParameters& parameters) {
    // do nothing
}

bool LoopPartitionManager::DetectPartitionTable(const Context& context) {
    return true;
}

PartitionTableType LoopPartitionManager::GetPartitionTableType() const {
    return PartitionTableType::Loop;
}

uint64_t LoopPartitionManager::GetPartitionOffset(const Context& context, uint32_t partitionNumber) const {
    return 0;
}

uint64_t LoopPartitionManager::GetPartitionSize(const Context& context, uint32_t partitionNumber) const {
    return context.CurrentDiskImage->getSize() / 512; // TODO: 512 IS BAD! Refactor for different sector sizes
}

void LoopPartitionManager::setPartitionType(Context& context, uint32_t partitionNumber, PartitionType partitionType) {
    // do nothing
}
