#pragma once

#include <event2/event.h>
#include <functional>
#include <memory>
#include <vector>
#include <tuple>

#include <boost/thread/mutex.hpp>

namespace thread_loop {

/** \brief Handler to execute functions in some thread to which this Handler belongs.
 *
 * Handler can be created without event_base, in this case handler will belong to nothing,
 *     and all functions will be added to queue. You can bind this Handler to Some thread by calling bindTo method,
 *     all functions from queue will be added to Thread queue in bindTo method.
 */
class Handler {
public:
    Handler();
    Handler(event_base *eventBase);

    /** \brief Bind this handler to thread.
     *
     * All queued functions will be added to specified thread loop.
     *
     * You can call this method only if this Handler wasn't bind to any thread.
     */
    void bindTo(event_base *eventBase);

    /** \brief Calls function in thread this handler belongs to.
     *
     * If handler doesn't belong to any thread, function will be called when handler is bound to thread.
     *
     * @param fun: Function to be called in thread this handler belongs to.
     */
    template <class Functor>
    void post(const Functor &fun) { postInner(new std::function<void()>(fun)); }
    void post(std::function<void()> &&fun) { postInner(new std::function<void()>(std::move(fun))); }

    /** \brief Calls function in thread this handler belongs to with specified delay in ms.
     *
     * If handler doesn't belong to any thread, function will be called when handler is bound to thread
     *     with delay == delayMs (delay after bind moment).
     *
     * @param fun: Function to be called in thread this handler belongs.
     * @param delayMs: The delay (in milliseconds) until the function will be called.
     */
    template<class Functor>
    inline void postDelayed(const Functor &fun, time_t delayMs);
    inline void postDelayed(std::function<void()> &&fun, time_t delayMs);

private:
    event_base *mEventBase = nullptr;
    boost::mutex mMutex;

    std::vector<std::unique_ptr<std::function<void()>>> mFuncsToPost;
    std::vector<std::tuple<std::unique_ptr<std::function<void()>>, time_t>> mFuncsToPostDelayed;

    void postInner(std::function<void()> *fun);
    void postDelayedInner(std::function<void()> *fun, time_t delayMs);

    void eventBaseOnce(std::function<void()> *fun);
    void eventBaseOnceDelayed(std::function<void()> *fun, time_t delayMs);
};
// --------------------------------------------

template<class Functor>
inline void Handler::postDelayed(const Functor &fun, time_t delayMs) {
    postDelayedInner(new std::function<void()>(fun), delayMs);
}

inline void Handler::postDelayed(std::function<void()> &&fun, time_t delayMs) {
    postDelayedInner(new std::function<void()>(std::move(fun)), delayMs);
}

}  // namespace thread_loop
