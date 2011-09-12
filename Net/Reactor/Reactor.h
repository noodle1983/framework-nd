#ifndef REACTOR_H
#define REACTOR_H

#include <boost/thread.hpp>
#include <event.h>

namespace Net
{
namespace Reactor
{
    class Reactor
    {
    public:
        Reactor();
        ~Reactor();

        void start();
        void dispatchLoop();
        void stop();

        inline struct event *newEvent(
                evutil_socket_t theSocket, 
                short theEvt, 
                event_callback_fn theFn, 
                void* theArg);
        inline void delEvent(struct event*& theEvent);
        
    private:
        struct event_base* evtBaseM; 
        boost::thread_group threadsM;
    };

//-----------------------------------------------------------------------------
struct event* Reactor::newEvent(
                evutil_socket_t theSocket, 
                short theEvt, 
                event_callback_fn theFn, 
                void* theArg)
{
    return event_new(evtBaseM, theSocket, theEvt, theFn, theArg);
}

//-----------------------------------------------------------------------------

void Reactor::delEvent(struct event*& theEvent)
{
    event_del(theEvent);
    event_free(theEvent);
    theEvent = NULL;
}

}
}




#endif /* REACTOR_H */
