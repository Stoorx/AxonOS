//
// Created by Stoorx on 23.06.2019.
//

#pragma once

#include <exception>
#include <string>

class IllegalStateException : std::exception {
public:
    explicit IllegalStateException(const std::string& msg);
    
    const char* what() const noexcept override;

protected:
    std::string message;
};

