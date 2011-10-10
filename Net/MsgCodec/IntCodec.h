#ifndef INTCODEC_H
#define INTCODEC_H

#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

namespace Net
{
namespace MsgCodec
{

    class CodecInterface
    {
    public:
        virtual ~CodecInterface()
        {
            if (nextCodecM)
                delete nextCodecM;
        }
        virtual void init(void* theMsg) = 0;
        virtual int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex, void* theMsg) = 0;
        virtual int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex, void* theMsg) = 0;

    protected:
        CodecInterface(CodecInterface* theNextCodec){ nextCodecM = theNextCodec;}
        CodecInterface* nextCodecM;
        
    };

    template<typename MsgType>
    class Uint32Codec : public CodecInterface
    {
    public:
        Uint32Codec(uint32_t MsgType::*thePointer, CodecInterface* theNextCodec)
            : CodecInterface(theNextCodec)
            , pFieldM(thePointer)
        {
        }

        ~Uint32Codec(){}

        void init(void* theMsg)
        {
            ((MsgType*)theMsg)->*pFieldM = 0;
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex, void* theMsg)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return -1;

            MsgType* msg = (MsgType*)theMsg;
            memcpy(&(msg->*pFieldM), theBuffer + theIndex, sizeof(uint32_t));
            msg->*pFieldM = ntohl(msg->*pFieldM);
            theIndex += sizeof(uint32_t);
            if (NULL == nextCodecM)
                return 0;

            return nextCodecM->decode(theBuffer, theLen, theIndex, theMsg);
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex, void* theMsg)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return -1;

            MsgType* msg = (MsgType*)theMsg;
            uint32_t netInt = htonl(msg->*pFieldM);
            memcpy(theBuffer + theIndex, &netInt, sizeof(uint32_t));
            theIndex += sizeof(uint32_t);
            if (NULL == nextCodecM)
                return 0;

            return nextCodecM->encode(theBuffer, theLen, theIndex, theMsg);
        }


    private:
        uint32_t MsgType::*pFieldM;

    };

}
}
#endif  /*INTCODEC_H*/

