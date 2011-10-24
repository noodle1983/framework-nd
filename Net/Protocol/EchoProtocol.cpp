#include "BoostProcessor.h"
#include "Protocol/EchoProtocol.h"
#include "Connection/SocketConnection.h"
#include "Reactor.h"


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

int EchoProtocol::asynHandleInput(int theFd, Connection::SocketConnectionPtr connection)
{
    return processorM->process(theFd + 1, new Processor::Job(boost::bind(&EchoProtocol::handleInput, this, connection)));
}

//-----------------------------------------------------------------------------

int EchoProtocol::handleInput(Connection::SocketConnectionPtr connection)
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


