//
// Created by Stoorx on 12.07.2019.
//

#include "NewFsCommand.hpp"
#include <Exceptions/IncorrectParameterException.hpp>
#include <Model/FAT32/Fat32FsManager.hpp>
#include <Util/StringUtil.hpp>

std::shared_ptr<Command> NewFsCommand::CreateFsCommandByType(
    const std::string& fsType,
    const uint32_t partitionNumber,
    const std::map<std::string, std::string>& params
) {
    if (Fat32FsManager::CheckFsSupport(fsType)) {
        return std::shared_ptr<Command>(new NewFat32FsCommand(fsType, partitionNumber, params));
    }
        // Put detections for other FSes here
    else {
        throw IncorrectParameterException("FS Type", fsType, "Unsupported file system type");
    }
}

void NewFat32FsCommand::Execute(Context& context) {
    auto currentFsManager = std::shared_ptr<FsManager>(new Fat32FsManager(context, PartitionNumber));
    
    context.FilesystemManagers[PartitionNumber] = currentFsManager;
    
    auto params = Fat32FsFormatPartitionParameters();
    
    auto oemNameIt = Params.find("-p_oem");
    if (oemNameIt != Params.end())
        params.setOemName(oemNameIt->second);
    
    auto bpsIt = Params.find("-p_bytes_per_sector");
    if (bpsIt != Params.end())
        params.setBytesPerSector(StringUtil::ToLong(bpsIt->second));
    
    auto spcIt = Params.find("-p_sectors_per_cluster");
    if (spcIt != Params.end())
        params.setSectorsPerCluster(StringUtil::ToLong(spcIt->second));
    
    auto rcIt = Params.find("-p_reserved_sectors");
    if (rcIt != Params.end())
        params.setReservedSectorsCount(StringUtil::ToLong(rcIt->second));
    
    auto fatNumberIt = Params.find("-p_fats_number");
    if (fatNumberIt != Params.end())
        params.setNumberOfFats(StringUtil::ToLong(fatNumberIt->second));
    
    auto mediaIt = Params.find("-p_media");
    if (mediaIt != Params.end())
        params.setMediaType(StringUtil::ToLong(mediaIt->second));
    
    auto rootIt = Params.find("-p_root_cluster");
    if (rootIt != Params.end())
        params.setRootDirectoryCluster(StringUtil::ToLong(rootIt->second));
    
    auto fsInfoIt = Params.find("-p_fsinfo");
    if (fsInfoIt != Params.end())
        params.setFsInfoSector(StringUtil::ToLong(fsInfoIt->second));
    
    auto bsBackupIt = Params.find("-p_backup_bs");
    if (bsBackupIt != Params.end())
        params.setBackupBootsectorSector(StringUtil::ToLong(bsBackupIt->second));
    
    auto dNumberIt = Params.find("-p_dnumber");
    if (dNumberIt != Params.end())
        params.setDriveNumber(StringUtil::ToLong(dNumberIt->second));
    
    auto labelIt = Params.find("-p_label");
    if (labelIt != Params.end())
        params.setVolumeLabel(labelIt->second);
    
    auto bsFileIt = Params.find("-p_boot");
    if (bsFileIt != Params.end())
        params.setBootsectorFileName(bsFileIt->second);
    
    currentFsManager->FormatPartition(params);
}
