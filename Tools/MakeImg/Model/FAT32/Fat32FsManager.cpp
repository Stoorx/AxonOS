//
// Created by Stoorx on 12.07.2019.
//

#include <time.h>
#include "Fat32FsManager.hpp"
#include "Exceptions/IncorrectParameterException.hpp"
#include "Util/Math.hpp"
#include <cstring>
#include <stdlib.h>

Fat32FsManager::Fat32FsManager(Context& context, uint32_t partitionNumber) :
    FsManager(context),
    PartitionNumber(partitionNumber),
    FatHeaderCache(),
    PartitionOffset(context.PartitionManager->GetPartitionOffset(context, partitionNumber)),
    PartitionSize(context.PartitionManager->GetPartitionSize(context, partitionNumber)) {
    
}

bool Fat32FsManager::CheckFsSupport(const std::string& fsName) {
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

void Fat32FsManager::FormatPartition(const FsFormatPartitionParameters& params) {
    auto partParams = dynamic_cast<const Fat32FsFormatPartitionParameters*>(&params);
    if (partParams != nullptr) {
        ImageContext.PartitionManager->setPartitionType(ImageContext, PartitionNumber, PartitionType::Fat32Lba);
        auto partOffset = this->ImageContext.PartitionManager->GetPartitionOffset(ImageContext, this->PartitionNumber);
        auto partSize   = this->ImageContext.PartitionManager->GetPartitionSize(ImageContext, this->PartitionNumber);
        
        Fat32BiosParametersBlock bpb = {};
        memcpy(bpb.OemName, partParams->getOemName().c_str(), 8);
        bpb.BytesPerSector       = partParams->getBytesPerSector();
        bpb.SectorsPerCluster    = partParams->getSectorsPerCluster();
        bpb.ReservedSectorsCount = partParams->getReservedSectorsCount();
        bpb.NumberOfFats         = partParams->getNumberOfFats();
        bpb.RootEntriesCount     = 0;
        bpb.TotalSectors16       = 0;
        bpb.MediaType            = partParams->getMediaType();
        bpb.TableSize_16         = 0;
        bpb.SectorsPerTrack      = 0;
        bpb.HeadSideCount        = 0;
        bpb.HiddenSectorsCount   = partOffset;
        bpb.TotalSectors_32      = partSize;
        bpb.TableSize_32         = calculateFatTableSize(
            bpb.TotalSectors_32,
            bpb.SectorsPerCluster,
            bpb.ReservedSectorsCount,
            bpb.NumberOfFats
        );
        
        auto clustersCount = (bpb.TotalSectors_32 - bpb.ReservedSectorsCount - bpb.NumberOfFats * bpb.TableSize_32)
                             / bpb.SectorsPerCluster;
        bpb.ExtendedFlags          = partParams->getExtendedFlags();
        bpb.FatVersion             = 0;
        bpb.RootDirectoryCluster   = partParams->getRootDirectoryCluster();
        bpb.FsInfoSector           = partParams->getFsInfoSector();
        bpb.BackupBootsectorSector = partParams->getBackupBootsectorSector();
        bpb.DriveNumber            = partParams->getDriveNumber();
        bpb.reserved_1             = 0;
        bpb.ExtendedBootSignature  = 0x29;
        bpb.VolumeSerialNumber     = time(nullptr); // TODO: Make better SN algorithm;
        memcpy(bpb.VolumeLabel, partParams->getVolumeLabel().c_str(), 11);
        memcpy(bpb.FilesystemName, partParams->getFilesystemName().c_str(), 8);
    
        std::vector<uint8_t> vbr;
        uint64_t             vbrSize = 0;
        std::ifstream        vbrFile;
        if (!partParams->getBootsectorFileName().empty()) {
            vbrFile.open(partParams->getBootsectorFileName(), std::ifstream::binary | std::ifstream::ate);
            if (!vbrFile.good()) {
                throw FileNotFoundException(partParams->getBootsectorFileName());
            }
            vbrSize = vbrFile.tellg();
            vbrFile.seekg(0);
            vbr.resize(vbrSize);
            vbrFile.read((char*)vbr.data(), vbrSize);
        }
        else {
            vbr.resize(512);
            vbrSize = 512;
            memset(vbr.data(), 0, 512);
            //add jump over BPB. Windows checks it.
            vbr[0] = 0xeb; // jmp short
            vbr[1] = 0x00; // [start]
            vbr[2] = 0x90; // nop
            vbr[510] = 0x55;
            vbr[511] = 0xAA;
        }
        this->FatHeaderCache = bpb;
        memcpy(vbr.data() + 3, (void*)&bpb, sizeof(bpb));
        ImageContext.CurrentDiskImage
                    ->writeBuffer(
                        bpb.HiddenSectorsCount * 512,
                        vbr.data(),
                        vbrSize
                    ); // TODO: 512 IS BAD! REFACTOR LATER
        ImageContext.CurrentDiskImage
                    ->writeBuffer((bpb.HiddenSectorsCount + bpb.BackupBootsectorSector) * 512, vbr.data(), vbrSize);
    
        Fat32FsInfoBlock fsInfo;
        fsInfo.NextFree  = 2;
        fsInfo.FreeCount = clustersCount;
        ImageContext.CurrentDiskImage
                    ->writeBuffer((bpb.HiddenSectorsCount + bpb.FsInfoSector) * 512,
                                  (uint8_t*)&fsInfo,
                                  512
                    ); // TODO: 512 IS BAD! REFACTOR LATER
    
        uint32_t fat0Entry = 0x0FFF'FF00U | bpb.MediaType;
        uint32_t fat1Entry = 0x0FFF'FFFF;
        uint32_t eoc       = 0x0FFF'FFF8;
        
        for (uint8_t i = 0; i < FatHeaderCache.NumberOfFats; ++i) {
            ImageContext.CurrentDiskImage
                        ->writeInt(
                            (this->GetFatFirstSector() + FatHeaderCache.TableSize_32 * i) *
                            FatHeaderCache.BytesPerSector +
                            sizeof(uint32_t) * 0, fat0Entry
                        );
            ImageContext.CurrentDiskImage
                        ->writeInt(
                            (this->GetFatFirstSector() + FatHeaderCache.TableSize_32 * i) *
                            FatHeaderCache.BytesPerSector +
                            sizeof(uint32_t) * 1, fat1Entry
                        );
            ImageContext.CurrentDiskImage
                        ->writeInt(
                            (this->GetFatFirstSector() + FatHeaderCache.TableSize_32 * i) *
                            FatHeaderCache.BytesPerSector +
                            sizeof(uint32_t) * 2, eoc
                        );
        }
    
        //TODO: Initialize FAT cache
    }
}

uint64_t Fat32FsManager::GetFatFirstSector() const {
    return FatHeaderCache.ReservedSectorsCount + FatHeaderCache.HiddenSectorsCount;
}

uint64_t Fat32FsManager::GetFatOffset() const {
    return GetFatFirstSector() * FatHeaderCache.BytesPerSector;
}

uint64_t Fat32FsManager::GetFirstDataSector() const {
    return GetFatFirstSector() + FatHeaderCache.NumberOfFats * FatHeaderCache.TableSize_32;
}

uint64_t Fat32FsManager::GetFirstSectorOfCluster(uint32_t clusterNumber) {
    //FirstSectorofCluster = DataStartSector + (N - 2) * BPB_SecPerClus;
    return GetFirstDataSector() + (clusterNumber - 2) * FatHeaderCache.SectorsPerCluster;
}

uint32_t Fat32FsManager::calculateFatTableSize(
    uint32_t totalSectors,
    uint8_t sectorsPerCluster,
    uint16_t reservedSectors,
    uint8_t numberOfFats
) const {
    auto tmpVal1 = totalSectors - reservedSectors;
    auto tmpVal2 = (256 * sectorsPerCluster) + numberOfFats;
    tmpVal2 /= 2;
    return (tmpVal1 + (tmpVal2 - 1)) / tmpVal2;
}

std::shared_ptr<File> Fat32FsManager::openFile(const std::string& path, OpenFileAttributes attributes) {
    return std::shared_ptr<File>(); // TODO: implement
}

void Fat32FsManager::deleteFile(const std::string& path) {
    // TODO: implement
}

std::shared_ptr<File> Fat32FsManager::createFile(const std::string& path, CreateFileAttributes attributes) {
    return std::shared_ptr<File>(); // TODO: implement
}

/*
 * Fat32FsFormatPartitionParameters methods
 */

Fat32FsFormatPartitionParameters::Fat32FsFormatPartitionParameters() {
    this->setOemName("ImgT19.1")
        .setBytesPerSector(512)
        .setSectorsPerCluster(8)
        .setReservedSectorsCount(2048)
        .setNumberOfFats(2)
        .setMediaType(0xF8)
        .setExtendedFlags(0)
        .setRootDirectoryCluster(2)
        .setFsInfoSector(1)
        .setBackupBootsectorSector(6)
        .setDriveNumber(0x80)
        .setVolumeLabel("");
    this->FilesystemName = "FAT32   ";
}

using FormatParams = Fat32FsFormatPartitionParameters;

FormatParams& Fat32FsFormatPartitionParameters::setOemName(const std::string& oemName) {
    if (oemName.size() > 8) {
        throw IncorrectParameterException(
            "OemName", oemName,
            "OemName can not be longer than 8 characters"
        );
    }
    this->OemName = oemName;
    while (this->OemName.size() < 8) {
        this->OemName.append(" ");
    }
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setBytesPerSector(uint64_t bytesPerSector) {
    if (bytesPerSector != 512 && bytesPerSector != 1024 &&
        bytesPerSector != 2048 && bytesPerSector != 4096) {
        throw IncorrectParameterException(
            "BytesPerSector", std::to_string(bytesPerSector),
            "BytesPerSector valid values are only 512, 1024, 2048, 4096"
        );
    }
    this->BytesPerSector = bytesPerSector;
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setSectorsPerCluster(uint8_t sectorsPerCluster) {
    if (!Math::isPowerOfTwo(sectorsPerCluster) || sectorsPerCluster > 128) {
        throw IncorrectParameterException(
            "SectorsPerCluster", std::to_string(sectorsPerCluster),
            "SectorsPerCluster value must be a power of 2 and no more than 128"
        );
    }
    this->SectorsPerCluster = sectorsPerCluster;
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setReservedSectorsCount(uint16_t reservedSectorsCount) {
    if (reservedSectorsCount == 0) {
        throw IncorrectParameterException(
            "ReservedSectorsCount", std::to_string(reservedSectorsCount),
            "ReservedSectorsCount valid value can not be 0"
        );
    }
    this->ReservedSectorsCount = reservedSectorsCount;
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setNumberOfFats(uint8_t numberOfFats) {
    if (numberOfFats != 1 && numberOfFats != 2) {
        throw IncorrectParameterException(
            "NumberOfFats", std::to_string(numberOfFats),
            "NumberOfFats valid values are only 1 or 2"
        );
    }
    this->NumberOfFats = numberOfFats;
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setMediaType(uint8_t mediaType) {
    if (mediaType != 0xF0 && !(mediaType >= 0xF8 && mediaType <= 0xFF)) {
        throw IncorrectParameterException(
            "MediaType", std::to_string(mediaType),
            "MediaType valid values are only 0xF0, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE and 0xFF"
        );
    }
    this->MediaType = mediaType;
    
    return *this;
}


FormatParams& Fat32FsFormatPartitionParameters::setExtendedFlags(uint16_t extendedFlags) {
    this->ExtendedFlags = extendedFlags;
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setRootDirectoryCluster(uint32_t rootDirectoryCluster) {
    this->RootDirectoryCluster = rootDirectoryCluster;
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setFsInfoSector(uint16_t fsInfoSector) {
    this->FsInfoSector = fsInfoSector;
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setBackupBootsectorSector(uint16_t backupBootsectorSector) {
    this->BackupBootsectorSector = backupBootsectorSector;
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setDriveNumber(uint8_t driveNumber) {
    this->DriveNumber = driveNumber;
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setVolumeLabel(const std::string& volumeLabel) {
    if (volumeLabel.size() > 11) {
        throw IncorrectParameterException(
            "VolumeLabel", volumeLabel,
            "VolumeLabel can not be longer than 11 characters"
        );
    }
    this->VolumeLabel = volumeLabel;
    while (this->VolumeLabel.size() < 11) {
        this->VolumeLabel.append(" ");
    }
    
    return *this;
}

FormatParams& Fat32FsFormatPartitionParameters::setBootsectorFileName(const std::string& bootsectorFileName) {
    this->BootsectorFileName = bootsectorFileName;
    return *this;
}

const std::string& Fat32FsFormatPartitionParameters::getOemName() const {
    return OemName;
}

uint64_t Fat32FsFormatPartitionParameters::getBytesPerSector() const {
    return BytesPerSector;
}

uint8_t Fat32FsFormatPartitionParameters::getSectorsPerCluster() const {
    return SectorsPerCluster;
}

uint16_t Fat32FsFormatPartitionParameters::getReservedSectorsCount() const {
    return ReservedSectorsCount;
}

uint8_t Fat32FsFormatPartitionParameters::getNumberOfFats() const {
    return NumberOfFats;
}

uint8_t Fat32FsFormatPartitionParameters::getMediaType() const {
    return MediaType;
}

uint16_t Fat32FsFormatPartitionParameters::getExtendedFlags() const {
    return ExtendedFlags;
}

uint32_t Fat32FsFormatPartitionParameters::getRootDirectoryCluster() const {
    return RootDirectoryCluster;
}

uint16_t Fat32FsFormatPartitionParameters::getFsInfoSector() const {
    return FsInfoSector;
}

uint16_t Fat32FsFormatPartitionParameters::getBackupBootsectorSector() const {
    return BackupBootsectorSector;
}

uint8_t Fat32FsFormatPartitionParameters::getDriveNumber() const {
    return DriveNumber;
}

const std::string& Fat32FsFormatPartitionParameters::getVolumeLabel() const {
    return VolumeLabel;
}

const std::string& Fat32FsFormatPartitionParameters::getFilesystemName() const {
    return FilesystemName;
}

const std::string& Fat32FsFormatPartitionParameters::getBootsectorFileName() const {
    return BootsectorFileName;
}