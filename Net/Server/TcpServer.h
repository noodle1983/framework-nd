#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <event.h>

#include "Protocol.h"


struct timeval;
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
namespace Server{

//    template <typename Protocol>
    class TcpServer
    {
    public:
        TcpServer(
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor);
        virtual ~TcpServer();

        int startAt(const int thePort);
        void stop();

        //make sure the job executed in processor[fd]
        int asynAccept(int theFd, short theEvt);
        void onAccept(int theFd, short theEvt);

    private:
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;
        
        struct event* acceptEvtM;
        int portM;
        evutil_socket_t fdM;
    };

} /* Server */
} /* Net */

#endif /*TCPSERVER_H*/

