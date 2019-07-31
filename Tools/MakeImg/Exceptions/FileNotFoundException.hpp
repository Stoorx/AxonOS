//
// Created by Stoorx on 23.06.2019.
//

#pragma once

#include <exception>
#include <string>

class FileNotFoundException : public std::exception {
public:
    explicit FileNotFoundException(const std::string& fileName) :
        std::exception(), FileName(fileName),
        errorMessage(
            "File not found: " + fileName
        ) {}
    
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
    
    std::string FileName;
protected:
    std::string errorMessage;
};

