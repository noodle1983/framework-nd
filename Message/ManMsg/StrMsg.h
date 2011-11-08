#ifndef STRMSG_H
#define STRMSG_H

#include <string.h>
#include <string>
#include "IntMsg.h"

namespace Msg
{
    template<char theTag, typename Uintx>
    class TlvString
    {
    public:
        TlvString(const std::string& theContent)
            : valueM(theContent)
        {
        }
        TlvString(){}
        ~TlvString(){}

        enum {TAG = theTag};
        enum {MIN_BYTES = Uintx::MIN_BYTES + 1};

        void init()
        {
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theBuffer[theIndex] != TAG)
                return -1;
            theIndex++;

            Uintx length;
            length.decode(theBuffer, theLen, theIndex);
            if (theIndex + length.valueM > theLen)
                return -1;

            valueM.assign(theBuffer + theIndex, length.valueM);
            theIndex += length.valueM;
            return 0;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            Uintx length;
            length.valueM = valueM.length();
            int totalLen = sizeof(length.valueM) + valueM.length();
            if (theIndex + totalLen > theLen)
                return -1;

            theBuffer[theIndex++] = TAG;
            length.encode(theBuffer, theLen, theIndex);
            memcpy(theBuffer + theIndex, valueM.c_str(), valueM.length());
            theIndex += valueM.length();

            return 0;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            if (valueM.length() > 16)
            {
                theOut << valueM.substr(0, 16) << "..." << "len:" << valueM.length();
            }
            else
            {
                theOut << valueM;
            }
            return theOut;
        }
    public:
        std::string valueM;
    };

    /**
     * PlainString will eat up all the string up to the tail
     */
    class PlainString
    {
    public:
        PlainString(const std::string& theContent)
            : valueM(theContent)
        {
        }
        PlainString(){}
        ~PlainString(){}

        enum {MIN_BYTES = 0};

        void init()
        {
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theLen < theIndex)
                return -1;
            valueM.assign(theBuffer + theIndex, theLen - theIndex);
            theIndex = theLen;
            return 0;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int totalLen = valueM.length();
            if (theIndex + totalLen > theLen)
                return -1;

            memcpy(theBuffer + theIndex, valueM.c_str(), valueM.length());
            theIndex += valueM.length();

            return 0;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            if (valueM.length() > 16)
            {
                theOut << valueM.substr(0, 16) << "..." << "len:" << valueM.length();
            }
            else
            {
                theOut << valueM;
            }
            return theOut;
        }
    public:
        std::string valueM;
    };
}
#endif  /*STRMSG_H*/


