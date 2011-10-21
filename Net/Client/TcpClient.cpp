#include "TcpClient.h"
#include "Connection/SocketConnection.h"
#include "Protocol.h"
#include "Log.h"

#include <event.h>
#include <sys/types.h> 
#include <sys/socket.h>

using namespace Net::Client;

#ifndef WIN32
/* True iff e is an error that means an connect can be retried. */
#define ERR_CONNECT_RETRIABLE(e)         \
        ((e) == EINTR || (e) == EINPROGRESS)

#else

#define ERR_CONNECT_RETRIABLE(e)                 \
        ((e) == WSAEWOULDBLOCK ||                   \
                (e) == WSAEINTR ||                      \
                (e) == WSAEINPROGRESS ||                    \
                (e) == WSAEINVAL)

#endif

//-----------------------------------------------------------------------------

TcpClient::TcpClient(
            IClientProtocol* theProtocol,
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor)
    : protocolM(theProtocol)
    , reactorM(theReactor)
    , processorM(theProcessor)
{
    confReconTimesM = -1;
    confReconIntervelM = 5;

    leftReconTimesM = -1;
    nextReconIntervelM = 0;
}

//-----------------------------------------------------------------------------

TcpClient::~TcpClient()
{
	close();
}

//-----------------------------------------------------------------------------

int TcpClient::close()
{
    if (connectionM.get())    
    {
        connectionM->close();
        connectionM.reset();
    }
	return 0;
}

//-----------------------------------------------------------------------------

int TcpClient::connect(const std::string& thePeerAddr, const int thePeerPort)
{
    peerAddrM = thePeerAddr;
    peerPortM = thePeerPort;

    evutil_socket_t sock;
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    evutil_inet_pton(AF_INET, peerAddrM.c_str(), &sin.sin_addr);
    sin.sin_port = htons(peerPortM);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERROR("failed to do socket(AF_INET,...)!");
        return -1;
    }
    if (evutil_make_socket_nonblocking(sock) < 0)
    {
        ERROR("failed to make socket monblocking!");
        evutil_closesocket(sock);
        return -1;
    }
    if (::connect(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0) 
    {
        int e = errno;
        if (! ERR_CONNECT_RETRIABLE(e)) 
        {
            WARN("failed to connect to " << peerAddrM
                    << ":" << peerPortM
                    << ", errstr:" << evutil_socket_error_to_string(e));
            evutil_closesocket(sock);
            onError();
            return -1;
        }
    }
    Net::Connection::SocketConnection* connection = 
        new Net::Connection::SocketConnection(protocolM, reactorM, processorM, sock, this);
    connectionM = connection->self();
    return 0;

}

//-----------------------------------------------------------------------------

void TcpClient::onConnected(int theFd, Connection::SocketConnectionPtr theConnection)
{
    DEBUG("connected to " << peerAddrM 
            << ":" << peerPortM);
	protocolM->onConnected(theFd, theConnection);
}

//-----------------------------------------------------------------------------

void TcpClient::onError()
{
    WARN("connection lost from " << peerAddrM 
            << ":" << peerPortM);
    connectionM.reset();
	//reconnect
}

//-----------------------------------------------------------------------------

