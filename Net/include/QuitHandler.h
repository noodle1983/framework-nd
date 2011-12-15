#ifndef QUITHANDLER_H
#define QUITHANDLER_H

#include "ICmdHandler.h"

namespace Net
{
namespace Protocol
{
    class QuitHandler: public ICmdHandler
    {
	public:
		QuitHandler(){}
		virtual ~QuitHandler(){}
        //////////////////desc intent///////////////////////////////////////|
		virtual const char* getDesc() {return "quit/exit/q                   exit current cmd.";};
		virtual const char* getPrompt(){return "";};
		virtual void handle(TelnetCmdManager* theManager, CmdArgsList& theArgs);

    };
}
}

#endif /* QUITHANDLER_H */

