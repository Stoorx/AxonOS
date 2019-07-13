//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <Model/FsManager.hpp>
#include <Model/PartitionTableManager.hpp>
#include "Fat32FatCache.hpp"


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
    explicit Fat32FsManager(Context& context, uint32_t partitionNumber);
    
    bool CheckFsSupport(const std::string& fsName) override;
    
    void FormatPartition(
        Context& context,
        uint32_t number,
        const FsFormatPartitionParameters& params
    ) override;
    
    uint64_t GetFatFirstSector() const;
    
    uint64_t GetFatOffset() const;
    
    uint64_t GetFirstDataSector() const;
    
    uint64_t GetFirstSectorOfCluster(uint32_t clusterNumber);

protected:
    Fat32BiosParametersBlock       FatHeaderCache; // TODO: Make a new type for Header parameters
    std::unique_ptr<Fat32FatCache> FatCache = nullptr;
    uint32_t                       PartitionNumber;
    uint64_t                       PartitionOffset;
    uint64_t                       PartitionSize;
};

class Fat32FsFormatPartitionParameters : public FsFormatPartitionParameters {
public:
    Fat32FsFormatPartitionParameters(); // TODO: Make a default constructor
    //TODO: Make an extended constructor
    
    Fat32FsFormatPartitionParameters& setOemName(const std::string& oemName);
    Fat32FsFormatPartitionParameters& setBytesPerSector(uint64_t bytesPerSector);
    Fat32FsFormatPartitionParameters& setSectorsPerCluster(uint8_t sectorsPerCluster);
    Fat32FsFormatPartitionParameters& setReservedSectorsCount(uint16_t reservedSectorsCount);
    Fat32FsFormatPartitionParameters& setNumberOfFats(uint8_t numberOfFats);
    Fat32FsFormatPartitionParameters& setMediaType(uint8_t mediaType);
    Fat32FsFormatPartitionParameters& setHiddenSectorsCount(uint32_t hiddenSectorsCount);
    Fat32FsFormatPartitionParameters& setExtendedFlags(uint16_t extendedFlags);
    Fat32FsFormatPartitionParameters& setRootDirectoryCluster(uint32_t rootDirectoryCluster);
    Fat32FsFormatPartitionParameters& setFsInfoSector(uint16_t fsInfoSector);
    Fat32FsFormatPartitionParameters& setBackupBootsectorSector(uint16_t backupBootsectorSector);
    Fat32FsFormatPartitionParameters& setDriveNumber(uint8_t driveNumber);
    Fat32FsFormatPartitionParameters& setVolumeLabel(const std::string& volumeLabel);
    Fat32FsFormatPartitionParameters& setFilesystemName(const std::string& filesystemName);
protected:
    std::string OemName; // Check the string is not longer than 8 characters
    // BPB 1
    uint64_t    BytesPerSector; // Parameter it is 512 usually
    uint8_t     SectorsPerCluster; // Replace by Bytes per cluster
    uint16_t    ReservedSectorsCount; // Parameter
    uint8_t     NumberOfFats; // Parameter
    uint8_t     MediaType; // Replace by enum
    uint32_t    HiddenSectorsCount; // Take from PartitionManager TODO: add GetPartitionInfo to PartitionManager
    // BPB 2
    uint16_t    ExtendedFlags; // WTF? TODO: Find docs http://elm-chan.org/docs/fat_e.html
    uint32_t    RootDirectoryCluster; // Maybe parameter. Often it is 2
    uint16_t    FsInfoSector; // Maybe parameter. Often it is 1
    uint16_t    BackupBootsectorSector; // Maybe parameter. Often it is 6
    uint8_t     DriveNumber; // Maybe parameter. Usually it is 0x80
    std::string VolumeLabel; // Parameter/ 11 characters
    std::string FilesystemName;// Parameter/ 8 characters/ Default: FAT32
    
};

