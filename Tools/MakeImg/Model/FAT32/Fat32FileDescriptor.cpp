//
// Created by Stoorx on 18.07.2019.
//

#include "Fat32FileDescriptor.hpp"

uint32_t Fat32FileDescriptor::getFirstCluster() const {
    return ((uint32_t)FirstClusterHigh << 16u) | FirstClusterLow;
}

std::string Fat32FileDescriptor::getFileName() const {
    return std::string(FileName, 11);
}

void Fat32FileDescriptor::setFileName(const std::string& name) {
    memcpy(FileName, name.c_str(), 11);
}
