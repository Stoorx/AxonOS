//
// Created by Stoorx on 12.07.2019.
//

#include "Fat32FsManager.hpp"
#include "Exceptions/IncorrectParameterException.hpp"
#include "Util/Math.hpp"

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

void Fat32FsManager::FormatPartition(Context& context, uint32_t number, const FsFormatPartitionParameters& params) {
    // TODO: initialize FatCache
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
        .setHiddenSectorsCount(0)
        .setExtendedFlags(0)
        .setRootDirectoryCluster(2)
        .setFsInfoSector(2)
        .setBackupBootsectorSector(6)
        .setDriveNumber(0x80)
        .setVolumeLabel("")
        .setFilesystemName("FAT32   ");
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

FormatParams& Fat32FsFormatPartitionParameters::setHiddenSectorsCount(uint32_t hiddenSectorsCount) {
    this->HiddenSectorsCount = hiddenSectorsCount;
    
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

FormatParams& Fat32FsFormatPartitionParameters::setFilesystemName(const std::string& filesystemName) {
    if (filesystemName.size() > 8) {
        throw IncorrectParameterException(
            "FilesystemName", filesystemName,
            "FilesystemName can not be longer than 8 characters"
        );
    }
    this->FilesystemName = filesystemName;
    while (this->FilesystemName.size() < 8) {
        this->FilesystemName.append(" ");
    }
    
    return *this;
}

