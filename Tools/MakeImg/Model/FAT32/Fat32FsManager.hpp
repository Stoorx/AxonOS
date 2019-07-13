//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <Model/FsManager.hpp>
#include "Fat32FatCache.hpp"
#include <Model/PartitionTableManager.hpp>

#pragma pack(push, 1)
struct Fat32BiosParametersBlock {
    uint8_t  OemName[8];
    // BPB 1
    uint16_t BytesPerSector;
    uint8_t  SectorsPerCluster;
    uint16_t ReservedSectorsCount;
    uint8_t  NumberOfFats;
    uint16_t RootEntriesCount;
    uint16_t TotalSectors16;
    uint8_t  MediaType;
    uint16_t TableSize_16;
    uint16_t SectorsPerTrack;
    uint16_t HeadSideCount;
    uint32_t HiddenSectorsCount;
    uint32_t TotalSectors_32;
    // BPB 2
    uint32_t TableSize_32;
    uint16_t ExtendedFlags;
    uint16_t FatVersion;
    uint32_t RootDirectoryCluster;
    uint16_t FsInfoSector;
    uint16_t BackupBootsectorSector;
    uint8_t  _reserved_0[12];
    uint8_t  DriveNumber;
    uint8_t  reserved_1;
    uint8_t  ExtendedBootSignature;
    uint32_t VolumeSerialNumber;
    uint8_t  VolumeLabel[11];
    uint8_t  FilesystemName[8];
};
#pragma pack(pop)


class Fat32FsManager : public FsManager {
public:
    explicit Fat32FsManager(Context& context, uint32_t partitionNumber) :
        PartitionNumber(partitionNumber),
        FatHeaderCache(),
        PartitionOffset(context.PartitionManager->GetPartitionOffset(context, partitionNumber)),
        PartitionSize(context.PartitionManager->GetPartitionSize(context, partitionNumber)) {
        
    }
    
    bool CheckFsSupport(const std::string& fsName) override {
        auto fsNameLowercase = fsName;
        std::transform(
            fsName.begin(),
            fsName.end(),
            fsNameLowercase.begin(),
            [](char c) {
                return std::tolower(c);
            }
        );
        return fsNameLowercase == "fat32";
    }
    
    void FormatPartition(
        Context& context,
        uint32_t number,
        const FsFormatPartitionParameters& params
    ) override {
        // TODO: initialize FatCache
    }
    
    uint64_t GetFatFirstSector() const {
        return FatHeaderCache.ReservedSectorsCount + FatHeaderCache.HiddenSectorsCount;
    }
    
    uint64_t GetFatOffset() const {
        return GetFatFirstSector() * FatHeaderCache.BytesPerSector;
    }
    
    uint64_t GetFirstDataSector() const {
        return GetFatFirstSector() + FatHeaderCache.NumberOfFats * FatHeaderCache.TableSize_32;
    }
    
    uint64_t GetFirstSectorOfCluster(uint32_t clusterNumber) {
        //FirstSectorofCluster = DataStartSector + (N - 2) * BPB_SecPerClus;
        return GetFirstDataSector() + (clusterNumber - 2) * FatHeaderCache.SectorsPerCluster;
    }

protected:
    Fat32BiosParametersBlock       FatHeaderCache; // TODO: Make a new type for Header parameters
    std::unique_ptr<Fat32FatCache> FatCache = nullptr;
    uint32_t                       PartitionNumber;
    uint64_t                       PartitionOffset;
    uint64_t                       PartitionSize;
};

class Fat32FsFormatPartitionParameters : public FsFormatPartitionParameters {
protected:
    std::string OemName; // Check the string is not longer than 8 characters 
    // BPB 1
    uint64_t    BytesPerSector; // Parameter
    uint8_t     SectorsPerCluster; // Replace by Bytes per cluster
    uint16_t    ReservedSectorsCount; // Parameter
    uint8_t     NumberOfFats; // Parameter
    uint16_t    RootEntriesCount; // Not parameter fills automatically
    uint16_t    TotalSectors16; // Not parameter fills automatically
    uint8_t     MediaType; // Replace by enum
    uint16_t    TableSize_16; // Not parameter fills automatically
    uint16_t    SectorsPerTrack; // CHS support !
    uint16_t    HeadSideCount; // CHS support!
    uint32_t    HiddenSectorsCount; // Take from PartitionManager/ TODO: add GetPartitionInfo to PartitionManager
    uint32_t    TotalSectors_32; // Calculate from partition info
    // BPB 2
    uint32_t    TableSize_32; // Calculate
    uint16_t    ExtendedFlags; // WTF? TODO: Find docs http://elm-chan.org/docs/fat_e.html
    uint16_t    FatVersion; // Find docs
    uint32_t    RootDirectoryCluster; // Maybe parameter. Often it is 2
    uint16_t    FsInfoSector; // Maybe parameter. Often it is 1
    uint16_t    BackupBootsectorSector; // Maybe parameter. Often it is 6
    uint8_t     _reserved_0[12];
    uint8_t     DriveNumber; // Maybe parameter. Usually it is 0x80
    uint8_t     reserved_1;
    uint8_t     ExtendedBootSignature; // 0x29. Not a parameter
    uint32_t    VolumeSerialNumber; // Not a parameter. Calculated from time and data
    uint8_t     VolumeLabel[11]; // Parameter
    uint8_t     FilesystemName[8];// Parameter
};

