#ifndef UDPPACKET_H
#define UDPPACKET_H
#include <event.h>

namespace Net
{
    struct UdpPacket
    {
        UdpPacket()
            :addrlen(sizeof(struct sockaddr_in))
        {}
        enum { MAX_UDP_PACKAGE = 4096 };
        ev_socklen_t addrlen;
        struct sockaddr_in peerAddr;
        int contentLen;
        char content[MAX_UDP_PACKAGE];
    };
}
#endif /* UDPPACKET_H */

