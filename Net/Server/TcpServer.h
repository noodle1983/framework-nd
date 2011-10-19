#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <event.h>


namespace Processor
{
    class BoostProcessor;
}

namespace Net
{
class ProtocolInterface;
namespace Reactor
{
    class Reactor;
}
namespace Server{

    class TcpServer
    {
    public:
        TcpServer(
            ProtocolInterface* theProtocol,
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor);
        virtual ~TcpServer();

        void addAcceptEvent();
        int startAt(const int thePort);
        void stop();

        int asynAccept(int theFd, short theEvt);
        void onAccept(int theFd, short theEvt);

    private:
        ProtocolInterface* protocolM;
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;

        struct event* acceptEvtM;
        int portM;
        evutil_socket_t fdM;
    };

} /* Server */
} /* Net */

#endif /*TCPSERVER_H*/

