//
// Created by Stoorx on 03.07.2019.
//

#pragma once

#include <exception>
#include <stdint.h>
#include <string>

class IndexOutOfBoundsException : public std::exception {
public:
    IndexOutOfBoundsException(uint64_t index, uint64_t bound);
    
    const char* what() const noexcept override;
    
    const uint64_t Index;
    const uint64_t Bound;
private:
    std::string m_Message;
};



