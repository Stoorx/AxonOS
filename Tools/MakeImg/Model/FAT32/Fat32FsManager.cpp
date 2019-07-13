//
// Created by Stoorx on 12.07.2019.
//

#include "Fat32FsManager.hpp"

Fat32FsManager::Fat32FsManager(Context& context, uint32_t partitionNumber) :
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
