#ifndef INTMSG_H
#define INTMSG_H

#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

namespace GbApp
{
namespace Msg
{

    class Uint32
    {
    public:
        Uint32(){}
        ~Uint32(){}

        void init()
        {
            valueM = 0;
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return -1;

            memcpy(&valueM, theBuffer + theIndex, sizeof(uint32_t));
            valueM = ntohl(valueM);
            theIndex += sizeof(uint32_t);
            return 0;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(uint32_t) > theLen)
                return -1;

            uint32_t netInt = htonl(valueM);
            memcpy(theBuffer + theIndex, &netInt, sizeof(uint32_t));
            theIndex += sizeof(uint32_t);

            return 0;
        }

    public:
        uint32_t valueM;
    };

}
}
#endif  /*INTMSG_H*/

