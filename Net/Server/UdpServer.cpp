#include "BoostProcessor.h"
#include "UdpServer.h"
#include "Reactor.h"
#include "Protocol.h"
#include "Log.h"

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <err.h>


using namespace Net::Server;

//-----------------------------------------------------------------------------

void on_udp_server_read(int theFd, short theEvt, void *theArg)
{
    UdpServer* server = (UdpServer*)theArg;
    server->asynRead(theFd, theEvt);
}

//-----------------------------------------------------------------------------

UdpServer::UdpServer(
            IUdpProtocol* theProtocol,
            Reactor::Reactor* theReactor,
            Processor::BoostProcessor* theProcessor)
    : selfM(this)
    , readEvtM(NULL)
    , protocolM(theProtocol)
    , reactorM(theReactor)
    , processorM(theProcessor)
    , statusM(CloseE)
    , stopReadingM(false)
{
}

//-----------------------------------------------------------------------------

UdpServer::~UdpServer()
{
    evutil_closesocket(fdM);
}

//-----------------------------------------------------------------------------

int UdpServer::startAt(const int thePort)
{
    fdM = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in myAddr;
    evutil_make_socket_nonblocking(fdM);
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(thePort);
    myAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(fdM, (struct sockaddr*)&myAddr, sizeof(myAddr))<0) 
    {
        FATAL("bind failed");
        exit(-1);
    }
    readEvtM = reactorM->newEvent(fdM, EV_READ, on_udp_server_read, this);
    statusM = ActiveE;
    addReadEvent();
    return 0;
}

//-----------------------------------------------------------------------------

void UdpServer::addReadEvent()
{
    if (CloseE == statusM)
        return;
    if (-1 == event_add(readEvtM, NULL))
    {
        processorM->process(fdM, new Processor::Job(boost::bind(&UdpServer::addReadEvent, this)));
    }
}

//-----------------------------------------------------------------------------

int UdpServer::asynRead(int theFd, short theEvt)
{
    return processorM->process(fdM, new Processor::Job(boost::bind(&UdpServer::onRead, this, theFd, theEvt)));
}

//-----------------------------------------------------------------------------

void UdpServer::onRead(int theFd, short theEvt)
{
    Net::UdpPacket packet;

    while(inputQueueM.isHealthy())
    {
        packet.contentLen = recvfrom(fdM, (void*)packet.content, sizeof(packet.content), 0,
                                 (struct sockaddr*)&packet.peerAddr, &packet.addrlen);
        if (packet.contentLen < 0 )
        {
            break;
        }
        inputQueueM.put((char*)&packet.addrlen, sizeof(packet.addrlen));
        inputQueueM.put((char*)&packet.peerAddr, packet.addrlen);
        inputQueueM.put((char*)&packet.contentLen, sizeof(packet.contentLen));
        int putLen = inputQueueM.put(packet.content, packet.contentLen);
        assert(putLen == packet.contentLen);
    }

    if (!inputQueueM.isHealthy())
    {
        boost::lock_guard<boost::mutex> lock(stopReadingMutexM);
        stopReadingM = true;
    }
    else
    {
        addReadEvent();
    }
    protocolM->asynHandleInput(fdM, selfM);
}
//-----------------------------------------------------------------------------

bool UdpServer::getAPackage(Net::UdpPacket* thePackage)
{
    if (CloseE == statusM)
        return false;

    unsigned len = 0;
    len = inputQueueM.getn((char*)&thePackage->addrlen, sizeof(thePackage->addrlen));
    if (0 == len)
    {
        return false;
    }
    if (thePackage->addrlen > sizeof(thePackage->peerAddr))
    {
        FATAL("internal error");
        exit(-1);
    }

    len = inputQueueM.getn((char*)&thePackage->peerAddr, thePackage->addrlen);
    if (0 == len)
    {
        FATAL("internal error");
        exit(-1);
    }

    len = inputQueueM.getn((char*)&thePackage->contentLen, sizeof(thePackage->contentLen));
    if (0 == len || thePackage->contentLen > Net::UdpPacket::MAX_UDP_PACKAGE)
    {
        FATAL("internal error");
        exit(-1);
    }
    if (thePackage->contentLen > 0)
    {
        len = inputQueueM.getn(thePackage->content, thePackage->contentLen);
        if (0 == len)
        {
            FATAL("internal error");
            exit(-1);
        }
    }
    if (stopReadingM)
    {
        Net::Buffer::BufferStatus postBufferStatus = inputQueueM.getStatus();
        if (postBufferStatus == Net::Buffer::BufferLowE)
        {
            {
                boost::lock_guard<boost::mutex> lock(stopReadingMutexM);
                stopReadingM = false;
            }
            processorM->process(fdM, new Processor::Job(boost::bind(&UdpServer::addReadEvent, selfM)));
        }
    }
    return len;
}

//-----------------------------------------------------------------------------

bool UdpServer::sendAPackage(Net::UdpPacket* thePackage)
{
    if (CloseE == statusM || NULL == thePackage)
        return false;

    int len = 0;
    {
        boost::lock_guard<boost::mutex> lock(outputQueueMutexM);
        len = sendto(fdM, thePackage->content, thePackage->contentLen, 0, 
                (sockaddr*)&thePackage->peerAddr, thePackage->addrlen);
    }
    if (len < 0)
    {
        WARN("Socket failure[" << errno << "]:" << strerror(errno));
    }
    return (len > 0);
}

//-----------------------------------------------------------------------------

void UdpServer::close()
{
    if (CloseE == statusM)
        return;
    processorM->process(fdM, new Processor::Job(boost::bind(&UdpServer::_close, this)));
}

//-----------------------------------------------------------------------------

void UdpServer::_release()
{
    selfM.reset();
}

//-----------------------------------------------------------------------------

void UdpServer::_close()
{
    if (CloseE == statusM)
        return;
    statusM = CloseE;
    reactorM->delEvent(readEvtM);
    processorM->process(fdM, new Processor::Job(boost::bind(&UdpServer::_release, this)));
}

//-----------------------------------------------------------------------------


