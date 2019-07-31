//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <string>
#include <algorithm>
#include "File.hpp"
#include <memory>

class Context;

class FsFormatPartitionParameters {
public:
    virtual ~FsFormatPartitionParameters() = default;
};

enum class OpenFileAttributes {
    // TODO: add attributes
};

enum class CreateFileAttributes {
    Directory,
    Hidden,
    System,
    Truncate
    //TODO: Add more?
};

class FsManager { // abstract
public:
    explicit FsManager(Context& context) : ImageContext(context) {}
    
    virtual void FormatPartition(const FsFormatPartitionParameters& params) = 0;
    virtual std::shared_ptr<File> openFile(
        const std::string& path,
        OpenFileAttributes attributes
    ) = 0; // TODO: implement
    virtual void deleteFile(const std::string& path) = 0; // TODO: implement
    virtual std::shared_ptr<File> createFile(
        const std::string& path,
        CreateFileAttributes attributes
    ) = 0; //TODO: implement
    
protected:
    Context& ImageContext;
};

