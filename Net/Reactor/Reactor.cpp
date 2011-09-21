#include "Reactor.h"
#include "Log.h"

#include <event.h>
using namespace Net::Reactor;

//-----------------------------------------------------------------------------

void on_heartbeat(int theFd, short theEvt, void *theArg)
{
    //DEBUG("reactor heartbeat.");
}

//-----------------------------------------------------------------------------

Reactor::Reactor()
{
    evtBaseM = event_base_new(); 
    heartbeatEventM = NULL;
}

//-----------------------------------------------------------------------------

Reactor::~Reactor()
{
    event_base_free(evtBaseM);
    if (heartbeatEventM)
    {
        delEvent(heartbeatEventM);
    }
}


//-----------------------------------------------------------------------------

void Reactor::start()
{
    heartbeatEventM = event_new(evtBaseM, -1, EV_PERSIST, on_heartbeat, this);
    struct timeval tv;
    tv.tv_sec = 600;  //5 seconds
    tv.tv_usec = 0;
    event_add(heartbeatEventM, &tv);
    threadsM.create_thread(boost::bind(&Reactor::dispatchLoop, this));
}

//-----------------------------------------------------------------------------

void Reactor::dispatchLoop()
{
    DEBUG("enter event dispatch.");
    int ret = event_base_dispatch(evtBaseM);    
    if (-1 == ret)
    {
        ERROR("exit event dispatch with error.");
    }
    else
    {
        DEBUG("exit event dispatch.");
    }

}

//-----------------------------------------------------------------------------

void Reactor::stop()
{
    event_base_loopexit(evtBaseM, NULL);
    threadsM.interrupt_all();
    threadsM.join_all();
}



