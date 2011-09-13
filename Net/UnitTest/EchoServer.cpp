#include <event.h>
#include <event2/thread.h>
#include <TcpServer.h>
#include <BoostProcessor.h>
#include <Reactor.h>
#include <unistd.h>

int main()
{
    evthread_use_pthreads();
    Processor::BoostProcessor processor(1);
    processor.start();
	Net::Reactor::Reactor reactor;
    Net::Server::TcpServer server(&reactor, &processor);
    server.startAt(5555);
    reactor.start();
	while(1){sleep(1);};
    processor.stop();
    return 0;
}

