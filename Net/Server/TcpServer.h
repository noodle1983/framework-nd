#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Protocol.h"

namespace Net
{
namespace Server{

//    template <typename Protocol>
    class TcpServer
    {
    public:
        TcpServer(const int thePort);
        virtual ~TcpServer();

        int start();
        void onAccept(int theFd, short theEvent);

    private:

        int portM;
        int fdM;
    };

} /* Server */
} /* Net */

#endif /*TCPSERVER_H*/

