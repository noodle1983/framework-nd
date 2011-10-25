#include <BoostProcessor.h>
#include <Client/TcpClient.h>
#include <Reactor.h>
#include <Protocol.h>
#include <Log.h>

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

static int closed = false;
static boost::mutex closedMutexM;
static boost::condition_variable closedCondM;
void sig_stop(int sig)
{
    DEBUG("receive signal " << sig << ". stopping...");
    boost::lock_guard<boost::mutex> lock(closedMutexM);
    closed = true;
    closedCondM.notify_one();
}

class BatchDataProtocol: public Net::IClientProtocol
{
public:
    enum{TEST_TIMES = 1024 * 512};
    BatchDataProtocol()
        : proProcessorM(1)
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
        tcpClientM.connect("127.0.0.1", 5555);
    }

    int onConnected(int theFd, Net::Connection::SocketConnectionPtr theConnection)
    {
        struct timeval tv;
        evutil_gettimeofday(&tv, NULL);
        starttimeM = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        asynSend(theFd, theConnection);
        return 0;
    }

    int asynSend(int theFd, Net::Connection::SocketConnectionPtr theConnection)
    {
        return proProcessorM.process(theFd + 1,
                new Processor::Job(boost::bind(&BatchDataProtocol::send, this, theFd, theConnection)));
    }

    int send(int theFd, Net::Connection::SocketConnectionPtr theConnection)
    {
        Net::Buffer::BufferStatus oBufferStatus = theConnection->sendn(NULL, 0);
        while (Net::Buffer::BufferOkE == oBufferStatus || Net::Buffer::BufferLowE == oBufferStatus)
        {
            char* buffer = bufferM[wBufferCountM%10];
            oBufferStatus = theConnection->sendn(buffer, 1024);
            if (Net::Buffer::BufferNotEnoughE != oBufferStatus)
            {
                wBufferCountM++;
                //std::cout << "write buffer count:" << wBufferCountM << std::endl;
                if (wBufferCountM == TEST_TIMES)
                {
                    return 0;
                }
            }
        }
        if (Net::Buffer::BufferHighE == oBufferStatus || Net::Buffer::BufferNotEnoughE == oBufferStatus)
        {
            theConnection->setLowWaterMarkWatcher(
                    new Net::Connection::Watcher(boost::bind(&BatchDataProtocol::asynSend, this, _1, _2)));
        }
        return 0;
    }

    int asynHandleInput(int theFd, Net::Connection::SocketConnectionPtr theConnection)
    {
        return proProcessorM.process(theFd + 1,
                new Processor::Job(boost::bind(&BatchDataProtocol::handleInput, this, theConnection)));
    }

    int handleInput(Net::Connection::SocketConnectionPtr theConnection)
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
            for (unsigned i = 0; i < len; i++)
            {
                unsigned offset = readIndexM + i;
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
                return 0;
            }
        }
        return 0;
    }

private:
    Net::Reactor::Reactor reactorM;
    Processor::BoostProcessor proProcessorM;
    Processor::BoostProcessor netProcessorM;
    Net::Client::TcpClient tcpClientM;
    unsigned wBufferCountM;
    unsigned readIndexM;
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

    DEBUG("TestClient stopped.");
    return 0;
}

