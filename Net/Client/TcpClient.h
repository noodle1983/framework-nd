#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "Connection/SocketConnection.h"
#include <string>
#include <boost/shared_ptr.hpp>


namespace Net
{
namespace Client
{
    class TcpClient
    {
    public:
        TcpClient( 
            IProtocol* theProtocol,
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor);
        ~TcpClient();

        /**
         * connect in a async way.
         * return:
         *         -1 : there is an error.
         *         0  : connected or blocked
         */
        int connect(const std::string& thePeerAddr, const int thePeerPort);

        Net::Buffer::BufferStatus sendn(char* const theBuffer, const size_t theLen);


        /**
         * onXxx
         * Description: TcpClient will be notified if it is xxx.
         */
        void onConnected();
        void onError();

    private:
        IProtocol* protocolM;
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;
        
        std::string peerAddrM;
        int peerPortM;

        int confReconTimesM;
        int confReconIntervelM;
        int leftReconTimesM;
        int nextReconIntervelM;

        Net::Connection::SocketConnectionPtr connectionM;
    };

    inline Net::Buffer::BufferStatus 
    TcpClient::sendn(char* const theBuffer, const size_t theLen)
    {
        if (connectionM.get())    
        {
            return connectionM->sendn(theBuffer, theLen);
        }
        else
        {
            return Net::Buffer::BufferNotEnoughE; 
        }

    }
}
}

#endif /* TCPCLIENT_H */

