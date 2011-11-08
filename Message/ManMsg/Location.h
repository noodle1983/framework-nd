#ifndef LOCATION_H
#define LOCATION_H

#include <string.h>
#include <string>
#include "IntMsg.h"
#include "MsgErrorCode.h"

namespace Msg
{
    template<char theTag>
    class Location
    {
    public:
        Location(){}
        ~Location(){}

        enum {TAG = theTag};
        enum {MIN_BYTES = 2 +
                        Uint8::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        Uint16::MIN_BYTES +
                        0
        };

        void init()
        {
            flag.init();            
            mNC.init();            
            mCC.init();            
            lAC.init();            
            rAC.init();            
            cI.init();            
            sI.init();            
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
            if (length.valueM != MIN_BYTES - 2)
                return ERROR_E;

            ret = flag.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = mNC.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = mCC.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = lAC.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = rAC.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = cI.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = sI.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            return ret;
        }

        int encode(char* theBuffer, const unsigned theLen, unsigned& theIndex)
        {
            Uint8 length;
            length.valueM = MIN_BYTES - 2;
            int totalLen = MIN_BYTES;
            if (theIndex + totalLen > theLen)
                return -1;

            theBuffer[theIndex++] = TAG;
            length.encode(theBuffer, theLen, theIndex);

            int ret = SUCCESS_E;
            ret = flag.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = mNC.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = mCC.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = lAC.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = rAC.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = cI.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = sI.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            return 0;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            leadStr.append("    ");

            theOut << "\n" << leadStr << "flag: ";
            flag.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "mNC: ";
            mNC.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "mCC: ";
            mCC.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "lAC: ";
            lAC.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "rAC: ";
            rAC.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "cI: ";
            cI.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "sI: ";
            sI.dump(theOut, theLayer + 1);

            return theOut;
        }
    public:
        Uint8 flag;           
        Uint16 mNC;           
        Uint16 mCC;           
        Uint16 lAC;           
        Uint16 rAC;           
        Uint16 cI;           
        Uint16 sI;           
    };
}
#endif /* LOCATION_H */

