#include "BoostProcessor.h"
#include "EchoProtocol.h"
#include "SocketConnection.h"
#include "Reactor.h"
#include "ConfigCenter.h"

using namespace Net::Protocol;
using namespace Config;

//-----------------------------------------------------------------------------
EchoProtocol::EchoProtocol(
        Reactor::Reactor* theReactor,
        Processor::BoostProcessor* theProcessor)
    : IProtocol(theProcessor) 
    , reactorM(theReactor)
    , processorM(theProcessor)
{
}

//-----------------------------------------------------------------------------

EchoProtocol::~EchoProtocol()
{
}

//-----------------------------------------------------------------------------

void EchoProtocol::handleInput(Connection::SocketConnectionPtr connection)
{
    char buffer[1024];
    unsigned len = 1;
	bool canWrite = true;
    while (len > 0 && (canWrite = connection->isWBufferHealthy()))
    {
        len = connection->getInput(buffer, sizeof(buffer));
        connection->sendn(buffer, len);
    }
    if (!canWrite)
    {
        connection->setLowWaterMarkWatcher(new Net::Connection::Watcher(boost::bind(&EchoProtocol::asynHandleInput, this, _1, _2)));
    }
}

//-----------------------------------------------------------------------------

const std::string EchoProtocol::getAddr()
{
    return ConfigCenter::instance()->get("echo.s.addr", "0.0.0.0");
}

//-----------------------------------------------------------------------------

int EchoProtocol::getPort()
{
    return ConfigCenter::instance()->get("echo.s.port", 5460);
}

//-----------------------------------------------------------------------------

int EchoProtocol::getRBufferSizePower()
{
    return ConfigCenter::instance()->get("echo.s.rpower", 20);
}

//-----------------------------------------------------------------------------

int EchoProtocol::getWBufferSizePower()
{
    return ConfigCenter::instance()->get("echo.s.wpower", 20);
}

//-----------------------------------------------------------------------------



