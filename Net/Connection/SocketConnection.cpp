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
    , stopReadingM(false)
    , watcherM(NULL)
{
	readEvtM = reactorM->newEvent(fdM, EV_READ, on_read, this);
	writeEvtM = reactorM->newEvent(fdM, EV_WRITE, on_write, this);
    event_add(readEvtM, NULL);
}

//-----------------------------------------------------------------------------

SocketConnection::~SocketConnection()
{
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
	assert(putLen == (size_t)len);

    while(Net::Buffer::BufferOkE == inputQueueM.getStatus()
        || Net::Buffer::BufferLowE == inputQueueM.getStatus() )
	{
        len = read(theFd, buffer, sizeof(buffer));
        if (len <= 0 || len > SSIZE_MAX)
        {
            break;
        }
        putLen = inputQueueM.put(buffer, len);
		assert(putLen == (size_t)len);
    }

    if (Net::Buffer::BufferHighE == inputQueueM.getStatus()
            || Net::Buffer::BufferNotEnoughE == inputQueueM.getStatus())
    {
        printf("Flow Control:Socket %d stop reading\n", fdM); 
        boost::lock_guard<boost::mutex> lock(stopReadingMutexM);
        stopReadingM = true;
    }
    else
    {
        event_add(readEvtM, NULL);
    }
    protocolM->asynHandleInput(fdM, this);
}
//-----------------------------------------------------------------------------

size_t SocketConnection::getInput(char* const theBuffer, const size_t theLen)
{
    size_t len = inputQueueM.get(theBuffer, theLen);
    if (stopReadingM)
    {
        Net::Buffer::BufferStatus postBufferStatus = inputQueueM.getStatus();
        if (postBufferStatus == Net::Buffer::BufferLowE)
        {
            {
                boost::lock_guard<boost::mutex> lock(stopReadingMutexM);
                stopReadingM = false;
            }
            event_add(readEvtM, NULL);
        }
    }
    return len;
}

//-----------------------------------------------------------------------------

size_t SocketConnection::getnInput(char* const theBuffer, const size_t theLen)
{
    size_t len = inputQueueM.getn(theBuffer, theLen);
    if (stopReadingM)
    {
        Net::Buffer::BufferStatus postBufferStatus = inputQueueM.getStatus();
        if (postBufferStatus == Net::Buffer::BufferLowE)
        {
            {
                boost::lock_guard<boost::mutex> lock(stopReadingMutexM);
                stopReadingM = false;
            }
            event_add(readEvtM, NULL);
        }
    }
    return len;
}

//-----------------------------------------------------------------------------

Net::Buffer::BufferStatus 
SocketConnection::sendn(char* const theBuffer, const size_t theLen)
{
    if (theLen == 0)
        return outputQueueM.getStatus();

    size_t len = outputQueueM.putn(theBuffer, theLen);
    if (0 == len)
    {
        event_add(writeEvtM, NULL);
        return Net::Buffer::BufferNotEnoughE; 
    }
    event_add(writeEvtM, NULL);
    return outputQueueM.getStatus();
}

//-----------------------------------------------------------------------------

int SocketConnection::asynWrite(int theFd, short theEvt)
{
    if (CloseE == statusM)
        return -1;
    return processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::onWrite, this, theFd, theEvt)));
}

//-----------------------------------------------------------------------------

void SocketConnection::setLowWaterMarkWatcher(Watcher* theWatcher)
{
    boost::lock_guard<boost::mutex> lock(watcherMutexM);
    if (watcherM) 
    {
        delete watcherM;
    }
    watcherM = theWatcher;
}

//-----------------------------------------------------------------------------

void SocketConnection::onWrite(int theFd, short theEvt)
{
	char buffer[1024]= {0};
	size_t peekLen = outputQueueM.peek(buffer, sizeof(buffer));
    int writeLen = 0;
    while (CloseE != statusM && peekLen > 0)
    {
        writeLen = write(theFd, buffer, peekLen);
        if (writeLen < 0) 
        {
            if (errno == EINTR || errno == EAGAIN) 
            {
                break;
            }
            else 
            {
                printf("Socket write failure");
                statusM = CloseE;
                return;
            }
        }
        outputQueueM.commitRead(writeLen);
        peekLen = outputQueueM.peek(buffer, sizeof(buffer));
    }

    Net::Buffer::BufferStatus bufferStatus = outputQueueM.getStatus();
    if (watcherM && (bufferStatus == Net::Buffer::BufferLowE))
    {
        boost::lock_guard<boost::mutex> lock(watcherMutexM);
        if (watcherM) 
        {
            (*watcherM)(fdM, this);
            delete watcherM;
            watcherM = NULL;
        }
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


