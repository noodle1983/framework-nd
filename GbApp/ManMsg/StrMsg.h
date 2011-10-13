#ifndef STRMSG_H
#define STRMSG_H

#include <string.h>
#include <string>

namespace GbApp
{
namespace Msg
{
    template<char theTag>
    class TlvString 
    {
    public:
        TlvString(){availableM = false;}
        ~TlvString(){}

        enum {TAG = theTag};

        void init()
        {
            availableM = true;
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theBuffer[theIndex] != TAG)
                return -1;
            theIndex++;

            unsigned char strln = theBuffer[theIndex++];
            if (theIndex + strln > theLen)
                return -1;

            valueM.assign(theBuffer + theIndex, strln);
            availableM = true;
            theIndex += strln;
            return 0;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int totalLen = 2 + valueM.length();
            if (theIndex + totalLen > theLen)
                return -1;

            theBuffer[theIndex++] = TAG;
            theBuffer[theIndex++] = valueM.length();
            memcpy(theBuffer + theIndex, valueM.c_str(), valueM.length());
            theIndex += valueM.length();

            return 0;
        }

        bool availableM;
        std::string valueM;
    };
    typedef TlvString<0> OptionString;

}
}
#endif  /*STRMSG_H*/


