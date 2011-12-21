#ifndef ICMDHANDLER_H
#define ICMDHANDLER_H

#include <list>
#include <string>

#include <assert.h>

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

        //stateful cmd, define all or not to define any
        virtual void* newCmdSessionData(){assert(0); return NULL;}
        virtual void freeCmdSessionData(void* theData){assert(0);} 
		virtual void handle(TelnetCmdManager* theManager, CmdArgsList& theArgs, void* theSessionData) {assert(0);}
	};

}
}

#endif

