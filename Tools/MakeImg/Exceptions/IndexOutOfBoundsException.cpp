//
// Created by Stoorx on 03.07.2019.
//

#include "IndexOutOfBoundsException.hpp"

const char* IndexOutOfBoundsException::what() const noexcept {
    return m_Message.c_str();
}

IndexOutOfBoundsException::IndexOutOfBoundsException(uint64_t index, uint64_t bound) :
    std::exception(), Index(index), Bound(bound) {
    m_Message.append("OutOfBoundException. Index: ")
             .append(std::to_string(Index))
             .append(", bound: ")
             .append(std::to_string(Bound))
             .append(".");
}
