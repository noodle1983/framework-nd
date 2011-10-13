#ifndef INTCODEC_H
#define INTCODEC_H

#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

namespace GbApp
{
namespace MsgCodec
{

    class Uint32Codec
    {
    public:
        static inline void init(uint32_t& theField)
        {
            theField = 0;
        }

        static inline int decode(
                const char* theBuffer, 
                const unsigned theLen, 
                unsigned& theIndex, 
                uint32_t& theField)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return -1;

            memcpy(&theField, theBuffer + theIndex, sizeof(uint32_t));
            theField = ntohl(theField);
            theIndex += sizeof(uint32_t);
            return 0;
        }

        static inline int encode(
                char* theBuffer, 
                const unsigned theLen, 
                unsigned& theIndex, 
                const uint32_t& theField)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return -1;

            uint32_t netInt = htonl(theField);
            memcpy(theBuffer + theIndex, &netInt, sizeof(uint32_t));
            theIndex += sizeof(uint32_t);

            return 0;
        }
    };

}
}
#endif  /*INTCODEC_H*/

