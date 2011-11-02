#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "KfifoBuffer.h"
#include "UdpPacket.h"

#include <boost/thread.hpp>
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

class IUdpProtocol;
namespace Reactor
{
    class Reactor;
}

namespace Server
{

    class UdpServer;
    typedef boost::shared_ptr<UdpServer> UdpServerPtr;

    class UdpServer
    {
    public:
        UdpServer(
            IUdpProtocol* theProtocol,
            Reactor::Reactor* theReactor,
            Processor::BoostProcessor* theProcessor);
        ~UdpServer();


        //interface for reactor
        int asynRead(int theFd, short theEvt);
        int asynWrite(int theFd, short theEvt);

        //interface for upper protocol
        int startAt(const int thePort);
        void close();
        inline bool isClose() {return statusM == CloseE;}
        bool getAPackage(Net::UdpPacket* thePackage);
        bool sendAPackage(Net::UdpPacket* thePackage);



    private:
        friend class boost::function<void ()>;
        void addReadEvent();
        void onRead(int theFd, short theEvt);
        void _close();
        void _release();


    private:
        UdpServerPtr selfM;

        struct event* readEvtM;

        IUdpProtocol* protocolM;
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;

        evutil_socket_t fdM;

        //we ensure there is only 1 thread read/write the input queue
        //boost::mutex inputQueueMutexM;
        Net::Buffer::KfifoBuffer inputQueueM;
        boost::mutex outputQueueMutexM;

        enum Status{ActiveE = 0, CloseE = 1};
        mutable int statusM;

        boost::mutex stopReadingMutexM;
        bool stopReadingM;

    };

}
}

#endif /*UDPSERVER_H*/

