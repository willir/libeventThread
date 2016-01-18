#pragma once

#include <memory>

#include <event2/event.h>

namespace thread_loop {

typedef std::unique_ptr<event_base, decltype(&event_base_free)> UniqPtrEventBase;
typedef std::unique_ptr<event, decltype(&event_free)> UniqPtrEvent;

/**
 * @returns New event_base
 * @throw runtime_error if error occurs while creation.
 */
UniqPtrEventBase createBaseEvent();

}  // namespace thread_loop
