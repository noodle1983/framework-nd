#include <BoostProcessor.h>
#include <Server/TcpServer.h>
#include <Reactor/Reactor.h>
#include <Protocol/EchoProtocol.h>

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>

static int closed = false;
static boost::mutex closedMutexM;
static boost::condition_variable closedCondM;
void sigint(int)
{
    printf("stopping...\n");
    boost::lock_guard<boost::mutex> lock(closedMutexM);
    closed = true;
    closedCondM.notify_one();
}

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("failed to fork!\n");
        return -1;
    }
    else if (pid > 0)
    {
        printf("server start with pid %d.\n", pid);
        return 0;
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGINT, sigint);
    evthread_use_pthreads();
    Processor::BoostProcessor processor(4);
    processor.start();
	Net::Reactor::Reactor reactor;
    Net::Protocol::EchoProtocol echoProtocol(&reactor, &processor);
    Net::Server::TcpServer server(&echoProtocol, &reactor, &processor);
    server.startAt(5555);
    reactor.start();
    
    boost::unique_lock<boost::mutex> lock(closedMutexM);
	while(!closed)
    {
        closedCondM.wait(lock);
    }

    processor.stop();
    reactor.stop();
    return 0;
}

