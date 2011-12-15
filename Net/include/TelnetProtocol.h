#ifndef TelnetProtocol_h
#define TelnetProtocol_h

#include "Protocol.h"
#include "SocketConnection.h"

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
	typedef std::list<std::string> CmdArgsList;

	class ICmdHandler
	{
	public:
		ICmdHandler(){}
		virtual ~ICmdHandler(){}
		virtual int handle(CmdArgsList& theArgs) = 0;
		virtual const char* getUsage() = 0;
		virtual const char* getPrompt() = 0;
	};
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

