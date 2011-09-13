#include <sys/types.h>  
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "TcpServer.h"
#include "SocketConnection.h"
#include "BoostProcessor.h"
#include "Reactor.h"
using namespace Net::Server;
using namespace Net::Connection;

//-----------------------------------------------------------------------------
//libevent
//-----------------------------------------------------------------------------
void on_accept(int theFd, short theEvt, void *theArg)
{
    TcpServer* server = (TcpServer*)theArg;
    server->onAccept(theFd, theEvt);
}

//-----------------------------------------------------------------------------
//TcpServer
//-----------------------------------------------------------------------------

//template <typename Protocol>
TcpServer::TcpServer(Reactor::Reactor* theReactor, Processor::BoostProcessor* theProcessor)
    : reactorM(theReactor)
    , processorM(theProcessor)
    , acceptEvtM(NULL)
    , portM(0)
    , fdM(0)
{
}

//-----------------------------------------------------------------------------

//template <typename Protocol>
TcpServer::~TcpServer()
{
    stop();
}

//-----------------------------------------------------------------------------

int TcpServer::asynAccept(int theFd, short theEvt)
{
    return processorM->process(fdM, new Processor::Job(boost::bind(&TcpServer::onAccept, this, theFd, theEvt)));
}

//-----------------------------------------------------------------------------

//template <typename Protocol>
void TcpServer::onAccept(int theFd, short theEvt)
{
    evutil_socket_t clientFd;
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    clientFd = accept(theFd, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientFd < 0) 
    {
        warn("accept failed");
        return;
    }

    if (evutil_make_socket_nonblocking(clientFd) < 0)
    {
        warn("failed to set client socket non-blocking");
        return;
    }

    SocketConnection* connection = new SocketConnection(reactorM, processorM, clientFd);

    printf("Accepted connection from %s, fd:%d\n", 
        inet_ntoa(clientAddr.sin_addr), clientFd);
}

//-----------------------------------------------------------------------------

//template <typename Protocol>
int TcpServer::startAt(const int thePort)
{
    portM = thePort;

    //new a socket
    fdM = socket(AF_INET, SOCK_STREAM, 0);
    if (fdM < 0)
    {
        err(1, "listen failed");
        return -1;
    }
    //set socket option
    if (evutil_make_listen_socket_reuseable(fdM) < 0)
		err(1, "failed to set server socket to reuseable");
    if (evutil_make_socket_nonblocking(fdM) < 0)
        err(1, "failed to set server socket to non-blocking");

    //bind local addr
    struct sockaddr_in listenAddr;
    memset(&listenAddr, 0, sizeof(listenAddr));
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_addr.s_addr = INADDR_ANY;
    listenAddr.sin_port = htons(portM);
    if (bind(fdM, (struct sockaddr *)&listenAddr,
        sizeof(listenAddr)) < 0)
    {
        err(1, "bind failed");
        return -1;
    }

    //listen
    if (listen(fdM, 5) < 0)
    {
        err(1, "listen failed");
        return -1;
    }

	acceptEvtM = reactorM->newEvent(fdM, EV_READ|EV_PERSIST, on_accept, this);
    event_add(acceptEvtM, NULL);
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

