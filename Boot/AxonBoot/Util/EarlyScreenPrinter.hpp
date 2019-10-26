//
// Created by Stoorx on 26.10.2019.
//

#pragma once

#include <Types.h>

class EarlyScreenPrinter {
public:
    EarlyScreenPrinter() = default;
    
    void print(const Char8* message, UShort color = EarlyScreenPrinter::DefaultColor);
    
    void println(const Char8* message, UShort color = EarlyScreenPrinter::DefaultColor);
    
    void scrollUp(Byte rowsCount);
    
    void newLine();
    
    void clear();

protected:
    void adjustPositions();
    
    UShortPtr framebufferBase = (UShortPtr)0xB8000;
    Byte      currentColumn   = 0;
    Byte      currentRow      = 0;
    
    const Byte totalColumns = 80;
    const Byte totalRows    = 24;
    
    static const UShort DefaultColor = 0x0700;
};



