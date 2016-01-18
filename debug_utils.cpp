#include "debug_utils.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace debug_utils {

void LogAssert(const char* function, const char* file, int line, const char* expression) {
    cerr << file << "(" << line << ")" << ": ASSERT FAILED: " << expression << " @ " << function << endl;
}

}  // namespace thread_loop
