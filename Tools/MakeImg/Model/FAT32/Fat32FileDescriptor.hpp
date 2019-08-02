//
// Created by Stoorx on 18.07.2019.
//

#pragma once

#include <stdint.h>
#include <cstring>
#include <string>

#pragma pack(push, 1)

struct Fat32FileDescriptor {
    char     FileName[11];
    uint8_t  Attributes;
    uint8_t  NtReserved;
    uint8_t  CreateTimeTenth;
    uint16_t CreateTime;
    uint16_t CreateDate;
    uint16_t LastAccessDate;
    uint16_t FirstClusterHigh;
    uint16_t LastWriteTime;
    uint16_t LastWriteDate;
    uint16_t FirstClusterLow;
    uint32_t FileSize;
    
    enum class Fat32FileAttributes : uint8_t {
        ReadOnly      = 0x01U,
        Hidden        = 0x02U,
        System        = 0x04U,
        VolumeId      = 0x08U,
        Directory     = 0x10U,
        Archive       = 0x20U,
        LongNameEntry = ReadOnly | Hidden | System | VolumeId
    };
    
    uint32_t getFirstCluster() const;
    std::string getFileName() const;
    void setFileName(const std::string& name);
    // TODO: getDate/setDate. Create Date/Timestamp class!
};

#pragma pack(pop)


