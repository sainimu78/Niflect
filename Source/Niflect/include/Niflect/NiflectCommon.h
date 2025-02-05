#pragma once
#include <cassert>
#include <cstddef>//std::size_t, offsetof
#include <cstdio>//fflush

//¹«¹²define, typedef, const

enum { INDEX_NONE = -1 };

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef long long int64;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#define ASSERT(b)\
do {\
    bool cond = (b);\
    if (!cond)\
        fflush(stdout);\
    assert(cond);\
} while(0)

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