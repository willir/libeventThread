#include <iostream>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include <event2/thread.h>

#include "ThreadLooper.h"

using namespace thread_loop;

using std::cerr;
using std::endl;


void enableLibEventMultithreading() {
#if EVTHREAD_USE_PTHREADS_IMPLEMENTED
    evthread_use_pthreads();
#elif EVTHREAD_USE_WINDOWS_THREADS_IMPLEMENTED
    evthread_use_windows_threads();
#else
    static_assert(false, "nor pthread nor winapi");
#endif
}

int main() {
    enableLibEventMultithreading();

    ThreadLooper t("TestLoop");
    Handler *handler = t.getHandler();

    t.start();
    handler->post([] { cerr << "Some Method" << endl; });
    handler->post([] { cerr << "Some Method 2" << endl; });
    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    t.stop();

    cerr << "END" << endl;
    return 0;
}
