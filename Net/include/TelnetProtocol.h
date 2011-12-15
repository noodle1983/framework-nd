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
		int handle(CmdArgsList& theArgs);
		int handle();

		const char* getUsage() {return "";};
	private:
		static CmdMap allTopCmdsM;
		CmdHandlerStack subCmdStackM;
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

        virtual const std::string getAddr();
        virtual int getPort();
        virtual int getRBufferSizePower();
        virtual int getWBufferSizePower();
    private:
		Con2CmdManagerMap con2CmdManagerMapM;
    };

}
}

#endif

