#ifndef ICMDHANDLER_H
#define ICMDHANDLER_H

#include <list>
#include <string>

namespace Net
{
namespace Protocol
{
    class TelnetCmdManager;
	typedef std::list<std::string> CmdArgsList;
	class ICmdHandler
	{
	public:
		ICmdHandler(){}
		virtual ~ICmdHandler(){}
		virtual void handle(TelnetCmdManager* theManager, CmdArgsList& theArgs) = 0;
        //////////////////desc intent///////////////////////////////////////|
		virtual const char* getDesc() = 0;
		virtual const char* getPrompt() = 0;
	};

}
}

#endif

