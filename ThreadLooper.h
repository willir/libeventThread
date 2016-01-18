#pragma once

#include <memory>

// We use boost::thread to be able to get stack trace if exception has been raised
// See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55917
#include <boost/thread.hpp>

#include "Looper.h"

namespace thread_loop {

class ThreadLooper : public Looper {
public:
    using Looper::Looper;

    ~ThreadLooper();

    void start() override;
    void stop() override;

    boost::thread *getThread() const { return mThread.get(); }

private:
    std::unique_ptr<boost::thread> mThread;
};

}  // namespace thread_loop
