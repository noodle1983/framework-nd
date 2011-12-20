#ifndef MOBILEIDENTITY_H
#define MOBILEIDENTITY_H

#include "IntMsg.h"
#include "MsgErrorCode.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <Log.h>

namespace Msg
{
    template<char theTag>
    class MobileIdentity
    {
    public:
        MobileIdentity()
            : typeM(IMSI_E)
            , valueM(0)
        {}
        ~MobileIdentity(){}

        enum {TAG = theTag};
        enum {MIN_BYTES = 10};
        enum
        {
            IMSI_E   = 1,
            IMEI_E   = 2,
            IMEISV_E = 3,
            TMSI_E   = 4,
            PTMSI_E  = TMSI_E
        };
        enum
        {
            IMSI_LEN = 8,
            IMEI_LEN = 8,
            IMEISV_LEN = 9
        };

        void init()
        {
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            int ret = SUCCESS_E;
            Uint8 tag;
            ret = tag.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;
            if (tag.valueM != TAG)
                return ERROR_E;

            Uint8 length;
            ret = length.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;
            if (theIndex + length.valueM > theLen)
                return NOT_ENOUGH_BUFFER_E;

            const char* content = theBuffer + theIndex;
            theIndex += length.valueM;

            typeM = content[0] & 0x07;
            switch (typeM)
            {
            case IMEI_E:
            case IMSI_E:
                if (8 != length.valueM)
                {
                    LOG_WARN("expect len:8, actual:" << (int)length.valueM
                            << ", type:" << typeM);
                }
                return decodeBcdCode(content, length.valueM);

            case IMEISV_E:
                if (9 != length.valueM)
                {
                    LOG_WARN("expect len:9, actual:" << (int)length.valueM
                            << ", type:" << typeM);
                }
                return decodeBcdCode(content, length.valueM);

            case TMSI_E:
                if (5 != length.valueM)
                {
                    LOG_WARN("expect len:5, actual:" << (int)length.valueM
                            << ", type:" << typeM);
                }

                valueM = combine_bits2(content, 1, 8, 32);
                return SUCCESS_E;
            default:
                return ERROR_E;
            }

            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (typeM != IMSI_E && typeM != IMEI_E && typeM != IMEISV_E)
                return ERROR_E;

            char mobileIdentityStr[32] = {0};
            char msgContent[32];
            memset(msgContent, 0xff, sizeof(msgContent));

            sprintf(mobileIdentityStr, "%llu", (long long unsigned int)valueM);
            int mobileIdStrLen = strlen(mobileIdentityStr);
            int msgContentEncIndex = 0;
            msgContent[0] = typeM & 0x07;
            msgContent[0] |= (mobileIdStrLen % 2) ? 0x08 : 0x00;//0:event; 1:odd
            int halfbyte = 1;
            for(int i = 0; i < mobileIdStrLen; i++)
            {
                if (halfbyte)
                {
                    msgContent[msgContentEncIndex] |= (mobileIdentityStr[i] - '0') << 4;
                    msgContentEncIndex++;
                    halfbyte = 0;
                }
                else
                {
                    msgContent[msgContentEncIndex] = mobileIdentityStr[i] - '0';
                    halfbyte = 1;
                }

            }
            if (halfbyte)
            {
                msgContent[msgContentEncIndex] |= 0xf0;
                msgContentEncIndex++;
                halfbyte = 0;
            }

            Uint8 length;
            if (typeM == IMSI_E || typeM == IMEI_E)
            {
                length.valueM = IMSI_LEN;
            }
            else if (typeM == IMEISV_E)
            {
                length.valueM = IMEISV_LEN;
            }
            else
            {
                length.valueM = msgContentEncIndex;
            }

            int totalLen = 1 + sizeof(length.valueM) + length.valueM;
            if (theIndex + totalLen > theLen)
                return NOT_ENOUGH_BUFFER_E;

            theBuffer[theIndex++] = TAG;
            length.encode(theBuffer, theLen, theIndex);
            memcpy(theBuffer + theIndex, msgContent, length.valueM);
            theIndex += length.valueM;

            return 0;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << (  IMSI_E == typeM ? "IMSI"
                       : IMEI_E == typeM ? "IMEI"
                       : IMEISV_E == typeM ? "IMEISV"
                       : TMSI_E == typeM ? "TMSI"
                       : "UNKNOW")
                   << "=" << valueM;
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

        /*
        void bcdnumber2str(bcdnumber bcdnum, char *str)
        {
            sprintf(str, "%llu", (__int64)bcdnum);
            return str;
        }
        */

        int decodeBcdCode(const char* theData, const unsigned theLen)
        {
            char imsiStr[32] = {0};
            int imsiLen = convert_bcd_string(theData, theLen, 1, imsiStr, *theData & 0x08);
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

        unsigned int combine_bits2(const char* data, int sbyte, int sbit, int bits)
        {
            unsigned int  vi = 0;
            unsigned char vu = 0;

            data += sbyte;

            while (sbit < bits) {
                vu = *data & (0xff >> (8 - sbit));
                vi = (vi << 8) | vu;

                data++;
                bits -= sbit;
                sbit = 8;
            }

            vu = *data & ((0xff >> (8 - sbit)) & (0xff << (sbit - bits)));
            vi = (vi << 8) | vu;
            vi = vi >> (sbit - bits);

            return vi;
        }

    public:
        unsigned char typeM;
        guint64 valueM;
    };

}
#endif  /*MOBILEIDENTITY_H*/


