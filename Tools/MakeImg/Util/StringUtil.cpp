//
// Created by Stoorx on 11.07.2019.
//

#include "StringUtil.hpp"

std::vector<std::string> StringUtil::Split(const std::string& str, char delimiter) {
    uint32_t                 startPosition   = 0;
    uint32_t                 currentPosition = 0;
    std::vector<std::string> resultVector;
    
    for (; currentPosition < str.size(); currentPosition++) {
        if (str[currentPosition] == delimiter) {
            resultVector.emplace_back(str, startPosition, currentPosition - startPosition); // substring
            startPosition = currentPosition;
        }
    }
    resultVector.emplace_back(str, startPosition, str.size());
    return resultVector;
}
