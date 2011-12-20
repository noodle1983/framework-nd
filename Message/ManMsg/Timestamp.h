#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <string.h>
#include <string>
#include "IntMsg.h"
#include "MsgErrorCode.h"

namespace Msg
{
    template<char theTag>
    class Timestamp
    {
    public:
        Timestamp(){}
        ~Timestamp(){}

        enum {TAG = theTag};
        enum {MIN_BYTES = 1 +
                        Uint32::MIN_BYTES +
                        Uint32::MIN_BYTES +
                        0
        };

        void init()
        {
            seconds.init();
            fraction.init();
        }

        int decode(const char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + MIN_BYTES > theLen)
                return NOT_ENOUGH_BUFFER_E;

            int ret = SUCCESS_E;
            Uint8 tag;
            ret = tag.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;
            if (tag.valueM != TAG)
                return ERROR_E;

            ret = seconds.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;

            ret = fraction.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;

            return ret;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            if (theIndex + MIN_BYTES > theLen)
                return NOT_ENOUGH_BUFFER_E;

            theBuffer[theIndex++] = TAG;

            int ret = SUCCESS_E;
            ret = seconds.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;

            ret = fraction.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)
                return ret;

            return ret;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            leadStr.append("    ");

            theOut << "\n" << leadStr << "seconds: ";
            seconds.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "fraction: ";
            fraction.dump(theOut, theLayer + 1);

            return theOut;
        }
    public:
        Uint32 seconds;
        Uint32 fraction;
    };
}
#endif /* TIMESTAMP_H */

