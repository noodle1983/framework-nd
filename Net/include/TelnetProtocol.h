#ifndef TelnetProtocol_h
#define TelnetProtocol_h

#include <list>
#include <map>
#include <string>

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
		CmdHandler(){}
		virtual ~CmdHandler(){}
		virtual int handle(CmdArgsList& theArgs) = 0;
		virtual const string& getUsage() = 0;
	};

	class TelnetTopCmd: public CmdHandler
	{
		TelnetTopCmd();
		~TelnetTopCmd();

		int registCmd(
			const std::string& theCmdName,
			ICmdHandler* theHandler);
		int handle(CmdArgsList& theArgs);

		const string& getUsage() {return "";};
	private:
		CmdMap allTopCmdsM;
	};

    class TelnetProtocol:public Net::IProtocol
    {
    public:
        TelnetProtocol();
        ~TelnetProtocol();

        int handleInput(Connection::SocketConnectionPtr connection);

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

