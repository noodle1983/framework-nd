#ifndef PROTOCOL_H
#define PROTOCOL_H


namespace Net
{
    enum ProtocolAccept
    {
        REJECTE = -1,
        ACCEPTE = 0,
        BUFFERE = 1
    };
    class Protocol
    {
    public:
        virtual ~Protocol() = 0;

        virtual ProtocolAccept accept(const char* const theString, const size_t theLen) = 0;
        virtual int handleInput(const char* const theString, const size_t theLen) = 0;
        virtual int handleOutput(const char* const theString, const size_t theLen) = 0;
        virtual int handleError(const char* const theString, const size_t theLen) = 0;
    };

}

#endif /*PROTOCOL_H*/

