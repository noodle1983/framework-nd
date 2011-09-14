#ifndef PROTOCOL_H
#define PROTOCOL_H


namespace Net
{
namespace Connection
{
    class SocketConnection;
}
    enum ProtocolAccept
    {
        REJECTE = -1,
        ACCEPTE = 0,
        BUFFERE = 1
    };
    class ProtocolInterface
    {
    public:
        virtual ~ProtocolInterface() {};
        virtual int asynHandleInput(int theFd, Connection::SocketConnection* connection) = 0;
    };

}

#endif /*PROTOCOL_H*/

