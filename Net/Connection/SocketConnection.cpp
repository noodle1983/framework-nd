#include "SocketConnection.h"
#include "BoostProcessor.h"
#include "Reactor.h"

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <err.h>

using namespace Net::Connection;

//-----------------------------------------------------------------------------

void on_read(int theFd, short theEvt, void *theArg)
{
    SocketConnection* connection = (SocketConnection*)theArg;
    connection->asynRead(theFd, theEvt);
}

//-----------------------------------------------------------------------------

void on_write(int theFd, short theEvt, void *theArg)
{
    SocketConnection* connection = (SocketConnection*)theArg;
    connection->asynWrite(theFd, theEvt);
}

//-----------------------------------------------------------------------------
SocketConnection::SocketConnection(
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor, 
            evutil_socket_t theFd)
    : reactorM(theReactor)
    , processorM(theProcessor)
    , fdM(theFd)
{
	readEvtM = reactorM->newEvent(fdM, EV_READ, on_read, this);
	writeEvtM = reactorM->newEvent(fdM, EV_WRITE, on_write, this);
    event_add(readEvtM, NULL);
}

//-----------------------------------------------------------------------------

SocketConnection::~SocketConnection()
{
    std::list<Buffer*>::iterator it = outputQueueM.begin();
    for (; it != outputQueueM.end(); it++)
    {
        delete *it;
    }
    evutil_closesocket(fdM);
	reactorM->delEvent(readEvtM);
	reactorM->delEvent(writeEvtM);
}

//-----------------------------------------------------------------------------

int SocketConnection::asynRead(int theFd, short theEvt)
{
    return processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::onRead, this, theFd, theEvt)));
}

//-----------------------------------------------------------------------------

void SocketConnection::onRead(int theFd, short theEvt)
{
    Buffer* buffer = new Buffer;

    buffer->lenM = read(theFd, buffer->rawM, sizeof(buffer->rawM));
    if (buffer->lenM == 0) 
    {
        printf("Client disconnected.\n");
        close();
        return;
    }
    else if (buffer->lenM > SSIZE_MAX) 
    {
        printf("Socket failure, disconnecting client: %s",
            strerror(errno));
        close();
        return;
    }
    
    outputQueueM.push_back(buffer);
    event_add(writeEvtM, NULL);
	
	event_add(readEvtM, NULL);
}

//-----------------------------------------------------------------------------

int SocketConnection::asynWrite(int theFd, short theEvt)
{
    return processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::onWrite, this, theFd, theEvt)));
}


//-----------------------------------------------------------------------------

void SocketConnection::onWrite(int theFd, short theEvt)
{
    if (outputQueueM.empty())
    {
        //asynDelEvent(&writeEvtM);
        return;
    }
    Buffer* buffer = outputQueueM.front();

    int len = buffer->lenM - buffer->offsetM;
    len = write(theFd, buffer->rawM + buffer->offsetM, len);
    if (len == -1) 
    {
        if (errno == EINTR || errno == EAGAIN) 
        {
            return;
        }
        else 
        {
            printf("Socket write failure");
            //err(1, "write");
            return;
        }
    }
    else if ((buffer->offsetM + len) < buffer->lenM) 
    {
        buffer->offsetM += len;
        return;
    }
    else if ((buffer->offsetM + len) > buffer->lenM)
    {
        printf("Socket write failure");
        //err(1, "write");
        return;
    }
    outputQueueM.pop_front();
    delete buffer;

    /*if (outputQueueM.empty())
    {
        asynDelEvent(&writeEvtM);
    }
    */
    if (!outputQueueM.empty())
    {
		event_add(writeEvtM, NULL);
    }
    
}

//-----------------------------------------------------------------------------

void SocketConnection::release()
{
    delete this;
}

//-----------------------------------------------------------------------------

void SocketConnection::close()
{
	evutil_closesocket(fdM);
    event_del(readEvtM);
    event_del(writeEvtM);
    processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::release, this)));
}

//-----------------------------------------------------------------------------


