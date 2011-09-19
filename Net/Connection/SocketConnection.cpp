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
    : selfM(this)
    , protocolM(theProtocol)
    , reactorM(theReactor)
    , processorM(theProcessor)
    , fdM(theFd)
    , statusM(ActiveE)
    , stopReadingM(false)
    , watcherM(NULL)
{
	readEvtM = reactorM->newEvent(fdM, EV_READ, on_read, this);
	writeEvtM = reactorM->newEvent(fdM, EV_WRITE, on_write, this);
    addReadEvent();
}

//-----------------------------------------------------------------------------

SocketConnection::~SocketConnection()
{
    evutil_closesocket(fdM);
    if (watcherM)
    {
        delete watcherM;
        watcherM = NULL;
    }
    printf ("close fd:%d\n", fdM);
}

//-----------------------------------------------------------------------------

void SocketConnection::addReadEvent()
{
	if (CloseE == statusM)
        return;
	if (-1 == event_add(readEvtM, NULL))
	{
		processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::addReadEvent, this)));
	}
}

//-----------------------------------------------------------------------------

void SocketConnection::addWriteEvent()
{
	if (CloseE == statusM)
        return;
	if (-1 == event_add(writeEvtM, NULL))
	{
		processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::addWriteEvent, this)));
	}
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
	size_t readBufferLeft = inputQueueM.unusedSize();
	size_t readLen = (readBufferLeft < sizeof(buffer)) ? readBufferLeft : sizeof(buffer);
	if (readLen == 0)
	{
		if (!stopReadingM)
		{
			boost::lock_guard<boost::mutex> lock(stopReadingMutexM);
	        stopReadingM = true;
		}
		protocolM->asynHandleInput(fdM, selfM);
		return;
	}

    int len = read(theFd, buffer, readLen);
    if (len <= 0) 
    {
        printf("Client disconnected.\n");
        _close();
        return;
    }
    else if (len > SSIZE_MAX) 
    {
        printf("Socket failure, disconnecting client: %s",
            strerror(errno));
        _close();
        return;
    }
    size_t putLen = inputQueueM.put(buffer, len);
	assert(putLen == (size_t)len);

    while(Net::Buffer::BufferOkE == inputQueueM.getStatus()
        || Net::Buffer::BufferLowE == inputQueueM.getStatus() )
	{
        readBufferLeft = inputQueueM.unusedSize();
        readLen = (readBufferLeft < sizeof(buffer)) ? readBufferLeft : sizeof(buffer);
        len = read(theFd, buffer, readLen);
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
        //printf("Flow Control:Socket %d stop reading\n", fdM); 
        boost::lock_guard<boost::mutex> lock(stopReadingMutexM);
        stopReadingM = true;
    }
    else
    {
        addReadEvent();
    }
    protocolM->asynHandleInput(fdM, selfM);
}
//-----------------------------------------------------------------------------

size_t SocketConnection::getInput(char* const theBuffer, const size_t theLen)
{
    if (CloseE == statusM)
        return 0;

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
            processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::addReadEvent, selfM)));
        }
    }
    return len;
}

//-----------------------------------------------------------------------------

size_t SocketConnection::getnInput(char* const theBuffer, const size_t theLen)
{
    if (CloseE == statusM)
        return 0;

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
            processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::addReadEvent, selfM)));
        }
    }
    return len;
}

//-----------------------------------------------------------------------------

Net::Buffer::BufferStatus 
SocketConnection::sendn(char* const theBuffer, const size_t theLen)
{
    if (CloseE == statusM)
        return Net::Buffer::BufferNotEnoughE;

    if (theLen == 0)
        return outputQueueM.getStatus();

    size_t len = 0;
    {
        boost::lock_guard<boost::mutex> lock(outputQueueMutexM);
        len = outputQueueM.putn(theBuffer, theLen);
    }
    if (0 == len)
    {
        printf("failed to put to the write queue!\n");
		processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::addWriteEvent, selfM)));
        return Net::Buffer::BufferNotEnoughE; 
    }
    processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::addWriteEvent, selfM)));
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
                _close();
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
            (*watcherM)(fdM, selfM);
            delete watcherM;
            watcherM = NULL;
        }
    }

    if (CloseE != statusM && !outputQueueM.empty())
    {
		addWriteEvent();
    }
    
}

//-----------------------------------------------------------------------------

void SocketConnection::close()
{
    if (CloseE == statusM)
        return;
    processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::_close, this)));
}

//-----------------------------------------------------------------------------

void SocketConnection::_release()
{
    selfM.reset();
}

//-----------------------------------------------------------------------------

void SocketConnection::_close()
{
    if (CloseE == statusM)
        return;
    statusM = CloseE;
	reactorM->delEvent(readEvtM);
	reactorM->delEvent(writeEvtM);
    processorM->process(fdM, new Processor::Job(boost::bind(&SocketConnection::_release, this)));
}

//-----------------------------------------------------------------------------


