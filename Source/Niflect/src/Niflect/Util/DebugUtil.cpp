#include "Niflect/Util/DebugUtil.h"
#include <mutex>
#include <stdarg.h>

namespace NiflectUtil
{
    void Printf(const char* format, ...)
    {
//#define ENABLED_THREAD_SAFE_PRINTF
#ifdef ENABLED_THREAD_SAFE_PRINTF
        static std::mutex print_mutex;
        std::lock_guard<std::mutex> lock(print_mutex);
#endif

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}