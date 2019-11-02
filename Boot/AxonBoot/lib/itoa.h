//
// Created by Stoorx on 02.11.2019.
//

#ifndef ITOA_H
#define ITOA_H

#include <Types.h>

#ifdef __cplusplus
extern "C" {
#endif
Char8Ptr itoa(Int number, Char8Ptr buffer, Byte radix);
#ifdef __cplusplus
}
#endif
#endif //ITOA_H
