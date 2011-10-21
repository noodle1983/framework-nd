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
    class IProtocol
    {
    public:
        virtual ~IProtocol() {};

        /**
         *
         * interface: asynHandleInput
         * Description: the net framework will notify the protocol object the input event,
         *         For the performance, Protocol should handle the input in another thread.
         * the Args:
         *         theFd: which socket the input is from 
         *         connection: the socket connection which can be write to
         *
         */
        virtual int asynHandleInput(int theFd, Connection::SocketConnectionPtr theConnection) = 0;
    };

    class IClientProtocol: public IProtocol
    {
    public:
        virtual ~IClientProtocol() {};

        /**
         *
         * interface: asynHandleInput
         * Description: the net framework will notify the protocol object the input event,
         *         For the performance, Protocol should handle the input in another thread.
         * the Args:
         *         theFd: which socket the input is from 
         *         connection: the socket connection which can be write to
         *
         */
        virtual int onConnected(int theFd, Connection::SocketConnectionPtr theConnection) {return 0;}
    };


}

#endif /*PROTOCOL_H*/

