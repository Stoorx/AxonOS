//
// Created by Stoorx on 27.06.2019.
//

#pragma once

#include <stdint.h>
#include "Context.hpp"
#include <Exceptions/IllegalStateException.hpp>
#include <Model/DiskImage.hpp>

enum class PartitionTableType {
    NotSet,
    Invalid,
    MBR,
    GPT
};

class RegisterPartitionParameters {
public:
    virtual ~RegisterPartitionParameters() = default;
};

class MbrPartitionParameters : public RegisterPartitionParameters {
public:
    bool     Active;
    uint8_t  StartHead;
    uint8_t  StartSectorCylinderHigh;
    uint8_t  StartCylinderLow;
    uint8_t  PartitionType;
    uint8_t  EndHead;
    uint8_t  EndSectorCylinderHigh;
    uint8_t  EndCylinderLow;
    uint32_t StartLBA;
    uint32_t EndLBA;
};

struct MbrPartitionEntry {
    static const uint32_t FirstEntryOffset     = 0x1BE;
    static const uint8_t  ActivePartitionFlag  = 0x80;
    static const uint8_t  PassivePartitionFlag = 0x00;
    
    uint8_t  Active;
    uint8_t  StartHead;
    uint8_t  StartSectorCylinderHigh;
    uint8_t  StartCylinderLow;
    uint8_t  PartitionType;
    uint8_t  EndHead;
    uint8_t  EndSectorCylinderHigh;
    uint8_t  EndCylinderLow;
    uint32_t StartLBA;
    uint32_t EndLBA;
};

class PartitionTableManager {
public:
    const PartitionTableType PartitionType = PartitionTableType::NotSet;
    
    virtual void RegisterPartition(
        Context& context,
        uint32_t number,
        const RegisterPartitionParameters& parameters
    ) = 0;
    
    virtual void CreatePartitionTable() = 0; // TODO: Add parameters
    
};

class MbrPartitionTableManager : public PartitionTableManager {
public:
    MbrPartitionTableManager() = default;
    MbrPartitionTableManager(MbrPartitionTableManager&& tm) = default;
    
    void CreatePartitionTable() override { // TODO: Add parameters
        PartitionTableManager::CreatePartitionTable(); // TODO: OVERRIDE
    }
    
    void RegisterPartition(Context& context, uint32_t number, const RegisterPartitionParameters& parameters) override {
        auto params = dynamic_cast<const MbrPartitionParameters*>(&parameters);
        if (params != nullptr) {
            char mbr[512];
            context.DiskImage->readBuffer(0, mbr, 512);
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
            partitionEntry->EndLBA                  = params->EndLBA;
    
            context.DiskImage->writeBuffer(0, mbr, 512);
        }
        else {
            throw IllegalStateException("Bad cast");
        }
    }
};
