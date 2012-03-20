#ifndef TLVBCDSTRING_H
#define TLVBCDSTRING_H

#include "IntMsg.h"
#include "MsgErrorCode.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

namespace Msg
{
    template<char theTag, typename Uintx>
    class TlvBcdString
    {
    public:
        TlvBcdString(const gint64 theValue)
            : valueM(theValue)
        {}
        TlvBcdString()
            : valueM(0)
        {}
        ~TlvBcdString(){}

        enum {TAG = theTag};
        enum {MIN_BYTES = Uintx::MIN_BYTES + 1};

        void init()
        {
        }

        operator guint64()
        { return valueM; }
        const Uint64& operator =(const unsigned long long theValue)
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

            unsigned oriIndex = theIndex;
            theIndex += length.valueM;
            return decodeBcdCode(theBuffer + oriIndex, length.valueM);
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            char bcdStr[32] = {0};
            char msgContent[32];
            memset(msgContent, 0xff, sizeof(msgContent));

            sprintf(bcdStr, "%llu", (long long unsigned int)valueM);
            int bcdStrLen = strlen(bcdStr);
            int msgContentEncIndex = 0;
            int halfbyte = 0;
            for(int i = 0; i < bcdStrLen; i++)
            {
                if (halfbyte)
                {
                    msgContent[msgContentEncIndex] |= (bcdStr[i] - '0') << 4;
                    msgContentEncIndex++;
                    halfbyte = 0;
                }
                else
                {
                    msgContent[msgContentEncIndex] = bcdStr[i] - '0';
                    halfbyte = 1;
                }

            }
            if (halfbyte)
            {
                msgContent[msgContentEncIndex] |= 0xf0;
                msgContentEncIndex++;
                halfbyte = 0;
            }

            //encode
            if (theIndex + MIN_BYTES + msgContentEncIndex > theLen)
                return NOT_ENOUGH_BUFFER_E;

            Uintx length;
            length.valueM = msgContentEncIndex;
            theBuffer[theIndex++] = TAG;
            length.encode(theBuffer, theLen, theIndex);

            memcpy(theBuffer + theIndex, msgContent, msgContentEncIndex);
            theIndex += msgContentEncIndex;

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

        int convert_bcd_string(const char* data, int datalen, int half, char *dest, int odd)
        {
            int i = 0, halfbyte = half ? 1 : 0, len = 0;
            char ch;

            while (i < datalen) {
                ch = (*data >> (halfbyte ? 4 : 0)) & 0x0f;
                if (ch == 0x0f) {
                    *dest = 0;
                    return len;
                }
                if ((ch < 0) || (ch > 9))
                    return -1;

                *dest = '0' + ch;
                dest++;
                len++;

                halfbyte = 1 - halfbyte;
                if (halfbyte == 0) {
                    i++;
                    data++;
                }
            }
            *dest = 0;

            return len;
        }

        guint64 str2bcdnumber(char *str)
        {
            guint64 ull;

            if (str && *str)
            {
                char *ptr;

#ifdef WIN32
                ull = _atoi64(str);
                if ( ull )
                {
                    return ull;
                }
#else
                ull = strtoull(str, &ptr, 10);
                if (*ptr == 0)
                    return ull;
#endif

            }

            return 0;//llu;
        }

        int decodeBcdCode(const char* theData, const unsigned theLen)
        {
            char imsiStr[32] = {0};
            int imsiLen = convert_bcd_string(theData, theLen, 0, imsiStr, 0);
            if (imsiLen > 0)
            {
                valueM = str2bcdnumber(imsiStr);
                return SUCCESS_E;
            }
            else
            {
                return ERROR_E;
            }
        }

    public:
        guint64 valueM;
    };
    typedef TlvBcdString<0x15, Uint8> PhoneNumberIEI;

}
#endif  /*TLVBCDSTRING_H*/


