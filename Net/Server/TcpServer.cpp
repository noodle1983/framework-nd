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

void on_read(int theFd, short theEvt, void *theArg)
{
    SocketConnection* connection = (SocketConnection*)theArg;
    connection->onRead(theFd, theEvt);
}

//-----------------------------------------------------------------------------

void on_write(int theFd, short theEvt, void *theArg)
{
    SocketConnection* connection = (SocketConnection*)theArg;
    connection->onWrite(theFd, theEvt);
}

//-----------------------------------------------------------------------------

int setnonblock(int theFd)
{
    int flags;

    flags = fcntl(theFd, F_GETFL);
    if (flags < 0)
        return flags;
    flags |= O_NONBLOCK;
    if (fcntl(theFd, F_SETFL, flags) < 0)
        return -1;

    return 0;
}

//-----------------------------------------------------------------------------
//TcpServer
//-----------------------------------------------------------------------------

//template <typename Protocol>
TcpServer::TcpServer(Processor::BoostProcessor* theProcessor)
    : processorM(theProcessor)
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

//template <typename Protocol>
void TcpServer::onAccept(int theFd, short theEvt)
{
    int clientFd;
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    clientFd = accept(theFd, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientFd < 0) 
    {
        warn("accept failed");
        return;
    }

    if (setnonblock(clientFd) < 0)
    {
        warn("failed to set client socket non-blocking");
        return;
    }

    SocketConnection* connection = new SocketConnection(clientFd);
    event_set(&connection->readEvtM, clientFd, EV_READ|EV_PERSIST, on_read, connection);
    event_set(&connection->writeEvtM, clientFd, EV_WRITE|EV_PERSIST, on_write, connection);
    asynAddEvent(&connection->readEvtM, NULL);

    printf("Accepted connection from %s\n", 
        inet_ntoa(clientAddr.sin_addr));
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

    //set socket option
    int reuseaddr = 1;
    setsockopt(fdM, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    if (setnonblock(fdM) < 0)
        err(1, "failed to set server socket to non-blocking");

    //add event
    event_set(&acceptEvtM, fdM, EV_READ|EV_PERSIST, 
            on_accept, this);
    asynAddEvent(&acceptEvtM, NULL);
    return 0;    
}

//-----------------------------------------------------------------------------

void TcpServer::stop()
{
    if (fdM)
    {
        close(fdM);
        event_del(&acceptEvtM);
        fdM = 0;
    }
}


//-----------------------------------------------------------------------------

int TcpServer::asynAddEvent(struct event* theEvt, const struct timeval* theTimeout)
{
    return processorM->process(0, new Processor::Job(boost::bind(event_add, theEvt, theTimeout)));
}

//-----------------------------------------------------------------------------

int TcpServer::asynDelEvent(struct event* theEvt)
{
    return processorM->process(0, new Processor::Job(boost::bind(event_del, theEvt)));
}

