#include <BoostProcessor.h>
#include <TcpClient.h>
#include <Reactor.h>
#include <Protocol.h>
#include <Log.h>

#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>
#include <event2/util.h>
#include <iostream>
#include <string.h>
using namespace std;

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


int main()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, sig_stop);
    signal(SIGINT, sig_stop);

    int     sockfd;
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5555);
    char* addrStr = "127.0.0.1";
    evutil_inet_pton(AF_INET, addrStr, &servaddr.sin_addr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int wlen = sendto(sockfd, addrStr, strlen(addrStr), 0, (sockaddr*)&servaddr, sizeof(servaddr));
    assert(wlen > 0);
    char buffer[1024];
    int rLen = 0;
    rLen = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    buffer[rLen] = '\0';
    cout << "send:" << addrStr << " with len:" << strlen(addrStr) << endl;
    cout << "recv:" << buffer  << " with len:" << rLen << endl;
    cout << "stop" << endl;

    return 0;
}

