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
    namespace Server
    {
        class UdpServer;
        typedef boost::shared_ptr<UdpServer> UdpServerPtr;
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

        //Config
        virtual const std::string getAddr(){ return "0.0.0.0"; }
        virtual int getPort(){ return 5460; }
        virtual int getRBufferSizePower(){ return 20; }
        virtual int getWBufferSizePower(){ return 20; }
        
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

        virtual const std::string getAddr(){ return "127.0.0.1"; }
    };

    class IUdpProtocol
    {
    public:
        IUdpProtocol(Processor::BoostProcessor* theProcessor)
            : processorM(theProcessor)
        {
        }
        virtual ~IUdpProtocol() {};

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
        int asynHandleInput(int theFd, Server::UdpServerPtr theUdpServer)
        {
            return processorM->process(theFd,
                    new Processor::Job(boost::bind(&IUdpProtocol::handleInput, this, theUdpServer)));
        }
        
        virtual int handleInput(Net::Server::UdpServerPtr theUdpServer) = 0;
        
        //Config
        virtual int getRBufferSizePower(){ return 20; }
    private:
        Processor::BoostProcessor* processorM;
    };

}

#endif /*PROTOCOL_H*/

