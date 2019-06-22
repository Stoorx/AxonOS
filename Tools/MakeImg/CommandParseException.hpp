//
// Created by Stoorx on 22.06.2019.
//

#pragma once

#include <exception>
#include <string>

class CommandParseException : public std::exception {
public:
    CommandParseException(int position, const std::string& token, const std::string& reason) :
        Reason(reason), TokenPosition(position), Token(token) {
        errorString = (
            "CommandParseException at position: " +
            std::to_string(TokenPosition) +
            ",\n"
            "                         token: '" + Token +
            "',\n"
            "                         reason: '" + Reason + "'\n"
        );
        
    }
    
    virtual const char* what() const noexcept override {
        return errorString.c_str();
    }
    
    const int         TokenPosition;
    const std::string Token;
    const std::string Reason;
private:
    std::string errorString;
};
