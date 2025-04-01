#pragma once

#ifdef TESTMODULE0_API
#else
#ifdef WIN32
#ifdef TESTMODULE0_EXPORTS
#define TESTMODULE0_API __declspec(dllexport)
#else
#define TESTMODULE0_API __declspec(dllimport)
#endif
#else
#ifdef TESTMODULE0_EXPORTS
#define TESTMODULE0_API __attribute__((visibility("default")))
#else
#define TESTMODULE0_API 
#endif
#endif
#endif

#ifdef TESTMODULE0_C_API
#else
#ifdef WIN32
#ifdef TESTMODULE0_C_EXPORTS
#define TESTMODULE0_C_API extern "C" __declspec(dllexport)
#else
#define TESTMODULE0_C_API
#endif
#else
#ifdef TESTMODULE0_C_EXPORTS
#define TESTMODULE0_C_API extern "C" __attribute__((visibility("default")))
#else
#define TESTMODULE0_C_API 
#endif
#endif
#endif