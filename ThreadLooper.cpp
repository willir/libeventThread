#include "ThreadLooper.h"

#include <iostream>

using boost::thread;
using std::cerr;
using std::endl;

namespace thread_loop {

ThreadLooper::~ThreadLooper() {
    stop();
}

void ThreadLooper::start() {
    mThread.reset(new thread(&ThreadLooper::startLoop, this));
}

void ThreadLooper::stop() {
    if(!mThread) {
        cerr << "ThreadLooper::stop:" << getDebugName() << " already stopped" << endl;
        return;
    }

    stopLoop();
    mThread->join();
    mThread.reset();
    ASSERT(!started());
}

}  // namespace thread_loop
