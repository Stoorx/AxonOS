//
// Created by Stoorx on 29.10.2019.
//

#pragma once

#include <Types.h>
#include <Attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma push(pack, 1)
typedef struct {
    UInt eax;
    UInt ebx;
    UInt ecx;
    UInt edx;
    
    UInt esi;
    UInt edi;
    
    UInt eFlags;
    
    UInt ds;
    UInt es;
    UInt fs;
    UInt gs;
} RealModeContext;
#pragma pop(pack)


Int CDECL realModeIntExecute(RealModeContext* rmContext, UInt intNumber);

#ifdef __cplusplus
};
#endif

