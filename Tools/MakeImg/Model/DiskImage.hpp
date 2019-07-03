//
// Created by Stoorx on 19.06.2019.
//

#pragma once


#include <cstdint>
#include <fstream>

class DiskImage {
public:
    explicit DiskImage(const std::string& fileName, bool truncate = false);
    
    virtual ~DiskImage();
    
    DiskImage(DiskImage&& other) noexcept;
    
    static DiskImage CreateEmptyDiskImage(const std::string& fileName, uint64_t fileSize);
    
    void writeByte(int64_t index, uint8_t value);
    
    uint8_t readByte(int64_t index);
    
    
    void writeShort(int64_t index, uint16_t value);
    
    uint16_t readShort(int64_t index);
    
    void writeInt(int64_t index, uint32_t value);
    
    uint32_t readInt(int64_t index);
    
    void writeLong(int64_t index, uint64_t value);
    
    uint64_t readLong(int64_t index);
    
    void writeBuffer(int64_t index, const char* buffer, uint64_t bufSize);
    
    void readBuffer(int64_t index, char* buffer, uint64_t bufSize);
    void close();
    
    uint64_t getSize() const;
protected:
    std::fstream File;
    uint64_t     Size;
};



