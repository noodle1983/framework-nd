#ifndef STRMSG_H
#define STRMSG_H

#include <string.h>
#include <string>
#include "IntMsg.h"
#include "MsgErrorCode.h"

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
        operator std::string()
        { return valueM; }
        const TlvString& operator =(const std::string& theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            Uint8 tag;
            ret = tag.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;
            if (tag.valueM != TAG)
                return ERROR_E;

            Uintx length;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;
            if (theIndex + length.valueM > theLen)
                return NOT_ENOUGH_BUFFER_E;

            valueM.assign(theBuffer + theIndex, length.valueM);
            theIndex += length.valueM;
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            Uintx length;
            length.valueM = valueM.length();
            int totalLen = sizeof(length.valueM) + valueM.length();
            if (theIndex + totalLen > theLen)
                return NOT_ENOUGH_BUFFER_E;

            theBuffer[theIndex++] = TAG;
            length.encode(theBuffer, theLen, theIndex);
            memcpy(theBuffer + theIndex, valueM.c_str(), valueM.length());
            theIndex += valueM.length();

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            if (valueM.length() > 32)
            {
                theOut << valueM.substr(0, 32) << "..." << "len:" << valueM.length();
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
        operator std::string()
        { return valueM; }
        const PlainString& operator =(const std::string& theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theLen < theIndex)
                return NOT_ENOUGH_BUFFER_E;
            valueM.assign(theBuffer + theIndex, theLen - theIndex);
            theIndex = theLen;
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int totalLen = valueM.length();
            if (theIndex + totalLen > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(theBuffer + theIndex, valueM.c_str(), valueM.length());
            theIndex += valueM.length();

            return SUCCESS_E;
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


