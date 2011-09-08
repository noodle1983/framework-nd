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
namespace Server{

//    template <typename Protocol>
    class TcpServer
    {
    public:
        TcpServer(Processor::BoostProcessor* theProcessor);
        virtual ~TcpServer();

        int startAt(const int thePort);
        void stop();
        void onAccept(int theFd, short theEvent);

        int asynAddEvent(struct event* theEvt, const struct timeval* theTimeout);
        int asynDelEvent(struct event* theEvt);

    private:
        Processor::BoostProcessor* processorM;
        struct event acceptEvtM;
        int portM;
        int fdM;
    };

} /* Server */
} /* Net */

#endif /*TCPSERVER_H*/

