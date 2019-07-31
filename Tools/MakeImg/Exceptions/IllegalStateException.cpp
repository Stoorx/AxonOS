//
// Created by Stoorx on 23.06.2019.
//

#include "IllegalStateException.hpp"

IllegalStateException::IllegalStateException(const std::string& msg) : std::exception() {
    message = msg;
}

const char* IllegalStateException::what() const noexcept {
    return message.c_str();
}
