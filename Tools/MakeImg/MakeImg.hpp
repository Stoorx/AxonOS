//
// Created by Stoorx on 19.06.2019.
//

#pragma once

#include <string>
#include <vector>
#include <ExitProcess.hpp>
#include <iostream>

using namespace std;

class MakeImg {
public:
    static void Main(vector<string>& args) {
        if (args.empty())
            ExitProcess(1);
        if (args.at(0) == "-h")
            ExitWithUsage();
        
    }
    
    static void ExitWithUsage() {
        cout <<
        "MakeImg - tool for creating disk images. Takes a scenario from command line and executes it step by step.\n"
        "Usage:\n"
        "   makeimg [-n file_name -[sb|sk|sm|sg|ss] size]\n"
        "           [-i img_file_name]\n"
        "           [-nt [mbr|gpt]]\n"
        "           [-boot mbr_file]\n"
        "           [-np partition_number -fs fs_type -b begin -e end [-p parameters]]\n"
        "Commands:\n"
        "   -h - show this help\n"
        "   -n - create new image file of given size\n"
        "       -sb - in bytes\n"
        "       -sk - in kilobytes\n"
        "       -sm - in megabytes\n"
        "       -sg - in gigabytes\n"
        "       -ss - in sectors\n"
        "   -i - take an existing file to create image inside\n"
        "   -nt - create new partition table\n"
        "       mbr - MS-DOS style partition table\n"
        "       gpt - GPT style\n"
        "   -boot - load bootsector code from given file (for mbr partition table only)\n"
        "   -np - create new partition\n"
        "       -fs - set the type of filesystem\n"
        "       -b - begin sector of the partition\n"
        "       -e - end sector of the partition\n"
        "       -p - parameters string for FS formatting tool\n";
    
        ExitProcess(0);
    }
    
    static void ExitWithError(const string& message, int exitCode) {
        cout << message << endl;
        ExitProcess(exitCode);
    }
};



