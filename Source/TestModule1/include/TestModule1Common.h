#pragma once

#ifdef TESTMODULE1_API
#else
#ifdef WIN32
#ifdef TESTMODULE1_EXPORTS
#define TESTMODULE1_API __declspec(dllexport)
#else
#define TESTMODULE1_API __declspec(dllimport)
#endif
#else
#ifdef TESTMODULE1_EXPORTS
#define TESTMODULE1_API __attribute__((visibility("default")))
#else
#define TESTMODULE1_API 
#endif
#endif
#endif
