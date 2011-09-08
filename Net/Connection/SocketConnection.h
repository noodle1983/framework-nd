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
        SocketConnection(Processor::BoostProcessor* theProcessor, const int theFd);
        ~SocketConnection();
        
        //executed in processor[fd]
        void onRead(int theFd, short theEvt);
        void onWrite(int theFd, short theEvt);
        void release();

        //executed in processor[0]
        int asynAddEvent(struct event* theEvt, const struct timeval* theTimeout);
        int asynDelEvent(struct event* theEvt);
        void asynClose();
        void close();

    public:
        struct event readEvtM;
        struct event writeEvtM;
        
    private:
        Processor::BoostProcessor* processorM;
        int fdM;
        std::list<Buffer*> outputQueueM;

    };
    
}
}

#endif /*SOCKETCONNECTION_H*/

