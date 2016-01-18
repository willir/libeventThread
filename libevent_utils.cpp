#include "libevent_utils.h"

#include <event2/thread.h>

namespace thread_loop {

UniqPtrEventBase createBaseEvent() {
    UniqPtrEventBase eventBase(event_base_new(), &event_base_free);
    if (!eventBase)
        throw std::runtime_error("Failed to create event_base in main loop.");

    if(0 != evthread_make_base_notifiable(eventBase.get()))
        throw std::runtime_error("Failed to make_base_notifiable in main loop.");

    return eventBase;
}

}  // namespace thread_loop
