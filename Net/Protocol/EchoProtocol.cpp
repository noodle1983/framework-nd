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
    while (len > 0 && connection->isWBufferHealthy())
    {
        len = connection->getInput(buffer, sizeof(buffer));
        connection->sendn(buffer, len);
    }
    if (!connection->isWBufferHealthy())
    {
        connection->setLowWaterMarkWatcher(new Net::Connection::Watcher(boost::bind(&EchoProtocol::asynHandleInput, this, _1, _2)));
    }
    return 0;
}



//-----------------------------------------------------------------------------


