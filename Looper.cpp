#include "Looper.h"

#include <iostream>

namespace ph = std::placeholders;
using std::cerr;
using std::endl;
using std::to_string;

namespace thread_loop {

static void fakeEventCallback(evutil_socket_t, short, void *) { }

Looper::Looper(const std::string &debugName)
        : Looper(std::bind(&Looper::addFakeEvent, this, ph::_1), debugName) { }

Looper::Looper(const CallBackOnCreateEvent &onCreateEvents, const std::string &debugName)
        : mOnCreateEvents(onCreateEvents),
          mDebugName(debugName),
          mEventBase(nullptr, &event_base_free),
          mHandler(new Handler()),
          mFakeEvent(nullptr, &event_free) { }

Looper::~Looper() {
    ASSERT_X(!mEventBase, "You have to stop, and wait for full stop before destroy this Super Class");
}

event_base *Looper::getEventBase() const {
    ASSERT(mEventBase);
    return mEventBase.get();
}

Handler *Looper::getHandler() const {
    ASSERT(mHandler);
    return mHandler.get();
}

void Looper::startLoop() {
    int res = 0;
    mEventBase = createBaseEvent();

    mHandler->bindTo(mEventBase.get());

    mOnCreateEvents(mEventBase.get());

    cerr << "starting " << mDebugName << " loop" << endl;
    if (0 != (res = event_base_dispatch(mEventBase.get())))
        cerr << "Failed to run " << mDebugName << " message loop. res:" << res << endl;
    cerr << "Exit from " << mDebugName << " message loop." << endl;
    mEventBase.reset();
}

void Looper::stopLoop() {
    cerr << "stopping event_base " << mDebugName << "..." << endl;
    if(!started()) {
        cerr << "mEventBase is absent" << endl;
        return;
    }

    if(0 != event_base_loopbreak(mEventBase.get())) {
        cerr << "error event_base_loopbreak for:" << mDebugName << endl;
    }
}

void Looper::addFakeEvent(event_base *eventBase) {
    // Without timeout this trick doesn't work, I think it is because of bug in libevent.
    static const struct timeval tenMin = {600, 0};

    mFakeEvent.reset(event_new(eventBase, -1, EV_PERSIST, &fakeEventCallback, nullptr));
    int res = event_add(mFakeEvent.get(), &tenMin);
    if(0 != res) {
        throw std::runtime_error("Failed to add fake event for thread:" + mDebugName + "res:" + to_string(res));
    }
}

}  // namespace thread_loop
