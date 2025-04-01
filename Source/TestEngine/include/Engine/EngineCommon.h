#pragma once

#ifdef ENGINE_API
#else
#ifdef WIN32
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else
#ifdef ENGINE_EXPORTS
#define ENGINE_API __attribute__((visibility("default")))
#else
#define ENGINE_API 
#endif
#endif
#endif

#ifdef ENGINE_C_API
#else
#ifdef WIN32
#ifdef ENGINE_C_EXPORTS
#define ENGINE_C_API extern "C" __declspec(dllexport)
#else
#define ENGINE_C_API
#endif
#else
#ifdef ENGINE_C_EXPORTS
#define ENGINE_C_API extern "C" __attribute__((visibility("default")))
#else
#define ENGINE_C_API 
#endif
#endif
#endif