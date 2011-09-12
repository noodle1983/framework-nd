#include "Reactor.h"

#include <event.h>
using namespace Net::Reactor;

//-----------------------------------------------------------------------------

Reactor::Reactor()
{
    evtBaseM = event_base_new(); 
}

//-----------------------------------------------------------------------------

Reactor::~Reactor()
{
    event_base_free(evtBaseM);
}


//-----------------------------------------------------------------------------

void Reactor::start()
{
    threadsM.create_thread(boost::bind(&Reactor::dispatchLoop, this));
}

//-----------------------------------------------------------------------------

void Reactor::dispatchLoop()
{
    event_base_dispatch(evtBaseM);    
}

//-----------------------------------------------------------------------------

void Reactor::stop()
{
    event_base_loopexit(evtBaseM, NULL);
    threadsM.interrupt_all();
    threadsM.join_all();
}



