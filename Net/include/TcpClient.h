#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "SocketConnection.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <event.h>


namespace Net
{
class IClientProtocol;
namespace Client
{
    class TcpClient
    {
    public:
        TcpClient(
            IClientProtocol* theProtocol,
            Reactor::Reactor* theReactor,
            Processor::BoostProcessor* theProcessor);
        ~TcpClient();

        /**
         * connect in a async way.
         * return:
         *         -1 : there is an error.
         *         0  : connected or blocked
         */
        int connect();
        int close();
        bool isClose(){return !connectionM.get() || connectionM->isClose();}
        bool isConnected(){return !connectionM.get() || isConnectedM;}
        unsigned sendn(char* const theBuffer, const unsigned theLen);


        /**
         * onXxx
         * Description: TcpClient will be notified if it is xxx.
         */
        void onConnected(int theFd, Connection::SocketConnectionPtr theConnection);
        void onError();

        static void checkConnecting(int theFd, short theEvt, void *theArg);
    private:
        void startConnectTimer();
        int _connect();

        IClientProtocol* protocolM;
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;

        std::string peerAddrM;
        int peerPortM;

        int confReconTimesM;
        int confReconIntervelM;
        int leftReconTimesM;
        int nextReconIntervelM;
        mutable bool isClosedM;
        mutable bool isConnectedM;
        mutable struct event* connectTimerM;

        Net::Connection::SocketConnectionPtr connectionM;
    };

    inline unsigned
    TcpClient::sendn(char* const theBuffer, const unsigned theLen)
    {
        if (isConnectedM && connectionM.get())
        {
            return connectionM->sendn(theBuffer, theLen);
        }
        else
        {
            return 0;
        }

    }
}
}

#endif /* TCPCLIENT_H */

