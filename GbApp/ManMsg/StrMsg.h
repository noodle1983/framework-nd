#ifndef STRMSG_H
#define STRMSG_H

#include <string.h>
#include <string>
#include "IntMsg.h"

namespace GbApp
{
namespace Msg
{
    template<char theTag>
    class TlvString 
    {
    public:
        TlvString(){}
        ~TlvString(){}

        enum {TAG = theTag};

        void init()
        {
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theBuffer[theIndex] != TAG)
                return -1;
            theIndex++;

            Uint16 length;
            length.decode(theBuffer, theLen, theIndex);
            if (theIndex + strln > theLen)
                return -1;

            valueM.assign(theBuffer + theIndex, length.valueM);
            theIndex += length.valueM;
            return 0;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int totalLen = 2 + valueM.length();
            if (theIndex + totalLen > theLen)
                return -1;

            theBuffer[theIndex++] = TAG;
            Uint16 length;
            length.valueM = valueM.length();
            length.encode(theBuffer, theLen, theIndex);
            memcpy(theBuffer + theIndex, valueM.c_str(), valueM.length());
            theIndex += valueM.length();

            return 0;
        }

    public:
        std::string valueM;
    };
//    typedef TlvString<0> OptionString;

}
}
#endif  /*STRMSG_H*/


