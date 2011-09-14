#ifndef ECHOPROTOCOL_H
#define ECHOPROTOCOL_H

#include "Protocol.h"

namespace Processor
{
    class BoostProcessor;
}
namespace Net
{
    namespace Reactor
    {
        class Reactor;
    }
    namespace Connection
    {
        class SocketConnection;
    }
namespace Protocol
{

    class EchoProtocol:public Net::ProtocolInterface
    {
    public:
        EchoProtocol(
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor);
        ~EchoProtocol();

        int asynHandleInput(int theFd, Connection::SocketConnection* connection);
    private:
        int handleInput(Connection::SocketConnection* connection);
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;
    };

}
}

#endif /* ECHOPROTOCOL_H */

