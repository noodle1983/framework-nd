#include "QuitHandler.h"
#include "TelnetCmdManager.h"

using namespace Net::Protocol;
//-----------------------------------------------------------------------------

void QuitHandler::handle(TelnetCmdManager* theManager, CmdArgsList& theArgs)
{
    theManager->exitCurCmd();
}

