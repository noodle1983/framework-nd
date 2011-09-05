#include <event.h>
#include <TcpServer.h>


int main()
{
    event_init();
    Net::Server::TcpServer server(5555);
    server.start();
    event_dispatch();
    return 0;
}

