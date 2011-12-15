#include "TelnetCmdManager.h"
#include "SocketConnection.h"
#include "Log.h"

using namespace Net;
using namespace Net::Protocol;

bool TelnetCmdManager::isTopCmdsMInitedM = false;
CmdMap TelnetCmdManager::allTopCmdsM;
boost::shared_mutex TelnetCmdManager::topCmdMutexM;
QuitHandler TelnetCmdManager::quitHandlerM;
//-----------------------------------------------------------------------------

void TelnetCmdManager::initTopCmd()
{
    if (!isTopCmdsMInitedM)
    {
        boost::unique_lock<boost::shared_mutex> lock(topCmdMutexM);
        if(!isTopCmdsMInitedM)
        {
            allTopCmdsM["quit"] = &quitHandlerM;
            allTopCmdsM["exit"] = &quitHandlerM;
            allTopCmdsM["q"] = &quitHandlerM;
            isTopCmdsMInitedM = true;
        }
    }
}

//-----------------------------------------------------------------------------
TelnetCmdManager::TelnetCmdManager(const struct sockaddr_in& thePeerAddr,
				Connection::SocketConnectionPtr theConnection)
	: bufferLenM(0)
    , peerAddrM(thePeerAddr)
	, connectionM(theConnection)
{
    initTopCmd();
}

//-----------------------------------------------------------------------------

TelnetCmdManager::~TelnetCmdManager()
{
}

//-----------------------------------------------------------------------------

bool TelnetCmdManager::validate(const sockaddr_in& thePeerAddr)
{
	return thePeerAddr.sin_family == peerAddrM.sin_family
		&& thePeerAddr.sin_port == peerAddrM.sin_port
		&& thePeerAddr.sin_addr.s_addr == peerAddrM.sin_addr.s_addr;
}

//-----------------------------------------------------------------------------

void TelnetCmdManager::registCmd(
	const std::string& theCmdName,
	ICmdHandler* theHandler)
{
}

//-----------------------------------------------------------------------------

void TelnetCmdManager::send(const char* const theStr, unsigned theLen)
{
    Net::Connection::SocketConnectionPtr connection = connectionM.lock();
    if (connection.get())
    {
        connection->sendn(theStr, theLen);
    }
}

//-----------------------------------------------------------------------------

void TelnetCmdManager::sendPrompt()
{
    Net::Connection::SocketConnectionPtr connection = connectionM.lock();
    if (connection.get())
    {
        const char* const prompt = subCmdStackM.empty() ? "> "
            : subCmdStackM.back()->getPrompt();
        connection->sendn(prompt, strlen(prompt));
    }
}

//-----------------------------------------------------------------------------

int TelnetCmdManager::handleCmd(const unsigned theStart, const unsigned theEnd)
{
    CmdArgsList argsList;
    //trim
    unsigned start = theStart;
    unsigned end = theEnd;
    while (' ' == cmdBufferM[start] ||
           '\t' == cmdBufferM[start] ||
           '\r' == cmdBufferM[start] ||
           '\n' == cmdBufferM[start])
    {
        start++;
        if (start >= theEnd)
        {
            break;
        }
    }
    while (' ' == cmdBufferM[end - 1] ||
           '\t' == cmdBufferM[end - 1] ||
           '\r' == cmdBufferM[end - 1] ||
           '\n' == cmdBufferM[end - 1])
    {
        end--;
        if (end <= start)
        {
            break;
        }
    }

    if (start < end)    
    {
        LOG_DEBUG("handle telnet cmd:" << std::string(cmdBufferM + start, cmdBufferM + end));
    }

    unsigned argsStart = start;
    unsigned i = start;
    for (; i  < end; i++)
    {
        if (' ' == cmdBufferM[i] ||
            '\t' == cmdBufferM[i])
        {
            if (argsStart < i)
                argsList.push_back(std::string(cmdBufferM + argsStart, cmdBufferM + i));
            argsStart = i + 1;
        }

    }
    if (argsStart < i)
    {
        argsList.push_back(std::string(cmdBufferM + argsStart, cmdBufferM + i));
    }
    
    if (argsList.empty())
    {
        sendPrompt();
        return 0;
    }

    // the cmdHandler is not thread-safe, 
    // please make sure it is valid until the man Processor stops
    std::string cmd = argsList.front();
    argsList.pop_front();
    ICmdHandler* cmdHandler = NULL; 
    {
        boost::shared_lock<boost::shared_mutex> lock(topCmdMutexM);
        CmdMap::iterator it = allTopCmdsM.find(cmd);
        if (it == allTopCmdsM.end())
        {
            const char* const errstr = "command not found!\n";
            send(errstr, strlen(errstr));
            sendPrompt();
            return 0;
        }
        cmdHandler = it->second;
    }
    cmdHandler->handle(this, argsList);

    return 0;
}

//-----------------------------------------------------------------------------

int TelnetCmdManager::handleInput()
{
    Net::Connection::SocketConnectionPtr connection = connectionM.lock();
    if (NULL == connection.get())
    {
        return -1;
    }
    while (true)
    {
        unsigned getLen = connection->getInput(cmdBufferM + bufferLenM, sizeof(cmdBufferM) - bufferLenM);
        if (0 == getLen)
        {
            break;
        }
        //Ctrl + C
        if (5 == getLen && 0 == memcmp(cmdBufferM + bufferLenM, "\377\364\377\375\006", 5))
        {
            connection->close();
            return 0;
        }
        bufferLenM += getLen;
        unsigned cmdStart = 0;
        for (unsigned i = 0; i < bufferLenM; i++)
        {
            if ('\n' == cmdBufferM[i]) 
            {
                handleCmd(cmdStart, i);
                cmdStart = i + 1;
            }
        }

        //if we have handled a cmd
        if (cmdStart > 0)
        {
            memcpy(cmdBufferM, cmdBufferM + cmdStart, bufferLenM - cmdStart); 
            bufferLenM -= cmdStart;
        }

        //if the cmd is too long
        if (bufferLenM >= sizeof(cmdBufferM))
        {
            //clear buffer
            bufferLenM = 0;
            connection->getInput(NULL, connection->getRBufferSize());
            
            //report error
            const char* const errstr = "cmd is too long!\n";
            connection->sendn(errstr, strlen(errstr));
            sendPrompt();
            return 0;
        }

    }
	return 0;
}

//-----------------------------------------------------------------------------

void TelnetCmdManager::exitCurCmd()
{
    if (!subCmdStackM.empty())
    {
        subCmdStackM.pop_back();
        sendPrompt();
    }
    else
    {
        Net::Connection::SocketConnectionPtr connection = connectionM.lock();
        if (connection.get())
        {
            connection->close();
        }
    }

}

//-----------------------------------------------------------------------------

