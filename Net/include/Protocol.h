#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <boost/bind.hpp>

namespace Net
{
    namespace Connection
    {
        class SocketConnection;
        typedef boost::shared_ptr<SocketConnection> SocketConnectionPtr;
    }
    enum ProtocolAccept
    {
        REJECTE = -1,
        ACCEPTE = 0,
        BUFFERE = 1
    };
    class IProtocol
    {
    public:
        virtual ~IProtocol() {};
        virtual int asynHandleInput(int theFd, Connection::SocketConnectionPtr connection) = 0;
    };

}

#endif /*PROTOCOL_H*/

