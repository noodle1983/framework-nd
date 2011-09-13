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
			Protocol* theProtocol,
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor, 
            evutil_socket_t theFd)
    : protocolM(theProtocol)
    , reactorM(theReactor)
    , processorM(theProcessor)
    , fdM(theFd)
    , statusM(ActiveE)
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
    printf ("close fd:%d\n", fdM);
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
    if (buffer->lenM <= 0) 
    {
        delete buffer;
        printf("Client disconnected.\n");
        close();
        return;
    }
    else if (buffer->lenM > SSIZE_MAX) 
    {
        delete buffer;
        printf("Socket failure, disconnecting client: %s",
            strerror(errno));
        close();
        return;
    }
    outputQueueM.push_back(buffer);

    while(outputQueueM.size()<100){
        buffer = new Buffer;
        buffer->lenM = read(theFd, buffer->rawM, sizeof(buffer->rawM));
        if (buffer->lenM <= 0 || buffer->lenM > SSIZE_MAX)
        {
            delete buffer;
            break;
        }
        outputQueueM.push_back(buffer);
    }
    event_add(writeEvtM, NULL);
	event_add(readEvtM, NULL);
}

//-----------------------------------------------------------------------------

int SocketConnection::asynWrite(int theFd, short theEvt)
{
    if (CloseE == statusM)
        return -1;
    return processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::onWrite, this, theFd, theEvt)));
}


//-----------------------------------------------------------------------------

void SocketConnection::onWrite(int theFd, short theEvt)
{
    while (CloseE != statusM && !outputQueueM.empty())
    {
        Buffer* buffer = outputQueueM.front();

        int len = buffer->lenM - buffer->offsetM;
        len = write(theFd, buffer->rawM + buffer->offsetM, len);
        if (len == -1) 
        {
            if (errno == EINTR || errno == EAGAIN) 
            {
                event_add(writeEvtM, NULL);
                return;
            }
            else 
            {
                printf("Socket write failure");
                statusM = CloseE;
                return;
            }
        }
        else if ((buffer->offsetM + len) < buffer->lenM) 
        {
            buffer->offsetM += len;
            event_add(writeEvtM, NULL);
            return;
        }
        else if ((buffer->offsetM + len) > buffer->lenM)
        {
            printf("Socket write failure");
            statusM = CloseE;
            return;
        }
        outputQueueM.pop_front();
        delete buffer;
    }

    if (CloseE != statusM && !outputQueueM.empty())
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
    statusM = CloseE;
	reactorM->delEvent(readEvtM);
	reactorM->delEvent(writeEvtM);
    processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::release, this)));
}

//-----------------------------------------------------------------------------


