//
// Created by Stoorx on 11.07.2019.
//

#pragma once

#include <string>
#include <vector>

class StringUtil {
public:
    static std::vector<std::string> Split(const std::string& str, char delimiter);
    static uint64_t ToLong(const std::string& str);
    static std::string toUpperCase(const std::string& str);
};



