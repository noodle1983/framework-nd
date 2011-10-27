#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "BoostProcessor.h"
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
        IProtocol(Processor::BoostProcessor* theProcessor)
            : processorM(theProcessor)
        {
        }
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
        int asynHandleInput(int theFd, Connection::SocketConnectionPtr theConnection)
        {
            return processorM->process(theFd + 1,
                    new Processor::Job(boost::bind(&IProtocol::handleInput, this, theConnection)));
        }
        
        virtual int handleInput(Net::Connection::SocketConnectionPtr theConnection) = 0;
    private:
        Processor::BoostProcessor* processorM;
    };

    class IClientProtocol: public IProtocol
    {
    public:
        IClientProtocol(Processor::BoostProcessor* theProcessor)
            :IProtocol(theProcessor)
        {
        }
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

