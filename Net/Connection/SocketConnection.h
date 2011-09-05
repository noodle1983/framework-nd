#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H
#include <list>
#include <event.h>

namespace Net{
namespace Connection{

    struct Buffer 
    {
        Buffer(): lenM(0), offsetM(0)
        {}
        unsigned lenM;
        unsigned offsetM;
        char rawM[1024];
    };

    class SocketConnection
    {
    public:
        SocketConnection(const int theFd);
        ~SocketConnection();
        void onRead(int theFd, short theEvt);
        void onWrite(int theFd, short theEvt);

    public:
        struct event readEvtM;
        struct event writeEvtM;
        
    private:
        int fdM;
        std::list<Buffer*> outputQueueM;

    };
    
}
}

#endif /*SOCKETCONNECTION_H*/

