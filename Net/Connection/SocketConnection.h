#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#include "Buffer/KfifoBuffer.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <event.h>

struct timeval;
namespace Processor
{
    typedef boost::function<void ()> Job;
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

    class SocketConnection;
    typedef boost::shared_ptr<SocketConnection> SocketConnectionPtr;
    typedef boost::function<void (int, SocketConnectionPtr)> Watcher;

    class SocketConnection
    {
    public:
        SocketConnection(
            ProtocolInterface* theProtocol,
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor, 
            evutil_socket_t theFd);
        ~SocketConnection();

        //interface for reactor 
        int asynRead(int theFd, short theEvt);
        int asynWrite(int theFd, short theEvt);

        //interface for upper protocol
        void close();
        inline bool isClose() {return statusM == CloseE;}

        size_t getInput(char* const theBuffer, const size_t theLen);
        size_t getnInput(char* const theBuffer, const size_t theLen);
        Net::Buffer::BufferStatus sendn(char* const theBuffer, const size_t theLen);

        void setLowWaterMarkWatcher(Watcher* theWatcher);

    private:
        friend class boost::function<void ()>;
        void addReadEvent();
        void addWriteEvent();
        void onRead(int theFd, short theEvt);
        void onWrite(int theFd, short theEvt);
        void _close();
        void _release();


    private:
        //for reactor:
        //  this is enough
        //  we ensure after delete there is no msg from the libevent
        //for uppper protocol:
        //  selfM should be applied
        //  in case there is a msg after connection is delete
        SocketConnectionPtr selfM;

        struct event* readEvtM;
        struct event* writeEvtM;

        ProtocolInterface* protocolM;
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;

        evutil_socket_t fdM;

        //we ensure there is only 1 thread read/write the input queue
        //boost::mutex inputQueueMutexM;
        Net::Buffer::KfifoBuffer inputQueueM;
        boost::mutex outputQueueMutexM;
        Net::Buffer::KfifoBuffer outputQueueM;

        enum Status{ActiveE = 0, CloseE = 1};
        mutable int statusM;

        boost::mutex stopReadingMutexM;
        bool stopReadingM;
        boost::mutex watcherMutexM;
        Watcher* watcherM;

    };

}
}

#endif /*SOCKETCONNECTION_H*/

