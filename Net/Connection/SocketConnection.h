#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H
#include <list>
#include <event.h>

struct timeval;
namespace Processor
{
    class BoostProcessor;
}

namespace Net{

namespace Reactor
{
    class Reactor;
}

namespace Connection{

    struct Buffer 
    {
        Buffer(): lenM(0), offsetM(0)
        {}
        unsigned lenM;
        unsigned offsetM;
        char rawM[1024];
    };

    class SocketConnection
    {
    public:
        SocketConnection(
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor, 
            evutil_socket_t theFd);
        ~SocketConnection();

        
        int asynRead(int theFd, short theEvt);
        int asynWrite(int theFd, short theEvt);
        void release();
        void onRead(int theFd, short theEvt);
        void onWrite(int theFd, short theEvt);

        void close();

    public:
        struct event* readEvtM;
        struct event* writeEvtM;
        
    private:
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;
        evutil_socket_t fdM;
        std::list<Buffer*> outputQueueM;

    };
    
}
}

#endif /*SOCKETCONNECTION_H*/

