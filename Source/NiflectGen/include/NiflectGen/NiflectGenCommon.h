#pragma once

#ifdef NIFLECTGEN_API
#else
#ifdef WIN32
#ifdef NIFLECTGEN_EXPORTS
#define NIFLECTGEN_API __declspec(dllexport)
#else
#define NIFLECTGEN_API __declspec(dllimport)
#endif
#else
#ifdef NIFLECTGEN_EXPORTS
#define NIFLECTGEN_API __attribute__((visibility("default")))
#else
#define NIFLECTGEN_API 
#endif
#endif
#endif