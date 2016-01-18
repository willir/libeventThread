#include "Handler.h"

#include <memory>

#include <boost/thread/lock_guard.hpp>

#include "debug_utils.h"

using std::unique_ptr;
using std::function;
using std::make_tuple;
using std::tuple;
using boost::lock_guard;
using boost::mutex;

namespace thread_loop {

static void onPost(evutil_socket_t, short, void *vPtr) {
    ASSERT(nullptr != vPtr);
    std::function<void()> *fun = reinterpret_cast<std::function<void()> *>(vPtr);
    (*fun)();
    delete fun;
}

Handler::Handler() : Handler(nullptr) {}
Handler::Handler(event_base *eventBase) : mEventBase(eventBase) { }

void Handler::bindTo(event_base *eventBase) {
    lock_guard<mutex> mutex(mMutex);
    ASSERT(mEventBase == nullptr);

    mEventBase = eventBase;
    for(unique_ptr<function<void()>> &fun : mFuncsToPost) {
        eventBaseOnce(fun.release());
    }
    for(auto &item : mFuncsToPostDelayed) {
        eventBaseOnceDelayed(std::get<0>(item).release(), std::get<1>(item));
    }
}

void Handler::postInner(function<void()> *fun) {
    lock_guard<mutex> mutex(mMutex);

    if(!mEventBase) {
        mFuncsToPost.push_back(unique_ptr<function<void()>>(fun));
    } else {
        eventBaseOnce(fun);
    }
}

void Handler::postDelayedInner(function<void()> *fun, time_t delayMs) {
    lock_guard<mutex> mutex(mMutex);

    if(!mEventBase) {
        mFuncsToPostDelayed.push_back(make_tuple(unique_ptr<function<void()>>(fun), delayMs));
    } else {
        eventBaseOnceDelayed(fun, delayMs);
    }
}

void Handler::eventBaseOnce(function<void()> *fun) {
    ASSERT(nullptr != mEventBase);

    if(0 != event_base_once(mEventBase, -1, EV_TIMEOUT, &onPost, fun, nullptr)) {
        throw new std::runtime_error("Error creating event_base_once.");
    }
}

void Handler::eventBaseOnceDelayed(function<void()> *fun, time_t delayMs) {
    ASSERT(nullptr != mEventBase);

    time_t sec = delayMs / 1000;
    time_t usec = (delayMs % 1000) * 1000;

    struct timeval timeout = {sec, usec};

    if(0 != event_base_once(mEventBase, -1, EV_TIMEOUT, &onPost, fun, &timeout)) {
        throw new std::runtime_error("Error creating event_base_once.");
    }
}

}  // namespace thread_loop
