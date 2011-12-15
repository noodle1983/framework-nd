#include "TelnetProtocol.h"
#include "ConfigCenter.h"
#include "SocketConnection.h"

using namespace Net;
using namespace Net::Protocol;
using namespace Config;

//-----------------------------------------------------------------------------
TelnetCmdManager::TelnetCmdManager(const struct sockaddr_in& thePeerAddr,
				Connection::SocketConnectionPtr theConnection)
	: peerAddrM(thePeerAddr)
	, connectionM(theConnection)
{

}

//-----------------------------------------------------------------------------

TelnetCmdManager::~TelnetCmdManager()
{
}

//-----------------------------------------------------------------------------

bool TelnetCmdManager::validate(const sockaddr_in& thePeerAddr)
{
	return thePeerAddr.sin_family == peerAddrM.sin_family
		&& thePeerAddr.sin_port == peerAddrM.sin_port
		&& thePeerAddr.sin_addr.s_addr == peerAddrM.sin_addr.s_addr;
}

//-----------------------------------------------------------------------------

int TelnetCmdManager::registCmd(
	const std::string& theCmdName,
	ICmdHandler* theHandler)
{
	return 0;
}

//-----------------------------------------------------------------------------

int TelnetCmdManager::handle()
{
	return 0;
}


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
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
	TelnetCmdManager* cmdManager = NULL;
	Con2CmdManagerMap::iterator it = con2CmdManagerMapM.find(theConnection->getFd());
	if (it == con2CmdManagerMapM.end())
	{
		cmdManager = new TelnetCmdManager(theConnection->getPeerAddr(), theConnection);		
		con2CmdManagerMapM[theConnection->getFd()] = cmdManager;
	}
	else if (!it->second->validate(theConnection->getPeerAddr()))
	{
		delete it->second;
		cmdManager = new TelnetCmdManager(theConnection->getPeerAddr(), theConnection);		
		it->second = cmdManager;
	}
	else
	{
		cmdManager = it->second;
	}

	cmdManager->handle();
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

