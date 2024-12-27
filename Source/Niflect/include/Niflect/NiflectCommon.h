#pragma once
#include <cassert>
#include <cstddef>//std::size_t, offsetof
#include "IndexingConst.h"
#include <cstdio>//fflush

//¹«¹²define, typedef, const

#ifndef ASSERT
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef long long int64;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#define ASSERT(b)\
{\
    fflush(stdout);\
    assert(b);\
} do{} while(0)
#endif

#ifndef LogError
#define LogError(fmt, ...)\
{\
printf(fmt "\n", ##__VA_ARGS__);\
ASSERT(false);\
} do{} while(0)
#endif

#ifdef NIFLECT_API
#else
    #ifdef WIN32
        #ifdef NIFLECT_EXPORTS
            #define NIFLECT_API __declspec(dllexport)
        #else
            #define NIFLECT_API __declspec(dllimport)
        #endif
    #else
        #ifdef NIFLECT_EXPORTS
		    #define NIFLECT_API __attribute__((visibility("default")))
	    #else
		    #define NIFLECT_API 
	    #endif
    #endif
#endif

//#ifdef WIN32
//#define NIFLECTMODULEREG_API extern "C" __declspec(dllexport)
//#else
//#define NIFLECTMODULEREG_API extern "C" __attribute__((visibility("default")))
//#endif

//#ifdef WIN32
//#define NIFLECTTYPEREG_API __declspec(dllexport)
//#else
//#define NIFLECTTYPEREG_API __attribute__((visibility("default")))
//#endif