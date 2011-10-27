#include <BoostProcessor.h>
#include <TcpClient.h>
#include <Reactor.h>
#include <Protocol.h>
#include <Log.h>

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>

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

class SingleDataProtocol: public Net::IClientProtocol
{
public:
    SingleDataProtocol()
        : IClientProtocol(&netProcessorM)
        , proProcessorM(1)
        , netProcessorM(1)
        , tcpClientM(this, &reactorM, &netProcessorM)
    {
        proProcessorM.start();
        netProcessorM.start();
        reactorM.start();
    }

    ~SingleDataProtocol()
    {
        proProcessorM.stop();
        netProcessorM.stop();
        reactorM.stop();
    }

    void startTest()
    {
        tcpClientM.connect("127.0.0.1", 5555);
        tcpClientM.sendn("Hello", 5);
    }

    int handleInput(Net::Connection::SocketConnectionPtr theConnection)
    {
        char buffer[1024];
        unsigned len = 1;
        len = theConnection->getInput(buffer, sizeof(buffer));
        assert(len == strlen("Hello"));
        assert(0 == memcmp("Hello", buffer, strlen("Hello")));
        tcpClientM.close();
        sig_stop(0);
        return 0;
    }

private:
    Net::Reactor::Reactor reactorM;
    Processor::BoostProcessor proProcessorM;
    Processor::BoostProcessor netProcessorM;
    Net::Client::TcpClient tcpClientM;
};

int main()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, sig_stop);
    signal(SIGINT, sig_stop);
    evthread_use_pthreads();

    SingleDataProtocol singleDataProtocol;
    singleDataProtocol.startTest();

    boost::unique_lock<boost::mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }

    DEBUG("TestClient stopped.");
    return 0;
}

