#include "HelpHandler.h"
#include "TelnetCmdManager.h"

using namespace Net::Protocol;
//-----------------------------------------------------------------------------

void HelpHandler::handle(TelnetCmdManager* theManager, CmdArgsList& theArgs)
{
    theManager->printUsage();
}

