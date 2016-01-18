#pragma once

#include <memory>
#include <functional>

#include <event2/event.h>

#include "debug_utils.h"
#include "libevent_utils.h"
#include "Handler.h"

namespace thread_loop {

class Looper {
public:
    typedef std::function<void(event_base *)> CallBackOnCreateEvent;

    Looper(const std::string &debugName);
    Looper(const CallBackOnCreateEvent &onCreateEvents, const std::string &debugName);
    virtual ~Looper();

    virtual void start() =0;
    virtual void stop() =0;
    bool started() const { return nullptr != mEventBase.get(); }

    const std::string &getDebugName() const { return mDebugName; }
    event_base *getEventBase() const;
    Handler *getHandler() const;

protected:
    void startLoop();
    void stopLoop();

private:
    const CallBackOnCreateEvent mOnCreateEvents;
    const std::string mDebugName;
    UniqPtrEventBase mEventBase;
    const std::unique_ptr<Handler> mHandler;
    UniqPtrEvent mFakeEvent;

    /** \brief Adds fake event to event_base.
     *
     * This is needed because event_basea has to have at least one event
     *     or event_base_loopbreak will return immediately.
     * This function is used in Constructor without callback.
     */
    void addFakeEvent(event_base *eventBase);
};

}  // namespace thread_loop
