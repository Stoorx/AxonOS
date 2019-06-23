//
// Created by Stoorx on 22.06.2019.
//

#pragma once

#include <exception>
#include <string>

class CommandParseException : public std::exception {
public:
    CommandParseException(int position, const std::string& token, const std::string& reason);
    
    const char* what() const noexcept override;
    
    const int         TokenPosition;
    const std::string Token;
    const std::string Reason;
private:
    std::string errorString;
};
