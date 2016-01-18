#pragma once

namespace debug_utils {

// LogAssert writes information about an assertion to the log. It's called by
// Assert (and from the ASSERT macro in debug mode) before any other action
// is taken (e.g. breaking the debugger, abort()ing, etc.).
void LogAssert(const char* function, const char* file, int line,
               const char* expression);

}  // namespace debug_utils

#if ENABLE_DEBUG

#include <cstdlib>

#define LOG_ASSERT_AND_BREAK(msg)                                                       \
    do {                                                                                \
        ::debug_utils::LogAssert(__FUNCTION__, __FILE__, __LINE__, msg);                \
        abort();                                                                        \
    } while(0)

#ifndef ASSERT
#define ASSERT(x)                                                                      \
    do {                                                                               \
        if (!(x)) LOG_ASSERT_AND_BREAK(#x);                                            \
    } while(0)
#endif

#ifndef ASSERT_X
#include <sstream>
#define ASSERT_X(x, msgStr)                                                            \
    do {                                                                               \
        bool cond = x;                                                                 \
        if (cond) break;                                                               \
        std::stringstream ss;                                                          \
        ss << #x << " ";                                                               \
        ss << msgStr;                                                                  \
        LOG_ASSERT_AND_BREAK(ss.str().c_str());                                        \
    } while(0)
#endif

#else // ENABLE_DEBUG

#ifndef ASSERT
#define ASSERT(x) (void)0
#endif

#ifndef ASSERT_X
#define ASSERT_X(x, msg) (void)0
#endif

#endif // ENABLE_DEBUG
