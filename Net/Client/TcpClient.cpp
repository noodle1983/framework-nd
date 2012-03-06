#include "TcpClient.h"
#include "SocketConnection.h"
#include "Protocol.h"
#include "Log.h"
#include "Reactor.h"
#include "ConfigCenter.h"

#include <event.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace Net::Client;
using namespace Config;

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
    , isClosedM(0)
    , isConnectedM(0)
{
}

//-----------------------------------------------------------------------------

TcpClient::~TcpClient()
{
    close();
}

//-----------------------------------------------------------------------------

int TcpClient::close()
{
    isClosedM = true;
    isConnectedM = false;

    boost::lock_guard<boost::mutex> lock(connectionMutexM);
    if (connectionM.get())
    {
        connectionM->rmClient();
        connectionM->close();
        connectionM.reset();
    }
    return 0;
}

//-----------------------------------------------------------------------------

int TcpClient::connect()
{
    boost::lock_guard<boost::mutex> lock(connectionMutexM);
    //connect
    peerAddrM = protocolM->getAddr();
    peerPortM = protocolM->getPort();

    if (isClosedM)
    {
        LOG_ERROR("TcpClient to[" << peerAddrM << ":" << peerPortM << "is already closed");
        return -1;
    }
    LOG_DEBUG("connecting to " << peerAddrM
            << ":" << peerPortM);

    //init attr
    isConnectedM = false;

    evutil_socket_t sock;
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    evutil_inet_pton(AF_INET, peerAddrM.c_str(), &sin.sin_addr);
    sin.sin_port = htons(peerPortM);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LOG_ERROR("failed to do socket(AF_INET,...)!"
                    << ", errstr:" << evutil_socket_error_to_string(errno));
        return -1;
    }
    if (evutil_make_socket_nonblocking(sock) < 0)
    {
        LOG_ERROR("failed to make socket monblocking!"
                    << ", errstr:" << evutil_socket_error_to_string(errno));
        evutil_closesocket(sock);
        return -1;
    }
    if (::connect(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        int e = errno;
        if (! ERR_CONNECT_RETRIABLE(e))
        {
            LOG_WARN("failed to connect to " << peerAddrM
                    << ":" << peerPortM
                    << ", errstr:" << evutil_socket_error_to_string(e));
            evutil_closesocket(sock);
            onError();
            return -1;
        }
    }
    else
    {
        //connected
        isConnectedM = true;
    }
    Net::Connection::SocketConnection* connection =
        new Net::Connection::SocketConnection(protocolM, reactorM, processorM, sock, this);
    connectionM = connection->self();
    if (!isConnectedM)
    {
        connectionM->addClientTimer(protocolM->getReConnectInterval());
    }
    return 0;
}

//-----------------------------------------------------------------------------

void TcpClient::onConnected(int theFd, Connection::SocketConnectionPtr theConnection)
{
    LOG_DEBUG("connected to " << peerAddrM
            << ":" << peerPortM);
    {
        boost::lock_guard<boost::mutex> lock(connectionMutexM);
        isConnectedM = true;
    }
    protocolM->asynHandleConnected(theFd, theConnection);
}

//-----------------------------------------------------------------------------

void TcpClient::onError()
{
    LOG_WARN("connection lost from " << peerAddrM
            << ":" << peerPortM);
    {
        boost::lock_guard<boost::mutex> lock(connectionMutexM);
        connectionM.reset();
        //reconnect
        isConnectedM = false;
    }
    if (!isClosedM)
    {
        connect();
    }
}

//-----------------------------------------------------------------------------

void TcpClient::onClientTimeout()
{
    if (isClosedM)
    {
        return;
    }
    if (!isConnectedM)
    {
        connect();
    }
}

//-----------------------------------------------------------------------------

