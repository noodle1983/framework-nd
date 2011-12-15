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
		virtual int handle(TelnetCmdManager* theManager, CmdArgsList& theArgs) = 0;
		virtual const char* getUsage() = 0;
		virtual const char* getPrompt() = 0;
	};

}
}

#endif

