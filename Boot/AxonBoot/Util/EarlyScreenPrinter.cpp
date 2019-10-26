//
// Created by Stoorx on 26.10.2019.
//

#include "EarlyScreenPrinter.hpp"

void EarlyScreenPrinter::print(const Char8* message, UShort color) {
    auto     * currentPosition = framebufferBase + currentRow * totalColumns + currentRow;
    for (auto* currentChar     = (BytePtr)message;
         *currentChar != '\0';
         currentChar++, currentPosition++, currentColumn++) {
        if (*currentChar == '\n') {
            newLine();
            continue;
        }
        adjustPositions();
        *framebufferBase = *currentChar | color;
    }
}

void EarlyScreenPrinter::println(const Char8* message, UShort color) {
    print(message, color);
    newLine();
}

void EarlyScreenPrinter::scrollUp(Byte rowsCount) {
    auto* toChar   = framebufferBase;
    auto* fromChar = framebufferBase + totalColumns * rowsCount;
    for (
        UShort i = 0;
        i < totalColumns * (totalRows - rowsCount);
        i++, fromChar++, toChar++
        ) {
        *toChar = *fromChar;
    }
    for (
        UShort i = totalColumns * (totalRows - rowsCount);
        i < totalRows * totalColumns;
        i++, toChar++
        ) {
        *toChar = 0;
    }
    currentRow -= rowsCount;
}

void EarlyScreenPrinter::newLine() {
    currentRow++;
    currentColumn = 0;
    adjustPositions();
}

void EarlyScreenPrinter::clear() {
    scrollUp(totalRows);
    currentColumn = 0;
    currentRow    = 0;
}

void EarlyScreenPrinter::adjustPositions() {
    if (currentColumn >= totalColumns) {
        currentColumn = 0;
        currentRow++;
        if (currentRow >= totalColumns) {
            scrollUp(1);
        }
    }
}
