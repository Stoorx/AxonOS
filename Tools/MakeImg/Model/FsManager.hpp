//
// Created by Stoorx on 12.07.2019.
//

#pragma once

#include <string>
#include <algorithm>

class FsManager { // interface
public:
    virtual bool CheckFsSupport(const std::string& fsName) = 0;
    // TODO: add format partition
    // TODO: add files operations
};

class Fat32FsManager : public FsManager {
public:
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
};

