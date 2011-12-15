#ifndef TELNETCMDMANAGER_H
#define TELNETCMDMANAGER_H

#include "Protocol.h"
#include "SocketConnection.h"
#include "ICmdHandler.h"

#include <list>
#include <map>
#include <string>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

namespace Processor
{
	class BoostProcessor;
}

namespace Net
{
namespace Protocol
{
	typedef std::list<ICmdHandler*> CmdHandlerStack;
	typedef std::map<std::string, ICmdHandler*> CmdMap;

	class TelnetCmdManager
	{
	public:
		TelnetCmdManager(const struct sockaddr_in& thePeerAddr,
				Connection::SocketConnectionPtr theConnection);
		~TelnetCmdManager();

		bool validate(const sockaddr_in& thePeerAddr);

		static int registCmd(
			const std::string& theCmdName,
			ICmdHandler* theHandler);
		int handleInput();
        int handleCmd(const unsigned theStart, const unsigned theEnd);
        void send(const char* const theStr, unsigned theLen);
        void sendPrompt(); 

	private:
		static CmdMap allTopCmdsM;
        static boost::shared_mutex topCmdMutexM;

		CmdHandlerStack subCmdStackM;
        char cmdBufferM[256];
        unsigned bufferLenM;

		struct sockaddr_in peerAddrM;
		Connection::SocketConnectionWPtr connectionM;
	};
}
}

#endif

