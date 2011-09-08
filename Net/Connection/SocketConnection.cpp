#include "SocketConnection.h"
#include "BoostProcessor.h"

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <err.h>

using namespace Net::Connection;

//-----------------------------------------------------------------------------
SocketConnection::SocketConnection(const int theFd)
    :fdM(theFd)
{
}

//-----------------------------------------------------------------------------

SocketConnection::~SocketConnection()
{
    std::list<Buffer*>::iterator it = outputQueueM.begin();
    for (; it != outputQueueM.end(); it++)
    {
        delete *it;
    }
    close(fdM);
}

//-----------------------------------------------------------------------------

void SocketConnection::onRead(int theFd, short theEvt)
{
    Buffer* buffer = new Buffer;

    buffer->lenM = read(theFd, buffer->rawM, sizeof(buffer->rawM));
    if (buffer->lenM == 0) 
    {
        printf("Client disconnected.\n");
        asynClose();
        return;
    }
    else if (buffer->lenM < 0) 
    {
        printf("Socket failure, disconnecting client: %s",
            strerror(errno));
        asynClose();
        return;
    }
    
    outputQueueM.push_back(buffer);
    asynAddEvent(&writeEvtM, NULL);
}

//-----------------------------------------------------------------------------

void SocketConnection::onWrite(int theFd, short theEvt)
{

    if (outputQueueM.empty())
    {
        asynDelEvent(&writeEvtM);
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

    if (outputQueueM.empty())
    {
        asynDelEvent(&writeEvtM);
    }
    
}

//-----------------------------------------------------------------------------

void SocketConnection::release()
{
    delete this;
}

//-----------------------------------------------------------------------------

int SocketConnection::asynAddEvent(struct event* theEvt, const struct timeval* theTimeout)
{
    return processorM->process(0, new Processor::Job(boost::bind(event_add, theEvt, theTimeout)));
}

//-----------------------------------------------------------------------------

int SocketConnection::asynDelEvent(struct event* theEvt)
{
    return processorM->process(0, new Processor::Job(boost::bind(event_del, theEvt)));
}

//-----------------------------------------------------------------------------

void SocketConnection::asynClose()
{
    return processorM->process(0, new Processor::Job(boost::bind(&SocketConnection::close, this)));
}

//-----------------------------------------------------------------------------

void SocketConnection::close()
{
    event_del(&readEvtM);
    event_del(&writeEvtM);
    processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::release, this)));
}

//-----------------------------------------------------------------------------


