#include <BoostProcessor.h>
#include <Server/TcpServer.h>
#include <Reactor/Reactor.h>
#include <Protocol/EchoProtocol.h>

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    evthread_use_pthreads();
    Processor::BoostProcessor processor(4);
    processor.start();
	Net::Reactor::Reactor reactor;
    Net::Protocol::EchoProtocol echoProtocol(&reactor, &processor);
    Net::Server::TcpServer server(&echoProtocol, &reactor, &processor);
    server.startAt(5555);
    reactor.start();
	while(1){sleep(1);};
    processor.stop();
    return 0;
}

