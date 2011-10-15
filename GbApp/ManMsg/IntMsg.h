#ifndef INTMSG_H
#define INTMSG_H

#include <string.h>
#include <glib.h>

namespace GbApp
{
namespace Msg
{
    class Uint8
    {
    public:
        Uint8(){}
        ~Uint8(){}

        void init()
        {
            valueM = 0;
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint8) > theLen)
                return -1;

            valueM = theBuffer[theIndex];
            theIndex += sizeof(guint8);
            return 0;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint8) > theLen)
                return -1;

            theBuffer[theIndex] = valueM;
            theIndex += sizeof(guint8);

            return 0;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            theOut << valueM;
            return theOut;
        } 


    public:
        guint8 valueM;
    };
    typedef Uint8 Length8;

    class Uint16
    {
    public:
        Uint16(){}
        ~Uint16(){}

        void init()
        {
            valueM = 0;
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint16) > theLen)
                return -1;

            memcpy(&valueM, theBuffer + theIndex, sizeof(guint16));
            valueM = GUINT16_FROM_BE(valueM);
            theIndex += sizeof(guint16);
            return 0;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint16) > theLen)
                return -1;

            guint16 netInt = GUINT16_TO_BE(valueM);
            memcpy(theBuffer + theIndex, &netInt, sizeof(guint16));
            theIndex += sizeof(guint16);

            return 0;
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

        void init()
        {
            valueM = 0;
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint32) > theLen)
                return -1;

            memcpy(&valueM, theBuffer + theIndex, sizeof(guint32));
            valueM = GUINT32_FROM_BE(valueM);
            theIndex += sizeof(guint32);
            return 0;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint32) > theLen)
                return -1;

            guint32 netInt = GUINT32_TO_BE(valueM);
            memcpy(theBuffer + theIndex, &netInt, sizeof(guint32));
            theIndex += sizeof(guint32);

            return 0;
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

    class Uint64
    {
    public:
        Uint64(){}
        ~Uint64(){}

        void init()
        {
            valueM = 0;
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint64) > theLen)
                return -1;

            memcpy(&valueM, theBuffer + theIndex, sizeof(guint64));
            valueM = GUINT64_FROM_BE(valueM);
            theIndex += sizeof(guint64);
            return 0;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + sizeof(guint64) > theLen)
                return -1;

            guint64 netInt = GUINT64_TO_BE(valueM);
            memcpy(theBuffer + theIndex, &netInt, sizeof(guint64));
            theIndex += sizeof(guint64);

            return 0;
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
}
#endif  /*INTMSG_H*/

