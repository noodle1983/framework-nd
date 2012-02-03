#include "BoostProcessor.h"
#include "EchoProtocol.h"
#include "SocketConnection.h"
#include "Reactor.h"
#include "ConfigCenter.h"
#include "Log.h"

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
    connection->resetHeartbeatTimeoutCounter();
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

int EchoProtocol::getHeartbeatInterval()
{ 
    return ConfigCenter::instance()->get("echo.s.hbi", 10);
}

//-----------------------------------------------------------------------------

int getMaxHeartbeatTimeout()
{
    return ConfigCenter::instance()->get("echo.s.hbm", 3);
}

//-----------------------------------------------------------------------------

void EchoProtocol::handleHeartbeat(Connection::SocketConnectionPtr theConnection)
{
    int timeoutCounter = theConnection->incHeartbeatTimeoutCounter();
    LOG_TRACE("handleHeartbeat time:" << timeoutCounter);
    if (timeoutCounter >= getMaxHeartbeatTimeout())
    {
        LOG_DEBUG("reach max heartbeat timeout:" << getMaxHeartbeatTimeout());
        theConnection->close();
    }
}

//-----------------------------------------------------------------------------

