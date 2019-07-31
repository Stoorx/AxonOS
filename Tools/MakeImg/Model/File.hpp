//
// Created by Stoorx on 18.07.2019.
//

#pragma once

#include <string>

class File { // interface
public:
    virtual std::string getName() = 0;
    virtual void setName() = 0;
    virtual void read(void* buffer, uint64_t offset, uint64_t bytesCount) = 0;
    virtual void write(const void* buffer, uint64_t offset, uint64_t bytesCount) = 0;
    virtual bool isDirectory() = 0;
    virtual uint64_t getSize() = 0;
    // TODO: Get/Set date/time
    // TODO: Add open/create/delete file
};



