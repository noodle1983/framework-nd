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
    Net::Buffer::BufferStatus iBufferStatus = connection->getInput(buffer, sizeof(buffer));
    Net::Buffer::BufferStatus oBufferStatus = connection->send(buffer, sizeof(buffer));
    while (Net::Buffer::BufferOkE == iBufferStatus 
		&& Net::Buffer::BufferOkE == oBufferStatus)
    {
        iBufferStatus = connection->getInput(buffer, sizeof(buffer));
		oBufferStatus = connection->send(buffer, sizeof(buffer));
    }
    return 0;
}



//-----------------------------------------------------------------------------


