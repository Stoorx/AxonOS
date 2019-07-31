//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <Model/FsManager.hpp>
#include <Model/PartitionTableManager.hpp>
#include "Fat32FatCache.hpp"
#include "Fat32FileDescriptor.hpp"
#include <Model/File.hpp>

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
    
    static bool CheckFsSupport(const std::string& fsName);
    
    void FormatPartition(const FsFormatPartitionParameters& params) override;
    
    uint64_t GetFatFirstSector() const;
    
    uint64_t GetFatOffset() const;
    
    uint64_t GetFirstDataSector() const;
    
    uint64_t GetFirstSectorOfCluster(uint32_t clusterNumber);
    virtual std::shared_ptr<File> openFile(const std::string& path, OpenFileAttributes attributes) override;
    virtual void deleteFile(const std::string& path) override;
    virtual std::shared_ptr<File> createFile(const std::string& path, CreateFileAttributes attributes) override;
    
    Fat32FileDescriptor getFileDescriptor(const std::string& path); // TODO: implement
protected:
    Fat32BiosParametersBlock       FatHeaderCache; // TODO: Make a new type for Header parameters
    std::unique_ptr<Fat32FatCache> FatCache = nullptr;
    uint32_t                       PartitionNumber;
    uint64_t                       PartitionOffset;
    uint64_t                       PartitionSize;
private:
    uint32_t calculateFatTableSize(
        uint32_t totalSectors,
        uint8_t sectorsPerCluster,
        uint16_t reservedSectors,
        uint8_t numberOfFats
    ) const;
};

class Fat32FsFormatPartitionParameters : public FsFormatPartitionParameters {
public:
    Fat32FsFormatPartitionParameters();
    //TODO: Make an extended constructor
    
    Fat32FsFormatPartitionParameters& setOemName(const std::string& oemName);
    Fat32FsFormatPartitionParameters& setBytesPerSector(uint64_t bytesPerSector);
    Fat32FsFormatPartitionParameters& setSectorsPerCluster(uint8_t sectorsPerCluster);
    Fat32FsFormatPartitionParameters& setReservedSectorsCount(uint16_t reservedSectorsCount);
    Fat32FsFormatPartitionParameters& setNumberOfFats(uint8_t numberOfFats);
    Fat32FsFormatPartitionParameters& setMediaType(uint8_t mediaType);
    Fat32FsFormatPartitionParameters& setExtendedFlags(uint16_t extendedFlags);
    Fat32FsFormatPartitionParameters& setRootDirectoryCluster(uint32_t rootDirectoryCluster);
    Fat32FsFormatPartitionParameters& setFsInfoSector(uint16_t fsInfoSector);
    Fat32FsFormatPartitionParameters& setBackupBootsectorSector(uint16_t backupBootsectorSector);
    Fat32FsFormatPartitionParameters& setDriveNumber(uint8_t driveNumber);
    Fat32FsFormatPartitionParameters& setVolumeLabel(const std::string& volumeLabel);
    Fat32FsFormatPartitionParameters& setBootsectorFileName(const std::string& bootsectorFileName);
    
    const std::string& getOemName() const;
    uint64_t getBytesPerSector() const;
    uint8_t getSectorsPerCluster() const;
    uint16_t getReservedSectorsCount() const;
    uint8_t getNumberOfFats() const;
    uint8_t getMediaType() const;
    uint16_t getExtendedFlags() const;
    uint32_t getRootDirectoryCluster() const;
    uint16_t getFsInfoSector() const;
    uint16_t getBackupBootsectorSector() const;
    uint8_t getDriveNumber() const;
    const std::string& getVolumeLabel() const;
    const std::string& getFilesystemName() const;
    const std::string& getBootsectorFileName() const;
protected:
    std::string OemName; // Check the string is not longer than 8 characters
    // BPB 1
    uint64_t    BytesPerSector; // Parameter it is 512 usually
    uint8_t     SectorsPerCluster; // Replace by Bytes per cluster
    uint16_t    ReservedSectorsCount; // Parameter
    uint8_t     NumberOfFats; // Parameter
    uint8_t     MediaType; // Replace by enum
    // BPB 2
    uint16_t    ExtendedFlags; // WTF? TODO: Find docs http://elm-chan.org/docs/fat_e.html
    uint32_t    RootDirectoryCluster; // Maybe parameter. Often it is 2
    uint16_t    FsInfoSector; // Maybe parameter. Often it is 1
    uint16_t    BackupBootsectorSector; // Maybe parameter. Often it is 6
    uint8_t     DriveNumber; // Maybe parameter. Usually it is 0x80
    std::string VolumeLabel; // Parameter/ 11 characters
    std::string FilesystemName;// Parameter/ 8 characters/ Default: FAT32
    std::string BootsectorFileName;
};

struct Fat32FsInfoBlock {
public:
    Fat32FsInfoBlock() :
        LeadSig(0x4161'5252),
        _reserved0(),
        StructSig(0x6141'7272),
        FreeCount(0xFFFF'FFFF),
        NextFree(0xFFFF'FFFF),
        _reserved1(),
        TrailSig(0xAA55'0000) {
        for (uint8_t& i : _reserved0) {
            i = 0;
        }
        for (uint8_t& i : _reserved1) {
            i = 0;
        }
    }
    
    uint32_t LeadSig; //0x41615252
    uint8_t  _reserved0[480]; // should be zero
    uint32_t StructSig; // 0x61417272
    uint32_t FreeCount; // Indicates count of free clusters; if it is 0xFFFF'FFFF it is incorrect
    uint32_t NextFree;
    uint8_t  _reserved1[12]; // should be zero
    uint32_t TrailSig; // 0xAA550000
};