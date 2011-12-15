#ifndef TELNETPROTOCOL_H
#define TELNETPROTOCOL_H

#include "Protocol.h"
#include "SocketConnection.h"
#include "ICmdHandler.h"
#include "TelnetCmdManager.h"

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
	typedef std::map<int, TelnetCmdManager*> Con2CmdManagerMap;

    class TelnetProtocol : public Net::IProtocol
    {
    public:
		TelnetProtocol(Processor::BoostProcessor* theProcessor);
        TelnetProtocol();
        ~TelnetProtocol();

        void handleInput(Connection::SocketConnectionPtr theConnection);
        void handleClose(Net::Connection::SocketConnectionPtr theConnection); 
        void handleConnected(Connection::SocketConnectionPtr theConnection);

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
    private:
		Con2CmdManagerMap con2CmdManagerMapM;
        boost::shared_mutex manMapMutexM;
    };

}
}

#endif

