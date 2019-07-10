//
// Created by Stoorx on 19.06.2019.
//

#include "DiskImage.hpp"
#include <Exceptions/IndexOutOfBoundsException.hpp>

DiskImage::DiskImage(const std::string& fileName, bool truncate) {
    auto flags =  std::fstream::binary | std::fstream::out;
    if(truncate)
        flags |= std::fstream::trunc;
    File.open(fileName, flags);
}

DiskImage::~DiskImage() {
    if (File.is_open())
        File.close();
}

DiskImage DiskImage::CreateEmptyDiskImage(const std::string& fileName, uint64_t fileSize) {
    auto di = DiskImage(fileName, true);
    di.Size = fileSize;
    di.writeByte(fileSize - 1, 0);
    return di;
}

void DiskImage::writeByte(int64_t index, uint8_t value) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File << value;
}

uint8_t DiskImage::readByte(int64_t index) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    uint8_t value;
    File >> value;
    return value;
}

void DiskImage::writeShort(int64_t index, uint16_t value) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File << value;
}

uint16_t DiskImage::readShort(int64_t index) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    uint16_t value;
    File >> value;
    return value;
}

void DiskImage::writeInt(int64_t index, uint32_t value) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File << value;
}

void DiskImage::writeLong(int64_t index, uint64_t value) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File << value;
}

uint32_t DiskImage::readInt(int64_t index) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    uint32_t value;
    File >> value;
    return value;
}

uint64_t DiskImage::readLong(int64_t index) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    uint64_t value;
    File >> value;
    return value;
}

void DiskImage::writeBuffer(int64_t index, const uint8_t* buffer, uint64_t bufSize) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekp(index);
    File.write(buffer, bufSize);
}

void DiskImage::readBuffer(int64_t index, uint8_t* buffer, uint64_t bufSize) {
    if (index >= Size) {
        throw IndexOutOfBoundsException(index, Size);
    }
    File.seekg(index);
    uint16_t value;
    File.read(buffer, bufSize);
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
