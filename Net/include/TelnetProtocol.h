#ifndef TelnetProtocol_h
#define TelnetProtocol_h

#include "Protocol.h"

#include <list>
#include <map>
#include <string>

namespace Processor
{
	class BoostProcessor;
}

namespace Net
{
namespace Protocol
{
	class ICmdHandler;
	class TelnetProtocol;
	typedef std::list<std::string> CmdArgsList;
	typedef std::list<ICmdHandler*> CmdHandlerStack;
	typedef std::map<int, CmdHandlerStack> Con2CmdStack;
	typedef std::map<std::string, ICmdHandler*> CmdMap;

	class ICmdHandler
	{
	public:
		ICmdHandler(){}
		virtual ~ICmdHandler(){}
		virtual int handle(CmdArgsList& theArgs) = 0;
		virtual const char* getUsage() = 0;
	};

	class TelnetTopCmd: public ICmdHandler
	{
		TelnetTopCmd();
		~TelnetTopCmd();

		int registCmd(
			const std::string& theCmdName,
			ICmdHandler* theHandler);
		int handle(CmdArgsList& theArgs);

		const char* getUsage() {return "";};
	private:
		CmdMap allTopCmdsM;
	};

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
		Con2CmdStack con2CmdStackM;
    };

}
}

#endif

