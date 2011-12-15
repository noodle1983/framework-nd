#include "BoostProcessor.h"
#include "TcpClient.h"
#include "Reactor.h"
#include "Protocol.h"
#include "Log.h"
#include "ConfigCenter.h"

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

using namespace Config;

static int closed = false;
static boost::mutex closedMutexM;
static boost::condition_variable closedCondM;
void sig_stop(int sig)
{
    LOG_DEBUG("receive signal " << sig << ". stopping...");
    boost::lock_guard<boost::mutex> lock(closedMutexM);
    closed = true;
    closedCondM.notify_one();
}

const uint64_t TEST_TIMES = 1024 * 1024 * 1/2;
class BatchDataProtocol: public Net::IClientProtocol
{
public:
    BatchDataProtocol()
        : IClientProtocol(&netProcessorM)
        , proProcessorM(1)
        , netProcessorM(1)
        , tcpClientM(this, &reactorM, &netProcessorM)
        , wBufferCountM(0)
        , readIndexM(0)
    {
        proProcessorM.start();
        netProcessorM.start();
        reactorM.start();
        for (int i = 0; i < 10; i++)
        {
            memset(bufferM[i], '0' + i, 1024);
        }
    }

    ~BatchDataProtocol()
    {
        proProcessorM.stop();
        netProcessorM.stop();
        reactorM.stop();
    }

    void startTest()
    {
        tcpClientM.connect();
    }

    void handleConnected(Net::Connection::SocketConnectionPtr theConnection)
    {
        struct timeval tv;
        evutil_gettimeofday(&tv, NULL);
        starttimeM = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        asynSend(theConnection->getFd(), theConnection);
    }

    int asynSend(int theFd, Net::Connection::SocketConnectionPtr theConnection)
    {
        return proProcessorM.process(theFd + 1,
                &BatchDataProtocol::send, this, theFd, theConnection);
    }

    void send(int theFd, Net::Connection::SocketConnectionPtr theConnection)
    {
		bool canWrite = true;
        while ((canWrite = theConnection->isWBufferHealthy()))
        {
            char* buffer = bufferM[wBufferCountM%10];
            unsigned len = theConnection->sendn(buffer, 1024);
            if (len > 0)
            {
                wBufferCountM++;
                //std::cout << "write buffer count:" << wBufferCountM << std::endl;
                if (wBufferCountM == TEST_TIMES)
                {
                    return;
                }
            }
        }
        if (!canWrite)
        {
            theConnection->setLowWaterMarkWatcher(
                    new Net::Connection::Watcher(boost::bind(&BatchDataProtocol::asynSend, this, _1, _2)));
        }
        return;
    }

    void handleInput(Net::Connection::SocketConnectionPtr theConnection)
    {
        char buffer[1024];
        while(1)
        {
            unsigned len = theConnection->getInput(buffer, sizeof(buffer));
            if (0 == len)
            {
                break;
            }
            //std::cout << "read len:" << len << std::endl;
            for (uint64_t i = 0; i < len; i++)
            {
                uint64_t offset = readIndexM + i;
                assert(unsigned(buffer[i]) == ('0' + offset/1024%10));

            }
            readIndexM += len;
            if (readIndexM == (1024 * TEST_TIMES))
            {
                struct timeval tv;
                evutil_gettimeofday(&tv, NULL);
                long long endtime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
                long long interval = endtime - starttimeM;
                std::cout << "send " << TEST_TIMES << " request in "
                    << interval << " msec. tps:" << (TEST_TIMES * 1000 / interval) << std::endl;
                std::cout << "exit" << std::endl;
                tcpClientM.close();
                sig_stop(0);
            }
        }
    }

    const std::string getAddr()
    {
        return ConfigCenter::instance()->get("echo.c.addr", "127.0.0.1");
    }

    int getPort()
    {
        return ConfigCenter::instance()->get("echo.c.port", 5460);
    }

    int getRBufferSizePower()
    {
        return ConfigCenter::instance()->get("echo.c.rpower", 20);
    }

    int getWBufferSizePower()
    {
        return ConfigCenter::instance()->get("echo.c.wpower", 20);
    }

private:
    Net::Reactor::Reactor reactorM;
    Processor::BoostProcessor proProcessorM;
    Processor::BoostProcessor netProcessorM;
    Net::Client::TcpClient tcpClientM;
    uint64_t wBufferCountM;
    uint64_t readIndexM;
    char bufferM[10][1024];
    long long starttimeM;
};

int main()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, sig_stop);
    signal(SIGINT, sig_stop);
    evthread_use_pthreads();

    BatchDataProtocol singleDataProtocol;
    singleDataProtocol.startTest();

    boost::unique_lock<boost::mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }

    LOG_DEBUG("TestClient stopped.");
    return 0;
}

