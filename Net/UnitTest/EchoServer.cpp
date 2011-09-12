#include <event.h>
#include <TcpServer.h>
#include <BoostProcessor.h>
#include <Reactor.h>


int main()
{
    event_init();

    Processor::BoostProcessor processor(4);
    processor.start();
	Net::Reactor::Reactor reactor;
    Net::Server::TcpServer server(&reactor, &processor);
    server.startAt(5555);
    reactor.start();
	while(1){};
    processor.stop();
    return 0;
}

