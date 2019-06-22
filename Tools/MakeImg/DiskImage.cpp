//
// Created by Stoorx on 19.06.2019.
//

#include "DiskImage.hpp"

DiskImage::DiskImage(const std::string& fileName) {
    file.open(fileName, std::fstream::binary | std::fstream::in | std::fstream::out);
}

DiskImage::~DiskImage() {
    file.close();
}

DiskImage DiskImage::CreateEmptyDiskImage(const std::string& fileName, uint64_t fileSize) {
    auto di = DiskImage(fileName);
    di.writeByte(fileSize - 1, 0);
    return di;
}

void DiskImage::writeByte(int64_t index, uint8_t value) {
    file.seekp(index);
    file << value;
}

uint8_t DiskImage::readByte(int64_t index) {
    file.seekg(index);
    uint8_t value;
    file >> value;
    return value;
}

void DiskImage::writeShort(int64_t index, uint16_t value) {
    file.seekp(index);
    file << value;
}

uint16_t DiskImage::readShort(int64_t index) {
    file.seekg(index);
    uint16_t value;
    file >> value;
    return value;
}

void DiskImage::writeInt(int64_t index, uint32_t value) {
    file.seekp(index);
    file << value;
}

void DiskImage::writeLong(int64_t index, uint64_t value) {
    file.seekp(index);
    file << value;
}

uint32_t DiskImage::readInt(int64_t index) {
    file.seekg(index);
    uint32_t value;
    file >> value;
    return value;
}

uint64_t DiskImage::readLong(int64_t index) {
    file.seekg(index);
    uint64_t value;
    file >> value;
    return value;
}

void DiskImage::writeBuffer(int64_t index, const char* buffer, uint64_t bufSize) {
    file.seekp(index);
    file.write(buffer, bufSize);
}

void DiskImage::readBuffer(int64_t index, char* buffer, uint64_t bufSize) {
    file.seekg(index);
    uint16_t value;
    file.read(buffer, bufSize);
}

DiskImage::DiskImage(DiskImage&& other) noexcept {
    this->file = std::move(other.file);
}
