#ifndef INTMSG_H
#define INTMSG_H

#include <string.h>
#include <glib.h>
#include "MsgErrorCode.h"

namespace Msg
{
    class Uint8
    {
    public:
        Uint8(){}
        ~Uint8(){}

        enum {MIN_BYTES = 1};

        void init()
        {
            valueM = 0;
        }

        operator unsigned char()
        { return valueM; }
        const Uint8& operator =(const unsigned char theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint8) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            valueM = theBuffer[theIndex];
            theIndex += sizeof(guint8);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint8) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            theBuffer[theIndex] = valueM;
            theIndex += sizeof(guint8);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << unsigned(valueM);
            return theOut;
        }


    public:
        unsigned char valueM;
    };
    typedef Uint8 Length8;

    class Uint16
    {
    public:
        Uint16(){}
        ~Uint16(){}

        enum {MIN_BYTES = 2};

        void init()
        {
            valueM = 0;
        }

        operator guint16()
        { return valueM; }
        const Uint16& operator =(const unsigned short theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint16) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(&valueM, theBuffer + theIndex, sizeof(guint16));
            valueM = GUINT16_FROM_BE(valueM);
            theIndex += sizeof(guint16);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint16) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            guint16 netInt = GUINT16_TO_BE(valueM);
            memcpy(theBuffer + theIndex, &netInt, sizeof(guint16));
            theIndex += sizeof(guint16);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

    public:
        guint16 valueM;
    };
    /* for the length of the Message, it is reset again at last.*/
    typedef Uint16 Length16;

    class Uint32
    {
    public:
        Uint32(){}
        ~Uint32(){}

        enum {MIN_BYTES = 4};

        void init()
        {
            valueM = 0;
        }

        operator guint32()
        { return valueM; }
        const Uint32& operator =(const unsigned theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint32) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(&valueM, theBuffer + theIndex, sizeof(guint32));
            valueM = GUINT32_FROM_BE(valueM);
            theIndex += sizeof(guint32);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint32) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            guint32 netInt = GUINT32_TO_BE(valueM);
            memcpy(theBuffer + theIndex, &netInt, sizeof(guint32));
            theIndex += sizeof(guint32);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

    public:
        guint32 valueM;
    };
    /* for the length of the Message, it is reset again at last.*/
    typedef Uint32 Length32;
    typedef Uint32 MsgId32;

    class PlainInt32
    {
    public:
        PlainInt32(){}
        ~PlainInt32(){}

        enum {MIN_BYTES = 4};

        void init()
        {
            valueM = 0;
        }

        operator guint32()
        { return valueM; }
        const PlainInt32& operator =(const unsigned theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint32) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(&valueM, theBuffer + theIndex, sizeof(guint32));
            theIndex += sizeof(guint32);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint32) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            guint32 netInt = valueM;
            memcpy(theBuffer + theIndex, &netInt, sizeof(guint32));
            theIndex += sizeof(guint32);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

    public:
        guint32 valueM;
    };

    class Uint48
    {
    public:
        Uint48(){}
        ~Uint48(){}

        enum {MIN_BYTES = 6};

        void init()
        {
            valueM = 0;
        }

        operator guint64()
        { return valueM; }
        const Uint48& operator =(const unsigned long long theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + MIN_BYTES > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memset(&valueM, 0, 2);
            memcpy((char*)(&valueM) + 2, theBuffer + theIndex, MIN_BYTES);
            valueM = GUINT64_FROM_BE(valueM);
            theIndex += MIN_BYTES;
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + MIN_BYTES > theLen)
                return NOT_ENOUGH_BUFFER_E;

            guint64 netInt = GUINT64_TO_BE(valueM);
            memcpy(theBuffer + theIndex, (char*)(&netInt) + 2, MIN_BYTES);
            theIndex += MIN_BYTES;

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

    public:
        guint64 valueM;
    };

    class Uint64
    {
    public:
        Uint64(){}
        ~Uint64(){}

        enum {MIN_BYTES = 8};

        void init()
        {
            valueM = 0;
        }

        operator guint64()
        { return valueM; }
        const Uint64& operator =(const unsigned long long theValue)
        { valueM = theValue; return *this;}

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint64) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            memcpy(&valueM, theBuffer + theIndex, sizeof(guint64));
            valueM = GUINT64_FROM_BE(valueM);
            theIndex += sizeof(guint64);
            return SUCCESS_E;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint64) > theLen)
                return NOT_ENOUGH_BUFFER_E;

            guint64 netInt = GUINT64_TO_BE(valueM);
            memcpy(theBuffer + theIndex, &netInt, sizeof(guint64));
            theIndex += sizeof(guint64);

            return SUCCESS_E;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        }

    public:
        guint64 valueM;
    };
    /* for the length of the Message, it is reset again at last.*/
    typedef Uint64 Length64;
}
#endif  /*INTMSG_H*/

