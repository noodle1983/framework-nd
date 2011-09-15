#include "BoostProcessor.h"
#include "Protocol/EchoProtocol.h"
#include "Connection/SocketConnection.h"
#include "Reactor/Reactor.h"


using namespace Net::Protocol;

//-----------------------------------------------------------------------------
EchoProtocol::EchoProtocol(
        Reactor::Reactor* theReactor, 
        Processor::BoostProcessor* theProcessor)
    : reactorM(theReactor)
    , processorM(theProcessor)
{
}

//-----------------------------------------------------------------------------

EchoProtocol::~EchoProtocol()
{
}

//-----------------------------------------------------------------------------

int EchoProtocol::asynHandleInput(int theFd, Connection::SocketConnection* connection)
{
    return processorM->process(theFd, new Processor::Job(boost::bind(&EchoProtocol::handleInput, this, connection)));
}

//-----------------------------------------------------------------------------

int EchoProtocol::handleInput(Connection::SocketConnection* connection)
{
	char buffer[1024];
    size_t len = 1;
    Net::Buffer::BufferStatus oBufferStatus = connection->sendn(NULL, 0);
    while (len > 0 && (Net::Buffer::BufferOkE == oBufferStatus || Net::Buffer::BufferLowE == oBufferStatus))
    {
        len = connection->getInput(buffer, sizeof(buffer));
        oBufferStatus = connection->sendn(buffer, len);
    }
    if (Net::Buffer::BufferHighE == oBufferStatus || Net::Buffer::BufferNotEnoughE == oBufferStatus)
    {
        connection->setLowWaterMarkWatcher(new Net::Connection::Watcher(boost::bind(&EchoProtocol::asynHandleInput, this, _1, _2)));
    }
    return 0;
}



//-----------------------------------------------------------------------------


