#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#include <Buffer/KfifoBuffer.h>

#include <event.h>

struct timeval;
namespace Processor
{
    class BoostProcessor;
}

namespace Net{

class ProtocolInterface;
namespace Reactor
{
    class Reactor;
}

namespace Connection{

    struct Buffer 
    {
        Buffer(): lenM(0), offsetM(0)
        {}
        int lenM;
        int offsetM;
        char rawM[1024];
    };

    class SocketConnection
    {
    public:
        SocketConnection(
            ProtocolInterface* theProtocol,
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor, 
            evutil_socket_t theFd);
        ~SocketConnection();

        
        int asynRead(int theFd, short theEvt);
        int asynWrite(int theFd, short theEvt);
        void onRead(int theFd, short theEvt);
        void onWrite(int theFd, short theEvt);

        void close();
        void release();

        int getInput(Buffer*& theBuffer);
        int send(Buffer* theBuffer);

    public:
        struct event* readEvtM;
        struct event* writeEvtM;
        
    private:
        ProtocolInterface* protocolM;
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;
        evutil_socket_t fdM;
        Buffer::KfifoBuffer inputQueueM;
        Buffer::KfifoBuffer outputQueueM;

        enum Status{ActiveE, CloseE};
        mutable int statusM;
    };
    
}
}

#endif /*SOCKETCONNECTION_H*/

