//
// Created by cognitio on 12.07.2019.
//

#pragma once

#include <exception>
#include <string>

class IncorrectParameterException : public std::exception {
public:
    IncorrectParameterException(const std::string& name, const std::string& value, const std::string& reason);
    
    const char* what() const noexcept override;
    
    const std::string ParameterName;
    const std::string ParameterValue;
    const std::string Reason;
private:
    std::string errorString;
};