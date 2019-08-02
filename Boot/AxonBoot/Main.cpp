//
// Created by Stoorx on 02.08.2019.
//

extern "C" void Main() {
    *((int*)0xB8000) = 0x07320733; // Prints "32"
}
