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
    Net::Connection::Buffer *buffer = NULL;
    connection->getInput(buffer);
    while (buffer)
    {
        connection->send(buffer);
        connection->getInput(buffer);
    }
    return 0;
}



//-----------------------------------------------------------------------------


