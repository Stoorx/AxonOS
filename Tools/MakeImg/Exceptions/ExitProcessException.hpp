//
// Created by Stoorx on 19.06.2019.
//

#pragma once

#include <exception>

class ExitProcessException : public std::exception {
public:
    explicit ExitProcessException(int exitCode);
    
    const int ExitCode;
    
    const char* what() const noexcept override;
};
