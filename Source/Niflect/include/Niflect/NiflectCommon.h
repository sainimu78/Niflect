#pragma once
//#include <cassert>//assert
//#include <stdexcept>//std::exception
#include <cstddef>//NULL
//#include <iostream>//std::cerr
#include <cstdio>//fprintf

//公共define, typedef, const

enum { INDEX_NONE = -1 };

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef long long int64;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK() (void)0  // 其他平台无操作
#endif

#ifndef NDEBUG
    #define ASSERT(b)\
        do { \
            bool cond = b;\
            if (!cond)\
            {\
                std::fprintf(stderr, "Assertion failed: %s, %d\n", __FILE__, __LINE__);\
                DEBUG_BREAK();\
            }\
        } while(0)
#else
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