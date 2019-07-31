//
// Created by Stoorx on 19.06.2019.
//

#include "DiskImage.hpp"
#include <Exceptions/IndexOutOfBoundsException.hpp>

DiskImage::DiskImage(const std::string& fileName, bool truncate) {
    auto flags = std::fstream::binary | std::fstream::out | std::fstream::in;
    if (truncate)
        flags |= std::fstream::trunc;
    File.open(fileName, flags);
}

DiskImage::~DiskImage() {
    if (File.is_open())
        File.close();
}

DiskImage DiskImage::CreateEmptyDiskImage(const std::string& fileName, uint64_t fileSize) {
    // Create file
    std::ofstream fs(fileName, std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
    fs.seekp(fileSize - 1);
    uint8_t zero = 0;
    fs.write((char*)&zero, sizeof(zero));
    fs.flush();
    fs.close();
    
    auto di = DiskImage(fileName);
    di.Size = fileSize;
    return di;
}

void DiskImage::writeByte(int64_t index, uint8_t value) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File.write((char*)&value, sizeof(value));
}

uint8_t DiskImage::readByte(int64_t index) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    uint8_t value;
    File.read((char*)&value, sizeof(value));
    return value;
}

void DiskImage::writeShort(int64_t index, uint16_t value) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File.write((char*)&value, sizeof(value));
}

uint16_t DiskImage::readShort(int64_t index) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    uint16_t value;
    File.read((char*)&value, sizeof(value));
    return value;
}

void DiskImage::writeInt(int64_t index, uint32_t value) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File.write((char*)&value, sizeof(value));
}

void DiskImage::writeLong(int64_t index, uint64_t value) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File.write((char*)&value, sizeof(value));
}

uint32_t DiskImage::readInt(int64_t index) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    uint32_t value;
    File.read((char*)&value, sizeof(value));
    return value;
}

uint64_t DiskImage::readLong(int64_t index) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    uint64_t value;
    File.read((char*)&value, sizeof(value));
    return value;
}

void DiskImage::writeBuffer(int64_t index, const uint8_t* buffer, uint64_t bufSize) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File.write((const char*)buffer, bufSize);
}

void DiskImage::readBuffer(int64_t index, uint8_t* buffer, uint64_t bufSize) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    File.read((char*)buffer, bufSize);
}

DiskImage::DiskImage(DiskImage&& other) noexcept {
    this->File = std::move(other.File);
    this->Size = other.Size;
    other.Size = 0;
}

void DiskImage::close() {
    if (File.is_open())
        File.close();
}

uint64_t DiskImage::getSize() const {
    return Size;
}
