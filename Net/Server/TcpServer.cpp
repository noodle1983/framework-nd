#include "BoostProcessor.h"
#include "Server/TcpServer.h"
#include "Connection/SocketConnection.h"
#include "Reactor/Reactor.h"
#include "Log.h"

#include <sys/types.h>  
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

using namespace Net::Server;
using namespace Net::Connection;

//-----------------------------------------------------------------------------
//libevent
//-----------------------------------------------------------------------------
void on_accept(int theFd, short theEvt, void *theArg)
{
    TcpServer* server = (TcpServer*)theArg;
    server->asynAccept(theFd, theEvt);
}

//-----------------------------------------------------------------------------
//TcpServer
//-----------------------------------------------------------------------------

TcpServer::TcpServer(
        ProtocolInterface* theProtocol,
        Reactor::Reactor* theReactor, 
        Processor::BoostProcessor* theProcessor)
    : protocolM(theProtocol)
    , reactorM(theReactor)
    , processorM(theProcessor)
    , acceptEvtM(NULL)
    , portM(0)
    , fdM(0)
{
}

//-----------------------------------------------------------------------------


TcpServer::~TcpServer()
{
    stop();
}

//-----------------------------------------------------------------------------

void TcpServer::addAcceptEvent()
{
    if (-1 == event_add(acceptEvtM, NULL))
    {
        processorM->process(fdM, new Processor::Job(boost::bind(&TcpServer::addAcceptEvent, this)));
    }
}

//-----------------------------------------------------------------------------

int TcpServer::asynAccept(int theFd, short theEvt)
{
    return processorM->process(fdM, new Processor::Job(boost::bind(&TcpServer::onAccept, this, theFd, theEvt)));
}

//-----------------------------------------------------------------------------


void TcpServer::onAccept(int theFd, short theEvt)
{
    evutil_socket_t clientFd;
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    clientFd = accept(theFd, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientFd < 0) 
    {
        WARN("accept failed");
        return;
    }
    while(clientFd >= 0)
    {
        if (evutil_make_socket_nonblocking(clientFd) < 0)
        {
            WARN("failed to set client socket non-blocking");
            return;
        }
        SocketConnection* connection = new SocketConnection(protocolM, reactorM, processorM, clientFd);
        DEBUG("Accepted connection from "<< inet_ntoa(clientAddr.sin_addr) 
                << ", fd:" << clientFd
                << ", con addr:" << std::hex << (size_t)connection); 

        clientFd = accept(theFd, (struct sockaddr *)&clientAddr, &clientLen);
    }
    addAcceptEvent();

}

//-----------------------------------------------------------------------------


int TcpServer::startAt(const int thePort)
{
    portM = thePort;

    //new a socket
    fdM = socket(AF_INET, SOCK_STREAM, 0);
    if (fdM < 0)
    {
        FATAL("listen failed on " << portM);
        exit(-1);
    }
    //set socket option
    if (evutil_make_listen_socket_reuseable(fdM) < 0)
    {
        FATAL("failed to set server socket to reuseable");
        exit(-1);
    }
    if (evutil_make_socket_nonblocking(fdM) < 0)
    {
        FATAL("failed to set server socket to non-blocking");
        exit(-1);
    }

    //bind local addr
    struct sockaddr_in listenAddr;
    memset(&listenAddr, 0, sizeof(listenAddr));
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_addr.s_addr = INADDR_ANY;
    listenAddr.sin_port = htons(portM);
    if (bind(fdM, (struct sockaddr *)&listenAddr,
        sizeof(listenAddr)) < 0)
    {
        FATAL("bind failed");
        exit(-1);
    }

    //listen
    if (listen(fdM, 5) < 0)
    {
        FATAL("listen failed");
        exit(-1);
    }

    acceptEvtM = reactorM->newEvent(fdM, EV_READ, on_accept, this);
    addAcceptEvent();

    DEBUG("Server has been listening at port " << portM);
    return 0;    
}

//-----------------------------------------------------------------------------

void TcpServer::stop()
{
    if (fdM)
    {
        evutil_closesocket(fdM);
        reactorM->delEvent(acceptEvtM);
        fdM = 0;
    }
}

