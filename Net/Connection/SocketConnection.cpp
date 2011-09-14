#include "BoostProcessor.h"
#include "Connection/SocketConnection.h"
#include "Reactor/Reactor.h"
#include "Protocol.h"

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
			ProtocolInterface* theProtocol,
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
    char buffer[1024]= {0};

    int len = read(theFd, buffer, sizeof(buffer));
    if (len <= 0) 
    {
        printf("Client disconnected.\n");
        close();
        return;
    }
    else if (len > SSIZE_MAX) 
    {
        printf("Socket failure, disconnecting client: %s",
            strerror(errno));
        close();
        return;
    }
    size_t putLen = inputQueueM.put(buffer, len);
	assert(putLen == len);

    while(Net::Buffer::BufferOkE == inputQueueM.getStatus())
	{
        len = read(theFd, buffer, sizeof(buffer));
        if (len <= 0 || len > SSIZE_MAX)
        {
            break;
        }
        putLen = inputQueueM.put(buffer, len);
		assert(putLen == len);
    }
    event_add(readEvtM, NULL);
    protocolM->asynHandleInput(fdM, this);
}
//-----------------------------------------------------------------------------

size_t SocketConnection::getInput(char* const theBuffer, const size_t theLen)
{
    return inputQueueM.get(theBuffer, theLen);
}
//-----------------------------------------------------------------------------

Net::Buffer::BufferStatus 
SocketConnection::send(char* const theBuffer, const size_t theLen)
{
    Net::Buffer::BufferStatus bufferStatus = outputQueueM.put(theBuffer, theLen);
    event_add(writeEvtM, NULL);
    return bufferStatus;
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
	char buffer[1024]= {0};
	Net::Buffer::BufferStatus bufferStatus = outputQueueM.peek(buffer, sizeof(buffer));
    while (CloseE != statusM && bufferStatus != Net::Buffer::BufferNotEnoughE)
    {
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


