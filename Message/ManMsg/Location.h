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
        enum 
        {
           MNC_E = 0x80,
           MCC_E = 0x40,
           LAC_E = 0x20,
           RAC_E = 0x10,
           CI_E  = 0x08,
           SI_E  = 0x04
        };

        void init()
        {
            flag.init();            
            mnc.init();            
            mcc.init();            
            lac.init();            
            rac.init();            
            ci.init();            
            si.init();            
        }

        void setMNC(const guint16 theMNC)
        {
            flag.valueM |= MNC_E;
            mnc.valueM = theMNC;
        }

        guint16 getMNC()
        {
            return (flag.valueM & MNC_E) ? mnc.valueM : 0;
        }


        void setMCC(const guint16 theMCC)
        {
            flag.valueM |= MCC_E;
            mcc.valueM = theMCC;
        }

        guint16 getMCC()
        {
            return (flag.valueM & MCC_E) ? mcc.valueM : 0;
        }

        void setLAC(const guint16 theLAC)
        {
            flag.valueM |= LAC_E;
            lac.valueM = theLAC;
        }

        guint16 getLAC()
        {
            return (flag.valueM & LAC_E) ? lac.valueM : 0;
        }

        void setRAC(const guint16 theRAC)
        {
            flag.valueM |= RAC_E;
            rac.valueM = theRAC;
        }

        guint16 getRAC()
        {
            return (flag.valueM & RAC_E) ? rac.valueM : 0;
        }

        void setCI(const guint16 theCI)
        {
            flag.valueM |= CI_E;
            ci.valueM = theCI;
        }

        guint16 getCI()
        {
            return (flag.valueM & CI_E) ? ci.valueM : 0;
        }
        
        void setSI(const guint16 theSI)
        {
            flag.valueM |= SI_E;
            si.valueM = theSI;
        }

        guint16 getSI()
        {
            return (flag.valueM & SI_E) ? si.valueM : 0;
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

            ret = mnc.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = mcc.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = lac.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = rac.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = ci.decode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = si.decode(theBuffer, theLen, theIndex);
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
                return NOT_ENOUGH_BUFFER_E;

            theBuffer[theIndex++] = TAG;
            length.encode(theBuffer, theLen, theIndex);

            int ret = SUCCESS_E;
            ret = flag.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = mnc.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = mcc.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = lac.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = rac.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = ci.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            ret = si.encode(theBuffer, theLen, theIndex);
            if (SUCCESS_E != ret)            
                return ret;

            return ret;
        }

        template<typename StreamType>
        StreamType& dump(StreamType& theOut, unsigned theLayer = 0)
        {
            std::string leadStr(theLayer * 4, ' ');
            leadStr.append("    ");

            theOut << "\n" << leadStr << "flag: ";
            flag.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "mnc: ";
            mnc.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "mcc: ";
            mcc.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "lac: ";
            lac.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "rac: ";
            rac.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "ci: ";
            ci.dump(theOut, theLayer + 1);


            theOut << "\n" << leadStr << "si: ";
            si.dump(theOut, theLayer + 1);

            return theOut;
        }
    public:
        Uint8 flag;           
        Uint16 mnc;           
        Uint16 mcc;           
        Uint16 lac;           
        Uint16 rac;           
        Uint16 ci;           
        Uint16 si;           
    };
}
#endif /* LOCATION_H */

