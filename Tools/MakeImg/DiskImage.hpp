//
// Created by Stoorx on 19.06.2019.
//

#pragma once


#include <cstdint>
#include <fstream>

class DiskImage {
public:
    DiskImage(std::string fileName){
        file = std::fstream();
    }
    
    void operator[](uint64_t index){
    
    }

protected:
    std::fstream file;
};



