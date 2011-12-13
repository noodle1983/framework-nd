#include "TelnetProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"

using namespace Net::Protocol;
using namespace Config;

//-----------------------------------------------------------------------------

TelnetProtocol::TelnetProtocol(Processor::BoostProcessor* theProcessor)
	:IProtocol(theProcessor)
{
}

//-----------------------------------------------------------------------------

TelnetProtocol::~TelnetProtocol()
{
}

//-----------------------------------------------------------------------------

void TelnetProtocol::handleInput(Connection::SocketConnectionPtr theConnection)
{
	CmdHandlerStack& handlerStack = con2CmdStackM[theConnection->getFd()];
	return ;
}

//-----------------------------------------------------------------------------

const std::string TelnetProtocol::getAddr()
{
    return ConfigCenter::instance()->get("cmd.addr", "0.0.0.0");
}

//-----------------------------------------------------------------------------

int TelnetProtocol::getPort()
{
    return ConfigCenter::instance()->get("cmd.port", 7510);
}

//-----------------------------------------------------------------------------

int TelnetProtocol::getRBufferSizePower()
{
    return 12;
}

//-----------------------------------------------------------------------------

int TelnetProtocol::getWBufferSizePower()
{
    return 13;
}

//-----------------------------------------------------------------------------

