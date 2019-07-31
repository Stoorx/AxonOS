//
// Created by Stoorx on 23.06.2019.
//

#include "NewImageCommand.hpp"

NewImageCommand::NewImageCommand(std::string& fileName, uint64_t fileSize) : Command(), FileName(fileName), FileSize(fileSize) {}

void NewImageCommand::Execute(Context& context) {
    if (context.DiskImage != nullptr) {
        throw IllegalStateException("Attempt of creating new image while active image exists");
    }
    context.DiskImage = std::make_shared<DiskImage>(DiskImage::CreateEmptyDiskImage(FileName, FileSize));
}
