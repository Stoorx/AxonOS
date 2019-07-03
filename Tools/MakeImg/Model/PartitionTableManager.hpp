//
// Created by Stoorx on 27.06.2019.
//

#pragma once

#include <stdint.h>
#include "Context.hpp"
#include <Exceptions/IllegalStateException.hpp>
#include <Model/DiskImage.hpp>
#include <memory.h>
#include <Exceptions/FileNotFoundException.hpp>

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

class CreatePartitionTableParameters {
public:
    virtual ~CreatePartitionTableParameters() = default;
};

class MbrCreatePartitionTableParameters : public CreatePartitionTableParameters {
public:
    std::string BootsectorFileName;
    bool        UseDefaultBootloader;
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
        Context& context, uint32_t number, const RegisterPartitionParameters& parameters
    ) = 0;
    
    virtual void CreatePartitionTable(Context& context, const CreatePartitionTableParameters& parameters) = 0;
    
    virtual bool DetectPartitionTable(const Context& context) = 0;
};

class MbrPartitionTableManager : public PartitionTableManager {
public:
    MbrPartitionTableManager() = default;
    MbrPartitionTableManager(MbrPartitionTableManager&& tm) = default;
    
    bool DetectPartitionTable(const Context& context) override;
    
    void CreatePartitionTable(Context& context, const CreatePartitionTableParameters& parameters) override;
    
    void RegisterPartition(Context& context, uint32_t number, const RegisterPartitionParameters& parameters) override;
};
