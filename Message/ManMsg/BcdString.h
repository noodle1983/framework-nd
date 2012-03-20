#ifndef BCDSTRING_H
#define BCDSTRING_H

#include "IntMsg.h"
#include "MsgErrorCode.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

namespace Msg
{
    template<int theFixLen>
    class BcdString
    {
    public:
        BcdString()
            : valueM(0)
        {}
        ~BcdString(){}

        enum {MIN_BYTES = theFixLen};

        void init()
        {
        }

        operator guint64()
        { return valueM; }
        const BcdString& operator =(const unsigned long long theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + MIN_BYTES > theLen)
                return NOT_ENOUGH_BUFFER_E;

            unsigned oriIndex = theIndex;
            theIndex += MIN_BYTES;
            return decodeBcdCode(theBuffer + oriIndex, MIN_BYTES);
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + MIN_BYTES > theLen)
                return NOT_ENOUGH_BUFFER_E;

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

            memcpy(theBuffer + theIndex, msgContent, MIN_BYTES);
            theIndex += MIN_BYTES;

            if (msgContentEncIndex > MIN_BYTES)
            {
                //valueM is too large
                return ERROR_E;
            }

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

    typedef BcdString<8> BcdUserId;
}
#endif  /*BCDSTRING_H*/


