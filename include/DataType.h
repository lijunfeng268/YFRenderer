#pragma once
#include <vector>

typedef char              Int8;
typedef unsigned char     UInt8;
typedef short             Int16;
typedef unsigned short    UInt16;
typedef int               Int32;
typedef unsigned int      UInt32;
typedef float             real;

struct OBJFace{
    std::vector<Int32> vtxIdx;
    std::vector<Int32> uvIdx;
    std::vector<Int32> normalIdx;
};