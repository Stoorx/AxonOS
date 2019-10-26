//
// Created by Stoorx on 02.06.2019.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// void
typedef void Void, * VoidPtr;

#ifdef __cplusplus
typedef bool Bool;
#else
typedef char Bool;
    #define true 1
    #define false 0
#endif

// 1 byte
typedef char SByte, * SBytePtr;
typedef unsigned char Byte, * BytePtr;

// 2 bytes
typedef short Short, * ShortPtr;
typedef unsigned short UShort, * UShortPtr;

// 4 bytes
typedef int Int, * IntPtr;
typedef unsigned int UInt, * UIntPtr;

// 8 bytes
typedef long long Long, * LongPtr;
typedef unsigned long long ULong, * ULongPtr;

// Floating point
typedef float Float, * FloatPtr;
typedef double Double, * DoublePtr;
typedef long double LongDouble, * LongDoublePtr;

// Machine dependent
typedef long SWord, * SWordPtr;
typedef unsigned long Word, * WordPtr;

#ifdef __cplusplus
};
#endif